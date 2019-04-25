/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_main.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_MAIN_H__
#define __ISP_MAIN_H__

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "hi_comm_3a.h"
#include "isp_debug.h"
#include "mkp_isp.h"
#ifdef ENABLE_JPEGEDCF
#include "isp_dcfinfo.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_MAX_ALGS_NUM 32

typedef struct hiISP_LIB_NODE_S
{
    HI_BOOL           bUsed;
    ALG_LIB_S         stAlgLib;
    union
    {
        ISP_AE_REGISTER_S  stAeRegsiter;
        ISP_AWB_REGISTER_S stAwbRegsiter;
        ISP_AF_REGISTER_S  stAfRegsiter;
    };
} ISP_LIB_NODE_S;

typedef struct hiISP_LIB_INFO_S
{
    HI_U32          u32ActiveLib;   /* the lib used present. */
    ISP_LIB_NODE_S  astLibs[MAX_REGISTER_ALG_LIB_NUM];
} ISP_LIB_INFO_S;

typedef struct hiISP_ALG_FUNC_S
{
    HI_S32 (*pfn_alg_init)(ISP_DEV IspDev);
    HI_S32 (*pfn_alg_run)(ISP_DEV IspDev,
        const HI_VOID *pStatInfo,
        HI_VOID *pRegCfg,
        HI_S32 s32Rsv);
    HI_S32 (*pfn_alg_ctrl)(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue);
    HI_S32 (*pfn_alg_exit)(ISP_DEV IspDev);
} ISP_ALG_FUNC_S;

typedef struct hiISP_ALG_NODE_S
{
    HI_BOOL         bUsed;
    ISP_ALG_MOD_E   enAlgType;
    ISP_ALG_FUNC_S  stAlgFunc;
} ISP_ALG_NODE_S;

typedef struct hiISP_LINKAGE_S
{
    HI_BOOL bDefectPixel;  // each alg need to check this status
    
    HI_U32  u32IspDgain;
    HI_U32  u32IspDgainShift;
    HI_U32  u32Iso;
    HI_U32  u32IsoMax;
    HI_U32  u32SensorIso;
    HI_U32  u32ExpRatio;
    HI_U32  u32Inttime;
    HI_U32  u32PirisGain;
    HI_U8   u8AERunInterval;
} ISP_LINKAGE_S;

typedef struct hiISP_PARA_REC_S
{
    HI_BOOL bWDRCfg;
    HI_BOOL bPubCfg;
    HI_BOOL bInit;
    HI_BOOL bRun;

    HI_BOOL bRunEn;
} ISP_PARA_REC_S;

typedef struct hiISP_SENSOR_IMAGE_MODE_S
{
    HI_U16   u16Width;
    HI_U16   u16Height;
    HI_FLOAT f32Fps;
}ISP_SENSOR_IMAGE_MODE_S;

typedef struct hiISP_CTX_S
{
    /* 1. ctrl param */
    HI_BOOL                 bMemInit;
    HI_BOOL                 bSnsReg;
    ISP_PARA_REC_S          stIspParaRec;    
    pthread_mutex_t         stLock;

    /* 2. algorithm ctrl param */
    HI_BOOL                 bFreezeFw;
    HI_U8                   u8PreSnsWDRMode;
    HI_U8                   u8SnsWDRMode;    
    HI_U32                  u32FrameCnt;    /* frame count for 3a libs which can decide the interval of calculate. */

    HI_U8                    u8IspImageModeFlag;
    ISP_SENSOR_IMAGE_MODE_S  stPreSnsImageMode;
    ISP_SENSOR_IMAGE_MODE_S  stSnsImageMode;

    /* 3. 3a register info */    
    ISP_BIND_ATTR_S         stBindAttr;
    ISP_LIB_INFO_S          stAeLibInfo;
    ISP_LIB_INFO_S          stAwbLibInfo;
    ISP_LIB_INFO_S          stAfLibInfo;

    /* 4. algorithm ctrl */
    ISP_ALG_NODE_S          astAlgs[ISP_MAX_ALGS_NUM];
    ISP_LINKAGE_S           stLinkage;

    /* 5. debug info */
    ISP_DBG_CTRL_S          stIspDbg;
#ifdef ENABLE_JPEGEDCF
    /* 6. DCF info*/
    ISP_DCFINFO_CTRL_S      stDcfInfoCtrl;
#endif
} ISP_CTX_S;

extern ISP_CTX_S  g_astIspCtx[ISP_MAX_DEV_NUM];
extern HI_S32     g_as32IspFd[ISP_MAX_DEV_NUM];

#define ISP_CHECK_OPEN(dev)\
do{\
    if (g_as32IspFd[dev] <= 0)\
    {\
        HI_U32 arg = (dev);\
        g_as32IspFd[dev] = open("/dev/isp_dev", O_RDONLY);\
        if (g_as32IspFd[dev] <= 0)\
        {\
            perror("open isp device error!\n");\
            return HI_ERR_ISP_NOT_INIT;\
        }\
        if (ioctl(g_as32IspFd[dev], ISP_DEV_SET_FD, &arg))\
        {\
            close(g_as32IspFd[dev]);\
            g_as32IspFd[dev] = -1;\
            return HI_ERR_ISP_NOT_INIT;\
        }\
    }\
}while(0)

#define ISP_CHECK_MEM_INIT(dev)\
do{\
    if (g_astIspCtx[dev].bMemInit != HI_TRUE)\
    {\
        if (ioctl(g_as32IspFd[dev], ISP_MEM_INFO_GET, &g_astIspCtx[dev].bMemInit))\
        {\
            ISP_TRACE(HI_DBG_ERR, "ISP[%d] get Mem info failed!\n", dev);\
            return HI_ERR_ISP_MEM_NOT_INIT;\
        }\
        if (g_astIspCtx[dev].bMemInit != HI_TRUE)\
        {\
            ISP_TRACE(HI_DBG_ERR, "ISP[%d] Mem NOT Init %d!\n", dev, g_astIspCtx[dev].bMemInit);\
            return HI_ERR_ISP_MEM_NOT_INIT;\
        }\
    }\
}while(0)

#define ISP_CHECK_ISP_WDR_CFG(dev)\
do{\
    if (g_astIspCtx[dev].stIspParaRec.bWDRCfg != HI_TRUE)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Isp[%d] WDR mode doesn't config!\n", dev);\
        return HI_ERR_ISP_ATTR_NOT_CFG;\
    }\
}while(0)

#define ISP_CHECK_ISP_PUB_ATTR_CFG(dev)\
do{\
    if (g_astIspCtx[dev].stIspParaRec.bPubCfg != HI_TRUE)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Isp[%d] Pub Attr doesn't config!\n", dev);\
        return HI_ERR_ISP_ATTR_NOT_CFG;\
    }\
}while(0)

#define ISP_CHECK_ISP_INIT(dev)\
do{\
    if (g_astIspCtx[dev].stIspParaRec.bInit != HI_TRUE)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Isp[%d] doesn't initialized!\n", dev);\
        return HI_ERR_ISP_NOT_INIT;\
    }\
}while(0)

#define ISP_GET_CTX(dev, pstIspCtx)\
do{\
    pstIspCtx = &g_astIspCtx[dev];\
}while(0)

/*
 * HIGH LEVEL functions used to set-up and run ISP.
 */
/*
 * This function executes the auto-exposure algorithm
 * and its results are used to drive sensor parameters,
 * drc engine and noise reduction algorithms,
 * Results are based on the collected image histogram data.
 */
HI_S32 ISP_Run(ISP_DEV IspDev);

HI_S32 ISP_SwitchWDRMode(ISP_DEV IspDev);
HI_S32 ISP_SwitchImageMode(ISP_DEV IspDev);

HI_S32 ISP_WDRCfgSet(ISP_DEV IspDev);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_MAIN_H__ */

