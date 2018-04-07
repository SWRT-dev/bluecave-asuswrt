
extern int vrx318_atm_proc_init(void *);
extern int vrx318_ptm_proc_init(void *);
extern void vrx318_atm_proc_exit(void *);
extern void vrx318_ptm_proc_exit(void *);
extern int vrx318_proc_init(struct vrx318_priv *);
extern void vrx318_proc_exit(struct vrx318_priv *);
extern int proc_read_ptm_wanmib(struct seq_file *, void *);
extern ssize_t proc_write_ptm_wanmib(struct file *, const char __user *,
		size_t , loff_t *);
extern int proc_read_atm_wanmib(struct seq_file *, void *);
extern ssize_t proc_write_atm_wanmib(struct file *, const char __user *,
		size_t, loff_t *);

#define set_vrx_dbg_flag(v, e, f) do {	\
	if (e > 0)			\
		v |= (uint32_t)(f);	\
	else				\
		v &= (uint32_t) (~f); }	\
	while (0)

