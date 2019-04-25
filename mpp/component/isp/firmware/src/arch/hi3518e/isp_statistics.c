/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_statistics.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/05/06
  Description   : 
  History       :
  1.Date        : 2013/05/06
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "mpi_sys.h"
#include "isp_statistics.h"
#include "isp_ext_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiISP_STA_S
{
    HI_U32          u32StatPhyaddr;
    HI_VOID        *pStatVirtAddr;

    HI_BOOL         bRead;
    ISP_STAT_INFO_S stStatInfo;
} ISP_STA_S;

ISP_STA_S g_astStatCtx[ISP_MAX_DEV_NUM] = {{0}};
#define STATISTICS_GET_CTX(dev, pstCtx)   pstCtx = &g_astStatCtx[dev]

extern HI_S32 g_as32IspFd[ISP_MAX_DEV_NUM];

HI_S32 ISP_StatisticsInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;

    STATISTICS_GET_CTX(IspDev, pstStat);

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_INIT, &pstStat->u32StatPhyaddr);
    if (HI_SUCCESS != s32Ret)
    {
        printf("init statistics bufs failed %x!\n", s32Ret);
        return s32Ret;
    }

    pstStat->pStatVirtAddr = HI_MPI_SYS_Mmap(pstStat->u32StatPhyaddr,
        sizeof(ISP_STAT_S) * MAX_ISP_STAT_BUF_NUM);
    if (HI_NULL == pstStat->pStatVirtAddr)
    {
        printf("mmap statistics bufs failed!\n");
        ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_EXIT);
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_StatisticsExit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;

    STATISTICS_GET_CTX(IspDev, pstStat);

    if (HI_NULL != pstStat->pStatVirtAddr)
    {
        HI_MPI_SYS_Munmap(pstStat->pStatVirtAddr,
            sizeof(ISP_STAT_S) * MAX_ISP_STAT_BUF_NUM);
        pstStat->pStatVirtAddr = HI_NULL;
    }
    
    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_EXIT);
    if (HI_SUCCESS != s32Ret)
    {
        printf("exit statistics bufs failed %x!\n", s32Ret);
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_StatisticsGetBuf(ISP_DEV IspDev, HI_VOID **ppStat)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;

    STATISTICS_GET_CTX(IspDev, pstStat);

    if (!pstStat->bRead)
    {
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_GET, &pstStat->stStatInfo);
        if (s32Ret)
        {
            return s32Ret;
        }

        if (HI_NULL != pstStat->pStatVirtAddr)
        {
            pstStat->stStatInfo.pVirtAddr = pstStat->pStatVirtAddr +
                (pstStat->stStatInfo.u32PhyAddr - pstStat->u32StatPhyaddr);
        }
        else
        {        
            pstStat->stStatInfo.pVirtAddr = HI_NULL;
        }
        pstStat->bRead = HI_TRUE;
    }

    if (HI_NULL == pstStat->stStatInfo.pVirtAddr)
    {
        return HI_FAILURE;
    }
    
    *ppStat = pstStat->stStatInfo.pVirtAddr;
    
    return HI_SUCCESS;
}

HI_S32 ISP_StatisticsPutBuf(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_STA_S *pstStat = HI_NULL;

    STATISTICS_GET_CTX(IspDev, pstStat);

    pstStat->stStatInfo.unKey.u32Key = hi_ext_system_statistics_ctrl_read();

    if (pstStat->bRead)
    {
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_STAT_BUF_PUT, &pstStat->stStatInfo);
        if (s32Ret)
        {
            printf("Release ISP stat buf Failed with ec %#x!\n", s32Ret);
        }
        pstStat->stStatInfo.pVirtAddr = HI_NULL;
        pstStat->bRead = HI_FALSE;
    }
    
    return HI_SUCCESS;
}

#if 0
HI_S32 ISP_StatisticsKeyMap(HI_VOID *pStat, ISP_STAT_KEY_U unKey, HI_VOID **ppKeyStat)
{
    ISP_STAT_S *pstStatInfo = HI_NULL;
    
    pstStatInfo = (ISP_STAT_S *)pStat;
    if (unKey.bit1AeStat1)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAeStat1;
    }
    else if (unKey.bit1AeStat2)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAeStat2;
    }
    else if (unKey.bit1AeStat3)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAeStat3;
    }
    else if (unKey.bit1AwbStat1)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAwbStat1;
    }
    else if (unKey.bit1AwbStat2)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAwbStat2;
    }
    else if (unKey.bit1AfStat)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stAfStat;
    }
    else if (unKey.bit1CommStat)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stCommStat;
    }
    else if (unKey.bit1DpStat)
    {
        *ppKeyStat = (HI_VOID *)pstStatInfo->stDpStat;
    }
    else
    {
        *ppKeyStat = pStat;
    }

    return HI_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

