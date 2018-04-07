#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>

#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ipt_synflood.h>

/*
   To avoid underflow, we multiply by 128 (ie. you get 128 credits per
   jiffy).  Hence a cost of 2^32-1, means one pass per 32768 seconds
   at 1024HZ (or one every 9 hours).  A cost of 1 means 12800 passes
   per second at 100HZ.  */

#define CREDITS_PER_JIFFY 128
/************************************************************/
/*
 * Information we keep per each source address.
 */
struct host {
	struct host *next;		/* Next entry with the same hash */
	unsigned long timestamp;		/* Last update time */
	struct in_addr ip_addr;	/* Source address */
	u_int32_t credit;			/* Total credit of addresses in the list */
};

/*
 * State information.
 */
static struct repo_state{
	spinlock_t lock;
	struct host list[LIST_SIZE];	/* List of source addresses */
	struct host *hash[HASH_SIZE];	/* Hash: pointers into the list */
	int index;			/* Oldest entry to be replaced */
#if 0
} src_state,dst_state;
#else
} dst_state;
#endif

/*
 * Convert an IP address into a hash table index.
 */
static inline int hashfunc(struct in_addr addr)
{
	unsigned int value;
	int hash;

	value = addr.s_addr;
	hash = 0;
	do {
		hash ^= value;
	} while ((value >>= HASH_LOG));

	return hash & (HASH_SIZE - 1);
}


/* Precision saver. */
static u_int32_t
user2credits(u_int32_t user)
{
	/* If multiplying would overflow... */
	if (user > 0xFFFFFFFF / (HZ*CREDITS_PER_JIFFY))
		/* Divide first. */
		return (user / IPT_LIMIT_SCALE) * HZ * CREDITS_PER_JIFFY;

	return (user * HZ * CREDITS_PER_JIFFY) / IPT_LIMIT_SCALE;
}

static int
match(const struct sk_buff *skb, const struct xt_match_param *par)
{
	unsigned long now;
	struct host **head,*last,*curr;
	int count,hash;
	struct repo_state *state = NULL;
	struct in_addr addr;

	/* Parameters from userspace */
	const struct ipt_rateinfo *rateinfo = par->matchinfo;

	struct iphdr *ip_h;
	/* IP header */
	ip_h = ip_hdr(skb); //skb->nh.iph;

	/* Get the source address, source & destination ports, and TCP flags */
	addr.s_addr = ip_h->saddr;

	/* We're using IP address 0.0.0.0 for a special purpose here, so don't let
	 * them spoof us. [DHCP needs this feature - HW] */
	if (!addr.s_addr) {
		return 0;
	}

#if 0
	if (rateinfo->addr_type == SOURCE) {
		addr.s_addr = ip_h->saddr;
		state = &src_state;
	} else {
		addr.s_addr = ip_h->daddr;
		state = &dst_state;
	}
#else
	addr.s_addr = ip_h->daddr;
	state = &dst_state;
#endif

	/* Use jiffies here not to depend on someone setting the time while we're
	 * running; we need to be careful with possible return value overflows. */
	now = jiffies;

	spin_lock(&(*state).lock);
	
	/* Do we know this source address already? */
	count = 0;
	last = NULL;
	if ((curr = *(head = &(*state).hash[hash = hashfunc(addr)])))
		do {
			if (curr->ip_addr.s_addr == addr.s_addr) break;
			count++;
			if (curr->next) last = curr;
		} while ((curr = curr->next));

	if(curr) {
		curr->credit += (now - xchg(&curr->timestamp, now)) * CREDITS_PER_JIFFY;
		if (curr->credit > rateinfo->credit_cap)
			curr->credit = rateinfo->credit_cap;
		if(curr->credit >= rateinfo->cost) {
			curr->credit -= rateinfo->cost;
			goto out_match;
		}
		goto out_no_match;
	}

	/* Got too many source addresses with the same hash value? Then remove the
	 * oldest one from the hash table, so that they can't take too much of our
	 * CPU time even with carefully chosen spoofed IP addresses. */
	if (count >= HASH_MAX && last) {
		last->next = NULL;
	}

	/* We're going to re-use the oldest list entry, so remove it from the hash
	 * table first (if it is really already in use, and isn't removed from the
	 * hash table already because of the HASH_MAX check above). */

	/* First, find it */
	if ((*state).list[(*state).index].ip_addr.s_addr)
		head = &(*state).hash[hashfunc((*state).list[(*state).index].ip_addr)];
	else
		head = &last;
	last = NULL;
	if ((curr = *head))
	do {
		if (curr == &(*state).list[(*state).index]) break;
		last = curr;
	} while ((curr = curr->next));

	/* Then, remove it */
	if (curr) {
		if (last)
			last->next = last->next->next;
		else if (*head)
			*head = (*head)->next;
	}

	/* Get our list entry */
	curr = &(*state).list[(*state).index++];
	if ((*state).index >= LIST_SIZE) (*state).index = 0;

	/* Link it into the hash table */
	head = &(*state).hash[hash];
	curr->next = *head;
	*head = curr;

	/* And fill in the fields */
	curr->timestamp = now;
	curr->ip_addr = addr;
	curr->credit = rateinfo->credit - rateinfo->cost;

out_match:
	spin_unlock(&(*state).lock);
	return (1 ^ rateinfo->invert);
out_no_match:
	spin_unlock(&(*state).lock);
	return (0 ^ rateinfo->invert);
}

static int
checkentry(const struct xt_mtchk_param *par)
{
	struct ipt_rateinfo *r = par->matchinfo;
	
	/* Check for overflow. */
	if (r->burst == 0
	    || user2credits(r->avg * r->burst) < user2credits(r->avg)) {
		printk("Call rusty: overflow in ipt_limit: %u/%u\n",
		       r->avg, r->burst);
		return 0;
	}

	/* User avg in seconds * IPT_LIMIT_SCALE: convert to jiffies *
	   128. */
	r->credit = user2credits(r->avg * r->burst);	 /* Credits full. */
	r->credit_cap = user2credits(r->avg * r->burst); /* Credits full. */
	r->cost = user2credits(r->avg);

	return 1;
}

static struct xt_match ipt_synflood_reg = {
	.list = { NULL, NULL },
	.name       = "synflood",
	.family = NFPROTO_IPV4,
	.match      = match,
	.checkentry = checkentry,
	.matchsize = sizeof(struct ipt_rateinfo),
	.destroy = NULL,
	.me = THIS_MODULE,
};
static int __init init(void)
{
	if (xt_register_match(&ipt_synflood_reg))
		return -EINVAL;
	return 0;
}

static void __exit fini(void)
{
	xt_unregister_match(&ipt_synflood_reg);
}

module_init(init);
module_exit(fini);
MODULE_LICENSE("GPL");

