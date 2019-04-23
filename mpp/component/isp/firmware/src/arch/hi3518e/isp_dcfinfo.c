/******************************************************************************

  Copyright (C), 2014-2019, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_dcfinfo.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/06/17
  Description   : 
  History       :
  1.Date        : 2014/06/17
    Author      : z00183560
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "mpi_sys.h"
#ifdef ENABLE_JPEGEDCF
#include "isp_dcfinfo.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

extern HI_S32     g_as32IspFd[ISP_MAX_DEV_NUM];
HI_S32 ISP_DCFInfoInit(ISP_DEV IspDev,ISP_DCFINFO_CTRL_S* pstDcfInfoCtrl)
{
    HI_S32 s32Ret;
        
    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_DCFINFO_BUF_INIT, &pstDcfInfoCtrl->u32DCFInfoPhyaddr);
    if (HI_SUCCESS != s32Ret)
    {
        pstDcfInfoCtrl->bSupportDCF = HI_FALSE;
        
        if (HI_ERR_ISP_NOT_SUPPORT == s32Ret)
        {
            s32Ret = HI_SUCCESS;
        }
        else
        {
            printf("init dcf bufs failed %x!\n", s32Ret);
        }
        return s32Ret;
    }

    pstDcfInfoCtrl->pstIspDCF = HI_MPI_SYS_Mmap(pstDcfInfoCtrl->u32DCFInfoPhyaddr,
        sizeof(ISP_DCF_INFO_S));
    if (HI_NULL == pstDcfInfoCtrl->pstIspDCF)
    {
        printf("mmap dcf info buf failed!\n");
        ioctl(g_as32IspFd[IspDev], ISP_DCFINFO_BUF_EXIT);
        return HI_ERR_ISP_NOMEM;
    }
    pstDcfInfoCtrl->bSupportDCF = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 ISP_DCFInfoExit(ISP_DEV IspDev,ISP_DCFINFO_CTRL_S* pstDcfInfoCtrl)
{   
    HI_S32 s32Ret;

    if (HI_FALSE == pstDcfInfoCtrl->bSupportDCF)
    {
        return HI_SUCCESS;
    }
    
    if (HI_NULL != pstDcfInfoCtrl->pstIspDCF)
    {
        HI_MPI_SYS_Munmap(pstDcfInfoCtrl->pstIspDCF,
            sizeof(ISP_DCF_INFO_S));
        pstDcfInfoCtrl->pstIspDCF = HI_NULL;
    }
    
    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_DCFINFO_BUF_EXIT);
    if (HI_SUCCESS != s32Ret)
    {
        printf("exit dcf info buf failed %x!\n", s32Ret);
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

