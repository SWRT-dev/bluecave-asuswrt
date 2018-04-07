#ifndef DATAPATH_PROC_H
#define DATAPATH_PROC_H

#include <linux/kernel.h>	/*kmalloc */
#include <linux/ctype.h>
#include <linux/proc_fs.h>	/*file_operations */
#include <linux/seq_file.h>	/*seq_file */
#include <linux/uaccess.h>	/*copy_from_user */

#define set_ltq_dbg_flag(v, e, f) do {;\
	if (e > 0)\
		v |= (uint32_t)(f);\
	else\
		v &= (uint32_t) (~f); } \
	while (0)

typedef void (*dp_proc_single_callback_t) (struct seq_file *);
typedef int (*dp_proc_callback_t) (struct seq_file *, int);
typedef int (*dp_proc_init_callback_t) (void);
typedef ssize_t(*dp_proc_write_callback_t) (struct file *file,
					     const char __user *input,
					     size_t size, loff_t *loff);

struct dp_proc_file_entry {
	dp_proc_callback_t multi_callback;
	dp_proc_single_callback_t single_callback;
	int pos;
	int single_call_only;
};

struct dp_proc_entry {
	char *name;
	dp_proc_single_callback_t single_callback;
	dp_proc_callback_t multi_callback;
	dp_proc_init_callback_t init_callback;
	dp_proc_write_callback_t write_callback;
	struct file_operations ops;
};

void dp_proc_entry_create(struct proc_dir_entry *parent_node,
			   struct dp_proc_entry *proc_entry);

int dp_atoi(unsigned char *str);
char *dp_strstri(char *string, char *substring);
int dp_strncmpi(const char *s1, const char *s2, size_t n);
int dp_strcmpi(char const *s1, char const *s2);
void dp_replace_ch(char *p, int len, char orig_ch, char new_ch);
void dp_remove_leading_whitespace(char **p, int *len);

/*Split buffer to multiple segment with seperator space.
And put pointer to array[].
By the way, original buffer will be overwritten with '\0' at some place.
*/
int dp_split_buffer(char *buffer, char *array[], int max_param_num);

#ifdef CONFIG_LTQ_DP_MPE_FASTHOOK_TEST
/*add the macro in order to be back-compatible with old MPE FW HOOK */
#define ltq_proc_entry dp_proc_entry
#define ltq_proc_file_entry dp_proc_file_entry

#define ltq_proc_entry_create dp_proc_entry_create

#define ltq_atoi dp_atoi
#define ltq_strstri dp_strstri
#define ltq_strncmpi dp_strncmpi
#define ltq_strcmpi dp_strcmpi
#define ltq_replace_ch dp_replace_ch
#define ltq_remove_leading_whitespace dp_remove_leading_whitespace
#define ltq_split_buffer dp_split_buffer
#endif /*CONFIG_LTQ_DP_MPE_FASTHOOK_TEST*/

void set_start_end_id(unsigned int new_start, unsigned int new_end,
		      unsigned int max_start, unsigned int max_end,
		      unsigned int default_start, unsigned int default_end,
		      unsigned int *start, unsigned int *end);



#endif				/*DATAPATH_PROC_H */
