/* udp_redirect_symb.c: Hook to receive packets directly from the network stack.
 *
 * Copyright (C) 2011  Lantiq Deutschland GmbH (www.lantiq.com)
 */

#if defined(CONFIG_LTQ_UDP_REDIRECT) || defined(CONFIG_LTQ_UDP_REDIRECT_MODULE)
/* ============================= */
/* Includes                      */
/* ============================= */
#include <linux/module.h>
#include <linux/udp_redirect.h>


/* ============================= */
/* Global variable definition    */
/* ============================= */
int (*udp_do_redirect_fn)(struct sock *sk, struct sk_buff *skb) = NULL;


/* ============================= */
/* Global function definition    */
/* ============================= */

EXPORT_SYMBOL(udp_do_redirect_fn);

#endif /* CONFIG_LTQ_UDP_REDIRECT* */
