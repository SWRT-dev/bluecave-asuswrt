/******************************************************************************

                         Copyright (c) 2012, 2014, 2015
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _LTQ_ITC_H
#define _LTQ_ITC_H


#define ITC_Block					0x1c300000
#define ERRCTL_ITC					(1 << 26)
#define ITC_BypassView					0x00000000
#define ITC_ControlView                                 0x00000008
#define ITC_EmptyFullSyncView                           0x00000010
#define ITC_EmptyFullTryView                            0x00000018
#define ITC_PVSyncView                                  0x00000020
#define ITC_PVTryView                                   0x00000028
#define ITC_En                                          0x00000001
#define ITC_E                                           0x00000001
#define ITC_NumEntries                                  18
#define ITC_FIFO_Entries				2
#define ITC_SEM_Entries					16 
#define ITC_AddrMask                                    0x3f            // 128K ITC address space
#define ITC_EntryGrain                                  0               // 128 bytes between Entries (Cells)

#define 	DEBUG_ITC

int32_t itc_init(void);
void itc_sem_wait(uint8_t semId);
void itc_sem_post(uint8_t semId);
uint32_t itc_sem_addr(uint8_t semId);
#endif
