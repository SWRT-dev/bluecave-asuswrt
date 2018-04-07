/******************************************************************************
**
** FILE NAME    : nf_contrack_ipsec.h
**
** DATE         : 22 Nov 2010
** AUTHOR       : Manamohan Shetty 
** DESCRIPTION  : IPSec IKE extension for connection tracking.
** COPYRIGHT    :       Copyright (c) 2009
**                      Lantiq Deutschland GmbH
**                      Am Campeon 3, 85579 Neubiberg, Germany
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date        $Author         	$Comment
** 22 Nov,2010  Manamohan Shetty    	First release
*******************************************************************************/



#ifndef _NF_CONNTRACK_IPSEC_IKEY_H
#define _NF_CONNTRACK_IPSEC_IKEY_H
/* IPSEC IKE tracking. */

/* This enum is exposed to userspace */

#ifdef __KERNEL__


/* This contains the information of Internet Security Association and Key Management Protocol */
struct ike_secassoci_hdr
{
	uint64_t init_cookie;
	uint64_t resp_cookie;
};

#define	 VENDORID	13

#define IKE_VENDORID_OFFSET	28
#define IKE_SAP_LENGTH_OFFSET	29

#define UNKNOWN_VENDORID_BNEC 	0x424E454300000004
#define UNKNOWN_VENDORID_NATSI 	0x4E61542D5349

#define	IPSEC_IKEY_PORT			500
#define	IPSEC_IKEY_DATA_PORT_SHASTA	10001
#define	IPSEC_IKEY_DATA_PORT		4500



#endif /* __KERNEL__ */

#endif /* _NF_CONNTRACK_IPSEC_IKE_H */
