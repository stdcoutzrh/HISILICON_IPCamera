/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_proc.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/08/06
  Description   : 
  History       :
  1.Date        : 2013/08/06
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifndef __ISP_PROC_H__
#define __ISP_PROC_H__

#include "hi_common.h"
#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 ISP_ProcInit(ISP_DEV IspDev);
HI_S32 ISP_ProcWrite(const ISP_ALG_NODE_S *astAlgs, ISP_DEV IspDev);
HI_S32 ISP_ProcExit(ISP_DEV IspDev);

#define ISP_PROC_PRINTF(proc, len, fmt...)                          \
do{                                                                 \
    snprintf((proc)->pcProcBuff, (proc)->u32BuffLen, ##fmt);        \
    (proc)->u32WriteLen = strlen((proc)->pcProcBuff);               \
    (proc)->pcProcBuff = &((proc)->pcProcBuff[(proc)->u32WriteLen]);\
    (proc)->u32BuffLen -= (proc)->u32WriteLen;                      \
    len += (proc)->u32WriteLen;                                     \
}while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
