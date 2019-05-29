/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_isp_ae.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/04/01
  Description   : 
  History       :
  1.Date        : 2014/04/01
    Author      : h00191408
    Modification: Created file

******************************************************************************/
#include <string.h>
#include <stdio.h>
    
#include "hi_comm_isp.h"
#include "hi_comm_3a.h"
#include "sample_ae_ext_config.h"
#include "isp_metering_mem_config.h"
#include "isp_histogram_mem_config.h" 
#include "isp_config.h"
#include "sample_ae_adp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


HI_S32 HI_MPI_ISP_SetExposureAttr(ISP_DEV IspDev, const ISP_EXPOSURE_ATTR_S *pstExpAttr)
{
    ALG_LIB_S stAeLib;        
    stAeLib.s32Id = 4;
    strncpy(stAeLib.acLibName, "hisi_ae_lib", sizeof("hisi_ae_lib"));

    if (pstExpAttr->stAuto.enAEMode >= AE_MODE_BUTT)
    {
        printf("Invalid AE mode!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_ae_mode_write((HI_U8)stAeLib.s32Id, (HI_U8)pstExpAttr->stAuto.enAEMode);
    
    
    return HI_SUCCESS;      
}

HI_S32 HI_MPI_ISP_GetExposureAttr(ISP_DEV IspDev, ISP_EXPOSURE_ATTR_S *pstExpAttr)
{   
    
    return HI_SUCCESS;
}



HI_S32 ISP_RouteCheck(HI_U8 u8Id, const ISP_AE_ROUTE_S *pstRoute)
{
    
    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_SetAERouteAttr(ISP_DEV IspDev, const ISP_AE_ROUTE_S *pstAERouteAttr)
{

    return HI_SUCCESS;    
}

HI_S32 HI_MPI_ISP_GetAERouteAttr(ISP_DEV IspDev, ISP_AE_ROUTE_S *pstAERouteAttr)
{    
   
       return HI_SUCCESS;   
}


HI_S32 HI_MPI_ISP_QueryExposureInfo(ISP_DEV IspDev, ISP_EXP_INFO_S *pstExpInfo)
{
    
    ALG_LIB_S stAeLib;        
    stAeLib.s32Id = 4;
    strncpy(stAeLib.acLibName, "hisi_ae_lib", sizeof("hisi_ae_lib"));
    
    pstExpInfo->u32AGain = hi_ext_system_query_exposure_again_read((HI_U8)stAeLib.s32Id);

    return HI_SUCCESS;
   
}


HI_S32 HI_MPI_ISP_SetIrisAttr(ISP_DEV IspDev, const ISP_IRIS_ATTR_S *pstIrisAttr)
{
  
    
    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_GetIrisAttr(ISP_DEV IspDev, ISP_IRIS_ATTR_S *pstIrisAttr)
{
        
    return HI_SUCCESS; 

}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


