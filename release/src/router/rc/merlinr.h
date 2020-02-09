/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Copyright 2019-2020, paldier <paldier@hotmail.com>.
 * All Rights Reserved.
 * 
 *
 */

#ifndef __MERLINR_H__
#define __MERLINR_H__
extern void merlinr_init(void);
extern void merlinr_init_done(void);
#ifdef RTCONFIG_UUPLUGIN
extern void exec_uu(void);
#endif
#ifdef RTCONFIG_FRS_LIVE_UPDATE
extern int merlinr_firmware_check_update_main(int argc, char *argv[]);
#endif
#if defined(RTCONFIG_SOFTCENTER)
enum {
	SOFTCENTER_WAN=1,
	SOFTCENTER_NAT,
	SOFTCENTER_MOUNT
};
extern void softcenter_eval(int sig);
#endif
#endif
