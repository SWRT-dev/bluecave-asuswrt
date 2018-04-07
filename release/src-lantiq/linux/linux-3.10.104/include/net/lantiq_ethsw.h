
#ifdef CONFIG_NETWORK_EXTMARK
#define LAN_PORT_SEP_START_BIT_POS 25
#define LAN_PORT_SEP_NO_OF_BITS 7
#define LAN_PORT_SEP_MASK 0xfe000000
#else
#define LAN_PORT_SEP_SHIFT 29 /* Currently we use upper 3 bits of skb mark to identify switch port */
#endif
