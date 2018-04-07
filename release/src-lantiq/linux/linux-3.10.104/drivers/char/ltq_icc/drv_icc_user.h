#ifndef _DRV_ICC_USER_H
#define _DRV_ICC_USER_H
/******************************************************************************

                              Copyright (c) 2012
                            Lantiq Deutschland GmbH
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

****************************************************************************
   Module      : drv_icc_user.h
   Description : This file contains the defines, the structures declarations
                 the tables declarations and the global functions declarations.
*******************************************************************************/
/* ============================= */
/* ICC Common defines            */
/* ============================= */
/*---------------------------------------------------------------------------*/

#define MAX_UPSTRM_DATAWORDS 4
#define ICC_PARAM_PTR 0x1
#define ICC_PARAM_NO_PTR 0x0
#define ICC_PARAM_PTR_IOCU 0x1
#define ICC_PARAM_PTR_NON_IOCU 0x0
#define CHECK_PTR(Attrs,Index) (Attrs&(ICC_PARAM_PTR<<(Index*2)))
#define CHECK_PTR_IOCU(Attrs,Index) (Attrs&(ICC_PARAM_PTR_IOCU<<((Index*2)+1)))
#define SET_PTR(Attrs,Index) (Attrs|(ICC_PARAM_PTR<<(Index*2)))
#define SET_PTR_IOCU(Attrs,Index) (Attrs|(ICC_PARAM_PTR_IOCU<<((Index*2)+1)))
#define CONVERT_KERNEL_TO_PHYADDR(addr) (addr&0x7FFFFFFF)
/*---------------------------------------------------------------------------*/
/* Device connection structure                                               */
/*---------------------------------------------------------------------------*/
typedef struct{
 uint8_t src_client_id;
 uint8_t dst_client_id;
 uint8_t msg_id;
 uint8_t param_attr;
 uint32_t param[MAX_UPSTRM_DATAWORDS];
} icc_msg_t;

typedef struct{
	uint32_t address[MAX_UPSTRM_DATAWORDS];
  uint32_t length[MAX_UPSTRM_DATAWORDS];
  uint32_t offset[MAX_UPSTRM_DATAWORDS];
	uint32_t count;
}icc_commit_t;

/*Always keep your new clients within InvalidClient and MAX_CLIENT*/
typedef enum
{
   ICC_Client,/*Icc client Id*/
	 IA,/*Image authenticator*/
	 SR,/*System reset driver*/
	 IR,/*Ioctl redirector*/
	 PR,/*procfs redirector*/
   MAX_CLIENT=21/*MAX_CLIENT_ID*/
} icc_devices;/*enum for all the possible clients*/
/******************************************************************************
 * Exported functions
 ******************************************************************************/
/** magic number */
#define ICC_MAGIC 'S'
#define ICC_IOC_REG_CLIENT _IOW(ICC_MAGIC,1,uint32_t)
#define ICC_IOC_MEM_COMMIT _IOW(ICC_MAGIC,2,icc_commit_t)
#define ICC_IOC_MEM_INVALIDATE _IOW(ICC_MAGIC,3,icc_commit_t)

#endif /* _DRV_ICC_USER_H */
