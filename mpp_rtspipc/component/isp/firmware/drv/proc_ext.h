/******************************************************************************
 
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 
 ******************************************************************************
  File Name     : proc_ext.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/12/09
  Description   : 
  History       :
  1.Date        : 2006/12/09
    Author      : c42025
    Modification: Created file

  2.Date        : 2007/12/08
    Author      : c42025
    Modification: add a name for "group" 

  3.Date        : 2008/03/03
    Author      : c42025
    Modification: modified macros about PRO_ENTRY_XXX. 
    add support write fuction.

  4.Date        : 2008/06/18
    Author      : c42025
    Modification: add macros about PRO_ENTRY_PCIV

  5.Date        : 2009/08/13
    Author      : y45339
    Modification: add macros about PROC_ENTRY_MST_LOG

******************************************************************************/

#ifndef __PROC_EXT_H__
#define __PROC_EXT_H__

#include <linux/seq_file.h>
#include "hi_type.h"
#include "hi_common.h"

/************MPP LICENSE DEFINITION**********************/
#define MPP_AUTHOR          "HiMPP GRP"
#define PRI_LICENSE         "Proprietary"
#define GPL_LICENSE         "GPL"

#ifdef HI_DEBUG
#define MPP_LICENSE         GPL_LICENSE
#else
#define MPP_LICENSE         PRI_LICENSE
#endif
/************MPP LICENSE DEFINITION END*****************/

#define PROC_ENTRY_VIU    MPP_MOD_VIU  
#define PROC_ENTRY_VOU 	  MPP_MOD_VOU 
#define PROC_ENTRY_VGS 	  MPP_MOD_VGS 
#define PROC_ENTRY_VENC	  MPP_MOD_VENC
#define PROC_ENTRY_VDEC	  MPP_MOD_VDEC
#define PROC_ENTRY_VDA    MPP_MOD_VDA  
#define PROC_ENTRY_H264E  MPP_MOD_H264E 
#define PROC_ENTRY_H265E  MPP_MOD_H265E 
#define PROC_ENTRY_H264D  MPP_MOD_H264D 
#define PROC_ENTRY_JPEGE  MPP_MOD_JPEGE 
#define PROC_ENTRY_JPEGD  MPP_MOD_JPEGD 
#define PROC_ENTRY_MPEG4E MPP_MOD_MPEG4E
#define PROC_ENTRY_CHNL   MPP_MOD_CHNL 
#define PROC_ENTRY_AI     MPP_MOD_AI  
#define PROC_ENTRY_AO     MPP_MOD_AO 
#define PROC_ENTRY_AENC   MPP_MOD_AENC  
#define PROC_ENTRY_ADEC   MPP_MOD_ADEC
#define PROC_ENTRY_ACODEC MPP_MOD_ACODEC
#define PROC_ENTRY_VB     MPP_MOD_VB  
#define PROC_ENTRY_GRP    MPP_MOD_GRP 
#define PROC_ENTRY_VPSS   MPP_MOD_VPSS
#define PROC_ENTRY_RGN    MPP_MOD_RGN 
#define PROC_ENTRY_SYS    MPP_MOD_SYS 
#define PROC_ENTRY_PCIV   MPP_MOD_PCIV
#define PROC_ENTRY_LOG    MPP_MOD_LOG
#define PROC_ENTRY_DCCM   MPP_MOD_DCCM
#define PROC_ENTRY_DCCS   MPP_MOD_DCCS
#define PROC_ENTRY_MST_LOG  MPP_MOD_MST_LOG
#define PROC_ENTRY_PCIVFMW  MPP_MOD_PCIVFMW
#define PROC_ENTRY_RC       MPP_MOD_RC
#define PROC_ENTRY_IVE  	MPP_MOD_IVE
#define PROC_ENTRY_FD 	    MPP_MOD_FD
#define PROC_ENTRY_MD		MPP_MOD_MD
#define PROC_ENTRY_HDMI	    MPP_MOD_HDMI
#define PROC_ENTRY_ISP	    MPP_MOD_ISP
#define PROC_ENTRY_FISHEYE 	MPP_MOD_FISHEYE 


#define CMPI_PROC_SHOW CMPI_PROC_READ
typedef HI_S32 (*CMPI_PROC_READ)(struct seq_file *, HI_VOID *);	
typedef HI_S32 (*CMPI_PROC_WRITE)(struct file * file, 
    const char __user * buf, size_t count, loff_t *ppos);

typedef struct struCMPI_PROC_ITEM
{
    HI_CHAR entry_name[32];
	struct proc_dir_entry *entry;
	CMPI_PROC_READ read;
    CMPI_PROC_WRITE write;
    HI_BOOL bDefault;
	HI_VOID *pData;
}CMPI_PROC_ITEM_S;

extern CMPI_PROC_ITEM_S *CMPI_CreateProc(HI_CHAR *, CMPI_PROC_SHOW, HI_VOID *);
extern HI_VOID CMPI_RemoveProc(HI_CHAR *);

extern HI_S32  CMPI_ProcInit(HI_VOID);
extern HI_VOID CMPI_ProcExit(HI_VOID);

extern HI_S32  CMPI_LogInit(HI_U32);
extern HI_VOID CMPI_LogExit(HI_VOID);


#endif
