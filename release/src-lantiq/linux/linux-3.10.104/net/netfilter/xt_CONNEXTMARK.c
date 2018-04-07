/*
 * This module is used to copy extended mark markings from packets
 * to connections, and restore extended mark markings from connections
 * back to packets.  This would normally be performed in conjunction
 * with the EXTMARK target and state match.
 *
 * Based somewhat on CONNMARK:
 *   Copyright (C) 2002,2004 MARA Systems AB <http://www.marasystems.com>
 *    by Henrik Nordstrom <hno@marasystems.com>
 *
 * (C) 2006,2008 Red Hat, Inc., James Morris <jmorris@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_CONNEXTMARK.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_ecache.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tapan Kumar Mishra <tapankumar.mishra@lantiq.com>");
MODULE_DESCRIPTION("Xtables: target for copying between connection and extended mark");
MODULE_ALIAS("ipt_CONNEXTMARK");
MODULE_ALIAS("ip6t_CONNEXTMARK");

/*
 * If the packet has a extended mark mark and the connection does not, copy
 * the extended mark mark from the packet to the connection.
 */
static void extmark_save(const struct sk_buff *skb)
{
	if (skb->extmark) {
		struct nf_conn *ct;
		enum ip_conntrack_info ctinfo;

		ct = nf_ct_get(skb, &ctinfo);
		if (ct && !ct->extmark) {
			ct->extmark = skb->extmark;
			nf_conntrack_event_cache(IPCT_EXTMARK, ct);
		}
	}
}

/*
 * If packet has no extended mark mark, and the connection does, restore the
 * extended mark mark from the connection to the packet.
 */
static void extmark_restore(struct sk_buff *skb)
{
	if (!skb->extmark) {
		const struct nf_conn *ct;
		enum ip_conntrack_info ctinfo;

		ct = nf_ct_get(skb, &ctinfo);
		if (ct && ct->extmark)
			skb->extmark = ct->extmark;
	}
}

static unsigned int
connextmark_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_connextmark_target_info *info = par->targinfo;

	switch (info->mode) {
	case CONNEXTMARK_SAVE:
		extmark_save(skb);
		break;

	case CONNEXTMARK_RESTORE:
		extmark_restore(skb);
		break;

	default:
		BUG();
	}

	return XT_CONTINUE;
}

static int connextmark_tg_check(const struct xt_tgchk_param *par)
{
	const struct xt_connextmark_target_info *info = par->targinfo;
	int ret;

	if (strcmp(par->table, "mangle") != 0 ) {
		pr_info("target only valid in the \'mangle\' table, not \'%s\'.\n", par->table);
		return -EINVAL;
	}

	switch (info->mode) {
	case CONNEXTMARK_SAVE:
	case CONNEXTMARK_RESTORE:
		break;

	default:
		pr_info("invalid mode: %hu\n", info->mode);
		return -EINVAL;
	}

	ret = nf_ct_l3proto_try_module_get(par->family);
	if (ret < 0)
		pr_info("cannot load conntrack support for proto=%u\n",
			par->family);
	return ret;
}

static void connextmark_tg_destroy(const struct xt_tgdtor_param *par)
{
	nf_ct_l3proto_module_put(par->family);
}

static struct xt_target connextmark_tg_reg __read_mostly = {
	.name       = "CONNEXTMARK",
	.revision   = 0,
	.family     = NFPROTO_UNSPEC,
	.checkentry = connextmark_tg_check,
	.destroy    = connextmark_tg_destroy,
	.target     = connextmark_tg,
	.targetsize = sizeof(struct xt_connextmark_target_info),
	.me         = THIS_MODULE,
};

static int __init connextmark_tg_init(void)
{
	return xt_register_target(&connextmark_tg_reg);
}

static void __exit connextmark_tg_exit(void)
{
	xt_unregister_target(&connextmark_tg_reg);
}

module_init(connextmark_tg_init);
module_exit(connextmark_tg_exit);

