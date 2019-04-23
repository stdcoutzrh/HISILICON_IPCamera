/******************************************************************************
 
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 
 ******************************************************************************
  File Name     : dev_ext.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/11/03
  Description   : 
  History       :
  1.Date        : 2006/11/03
    Author      : c42025
    Modification: Created file

  1.Date        : 2007/11/16
    Author      : c42025
    Modification: modified definition of UMAP_DEV_NUM
  2.Date        : 2008/10/16
    Author      : z44949
    Modification: Modify the log's device path from "umap/video" to "umap"
  3.Date        : 2009/08/13
    Author      : y45339
    Modification: add some proc define
        
******************************************************************************/
#ifndef _DEV_EXT_H_
#define _DEV_EXT_H_

#include <linux/version.h>
#include <linux/device.h>
#include <linux/fs.h>
#ifdef CONFIG_DEVFS_FS
#include <linux/devfs_fs_kernel.h>
#endif

#include "hi_type.h"
#include "himedia.h"

#define UMAP_DEV_NUM                64
#define UMAP_NAME_MAXLEN        	32

#define UMAP_VI_MINOR_BASE          0
#define UMAP_VO_MINOR_BASE          1
#define UMAP_VENC_MINOR_BASE        2
#define UMAP_VDEC_MINOR_BASE        3
#define UMAP_VDA_MINOR_BASE         4
#define UMAP_AI_MINOR_BASE          5
#define UMAP_AO_MINOR_BASE          6
#define UMAP_GRP_MINOR_BASE         7
#define UMAP_SYS_MINOR_BASE         8
#define UMAP_VB_MINOR_BASE          9
#define UMAP_VPSS_MINOR_BASE        10
#define UMAP_AIO_MINOR_BASE         11
#define UMAP_LOG_MINOR_BASE         12
#define UMAP_AENC_MINOR_BASE        13
#define UMAP_ADEC_MINOR_BASE        14
#define UMAP_MST_LOG_MINOR_BASE     15
#define UMAP_RGN_MINOR_BASE         16
#define UMAP_IVE_MINOR_BASE         17
#define UMAP_HDMI_MINOR_BASE        18
#define UMAP_VGS_MINOR_BASE         19

#define UMAP_ISP_MINOR_BASE         20
#define UMAP_RC_MINOR_BASE          21
#define UMAP_H264E_MINOR_BASE       22
#define UMAP_H265E_MINOR_BASE       23
#define UMAP_JPEGE_MINOR_BASE       24
#define UMAP_CHNL_MINOR_BASE        25
#define UMAP_MPEG4E_MINOR_BASE      26
#define UMAP_FISHEYE_MINOR_BASE     27
#define UMAP_PCIV_MINOR_BASE  		28
#define UMAP_PCIVFMW_MINOR_BASE		29
#define UMAP_FD_MINOR_BASE   		30



#define UMAP_GET_CHN(f)      ((HI_U32)((f)->private_data))
#define UMAP_SET_CHN(f, chn) (((f)->private_data) = (void*)(chn))

#define UMAP_NAME	            "umap"
#define UMAP_NAME_VIDEO         UMAP_NAME"/video/"
#define UMAP_NAME_AUDIO         UMAP_NAME"/audio/"

#define UMAP_DEVNAME_SYSCTL     MPP_MOD_SYS
#define UMAP_DEVNAME_LOG_BASE   MPP_MOD_LOG
#define UMAP_DEVNAME_MST_LOG_BASE   MPP_MOD_MST_LOG

#define UMAP_DEVNAME_VI_BASE    MPP_MOD_VIU
#define UMAP_DEVNAME_VO_BASE    MPP_MOD_VOU
#define UMAP_DEVNAME_VENC_BASE  MPP_MOD_VENC
#define UMAP_DEVNAME_VDEC_BASE  MPP_MOD_VDEC
#define UMAP_DEVNAME_DSU_BASE   MPP_MOD_DSU
#define UMAP_DEVNAME_VDA_BASE   MPP_MOD_VDA
#define UMAP_DEVNAME_VB_BASE    MPP_MOD_VB
#define UMAP_DEVNAME_VPSS_BASE  MPP_MOD_VPSS
#define UMAP_DEVNAME_GRP_BASE   MPP_MOD_GRP
#define UMAP_DEVNAME_RGN_BASE   MPP_MOD_RGN
#define UMAP_DEVNAME_IVE_BASE   MPP_MOD_IVE
#define UMAP_DEVNAME_FD_BASE    MPP_MOD_FD

#define UMAP_DEVNAME_AIO_BASE   MPP_MOD_AIO
#define UMAP_DEVNAME_AI_BASE    MPP_MOD_AI
#define UMAP_DEVNAME_AO_BASE    MPP_MOD_AO
#define UMAP_DEVNAME_AENC_BASE  MPP_MOD_AENC
#define UMAP_DEVNAME_ADEC_BASE  MPP_MOD_ADEC

#define UMAP_DEVNAME_HDMI_BASE  MPP_MOD_HDMI

#define UMAP_DEVNAME_VGS_BASE  MPP_MOD_VGS

#define UMAP_DEVNAME_FISHEYE_BASE MPP_MOD_FISHEYE

#define UMAP_DEVNAME_H264E_BASE   MPP_MOD_H264E
#define UMAP_DEVNAME_H265E_BASE   MPP_MOD_H265E
#define UMAP_DEVNAME_JPEGE_BASE   MPP_MOD_JPEGE
#define UMAP_DEVNAME_MPEG4E_BASE  MPP_MOD_MPEG4E
#define UMAP_DEVNAME_CHNL_BASE    MPP_MOD_CHNL
#define UMAP_DEVNAME_RC_BASE      MPP_MOD_RC
#define UMAP_DEVNAME_MPEG4E_BASE   MPP_MOD_MPEG4E

#define UMAP_DEVNAME_PCIV_BASE   	MPP_MOD_PCIV
#define UMAP_DEVNAME_PCIVFMW_BASE   MPP_MOD_PCIVFMW


typedef struct himedia_device UMAP_DEVICE_S, *PTR_UMAP_DEVICE_S;

extern HI_S32  CMPI_RegisterDevice(UMAP_DEVICE_S *umapd);
extern HI_VOID CMPI_UnRegisterDevice(UMAP_DEVICE_S *umapd);
extern HI_S32  CMPI_UserCopy(struct file *file, 
                unsigned cmd, unsigned long arg,
				int (*func)(struct file *file, unsigned cmd, unsigned long arg));

#endif /* _DEV_EXT_H_ */
