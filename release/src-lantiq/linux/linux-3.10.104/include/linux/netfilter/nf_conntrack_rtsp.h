/*
 * RTSP extension for IP connection tracking.
 * (C) 2003 by Tom Marshall <tmarshall@real.com>
 * based on ip_conntrack_irc.h
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */
#ifndef _IP_CONNTRACK_RTSP_H
#define _IP_CONNTRACK_RTSP_H

/* #define IP_NF_RTSP_DEBUG */
#define IP_NF_RTSP_VERSION "0.6.21"


/* port block types */
typedef enum {
    pb_single,  /* client_port=x */
    pb_range,   /* client_port=x-y */
    pb_discon   /* client_port=x/y (rtspbis) */
} portblock_t;


typedef enum
{
    /* RTP used by other players */
    IP_CT_RTSP_RTP,

    /* RDT used by Realplayer */
    IP_CT_RTSP_RDT
}nf_ct_rtsp_protocol_type;

/* We record seq number and length of rtsp headers here, all in host order. */

/*
 * This structure is per expected connection.  It is a member of struct
 * ip_conntrack_expect.  The TCP SEQ for the conntrack expect is stored
 * there and we are expected to only store the length of the data which
 * needs replaced.  If a packet contains multiple RTSP messages, we create
 * one expected connection per message.
 *
 * We use these variables to mark the entire header block.  This may seem
 * like overkill, but the nature of RTSP requires it.  A header may appear
 * multiple times in a message.  We must treat two Transport headers the
 * same as one Transport header with two entries.
 */
struct nf_ct_rtsp_expect
{
    u_int32_t   len;        /* length of header block */
    portblock_t pbtype;     /* Type of port block that was requested */
    u_int16_t   loport;     /* Port that was requested, low or first */
    u_int16_t   hiport;     /* Port that was requested, high or second */

    nf_ct_rtsp_protocol_type rtsp_protocol;

   /* RTCP from client to server or server to client */
    enum ip_conntrack_dir rtcp_dir;

    /* Store the translated ports */
    u_int16_t nat_rtp_port;  /* Translated loport for RTP */
    u_int16_t nat_rtcp_port; /* Translated hiport for RTCP if applicable */
    
    u_int32_t reply_source; /* Store source information from the REPLY message*/
};

/* This structure exists only once per master */
struct nf_ct_rtsp_master
{
    /* Empty (?) */
};




extern unsigned int (*nf_nat_rtsp_hook)(struct sk_buff **pskb,
                                 enum ip_conntrack_info ctinfo,
				 unsigned int matchoff, unsigned int matchlen,
                                 struct nf_ct_rtsp_expect *expinfo,
                                 struct nf_conntrack_expect *exp, unsigned int protoff);


extern void (*nf_nat_rtsp_hook_expectfn)(struct nf_conn *ct, struct nf_conntrack_expect *exp);

#ifdef __KERNEL__

#define RTSP_PORT   554

/* Protects rtsp part of conntracks */
extern spinlock_t ip_rtsp_lock;

/*
 *      Display an IP address in readable format.
 */

#define NIPQUAD(addr) \
        ((unsigned char *)&addr)[0], \
        ((unsigned char *)&addr)[1], \
        ((unsigned char *)&addr)[2], \
        ((unsigned char *)&addr)[3]

#endif /* __KERNEL__ */

#endif /* _IP_CONNTRACK_RTSP_H */

