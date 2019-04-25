/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_proc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/08/06
  Description   : 
  History       :
  1.Date        : 2013/08/06
    Author      : n0168968
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "mpi_sys.h"
#include "mkp_isp.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HISTOGRAM_BANDS 5

typedef struct hiISP_PROC_S
{
    HI_U32 u32IntCount;
    HI_U32 u32ProcParam;
    ISP_PROC_MEM_S stProcMem;    
} ISP_PROC_S;

ISP_PROC_S g_astProcCtx[ISP_MAX_DEV_NUM] = {{0}};
#define PROC_GET_CTX(dev, pstCtx)   pstCtx = &g_astProcCtx[dev]

HI_S32 ISP_UpdateProcParam(ISP_DEV IspDev);


HI_S32 ISP_ProcInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_PROC_S *pstProc = HI_NULL;

    PROC_GET_CTX(IspDev, pstProc);
    
    ISP_CHECK_OPEN(0);    

    s32Ret = ioctl(g_as32IspFd[0], ISP_PROC_PARAM_GET, &pstProc->u32ProcParam);
    if (s32Ret)
    {
        printf("get proc param %x!\n", s32Ret);
        return s32Ret;
    }

    if (0 == pstProc->u32ProcParam)
    {
        return HI_SUCCESS;
    }

    s32Ret = ioctl(g_as32IspFd[0], ISP_PROC_INIT, &pstProc->stProcMem);
    if (s32Ret)
    {
        printf("init proc ec %x!\n", s32Ret);
        return s32Ret;
    }

    pstProc->stProcMem.pProcVirtAddr = HI_MPI_SYS_Mmap(pstProc->stProcMem.u32ProcPhyAddr,
        pstProc->stProcMem.u32ProcSize);
    if (HI_NULL == pstProc->stProcMem.pProcVirtAddr)
    {
        printf("mmap proc mem failed!\n");
        s32Ret = HI_ERR_ISP_NOMEM;
        goto freeproc;
    }
    pstProc->u32IntCount = 0;

    return HI_SUCCESS;

freeproc:
    ioctl(g_as32IspFd[0], ISP_PROC_EXIT);
    return s32Ret;
}

HI_S32 ISP_ProcWrite(const ISP_ALG_NODE_S *astAlgs, ISP_DEV IspDev)
{
    HI_S32 s32Ret, i;
    ISP_PROC_S *pstProc = HI_NULL;
    ISP_CTRL_PROC_WRITE_S stProcCtrl;

    PROC_GET_CTX(IspDev, pstProc);

    ISP_CHECK_OPEN(0);
	
    ISP_UpdateProcParam(0);


    if (0 == pstProc->u32ProcParam)
    {
        return HI_SUCCESS;
    }
    
    if (HI_NULL == pstProc->stProcMem.pProcVirtAddr)
    {
        printf("the proc hasn't init!\n");
        return HI_FAILURE;
    }

    /* write proc info 1s a time */
    pstProc->u32IntCount++;
    if (0 != pstProc->u32IntCount % pstProc->u32ProcParam)
    {
        return HI_SUCCESS;
    }    
    pstProc->u32IntCount = 0;

    s32Ret = ioctl(g_as32IspFd[0], ISP_PROC_WRITE_ING);
    if (s32Ret)
    {
        printf("write proc failed, ec %x!\n", s32Ret);
        return s32Ret;
    }

    stProcCtrl.pcProcBuff = (HI_CHAR *)pstProc->stProcMem.pProcVirtAddr;
    stProcCtrl.u32BuffLen = pstProc->stProcMem.u32ProcSize-1;
    stProcCtrl.u32WriteLen = 0;
    for (i=0; i<ISP_MAX_ALGS_NUM; i++)
    {
        if (astAlgs[i].bUsed)
        {
            if (HI_NULL != astAlgs[i].stAlgFunc.pfn_alg_ctrl)
            {
                astAlgs[i].stAlgFunc.pfn_alg_ctrl(IspDev, ISP_PROC_WRITE, &stProcCtrl);
            }

            if (stProcCtrl.u32WriteLen > stProcCtrl.u32BuffLen)
            {
                printf("Warning!! Proc buff overflow!\n");
                stProcCtrl.u32WriteLen = stProcCtrl.u32BuffLen;
                break;
            }

            if (0 != stProcCtrl.u32WriteLen)
            {
                if ('\0' != stProcCtrl.pcProcBuff[stProcCtrl.u32WriteLen-1])
                {
                    printf("Warning!! alg %d's proc doesn't finished with endl!\n", astAlgs[i].enAlgType);
                }
                stProcCtrl.pcProcBuff[stProcCtrl.u32WriteLen-1] = '\n';
            }

            /* update the proc ctrl */
            stProcCtrl.pcProcBuff = &stProcCtrl.pcProcBuff[stProcCtrl.u32WriteLen];
            stProcCtrl.u32BuffLen = stProcCtrl.u32BuffLen - stProcCtrl.u32WriteLen;
            stProcCtrl.u32WriteLen = 0;
            if (0 == stProcCtrl.u32BuffLen)
            {
                break;
            }
        }
    }
    
    stProcCtrl.pcProcBuff[stProcCtrl.u32WriteLen] = '\0';
    ioctl(g_as32IspFd[0], ISP_PROC_WRITE_OK);
    
    return HI_SUCCESS;
}

HI_S32 ISP_ProcExit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    HI_VOID *pVirtAddr;
    ISP_PROC_S *pstProc = HI_NULL;

    PROC_GET_CTX(IspDev, pstProc);
    
    ISP_CHECK_OPEN(0);

    if (0 == pstProc->u32ProcParam)
    {
        return HI_SUCCESS;
    }

    if (HI_NULL != pstProc->stProcMem.pProcVirtAddr)
    {
        pVirtAddr = pstProc->stProcMem.pProcVirtAddr;
        pstProc->stProcMem.pProcVirtAddr = HI_NULL;
        HI_MPI_SYS_Munmap(pVirtAddr, pstProc->stProcMem.u32ProcSize);
    }

    s32Ret = ioctl(g_as32IspFd[0], ISP_PROC_EXIT);
    if (s32Ret)
    {
        printf("exit proc ec %x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}


HI_S32 ISP_UpdateProcParam(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_PROC_S *pstProc = HI_NULL;
	
    PROC_GET_CTX(IspDev, pstProc);	

    s32Ret = ioctl(g_as32IspFd[0], ISP_PROC_PARAM_GET, &pstProc->u32ProcParam);
    if (s32Ret)
    {
        printf("get proc param %x!\n", s32Ret);
        return s32Ret;
    }
	
	return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

