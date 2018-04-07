#include <linux/fs.h>
#include<linux/slab.h>
#include <linux/kernel.h>
#include <net/datapath_proc_api.h>
#include <net/datapath_api.h>
#include "datapath.h"

static int mode;
/* 0-seq_printf mode, not -1 printk mode, -1: no print */
/* remove  comemnts to enable debug feature for proc_api */
#undef local_dbg
#if 1
#define local_dbg(fmt, arg...) pr_debug(fmt, ##arg)
#else
#define local_dbg(fmt, arg...) do { pr_info(fmt, ##arg); } while (0)
#endif
static inline int lower_ch(int ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return ch + 'a' - 'A';
	return ch;
}

int dp_strcmpi(char const *s1, char const *s2)
{
	int c1, c2;

	if (!s1 || !s2)
		return 1;
	while (*s1 && *s2) {	/*same length */
		c1 = lower_ch(*s1);
		c2 = lower_ch(*s2);
		s1++;
		s2++;

		if (c1 != c2)
			return c1 - c2;
	}
	return *s1 - *s2;
}
EXPORT_SYMBOL(dp_strcmpi);

int dp_strncmpi(const char *s1, const char *s2, size_t n)
{
	int c1, c2;

	if (!s1 || !s2)
		return 1;
	for (; n > 0; s1++, s2++, --n) {
		c1 = lower_ch(*s1);
		c2 = lower_ch(*s2);
		if (c1 != c2)
			return c1 - c2;
		else if (c1 == '\0')
			return 0;
	}
	return 0;
}
EXPORT_SYMBOL(dp_strncmpi);

char *dp_strstri(char *string, char *substring)
{
	register char *a, *b;

/* First scan quickly through the two strings looking for a
* single-character match.  When it's found, then compare the
* rest of the substring.
*/
	if (!string || !substring)
		return (char *)0;
	b = substring;
	if (*b == 0)
		return string;

	for (; *string != 0; string += 1) {
		if (lower_ch(*string) != lower_ch(*b))
			continue;
		a = string;
		while (1) {
			if (*b == 0)
				return string;
			if (lower_ch(*a++) != lower_ch(*b++))
				break;
		}
		b = substring;
	}
	return (char *)0;
}
EXPORT_SYMBOL(dp_strstri);

void dp_replace_ch(char *p, int len, char orig_ch, char new_ch)
{
	int i;

	if (p)
		for (i = 0; i < len; i++) {
			if (p[i] == orig_ch)
				p[i] = new_ch;
		}
}
EXPORT_SYMBOL(dp_replace_ch);

static unsigned int btoi(char *str)
{
	unsigned int sum = 0;
	signed len = 0, i = 0;

	len = strlen(str);
	len = len - 1;
	while (len >= 0) {
		if (*(str + len) == '1')
			sum = (sum | (1 << i));
		i++;
		len--;
	}
	return sum;
}

int dp_atoi(unsigned char *str)
{
	unsigned int n = 0;
	int i = 0;
	int nega_sign = 0;

	if (!str)
		return 0;
	dp_replace_ch(str, strlen(str), '.', 0);
	dp_replace_ch(str, strlen(str), ' ', 0);
	dp_replace_ch(str, strlen(str), '\r', 0);
	dp_replace_ch(str, strlen(str), '\n', 0);
	if (str[0] == 0)
		return 0;

	if (str[0] == 'b' || str[0] == 'B') {	/*binary format */
		n = btoi(str + 1);
	} else if ((str[0] == '0') && ((str[1] == 'x') || (str[1] == 'X'))) {
		/*hex format */
		str += 2;

		while (str[i]) {
			n = n * 16;
			if (('0' <= str[i] && str[i] <= '9')) {
				n += str[i] - '0';
			} else if (('A' <= str[i] && str[i] <= 'F')) {
				n += str[i] - 'A' + 10;
				;
			} else if (('a' <= str[i] && str[i] <= 'f')) {
				n += str[i] - 'a' + 10;
				;
			} else
				PRINTK(KERN_ERR "Wrong value:%u\n", str[i]);

			i++;
		}

	} else {
		if (str[i] == '-') {	/*negative sign */
			nega_sign = 1;
			i++;
		}
		while (str[i]) {
			n *= 10;
			n += str[i] - '0';
			i++;
		}
	}
	if (nega_sign)
		n = -(int)n;
	return n;
}
EXPORT_SYMBOL(dp_atoi);

void dp_remove_leading_whitespace(char **p, int *len)
{
	while (*len && ((**p == ' ') || (**p == '\r') || (**p == '\r'))) {
		(*p)++;
		(*len)--;
	}
}
EXPORT_SYMBOL(dp_remove_leading_whitespace);

/*Split buffer to multiple segment with seperator space.
And put pointer to array[].
By the way, original buffer will be overwritten with '\0' at some place.
*/
int dp_split_buffer(char *buffer, char *array[], int max_param_num)
{
	int i, set_copy = 0;
	int res = 0;
	int len;

	for (i = 0; i < max_param_num; i++)
		array[i] = NULL;
	if (!buffer)
		return 0;
	len = strlen(buffer);
	for (i = 0; i < max_param_num;) {
		dp_remove_leading_whitespace(&buffer, &len);
		for (;
		     *buffer != ' ' && *buffer != '\0' && *buffer != '\r' &&
		     *buffer != '\n' && *buffer != '\t'; buffer++, len--) {
			/*Find first valid charactor */
			set_copy = 1;
			if (!array[i])
				array[i] = buffer;
		}

		if (set_copy == 1) {
			i++;
			if (*buffer == '\0' || *buffer == '\r' ||
			    *buffer == '\n') {
				*buffer = 0;
				break;
			}
			*buffer = 0;
			buffer++;
			len--;
			set_copy = 0;

		} else {
			if (*buffer == '\0' || *buffer == '\r' ||
			    *buffer == '\n')
				break;
			buffer++;
			len--;
		}
	}
	res = i;

	return res;
}
EXPORT_SYMBOL(dp_split_buffer);

void set_start_end_id(unsigned int new_start, unsigned int new_end,
		      unsigned int max_start, unsigned int max_end,
		      unsigned int default_start, unsigned int default_end,
		      unsigned int *start, unsigned int *end)
{
	if (!start || !end)
		return;

	if (new_start > new_end) {
		*start = default_start;
		*end = default_end;
	} else {
		*start = new_start;
		*end = new_end;
	}

	if (*start > max_start)
		*start = default_start;

	if (*end > max_end)
		*end = default_end;
}
EXPORT_SYMBOL(set_start_end_id);

static void *dp_seq_start(struct seq_file *s, loff_t *pos)
{
	struct dp_proc_file_entry *p = s->private;

	if (p->pos < 0)
		return NULL;

	return p;
}

static void *dp_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	struct dp_proc_file_entry *p = s->private;

	*pos = p->pos;

	if (p->pos >= 0)
		return p;
	else
		return NULL;
}

static void dp_seq_stop(struct seq_file *s, void *v)
{
}

static int dp_seq_show(struct seq_file *s, void *v)
{
	struct dp_proc_file_entry *p = s->private;

	if (p->pos >= 0) {
		if (p->multi_callback) {
			local_dbg("multiple call");
			p->pos = p->multi_callback(s, p->pos);
		} else if (p->single_callback) {
			local_dbg("single call: %p", p->single_callback);
			p->single_callback(s);
			p->pos = -1;
		}
	}
	return 0;
}

static const struct seq_operations dp_seq_ops = {
	.start = dp_seq_start,
	.next = dp_seq_next,
	.stop = dp_seq_stop,
	.show = dp_seq_show
};

void dummy_single_show(struct seq_file *s)
{
	SEQ_PRINTF(s, "Cat Not implemented yet !\n");
}

static int dp_proc_open(struct inode *inode, struct file *file)
{
	struct seq_file *s;
	struct dp_proc_file_entry *p;
	struct dp_proc_entry *entry;
	int ret;

	ret = seq_open(file, &dp_seq_ops);
	if (ret)
		return ret;

	s = file->private_data;
	p = kmalloc(sizeof(struct dp_proc_file_entry), GFP_KERNEL);

	if (!p) {
		(void)seq_release(inode, file);
		return -ENOMEM;
	}
	memset(p, 0, sizeof(*p));

	entry = PDE_DATA(inode);

	if (entry->multi_callback)
		p->multi_callback = entry->multi_callback;
	if (entry->single_callback)
		p->single_callback = entry->single_callback;
	else
		p->single_callback = dummy_single_show;

	if (entry->init_callback)
		p->pos = entry->init_callback();
	else
		p->pos = 0;

	s->private = p;

	return 0;
}

static int dp_proc_release(struct inode *inode, struct file *file)
{
	struct seq_file *s;

	s = file->private_data;
	kfree(s->private);

	return seq_release(inode, file);
}

static char *get_print_mode_string(void)
{
	if (mode == 0)
		return "seq_printf";
	else if (mode != -1)
		return "printk";
	else
		return "no print";

}

void proc_print_mode_read(struct seq_file *s)
{
	SEQ_PRINTF(s, "mode=%d: %s\n", mode, get_print_mode_string());
}

ssize_t proc_print_mode_write(struct file *file, const char *buf,
			      size_t count, loff_t *ppos)
{
	int len;
	char str[64];
	int num;
	char *param_list[2];

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	mode = dp_atoi(param_list[0]);
	PRINTK("new mode=%d: %s\n", mode, get_print_mode_string());
	return count;
}

static int dp_seq_single_show(struct seq_file *s, void *v)
{
	struct dp_proc_entry *p = s->private;
	p->single_callback(s);
	return 0;
}

static int dp_proc_single_open(struct inode *inode, struct file *file)
{
	return single_open(file, dp_seq_single_show, PDE_DATA(inode));
}

void dp_proc_entry_create(struct proc_dir_entry *parent_node,
			  struct dp_proc_entry *proc_entry)
{
	if (!proc_entry || !proc_entry->name)
		return;
	memset(&proc_entry->ops, 0, sizeof(struct file_operations));
	proc_entry->ops.owner = THIS_MODULE;

	if (proc_entry->single_callback) {
		proc_entry->ops.open = dp_proc_single_open;
		proc_entry->ops.release = single_release;
	} else if (proc_entry->multi_callback) {
		proc_entry->ops.open = dp_proc_open;
		proc_entry->ops.release = dp_proc_release;
	} else {
		proc_entry->ops.open = dp_proc_single_open;
		proc_entry->ops.release = single_release;
		proc_entry->single_callback = dummy_single_show;
	}

	proc_entry->ops.read = seq_read;
	proc_entry->ops.llseek = seq_lseek;
	proc_entry->ops.write = proc_entry->write_callback;
	proc_create_data(proc_entry->name, (S_IFREG | S_IRUGO), parent_node,
			 &proc_entry->ops, proc_entry);
}
EXPORT_SYMBOL(dp_proc_entry_create);
