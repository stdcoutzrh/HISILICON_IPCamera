/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_sync_task.c
  Version       : Initial Draft
  Author        : c00298856
  Created       : 2015/7/11
  Last Modified :
  Description   : isp sync task
  Function List :
  History       :
  1.Date        : 2015/7/11
    Author      : c00298856
    Modification: Created file

******************************************************************************/

#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <asm/errno.h>
#include <linux/kallsyms.h>
#include <linux/workqueue.h>
#include <linux/list.h>
#include "isp.h"
#include "isp_ext.h"
#include "sys_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


/* isp sync task */
#define  ISP_SYNC_TSK_MAX_NODES  16

#define ISP_SYNC_TSK_GET_CTX(dev) (&g_astIspSyncTskCtx[dev])

typedef void (*ISP_FUNC_PTR)(unsigned long data);

typedef enum hiISP_SYNC_TSK_TRIG_POS_E
{
    TRIG_POS_FIRST_PIXEL = 1 << 0,
    TRIG_POS_LAST_PIXEL  = 1 << 1,
    TRIG_POS_VSYNC_SIGNAL = 1 << 2,
    
    TRIG_POS_BUTT
} ISP_SYNC_TSK_TRIG_POS_E;
 
typedef struct hiLIST_ENTRY_S
{
    HI_U32 u32Num;
    struct list_head head;
    
} LIST_ENTRY_S;
 
 typedef struct hiISP_SYNC_TSK_CTX_S 
 {
    struct work_struct worker;
    struct tasklet_struct tsklet;   
    // list head
    LIST_ENTRY_S hwirq_list;
    LIST_ENTRY_S tsklet_list;
    LIST_ENTRY_S workqueue_list;
    
    struct semaphore sem;
    
 } ISP_SYNC_TSK_CTX_S; 

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/
 

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/
ISP_SYNC_TSK_CTX_S g_astIspSyncTskCtx[1];
/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

/*****************************************************************************
 Prototype    : ispSyncTskFIndAndExecute
 Description  : find a task and exe it
 Input        : struct list_head *head  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2015/7/14
    Author       : c00298856
    Modification : Created function

*****************************************************************************/

HI_S32 ispSyncTskFIndAndExecute(struct list_head *head)
{
    struct list_head *pos, *next;
    ISP_SYNC_TASK_NODE_S *pstSyncTskNode;

    if (!list_empty(head))
    {
        list_for_each_safe(pos, next, head)
        {
            pstSyncTskNode = list_entry(pos, ISP_SYNC_TASK_NODE_S, list);

            if (pstSyncTskNode->pfnIspSyncTskCallBack)
            {
                pstSyncTskNode->pfnIspSyncTskCallBack(pstSyncTskNode->u64Data);
            }

        }     
    }   

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : taskLetHandler
 Description  : a tasklet handler
 Input        : unsigned long data  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2015/7/14
    Author       : c00298856
    Modification : Created function

*****************************************************************************/
static void taskLetHandler(unsigned long data)
{
    ISP_SYNC_TSK_CTX_S *pstSyncTsk = (ISP_SYNC_TSK_CTX_S *)data;
    
    ispSyncTskFIndAndExecute(&pstSyncTsk->tsklet_list.head);
    
    return;
}

/*****************************************************************************
 Prototype    : workQueueHandler
 Description  : a workqueue handler
 Input        : struct work_struct *pstWorker  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2015/7/14
    Author       : c00298856
    Modification : Created function

*****************************************************************************/
static void workQueueHandler(struct work_struct *pstWorker)
{

   ISP_SYNC_TSK_CTX_S *pstSyncTsk = container_of(pstWorker, ISP_SYNC_TSK_CTX_S, worker);

    if (down_interruptible(&pstSyncTsk->sem))
    {
       return ;
    }

    ispSyncTskFIndAndExecute(&pstSyncTsk->workqueue_list.head);  

    up(&pstSyncTsk->sem);

    return ;
}

/*****************************************************************************
 Prototype    : IspSyncTaskProcess
 Description  : isp sync task process
 Input        : ISP_DEV dev  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2015/7/14
    Author       : c00298856
    Modification : Created function

*****************************************************************************/
HI_S32 IspSyncTaskProcess(ISP_DEV dev)
{
    ISP_SYNC_TSK_CTX_S *pstSyncTsk = ISP_SYNC_TSK_GET_CTX(dev);
    
    if (pstSyncTsk->hwirq_list.u32Num)
    {
        ispSyncTskFIndAndExecute(&pstSyncTsk->hwirq_list.head);
    }

    if (pstSyncTsk->tsklet_list.u32Num)
    {
        tasklet_schedule(&pstSyncTsk->tsklet);
    }

    if (pstSyncTsk->workqueue_list.u32Num)
    {
        schedule_work(&pstSyncTsk->worker);
    }

    return HI_SUCCESS;
    
}

/*****************************************************************************
 Prototype    : searchNode
 Description  : seach a node by id 
 Input        : struct list_head *head  
                const char *id          
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2015/7/14
    Author       : c00298856
    Modification : Created function

*****************************************************************************/
static struct list_head *searchNode(struct list_head *head, const char *id)
{
    struct list_head *pos, *next;
    ISP_SYNC_TASK_NODE_S *pstSyncTskNode;
    
    list_for_each_safe(pos, next, head) 
    {
        pstSyncTskNode = list_entry(pos, ISP_SYNC_TASK_NODE_S, list);    
        if (!strcmp(pstSyncTskNode->pszId, id))
        {
            return pos;
        }
    }  

    return HI_NULL;
}

/*****************************************************************************
 Prototype    : hi_isp_sync_task_register
 Description  : register a task, export to other module
 Input        : ISP_DEV dev                       
                ISP_SYNC_TASK_NODE_S *pstNewNode  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2015/7/14
    Author       : c00298856
    Modification : Created function

*****************************************************************************/
HI_S32 hi_isp_sync_task_register(ISP_DEV dev, ISP_SYNC_TASK_NODE_S *pstNewNode)
{
    ISP_SYNC_TSK_CTX_S *pstSyncTsk = ISP_SYNC_TSK_GET_CTX(dev);
    struct list_head *pstTargetList, *pos;
    LIST_ENTRY_S *pstListEntry;

    ISP_CHECK_POINTER(pstNewNode);
    
    if (ISP_SYNC_TSK_METHOD_HW_IRQ == pstNewNode->enMethod)
    {
        pstTargetList = &pstSyncTsk->hwirq_list.head;
    }
    else if ( ISP_SYNC_TSK_METHOD_TSKLET == pstNewNode->enMethod )
    {
        pstTargetList = &pstSyncTsk->tsklet_list.head;
    }
    else
    {
        pstTargetList = &pstSyncTsk->workqueue_list.head;
    }

    pstListEntry = list_entry(pstTargetList, LIST_ENTRY_S, head);

    pos = searchNode(pstTargetList, pstNewNode->pszId);
    if (pos)
    {
        return HI_FAILURE;
    }    
    
    if (down_interruptible(&pstSyncTsk->sem))
    {
       return -ERESTARTSYS;
    }
    
    list_add_tail(&pstNewNode->list, pstTargetList);
    pstListEntry->u32Num++;
    
    up(&pstSyncTsk->sem);

    return HI_SUCCESS; 
    
}

EXPORT_SYMBOL(hi_isp_sync_task_register);

/*****************************************************************************
 Prototype    : hi_isp_sync_task_unregister
 Description  : unregister a task export to other module 
 Input        : ISP_DEV dev                       
                ISP_SYNC_TASK_NODE_S *pstDelNode  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2015/7/14
    Author       : c00298856
    Modification : Created function

*****************************************************************************/
HI_S32 hi_isp_sync_task_unregister(ISP_DEV dev, ISP_SYNC_TASK_NODE_S *pstDelNode)
{
    ISP_SYNC_TSK_CTX_S *pstSyncTsk = ISP_SYNC_TSK_GET_CTX(dev);
    struct list_head *pstTargetList;
    LIST_ENTRY_S *pstListEntry;
    struct list_head *pos;
    HI_S32 bDelSuccess = HI_FAILURE;


    ISP_CHECK_POINTER(pstDelNode);

    if (ISP_SYNC_TSK_METHOD_HW_IRQ == pstDelNode->enMethod)
    {
        pstTargetList = &pstSyncTsk->hwirq_list.head;
    }
    else if ( ISP_SYNC_TSK_METHOD_TSKLET == pstDelNode->enMethod )
    {
        pstTargetList = &pstSyncTsk->tsklet_list.head;
    }
    else
    {
        pstTargetList = &pstSyncTsk->workqueue_list.head;
    }
    pstListEntry = list_entry(pstTargetList, LIST_ENTRY_S, head);  
    
    if (down_interruptible(&pstSyncTsk->sem))
    {
       return -ERESTARTSYS;
    }

    pos = searchNode(pstTargetList, pstDelNode->pszId);
 
    if (pos)
    {
        list_del(pos);
        pstListEntry->u32Num--;
        bDelSuccess = HI_SUCCESS;
    }
    
    up(&pstSyncTsk->sem);

    return bDelSuccess; 
    
}

EXPORT_SYMBOL(hi_isp_sync_task_unregister);


/*****************************************************************************
 Prototype    : SyncTaskInit
 Description  : isp sync task init
 Input        : ISP_DEV dev  
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2015/7/14
    Author       : c00298856
    Modification : Created function

*****************************************************************************/
void SyncTaskInit(ISP_DEV dev)
{
    ISP_SYNC_TSK_CTX_S *pstSyncTsk = ISP_SYNC_TSK_GET_CTX(dev);

    INIT_LIST_HEAD(&pstSyncTsk->hwirq_list.head);
    INIT_LIST_HEAD(&pstSyncTsk->tsklet_list.head);
    INIT_LIST_HEAD(&pstSyncTsk->workqueue_list.head);
    
    pstSyncTsk->hwirq_list.u32Num = 0;
    pstSyncTsk->tsklet_list.u32Num = 0;
    pstSyncTsk->workqueue_list.u32Num = 0;
    sema_init(&pstSyncTsk->sem, 1);

    tasklet_init(&pstSyncTsk->tsklet, taskLetHandler, (unsigned long)pstSyncTsk);
    INIT_WORK(&pstSyncTsk->worker, workQueueHandler);

    return;      
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

