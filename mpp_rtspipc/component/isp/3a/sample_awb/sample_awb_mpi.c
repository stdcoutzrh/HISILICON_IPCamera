/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_isp_awb.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/22
  Description   : 
  History       :
  1.Date        : 2013/01/22
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include <string.h>
#include <stdio.h>
    
#include "hi_comm_isp.h"
#include "hi_comm_3a.h"
#include "sample_awb_ext_config.h"
#include "isp_metering_mem_config.h"
#include "isp_config.h"
#include "sample_awb_adp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


HI_S32 HI_MPI_ISP_SetWBAttr(ISP_DEV IspDev, const ISP_WB_ATTR_S *pstWBAttr)
{
    ALG_LIB_S stAwbLib;

    AWB_CHECK_DEV(IspDev);
    AWB_CHECK_POINTER(pstWBAttr);

    stAwbLib.s32Id = 0;
    strncpy(stAwbLib.acLibName, "hisi_awb_lib", sizeof("hisi_awb_lib"));

    if (OP_TYPE_BUTT <= pstWBAttr->enOpType)	
    {
        printf("Invalid input of parameter enOpType!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_wb_type_write((HI_U8)stAwbLib.s32Id, pstWBAttr->enOpType);  
    
    return HI_SUCCESS;	
}


HI_S32 HI_MPI_ISP_GetWBAttr(ISP_DEV IspDev, ISP_WB_ATTR_S *pstWBAttr)
{
  
    return HI_SUCCESS;	
}

HI_S32 HI_MPI_ISP_SetAWBAttrEx(ISP_DEV IspDev, ISP_AWB_ATTR_EX_S *pstAWBAttrEx)
{

    return HI_SUCCESS;	
}

HI_S32 HI_MPI_ISP_GetAWBAttrEx(ISP_DEV IspDev, ISP_AWB_ATTR_EX_S *pstAWBAttrEx)
{

    return HI_SUCCESS;	
}

HI_S32 HI_MPI_ISP_SetCCMAttr(ISP_DEV IspDev, const ISP_COLORMATRIX_ATTR_S *pstCCMAttr)
{
    
    return HI_SUCCESS;	
}

HI_S32 HI_MPI_ISP_GetCCMAttr(ISP_DEV IspDev, ISP_COLORMATRIX_ATTR_S *pstCCMAttr)
{
	
    return HI_SUCCESS;	
}

HI_S32 HI_MPI_ISP_SetSaturationAttr(ISP_DEV IspDev, const ISP_SATURATION_ATTR_S *pstSatAttr)
{
 
        
    return HI_SUCCESS;
}
    
HI_S32 HI_MPI_ISP_GetSaturationAttr(ISP_DEV IspDev, ISP_SATURATION_ATTR_S *pstSatAttr)
{
  
            
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetColorToneAttr(ISP_DEV IspDev, const ISP_COLOR_TONE_ATTR_S *pstCTAttr)
{


    return HI_SUCCESS;
}
	
HI_S32 HI_MPI_ISP_GetColorToneAttr(ISP_DEV IspDev, ISP_COLOR_TONE_ATTR_S *pstCTAttr)
{

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_QueryWBInfo(ISP_DEV IspDev, ISP_WB_INFO_S *pstWBInfo)
{
    ALG_LIB_S stAwbLib;
    
    AWB_CHECK_DEV(IspDev);
    AWB_CHECK_POINTER(pstWBInfo);

    stAwbLib.s32Id = 0;
    strncpy(stAwbLib.acLibName, "hisi_awb_lib", sizeof("hisi_awb_lib"));


    pstWBInfo->u16ColorTemp = hi_ext_system_wb_detect_temp_read((HI_U8)stAwbLib.s32Id);
    
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

