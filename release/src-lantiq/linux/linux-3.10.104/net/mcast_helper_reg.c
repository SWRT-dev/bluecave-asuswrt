/******************************************************************************
**
** FILE NAME    : mcast_helper_reg.c
** AUTHOR       : 
** DESCRIPTION  : Multicast Helper Register function
** COPYRIGHT    :      Copyright (c) 2014 2015
**                Lantiq Beteiligungs-GmbH & Co. KG
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date         $Author                $Comment
** 26 AUG 2014                 	      Initial Version
**                                   
*******************************************************************************/



#ifdef CONFIG_LANTIQ_MCAST_HELPER_MODULE

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/netlink.h>
#include <net/netlink.h>
#include <net/net_namespace.h>
#include <linux/string.h>
#include <linux/rtnetlink.h>

#define SUCCESS 0x1
#define FAILURE 0x0
#define LTQ_MC_F_REGISTER 0x01
#define LTQ_MC_F_DEREGISTER 0x02
#define LTQ_MC_F_DIRECTPATH 0x04
#define LTQ_MC_F_FW_RESET 0x10
#define LTQ_MC_F_NEW_STA  0x20
#define MCAST_PROCESS_NAME "mcastd"



typedef void (*Mcast_module_callback_t)(unsigned int grpidx,struct net_device *netdev,void *mc_stream,unsigned int flag);
typedef struct _mcast_callback_t{
	int numCbf;
	struct net_device *netDev; /* Member NetDevice */
	struct module *modName;
	Mcast_module_callback_t callbackfun;
	struct list_head list;	/* mcast_member interface map  List  */
	uint32_t uflag;
} MCAST_CALLBACK_t;

LIST_HEAD(mch_callback_list_g);



static int  mcast_helper_send_netlink_msg_user(struct net_device *netdev ,int type, int queryflag)
{
        struct sk_buff *skb;
        struct nlmsghdr *nlh;
	struct ifinfomsg *ifm;
	struct net *net = dev_net(netdev);

	size_t if_info_size=NLMSG_ALIGN(sizeof(struct ifinfomsg));
        int res;
	struct task_struct *task;
	unsigned int pid;
        printk(KERN_INFO "Creating skb.\n");
	for_each_process(task) {
       		/* compare the  process name with each of the task struct process name*/    

        	//printk(KERN_INFO "task info process name :%s pid:%d \n",task->comm,task->pid);
		if(strstr(task->comm,MCAST_PROCESS_NAME) != NULL){
              		pid = task->pid;
			skb = nlmsg_new(if_info_size , GFP_KERNEL);
			if (!skb) {
				printk(KERN_INFO "Allocation failure.\n");
				return FAILURE;
        		}
			nlh = nlmsg_put(skb, 0, 0, type, sizeof(struct ifinfomsg), 0);
			if (nlh == NULL) {
				kfree_skb(skb);
				return -EMSGSIZE;
			}
			nlh->nlmsg_type = type; 
			ifm = nlmsg_data(nlh);
			ifm->ifi_family = AF_UNSPEC;
			ifm->__ifi_pad = 0;
			ifm->ifi_type = netdev->type;
			ifm->ifi_index = netdev->ifindex;
			if(type == RTM_NEWLINK)
				ifm->ifi_flags = IFF_UP ;
			if(queryflag)
				ifm->ifi_flags = ifm->ifi_flags | IFF_SLAVE ;

			ifm->ifi_change = 0;

        		printk(KERN_INFO "Sending  skb with PID :%d \n",pid);
			res = netlink_unicast(net->rtnl, skb, pid, MSG_DONTWAIT);
			return res;
		}
	}
        return FAILURE;
}
/** mch_add_gimc_record - Add gitxmc entry */
static int mcast_helper_reg_callback(struct net_device *netdev,
			   Mcast_module_callback_t *fun,struct module *modName,
			   struct list_head *head, unsigned int flags)
{
	MCAST_CALLBACK_t *mc_callback_rec = NULL;

	if(netdev == NULL)
                return FAILURE;

        mc_callback_rec = kmalloc(sizeof(MCAST_CALLBACK_t),GFP_KERNEL);
	if (mc_callback_rec == NULL)
		return FAILURE;

	mc_callback_rec->netDev = netdev;
	mc_callback_rec->callbackfun = (void *)fun;
	mc_callback_rec->modName = modName;
	mc_callback_rec->uflag = flags & LTQ_MC_F_DIRECTPATH;
	INIT_LIST_HEAD(&mc_callback_rec->list);
	list_add_tail(&mc_callback_rec->list, head);
	
	if((flags & LTQ_MC_F_FW_RESET) == LTQ_MC_F_FW_RESET) 
		mcast_helper_send_netlink_msg_user(netdev ,RTM_NEWLINK,0);
	return SUCCESS;
}

/** delete_gimc_record - delete gimc entry */
static int mcast_helper_dereg_callback(struct net_device *netdev,
			   Mcast_module_callback_t *fun,struct module *modName,
			   struct list_head *head , unsigned int flags)

{
	struct list_head *liter = NULL;
	struct list_head *gliter = NULL;
	MCAST_CALLBACK_t *mc_callback_rec = NULL;

	if(netdev == NULL)
                return FAILURE;

	list_for_each_safe(liter,gliter,head) {
		mc_callback_rec = list_entry(liter, MCAST_CALLBACK_t,list);
		if(mc_callback_rec != NULL ){
			if(mc_callback_rec->netDev->name !=NULL) {    
		                if (!strncmp (netdev->name, mc_callback_rec->netDev->name, strlen(mc_callback_rec->netDev->name))){ 
        			        if (!strncmp (modName->name, mc_callback_rec->modName->name, strlen(mc_callback_rec->netDev->name))) {
						list_del(&mc_callback_rec->list);
						kfree(mc_callback_rec);
						if((flags & LTQ_MC_F_FW_RESET) == LTQ_MC_F_FW_RESET) 
							mcast_helper_send_netlink_msg_user(netdev ,RTM_DELLINK,0);
	
						return SUCCESS;
					}
				}
			}
		}
	}

	return FAILURE;
}


/** Register callback function **/
void  mcast_helper_register_module (
	struct net_device *dev, /*Registered netdev e.g. wlan0 */
	struct module *modName, /* Kernel Module Name */
	char *addlName, /* Optional Additional Name*/
	Mcast_module_callback_t *fnCB, /* Callback Fn */
	unsigned int flags) /* Flag - LTQ_F_REGISTER or LTQ_F_DEREGISTER*/
{

	if(dev->name != NULL)
	{
		if((flags & LTQ_MC_F_REGISTER) == LTQ_MC_F_REGISTER) {
			mcast_helper_reg_callback(dev,fnCB,modName,&mch_callback_list_g, flags);
		}
		else if((flags & LTQ_MC_F_DEREGISTER) == LTQ_MC_F_DEREGISTER) {
			mcast_helper_dereg_callback(dev,fnCB,modName,&mch_callback_list_g,flags);
		}
		else if((flags & LTQ_MC_F_NEW_STA) == LTQ_MC_F_NEW_STA) {
			mcast_helper_send_netlink_msg_user(dev ,RTM_NEWLINK,1);
		}

	}

}

EXPORT_SYMBOL(mcast_helper_register_module);

int mcast_helper_invoke_callback(unsigned int grpidx,struct net_device *netdev,void *mc_stream,unsigned int flag, unsigned int iface_count)
{
	struct list_head *liter = NULL;
	MCAST_CALLBACK_t *mc_callback_rec = NULL;
	list_for_each(liter,&mch_callback_list_g) {
		mc_callback_rec = list_entry(liter, MCAST_CALLBACK_t,list);
		if((mc_callback_rec->netDev->name != NULL) && (netdev->name != NULL)){
			if (strcmp (netdev->name, mc_callback_rec->netDev->name)== 0)
			{
				if(mc_callback_rec->callbackfun != NULL)
				{
					if((iface_count == 1) && (mc_callback_rec->uflag == LTQ_MC_F_DIRECTPATH)) 
					{
						mc_callback_rec->callbackfun(grpidx,netdev,mc_stream,flag);
					}
					else if (mc_callback_rec->uflag != LTQ_MC_F_DIRECTPATH)
					{
						mc_callback_rec->callbackfun(grpidx,netdev,mc_stream,flag);
					}
				}
			}	
		}
	}
	return SUCCESS;
}

EXPORT_SYMBOL(mcast_helper_invoke_callback);

#endif
