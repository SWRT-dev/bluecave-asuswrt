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
 * Copyright 2019, paldier <paldier@hotmail.com>.
 * All Rights Reserved.
 * 
 *
 */

#if defined(RTCONFIG_LANTIQ)
#if !defined(K3C)
//extern void lantiq_init(void);
extern void lantiq_init_done(void);
#endif
#endif
#ifdef RTCONFIG_UUPLUGIN
extern void exec_uu_merlinr(void);
#endif
#ifdef RTCONFIG_FRS_LIVE_UPDATE
#if defined(RTCONFIG_BCMARM) || defined(RTCONFIG_LANTIQ) || defined(RTCONFIG_QCA) || defined(RTCONFIG_HND_ROUTER)
extern int merlinr_firmware_check_update_main(int argc, char *argv[]);
#endif
#endif

