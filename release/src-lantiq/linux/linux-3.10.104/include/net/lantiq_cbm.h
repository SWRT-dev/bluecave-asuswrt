#ifndef LANTIQ_CBM_H
#define LANTIQ_CBM_H

#define CBM_STD_BUF_SIZE 2048


struct cbm_desc{
    uint32_t desc0;
    uint32_t desc1;
    uint32_t desc2;
    uint32_t desc3;
};

extern void *cbm_buffer_alloc(uint32_t, uint32_t);
extern int cbm_buffer_free(uint32_t , uint32_t,  uint32_t );
extern int check_ptr_validation(uint32_t buf);
//extern int cbm_cpu_pkt_tx (struct sk_buff *skb, uint32_t egress_port, uint32_t flags);
extern int cbm_cpu_pkt_tx_raw (unsigned char *data, uint32_t egress_port, uint32_t flags, int len);

extern void cbm_enable_port (int pid);
extern void cbm_disable_port (int pid);
extern void reset_enq_counter(uint32_t index);
extern void reset_deq_counter(uint32_t index);
extern uint8_t get_lookup_qid_via_index(uint32_t index);
extern int cbm_cpu_dequeue(uint32_t pid, struct cbm_desc *desc, int *quota, uint32_t *buf_per_port);
extern void set_lookup_qid_via_index(uint32_t index, uint32_t qid);
extern uint32_t get_enq_counter(uint32_t index);
extern uint32_t get_deq_counter(uint32_t index);
extern void reset_enq_counter(uint32_t index);
extern void reset_deq_counter(uint32_t index);
extern void reset_enq_counter_all(void);
extern void reset_deq_counter_all(void);
extern void set_lookup_qid_via_index(uint32_t index, uint32_t qid);
extern uint8_t get_lookup_qid_via_index(uint32_t index);
extern uint8_t get_lookup_qid_via_bits(uint32_t ep, uint32_t classid, uint32_t mpe1, uint32_t mpe2, uint32_t enc, uint32_t dec, uint8_t flow_id, uint32_t dic);
extern void setup_DMA_p2p(void);
extern int turn_on_DMA_p2p (void);
extern int cbm_proc_install(struct proc_dir_entry *parent);
extern int32_t cbm_get_egress_port_info(uint32_t cbm_port,uint32_t *tx_ch,uint32_t *flags );
#endif
