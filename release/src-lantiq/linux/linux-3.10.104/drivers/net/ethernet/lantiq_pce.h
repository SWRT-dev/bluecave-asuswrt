/*
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License version 2 as published
 *   by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Copyright (C) 2010 Lantiq Deutschland GmbH
 *   Copyright (C) 2012 John Crispin <blogic@openwrt.org>
 *
 *   PCE microcode extracted from UGW5.2 switch api
 */

/* Switch API Micro Code V0.3 */
enum {
	OUT_MAC0 = 0,
	OUT_MAC1,
	OUT_MAC2,
	OUT_MAC3,
	OUT_MAC4,
	OUT_MAC5,
	OUT_ETHTYP,
	OUT_VTAG0,
	OUT_VTAG1,
	OUT_ITAG0,
	OUT_ITAG1,	/*10 */
	OUT_ITAG2,
	OUT_ITAG3,
	OUT_IP0,
	OUT_IP1,
	OUT_IP2,
	OUT_IP3,
	OUT_SIP0,
	OUT_SIP1,
	OUT_SIP2,
	OUT_SIP3,	/*20*/
	OUT_SIP4,
	OUT_SIP5,
	OUT_SIP6,
	OUT_SIP7,
	OUT_DIP0,
	OUT_DIP1,
	OUT_DIP2,
	OUT_DIP3,
	OUT_DIP4,
	OUT_DIP5,	/*30*/
	OUT_DIP6,
	OUT_DIP7,
	OUT_SESID,
	OUT_PROT,
	OUT_APP0,
	OUT_APP1,
	OUT_IGMP0,
	OUT_IGMP1,
	OUT_IPOFF,	/*39*/
	OUT_NONE =  63
};

/* parser's microcode length type */
#define INSTR		0
#define IPV6		1
#define LENACCU		2

/* parser's microcode flag type */
enum {
	FLAG_ITAG =  0,
	FLAG_VLAN,
	FLAG_SNAP,
	FLAG_PPPOE,
	FLAG_IPV6,
	FLAG_IPV6FL,
	FLAG_IPV4,
	FLAG_IGMP,
	FLAG_TU,
	FLAG_HOP,
	FLAG_NN1,	/*10 */
	FLAG_NN2,
	FLAG_END,
	FLAG_NO,	/*13*/
};

/* Micro code version V2_11 (extension for parsing IPv6 in PPPoE) */
#define MC_ENTRY(val, msk, ns, out, len, type, flags, ipv4_len) \
	{ {val, msk, (ns<<10 | out<<4 | len>>1), (len&1)<<15 | type<<13 | flags<<9 | ipv4_len<<8 }}
struct pce_microcode {
	unsigned short val[4];
/*	unsigned short val_2;
	unsigned short val_1;
	unsigned short val_0;*/
} pce_microcode[] = {
	/*      value    mask    ns  fields      L  type     flags       ipv4_len */
	MC_ENTRY(0x88c3, 0xFFFF,  1, OUT_ITAG0,  4, INSTR,   FLAG_ITAG,  0),
	MC_ENTRY(0x8100, 0xFFFF,  2, OUT_VTAG0,  2, INSTR,   FLAG_VLAN,  0),
	MC_ENTRY(0x88A8, 0xFFFF,  1, OUT_VTAG0,  2, INSTR,   FLAG_VLAN,  0),
	MC_ENTRY(0x8100, 0xFFFF,  1, OUT_VTAG0,  2, INSTR,   FLAG_VLAN,  0),
	MC_ENTRY(0x8864, 0xFFFF, 17, OUT_ETHTYP, 1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0800, 0xFFFF, 21, OUT_ETHTYP, 1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x86DD, 0xFFFF, 22, OUT_ETHTYP, 1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x8863, 0xFFFF, 16, OUT_ETHTYP, 1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0xF800, 10, OUT_NONE,   0, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0x0000, 38, OUT_ETHTYP, 1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0600, 0x0600, 38, OUT_ETHTYP, 1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0x0000, 12, OUT_NONE,   1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0xAAAA, 0xFFFF, 14, OUT_NONE,   1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0300, 0xFF00, 39, OUT_NONE,   0, INSTR,   FLAG_SNAP,  0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_DIP7,   3, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0x0000, 18, OUT_DIP7,   3, INSTR,   FLAG_PPPOE, 0),
	MC_ENTRY(0x0021, 0xFFFF, 21, OUT_NONE,   1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0057, 0xFFFF, 22, OUT_NONE,   1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x4000, 0xF000, 24, OUT_IP0,    4, INSTR,   FLAG_IPV4,  1),
	MC_ENTRY(0x6000, 0xF000, 27, OUT_IP0,    3, INSTR,   FLAG_IPV6,  0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0x0000, 25, OUT_IP3,    2, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0x0000, 26, OUT_SIP0,   4, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0x0000, 38, OUT_NONE,   0, LENACCU, FLAG_NO,    0),
	MC_ENTRY(0x1100, 0xFF00, 37, OUT_PROT,   1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0600, 0xFF00, 37, OUT_PROT,   1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0xFF00, 33, OUT_IP3,   17, INSTR,   FLAG_HOP,   0),
	MC_ENTRY(0x2B00, 0xFF00, 33, OUT_IP3,   17, INSTR,   FLAG_NN1,   0),
	MC_ENTRY(0x3C00, 0xFF00, 33, OUT_IP3,   17, INSTR,   FLAG_NN2,   0),
	MC_ENTRY(0x0000, 0x0000, 37, OUT_PROT,   1, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0xFF00, 33, OUT_NONE,   0, IPV6,    FLAG_HOP,   0),
	MC_ENTRY(0x2B00, 0xFF00, 33, OUT_NONE,   0, IPV6,    FLAG_NN1,   0),
	MC_ENTRY(0x3C00, 0xFF00, 33, OUT_NONE,   0, IPV6,    FLAG_NN2,   0),
	MC_ENTRY(0x0000, 0x0000, 38, OUT_PROT,   1, IPV6,    FLAG_NO,    0),
	MC_ENTRY(0x0000, 0x0000, 38, OUT_SIP0,  16, INSTR,   FLAG_NO,    0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_APP0,   4, INSTR,   FLAG_IGMP,  0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
	MC_ENTRY(0x0000, 0x0000, 39, OUT_NONE,   0, INSTR,   FLAG_END,   0),
};
