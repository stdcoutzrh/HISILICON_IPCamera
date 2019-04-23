/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_debug.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_DEBUG_H__
#define __ISP_DEBUG_H__

#include <stdio.h>
#include "hi_type.h"
#include "hi_errno.h"
#include "hi_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiISP_DBG_CTRL_S
{
    HI_BOOL bDebugEn;
    HI_U32  u32PhyAddr;
    HI_U32  u32Depth;
    
    HI_U32  u32Size;
    ISP_DBG_ATTR_S   *pstDbgAttr;
    ISP_DBG_STATUS_S *pstDbgStatus;
} ISP_DBG_CTRL_S;

HI_S32 ISP_DbgSet(const ISP_DEBUG_INFO_S *pstDbgInfo);
HI_S32 ISP_DbgGet(ISP_DEBUG_INFO_S *pstDbgInfo);
HI_S32 ISP_DbgRunBgn(ISP_DBG_CTRL_S *pstDbg, HI_U32 u32FrmCnt);
HI_S32 ISP_DbgRunEnd(ISP_DBG_CTRL_S *pstDbg, HI_U32 u32FrmCnt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __HISI_ISP_DEBUG_H__ */


