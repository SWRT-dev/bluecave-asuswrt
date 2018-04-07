/*******************************************************************************

  Intel SmartPHY DSL PCIe TC driver
  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

*******************************************************************************/
#ifndef __TC_PROC_H__
#define __TC_PROC_H__

#define TC_PROC_DIR "driver/vrx518"
#define TC_PROC_ATM_DIR "atm"
#define TC_PROC_PTM_DIR "ptm"
enum {
	PPE_PROC_READ = 1,
	PPE_PROC_WRITE
};

#define set_vrx_dbg_flag(v, e, f) do {	\
	if (e > 0)			\
		v |= (uint32_t)(f);	\
	else				\
		v &= (uint32_t) (~f); }	\
	while (0)

struct tc_proc_list {
	char proc_name[32];
	umode_t mode;
	const struct file_operations *fops;
	int is_folder;
};

#define CW_PAGE_SIZE 17
struct atm_cw_ctrl_t {
	unsigned int	state:1;
	unsigned int	bad:1;
	unsigned int	ber:9;
	unsigned int	drop:1;
	unsigned int	idle:1;
	unsigned int	csp:9;
	unsigned int	bsm:1;
	unsigned int	cvc:8;
	unsigned int	cvm:1;
};

struct ptm_cw_ctrl_t {
	unsigned int	state:1;
	unsigned int	bad:1;
	unsigned int	ber:9;
	unsigned int	spos:7;
	unsigned int	ffbn:7;
	unsigned int	shrt:1;
	unsigned int	preempt:1;
	unsigned int	cwer:2;
	unsigned int	cwid:3;
};

extern int tc_proc_init(struct tc_priv *);
extern void tc_proc_exit(struct tc_priv *);
extern int ptm_tc_proc_init(void *);
extern void ptm_tc_proc_exit(void *);
extern int atm_tc_proc_init(void *);
extern int atm_tc_proc_exit(void *);
extern int vrx_split_buffer(char *, char **, int);
extern int vrx_strcmpi(char const *, char const *);

#endif /*__TC_PROC_H__ */
