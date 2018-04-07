/******************************************************************************
**
** FILE NAME	: ltq_rcu_test.c
** MODULES		: RCU (Reset Control Unit) Test Kernel Module
**
** DESCRIPTION	: RCU driver test application source file
** COPYRIGHT	:		Copyright (c) 2006
**						Infineon Technologies AG
**	Am Campeon 1-12, 85579 Neubiberg, Germany
**
**	  This program is free software; you can redistribute it and/or modify
**	  it under the terms of the GNU General Public License as published by
**	  the Free Software Foundation; either version 2 of the License, or
**	  (at your option) any later version.
**
*****************************************************************************/

/*
 * ####################################
 *				Head File
 * ####################################
 */

/*
 *	Common Head File
 */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>

/*
 *	Chip Specific Head File
 */
#include <lantiq_soc.h>
#include "ltq_rcu.h"



/*
 * ####################################
 *				Definition
 * ####################################
 */
#define LTQ_SUCCESS 0


/*
 * ####################################
 *			   Declaration
 * ####################################
 */

/*
 *	Proc File Functions
 */
static inline void proc_file_create(void);
static inline void proc_file_delete(void);

/*
 *	Proc Help Functions
 */
static int proc_write_test(struct file *, const char *, unsigned long, void *);
static int get_token(char **, char **, int *, int *);
static int stricmp(const char *, const char *);
static int strincmp(const char *, const char *, int);
static int rcu_test_callbackfn(unsigned int, unsigned int, int, unsigned long);



/*
 * ####################################
 *			  Global Variable
 * ####################################
 */

static struct proc_dir_entry *g_rcu_test_dir;

static LTQ_RCU_DECLARE_DOMAIN_NAME(g_domain_name);
static LTQ_RCU_DECLARE_MODULE_NAME(g_module_name);


/*
 * ####################################
 *			  Local Function
 * ####################################
 */

static inline void proc_file_create(void)
{
	struct proc_dir_entry *res;

	g_rcu_test_dir = proc_mkdir("driver/ltq_rcu_test", NULL);

	res = create_proc_entry("test",
							0,
							g_rcu_test_dir);
	if (res != NULL)
		res->write_proc = proc_write_test;
}

static inline void proc_file_delete(void)
{
	remove_proc_entry("test", g_rcu_test_dir);

	remove_proc_entry("driver/ltq_rcu_test", NULL);
}

static int proc_write_test(struct file *file, const char *buf,
	unsigned long count, void *data)
{
	static char *cmd_str[] = {
		"request",
		"free",
		"reset",
		"domain",
		"module",
		"stat"
	};

	char *p1, *p2;
	int len;
	int colon;
	char local_buf[1024];
	int cmd;
	int domain;
	int module;
	int name_len;
	int ret = -1;

	len = sizeof(local_buf) < count ? sizeof(local_buf) : count;
	len = len - copy_from_user(local_buf, buf, len);
	local_buf[len] = 0;

	p1 = local_buf;
	/* p2 = NULL; */
	colon = 1;
	cmd = -1;
	while (get_token(&p1, &p2, &len, &colon)) {
		for (cmd = 0; cmd < NUM_ENTITY(cmd_str); cmd++)
			if (stricmp(p1, cmd_str[cmd]) == 0)
				goto PROC_WRITE_TEST_CMD_FOUND;

		p1 = p2;
		colon = 1;
	}

PROC_WRITE_TEST_CMD_FOUND:
	if (cmd < 0 || cmd >= 3) {
		if (cmd < 0 || cmd >= 6) {
			printk("usage:\n");
			printk("1. access ltq_rcu driver\n");
			printk("   echo <request/free/reset> <domain name> <module name> > /proc/driver/ltq_rcu_test/test\n");
			printk("2. list domain name\n");
			printk("   echo domain > /proc/driver/ltq_rcu_test/test\n");
			printk("3. list module name\n");
			printk("   echo module > /proc/driver/ltq_rcu_test/test\n");
			printk("4. list module stat\n");
			printk("   echo stat > /proc/driver/ltq_rcu_test/test\n");
		} else if (cmd == 3) {
			printk("domain name:\n");
			for (domain = 0;
				domain < NUM_ENTITY(g_domain_name);
				domain++)
				printk("%2d. %s\n",
					domain,
					g_domain_name[domain]);
		} else if (cmd == 4) {
			printk("module name:\n");
			for (module = 0;
				module < NUM_ENTITY(g_module_name);
				module++)
				printk("%2d. %s\n",
					module,
					g_module_name[module]
					);
		} else if (cmd == 5) {
					printk("RCU STAT Value: 0x%x\n",
						ltq_rcu_r32(LTQ_RCU_RST_STAT));
					printk("domain status:\n");
					for (domain = 0;
						domain < NUM_ENTITY(g_domain_name);
						domain++)
						printk("%2d. %s , %x\n",
						domain, g_domain_name[domain],
						ltq_rcu_stat_get(domain)
						);
				}

		return count;
	}

	p2++;
	len--;

	/* printk("p2 = %s\n", p2); */
	for (domain = 0; domain < NUM_ENTITY(g_domain_name); domain++) {
		name_len = strlen(g_domain_name[domain]);
		if (len >= name_len
			&& strincmp(p2, g_domain_name[domain], name_len) == 0) {
			p2 += name_len + 1;
			len -= name_len + 1;
			break;
		}
	}
	if (domain == NUM_ENTITY(g_domain_name)) {
		printk("unknown domain\n");
		return count;
	}

	/* printk("p2 = %s\n", p2); */
	for (module = 0; module < NUM_ENTITY(g_module_name); module++) {
		name_len = strlen(g_module_name[module]);
		if (len >= name_len
			&& strincmp(p2, g_module_name[module], name_len) == 0)
			break;

	}
	if (module == NUM_ENTITY(g_module_name)) {
		printk("unknown module\n");
		return count;
	}

	/* printk("%s %s %s\n",
		cmd_str[cmd],
		g_domain_name[domain],
		g_module_name[module]); */

	switch (cmd) {
	case 0:
		ret = ltq_rcu_request(domain,
				module,
				rcu_test_callbackfn,
				module);
		break;
	case 1:
		ret = ltq_rcu_free(domain, module);
		break;
	case 2:
		ret = ltq_rcu_rst(domain, module);
		break;
	}
	if (ret == LTQ_SUCCESS)
		printk("%s successfully!\n", cmd_str[cmd]);
	else
		printk("%s fail!\n", cmd_str[cmd]);

	return count;
}

static int get_token(char **p1, char **p2, int *len, int *colon)
{
	int tlen = 0;

	while (*len && !((**p1 >= 'A' && **p1 <= 'Z')
		|| (**p1 >= 'a' && **p1 <= 'z'))) {
		(*p1)++;
		(*len)--;
	}
	if (!*len)
		return 0;

	if (*colon) {
		*colon = 0;
		*p2 = *p1;
		while (*len && **p2 > ' ' && **p2 != ',') {
			if (**p2 == ':') {
				*colon = 1;
				break;
			}
			(*p2)++;
			(*len)--;
			tlen++;
		}
		**p2 = 0;
	} else {
		*p2 = *p1;
		while (*len && **p2 > ' ' && **p2 != ',') {
			(*p2)++;
			(*len)--;
			tlen++;
		}
		**p2 = 0;
	}

	return tlen;
}

static int stricmp(const char *p1, const char *p2)
{
	int c1, c2;

	while (*p1 && *p2) {
		c1 = *p1 >= 'A' && *p1 <= 'Z' ? *p1 + 'a' - 'A' : *p1;
		c2 = *p2 >= 'A' && *p2 <= 'Z' ? *p2 + 'a' - 'A' : *p2;
		c1 -= c2;
		if (c1)
			return c1;
		p1++;
		p2++;
	}

	return *p1 - *p2;
}

static int strincmp(const char *p1, const char *p2, int n)
{
	int c1 = 0, c2;

	while (n && *p1 && *p2) {
		c1 = *p1 >= 'A' && *p1 <= 'Z' ? *p1 + 'a' - 'A' : *p1;
		c2 = *p2 >= 'A' && *p2 <= 'Z' ? *p2 + 'a' - 'A' : *p2;
		c1 -= c2;
		if (c1)
			return c1;
		p1++;
		p2++;
		n--;
	}

	return n ? *p1 - *p2 : c1;
}

static int rcu_test_callbackfn(unsigned int reset_domain_id,
	unsigned int module_id,
	int f_after_reset,
	unsigned long arg)
{
	printk("%s callback - domain: %s, module: %s, after reset: %s, arg: %#08lx\n",
		g_module_name[arg],
		g_domain_name[reset_domain_id],
		g_module_name[module_id],
		f_after_reset ? "yes" : "no", arg
		);
	return LTQ_SUCCESS;
}



/*
 * ####################################
 *			 Global Function
 * ####################################
 */



/*
 * ####################################
 *			 Init/Cleanup API
 * ####################################
 */

static int __init ltq_rcu_test_init(void)
{
	proc_file_create();

	return LTQ_SUCCESS;
}

static void __exit ltq_rcu_test_exit(void)
{
	proc_file_delete();
}

module_init(ltq_rcu_test_init);
module_exit(ltq_rcu_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LTQ RCU Driver Test Module");
MODULE_AUTHOR("Jason Deng yuhong");

