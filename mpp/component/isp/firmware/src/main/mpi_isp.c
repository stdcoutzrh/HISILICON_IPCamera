/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_isp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/01/05
  Description   : 
  History       :
  1.Date        : 2011/01/05
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "hi_comm_isp.h"
#include "mpi_sys.h"
#include "mpi_isp.h"
#include "mkp_isp.h"
#include "hi_comm_3a.h"
#include "hi_ae_comm.h"
#include "hi_awb_comm.h"

#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_histogram_mem_config.h"
#include "isp_shading_mem_config.h"
#include "isp_metering_mem_config.h"
#include "isp_gamma_rgb_mem_config.h"
#include "isp_gamma_fe0_mem_config.h"
#include "isp_gamma_fe1_mem_config.h"
#include "isp_debug.h"
#include "isp_main.h"

#include "hi_vreg.h"

#include "hi_comm_vi.h"
#include "mkp_vi.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/


/*****************************************************************************
 Prototype       : isp pub configure
 Description     : need I/F cowork:
                    OB area, need to configure window, and I/F font edge;
                    others configure I/F as back edge;                    
 Input           : None
 Output          : None
 Return Value    : 
 Process         : 
 Note             : 

  History         
  1.Date         : 2011/1/14
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_SetPubAttr(ISP_DEV IspDev, const ISP_PUB_ATTR_S *pstPubAttr)
{
    HI_VOID *pValue = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstPubAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
   
    if ((pstPubAttr->stWndRect.u32Width < 480) || (pstPubAttr->stWndRect.u32Width > 2048))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image Width:%d!\n", pstPubAttr->stWndRect.u32Width);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stWndRect.u32Height < 240) || (pstPubAttr->stWndRect.u32Height > 1208))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image Height:%d!\n", pstPubAttr->stWndRect.u32Height);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stWndRect.s32X < 0) || (pstPubAttr->stWndRect.s32X > pstPubAttr->stWndRect.u32Width - 480))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image X:%d!\n", pstPubAttr->stWndRect.s32X);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->stWndRect.s32Y < 0) || (pstPubAttr->stWndRect.s32Y > pstPubAttr->stWndRect.u32Height - 240))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Image Y:%d!\n", pstPubAttr->stWndRect.s32Y);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstPubAttr->f32FrameRate <= 0.0) || (pstPubAttr->f32FrameRate > 255.0))
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid FrameRate:%f!\n", pstPubAttr->f32FrameRate);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstPubAttr->enBayer >= BAYER_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Bayer Pattern:%d!\n", pstPubAttr->enBayer);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_top_res_switch_write(HI_FALSE);
	//modify by qlp 图像高（减1），如1920图像高度，配1919
    //hi_isp_top_active_width_write(pstPubAttr->stWndRect.u32Width-1);
    //hi_isp_top_active_height_write(pstPubAttr->stWndRect.u32Height-1);
    //hi_isp_top_rggb_start_write((HI_U8)pstPubAttr->enBayer);

	hi_isp_crop_en_write(HI_TRUE);
	hi_isp_crop_x_start_write(pstPubAttr->stWndRect.s32X);
	hi_isp_crop_y_start_write(pstPubAttr->stWndRect.s32Y);
	hi_isp_crop_width_write(pstPubAttr->stWndRect.u32Width-1);
	hi_isp_crop_height_write(pstPubAttr->stWndRect.u32Height-1);
	hi_isp_wch_y_width_write(pstPubAttr->stWndRect.u32Width-1);
	hi_isp_wch_y_height_write(pstPubAttr->stWndRect.u32Height-1);
	hi_isp_wch_c_width_write(pstPubAttr->stWndRect.u32Width-1);
	hi_isp_wch_c_height_write(pstPubAttr->stWndRect.u32Height-1);

	hi_isp_yuv444_width_write(pstPubAttr->stWndRect.u32Width-1);
	hi_isp_yuv444_height_write(pstPubAttr->stWndRect.u32Height-1);
	hi_isp_yuv422_width_write(pstPubAttr->stWndRect.u32Width-1);
	hi_isp_yuv422_height_write(pstPubAttr->stWndRect.u32Height-1);

    hi_ext_sync_total_width_write(pstPubAttr->stWndRect.u32Width);
    hi_ext_sync_total_height_write(pstPubAttr->stWndRect.u32Height);

	hi_isp_yuv444_rggb_start_write((HI_U8)pstPubAttr->enBayer);
	hi_isp_yuv422_rggb_start_write((HI_U8)pstPubAttr->enBayer);

	hi_ext_system_isp_pixel_format_write(HI_ISP_PIXEL_FORMAT_420_DEFAULT);
    hi_isp_pix_format_write(HI_ISP_PIXEL_FORMAT_420_DEFAULT);

    pValue = (HI_VOID *)(&pstPubAttr->f32FrameRate);
    hi_ext_system_fps_base_write(*(HI_U32 *)pValue);

	hi_isp_yuv444_timing_stat_write(HI_ISP_CPI_FIX_TIMING_STAT);
	hi_isp_yuv422_timing_stat_write(HI_ISP_CPI_FIX_TIMING_STAT);

	hi_isp_ch_ctrl_en_write(HI_TRUE);

    pstIspCtx->stIspParaRec.bPubCfg = HI_TRUE;
    hi_ext_top_pub_attr_cfg_write(pstIspCtx->stIspParaRec.bPubCfg);
   
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetPubAttr(ISP_DEV IspDev, ISP_PUB_ATTR_S *pstPubAttr)
{
    HI_U8 u8Bayer;
    HI_U32 u32Value = 0;
    HI_VOID *pValue = HI_NULL;
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstPubAttr);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u8Bayer = hi_isp_yuv444_rggb_start_read();
    if (u8Bayer >= BAYER_BUTT)
    {
        pstPubAttr->enBayer = BAYER_BUTT;
    }
    else
    {
        pstPubAttr->enBayer = u8Bayer;
    }
    
    u32Value = hi_ext_system_fps_base_read();
    pValue = (HI_VOID *)&u32Value;
    pstPubAttr->f32FrameRate = *(HI_FLOAT *)pValue;

    pstPubAttr->stWndRect.s32X      = hi_isp_crop_x_start_read();
    pstPubAttr->stWndRect.u32Width  = hi_isp_yuv444_width_read()+1;
    pstPubAttr->stWndRect.s32Y      = hi_isp_crop_y_start_read();
    pstPubAttr->stWndRect.u32Height = hi_isp_yuv444_height_read()+1;
	
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetFMWState(ISP_DEV IspDev, const ISP_FMW_STATE_E enState)
{
    ISP_CHECK_DEV(IspDev);

    if (enState >= ISP_FMW_STATE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid firmware state %d in %s!\n", enState, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_freeze_firmware_write((HI_U8)enState);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetFMWState(ISP_DEV IspDev, ISP_FMW_STATE_E *penState)
{
    HI_U8 u8FMWState;
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(penState);

    u8FMWState = hi_ext_system_freeze_firmware_read();

    if (u8FMWState >=  ISP_FMW_STATE_BUTT)
    {
        *penState = ISP_FMW_STATE_BUTT;
    }
    else
    {
        *penState = u8FMWState;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetWDRMode(ISP_DEV IspDev, const ISP_WDR_MODE_S *pstWDRMode)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstWDRMode);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);
    
    if (pstWDRMode->enWDRMode >= WDR_MODE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid WDR mode %d in %s!\n", pstWDRMode->enWDRMode, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_top_wdr_switch_write(HI_FALSE);

    pstIspCtx->stIspParaRec.bWDRCfg = HI_TRUE;
    hi_ext_top_wdr_cfg_write(pstIspCtx->stIspParaRec.bWDRCfg);

    if ((HI_U8)pstWDRMode->enWDRMode == hi_ext_system_sensor_wdr_mode_read())
    {
        hi_ext_top_wdr_switch_write(HI_TRUE);
        return HI_SUCCESS;
    }
    
    hi_ext_system_sensor_wdr_mode_write((HI_U8)pstWDRMode->enWDRMode);
  
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetWDRMode(ISP_DEV IspDev, ISP_WDR_MODE_S *pstWDRMode)
{
    HI_U8 u8WDRmode;
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstWDRMode);
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    u8WDRmode = hi_ext_system_sensor_wdr_mode_read();
    if (u8WDRmode >= WDR_MODE_BUTT)
    {
        pstWDRMode->enWDRMode = WDR_MODE_BUTT;
    }
    else
    {
        pstWDRMode->enWDRMode = u8WDRmode;
    }   
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetModuleControl(ISP_DEV IspDev, const ISP_MODULE_CTRL_U *punModCtrl)
{
    HI_U32 u32Key = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(punModCtrl);

    u32Key = punModCtrl->u32Key;

	hi_ext_system_isp_bit_bypass_write(u32Key);	
	hi_isp_colorbar_cfg_write(!(u32Key&0x1));
	hi_ext_system_isp_balancefe_enable_write(!((u32Key >> 1)&0x1));
	hi_isp_dg_enable_write(!((u32Key >> 2)&0x1));
	hi_isp_gamma_fe_enable_write(!((u32Key >> 3)&0x1));
	hi_ext_system_ge_enable_write(!((u32Key >> 4)&0x1));
	hi_ext_system_dpc_dynamic_cor_enable_write(!((u32Key >> 5)&0x1));
	hi_isp_2dnr_cfg_enable_write(!((u32Key >> 6)&0x1));
	hi_isp_dehaze_en_write(!((u32Key >> 7)&0x1));
    hi_isp_wb_enable_write(!((u32Key >> 8)&0x1));
	hi_isp_lsc_cfg_enable_write(!((u32Key >> 9)&0x1));
	hi_isp_acm_enable_write(!((u32Key >> 10)&0x1));
	hi_isp_drc_enable_write(!((u32Key >> 11)&0x1));
	hi_isp_demosaic_cfg_enable_write(!((u32Key >> 12)&0x1));
	hi_isp_cc_enable_write(!((u32Key >> 13)&0x1));
	hi_isp_gamma_enable_write(!((u32Key >> 14)&0x1));
	hi_ext_system_fswdr_enable_write(!((u32Key >> 15)&0x1));
	hi_ext_system_gammafe_position_write(!((u32Key >> 16)&0x1));
	hi_ext_system_top_bit_reserve3_write((u32Key >> 17)&0x3);
	hi_isp_csc_enable_write(!((u32Key >> 19)&0x1));
    hi_ext_system_top_bit_reserve4_write((u32Key >> 20)&0x3);
	hi_isp_sharpen_cfg_enable_write(!((u32Key >> 22)&0x1));
	hi_ext_system_uvnr_enable_write(!((u32Key >> 23)&0x1));
	hi_ext_system_isp_chnswitch_write(!((u32Key >> 24)&0x1));
	hi_ext_system_isp_bypassmode_write((u32Key >> 25)&0x3);
	hi_isp_rgbir_en_write(!((u32Key >> 27)&0x01));
	hi_ext_system_isp_bypassall_write(!((u32Key >> 28)&0x01));
	hi_ext_system_top_bit_reserve5_write((u32Key >> 29)&0x7);
	
	

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetModuleControl(ISP_DEV IspDev, ISP_MODULE_CTRL_U *punModCtrl)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(punModCtrl);

	punModCtrl->bitBypassVideoTest = !(hi_isp_colorbar_cfg_read());
	punModCtrl->bitBypassBalanceFe = !(hi_ext_system_isp_balancefe_enable_read());
    punModCtrl->bitBypassISPDGain = !(hi_isp_dg_enable_read());
	punModCtrl->bitBypassGammaFe = !(hi_isp_gamma_fe_enable_read());
	punModCtrl->bitBypassCrosstalkR = !(hi_ext_system_ge_enable_read());
	punModCtrl->bitBypassDPC = !(hi_ext_system_dpc_dynamic_cor_enable_read());
	punModCtrl->bitBypassNR = !(hi_isp_2dnr_cfg_enable_read());
	punModCtrl->bitBypassDehaze = !(hi_isp_dehaze_en_read());
    punModCtrl->bitBypassWBGain = !(hi_isp_wb_enable_read());
    punModCtrl->bitBypassShading = !(hi_isp_lsc_cfg_enable_read());
    punModCtrl->bitBypassACM = !(hi_isp_acm_enable_read());
    punModCtrl->bitBypassDRC = !(hi_isp_drc_enable_read());
    punModCtrl->bitBypassDemosaic = !(hi_isp_demosaic_cfg_enable_read());
    punModCtrl->bitBypassColorMatrix = !(hi_isp_cc_enable_read());
    punModCtrl->bitBypassGamma = !(hi_isp_gamma_enable_read());
    punModCtrl->bitBypassCsConv = !(hi_isp_csc_enable_read());
    punModCtrl->bitBypassSharpen = !(hi_isp_sharpen_cfg_enable_read());
    punModCtrl->bitBypassUVNR = !(hi_ext_system_uvnr_enable_read());
	punModCtrl->bitBypassFSWDR = !(hi_ext_system_fswdr_enable_read());
	punModCtrl->bitGammaFePosition = !(hi_ext_system_gammafe_position_read());
	punModCtrl->bit2Rsv3 = hi_ext_system_top_bit_reserve3_read();
	punModCtrl->bit2Rsv4 = hi_ext_system_top_bit_reserve4_read();
	punModCtrl->bit5Rsv5 = hi_ext_system_top_bit_reserve5_read();
	punModCtrl->bit2BypassMode = hi_ext_system_isp_bypassmode_read();
	punModCtrl->bitBypassAll = !(hi_ext_system_isp_bypassall_read());
	punModCtrl->bitBypassRGBIR = !(hi_isp_rgbir_en_read());
	punModCtrl->bitChnSwitch = !(hi_ext_system_isp_chnswitch_read());
	
	
 
    return HI_SUCCESS;
}

/* General Function Settings */
HI_S32 HI_MPI_ISP_SetDRCAttr(ISP_DEV IspDev, const ISP_DRC_ATTR_S *pstDRC)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDRC);
    ISP_CHECK_BOOL(pstDRC->bEnable);

    if (pstDRC->u16BrightGainLmt > 0xA0)
    {
        printf("Invalid u16BrightGainLmt Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	 if (pstDRC->u16DarkGainLmtC > 0x85)
    {
        printf("Invalid u16DarkGainLmtC Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	  if (pstDRC->u16DarkGainLmtY> 0x85)
    {
        printf("Invalid u16DarkGainLmtY Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstDRC->u8Asymmetry > 0x1E)||(pstDRC->u8Asymmetry < 0x1))
    {
        printf("Invalid u8Asymmetry Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstDRC->u8LocalMixingThres > 0xA) || (pstDRC->u8LocalMixingThres < 0x2))
    {
        printf("Invalid u8LocalMixingThres Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDRC->u8RangeVar > 0xF)
    {
        printf("Invalid u8RangeVar Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	if ((pstDRC->u8SecondPole > 0xD2)||(pstDRC->u8SecondPole < 0x96))
    {
        printf("Invalid u8SecondPole Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	if (pstDRC->u8SpatialVar > 0xF)
    {
        printf("Invalid u8SpatialVar Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	if ((pstDRC->u8Stretch > 0x3C) ||(pstDRC->u8Stretch < 0x1E))
    {
        printf("Invalid u8Stretch Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    switch (pstDRC->enOpType)
    {
        case OP_TYPE_AUTO:
            if (pstDRC->stAuto.u8Strength > 0xFF)
            {
                ISP_TRACE(HI_DBG_ERR, "Invalid u8StrengthTarget Parameter Input in %s!\n", __FUNCTION__);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            if ((pstDRC->stAuto.u8LocalMixingBrigtht >0x80) ||( pstDRC->stAuto.u8LocalMixingBrigtht < 0x0))
            {
                printf("Invalid u8LocalMixingBrigtht Parameter Input!\n");
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            if ((pstDRC->stAuto.u8LocalMixingDark > 0x60) || (pstDRC->stAuto.u8LocalMixingDark < 0x0))
            {
                printf("Invalid u8LocalMixingDark Parameter Input!\n");
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            hi_ext_system_drc_manual_mode_write(0);
            hi_ext_system_drc_auto_strength_write(pstDRC->stAuto.u8Strength);
            hi_ext_system_drc_auto_localmixingbrigtht_write(pstDRC->stAuto.u8LocalMixingBrigtht);            
            hi_ext_system_drc_auto_localmixingdark_write(pstDRC->stAuto.u8LocalMixingDark);
            break;
        case OP_TYPE_MANUAL:
            if (pstDRC->stManual.u8Strength > 0xFF)
            {
                ISP_TRACE(HI_DBG_ERR, "Invalid u8Strength Parameter Input in %s!\n", __FUNCTION__);
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            if ((pstDRC->stManual.u8LocalMixingBrigtht >0x80) ||( pstDRC->stManual.u8LocalMixingBrigtht < 0x0))
            {
                printf("Invalid u8LocalMixingBrigtht Parameter Input!\n");
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            if ((pstDRC->stManual.u8LocalMixingDark > 0x60) || (pstDRC->stManual.u8LocalMixingDark < 0x0))
            {
                printf("Invalid u8LocalMixingDark Parameter Input!\n");
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
            hi_ext_system_drc_manual_mode_write(1);
            hi_ext_system_drc_manual_strength_write(pstDRC->stManual.u8Strength);
            hi_ext_system_drc_manual_localmixingbrigtht_write(pstDRC->stManual.u8LocalMixingBrigtht);            
            hi_ext_system_drc_manual_localmixingdark_write(pstDRC->stManual.u8LocalMixingDark);
            break;
        default:
            ISP_TRACE(HI_DBG_ERR, "Invalid DRC Op mode in %s!\n", __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
            break;
    }

    hi_isp_drc_enable_write(pstDRC->bEnable);

	hi_ext_system_drc_bright_gain_lmt_write(pstDRC->u16BrightGainLmt);
	hi_ext_system_drc_dark_gain_lmt_c_write(pstDRC->u16DarkGainLmtC);
	hi_ext_system_drc_dark_gain_lmt_y_write(pstDRC->u16DarkGainLmtY);

	hi_ext_system_drc_asymmetry_write(pstDRC->u8Asymmetry);
	hi_ext_system_drc_secondpole_write(pstDRC->u8SecondPole);
	hi_ext_system_drc_stretch_write(pstDRC->u8Stretch);

	hi_isp_drc_detail_mixing_thres_write(pstDRC->u8LocalMixingThres);

	hi_isp_drc_var_spatial_write(pstDRC->u8SpatialVar);
	hi_isp_drc_var_range_write(pstDRC->u8RangeVar);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDRCAttr(ISP_DEV IspDev, ISP_DRC_ATTR_S *pstDRC)
{    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDRC);

    pstDRC->enOpType = (ISP_OP_TYPE_E)hi_ext_system_drc_manual_mode_read();
    pstDRC->stAuto.u8Strength = hi_ext_system_drc_auto_strength_read();
    pstDRC->stAuto.u8LocalMixingBrigtht = hi_ext_system_drc_auto_localmixingbrigtht_read();
    pstDRC->stAuto.u8LocalMixingDark = hi_ext_system_drc_auto_localmixingdark_read();
 
    pstDRC->stManual.u8Strength = hi_ext_system_drc_manual_strength_read();
    pstDRC->stManual.u8LocalMixingBrigtht = hi_ext_system_drc_manual_localmixingbrigtht_read();
    pstDRC->stManual.u8LocalMixingDark = hi_ext_system_drc_manual_localmixingdark_read();
    pstDRC->bEnable = hi_isp_drc_enable_read();
	
    pstDRC->u16BrightGainLmt = hi_ext_system_drc_bright_gain_lmt_read();
	pstDRC->u16DarkGainLmtC  = hi_ext_system_drc_dark_gain_lmt_c_read();
	pstDRC->u16DarkGainLmtY = hi_ext_system_drc_dark_gain_lmt_y_read();

	pstDRC->u8Asymmetry = hi_ext_system_drc_asymmetry_read();
	pstDRC->u8SecondPole = hi_ext_system_drc_secondpole_read();
	pstDRC->u8Stretch = hi_ext_system_drc_stretch_read();

	pstDRC->u8LocalMixingThres = hi_isp_drc_detail_mixing_thres_read();

	pstDRC->u8RangeVar = hi_isp_drc_var_range_read();
	pstDRC->u8SpatialVar = hi_isp_drc_var_spatial_read();
	
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetFSWDRAttr(ISP_DEV IspDev, const ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFSWDRAttr);
    ISP_CHECK_BOOL(pstFSWDRAttr->bMotionComp);

    if (pstFSWDRAttr->u16ShortThresh > 0xFFF)
    {
        printf("Invalid u16ShortThresh %d in %s!\n", pstFSWDRAttr->u16ShortThresh, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->u16LongThresh > 0xFFF)
    {
        printf("Invalid u16LongThresh %d in %s!\n", pstFSWDRAttr->u16LongThresh, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->u16LongThresh > pstFSWDRAttr->u16ShortThresh)
    {
        printf("u16LongThresh should NOT be larger than u16ShortThresh in %s!\n", __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->enFSWDRComMode >= FS_WDR_COMBINE_BUTT)
    {
        printf("Invalid enFSWDRComMode %d in %s!\n", pstFSWDRAttr->enFSWDRComMode, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstFSWDRAttr->enExpRatioType >= OP_TYPE_BUTT)
    {
        printf("Invalid enExpRatioType %d in %s!\n", pstFSWDRAttr->enExpRatioType, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    if (pstFSWDRAttr->enExpRatioType)
    {
        if ((pstFSWDRAttr->u32ExpRatio < 0x40) || (pstFSWDRAttr->u32ExpRatio > 0x400))
        {
            printf("Invalid u32ExpRatio %d in %s!\n", pstFSWDRAttr->u32ExpRatio, __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM; 
        }
        hi_ext_wdr_manual_exp_ratio_enable_write(1);
        hi_ext_wdr_exp_ratio_target_write(pstFSWDRAttr->u32ExpRatio);
    }
    else 
    {
        hi_ext_wdr_manual_exp_ratio_enable_write(0);
    }

    hi_isp_wdr_use_stitching_error_write(pstFSWDRAttr->bMotionComp);
    hi_isp_wdr_short_thresh_write(pstFSWDRAttr->u16ShortThresh);
    hi_isp_wdr_long_thresh_write(pstFSWDRAttr->u16LongThresh);
    hi_isp_wdr_use_long_override_write(pstFSWDRAttr->enFSWDRComMode); 

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetFSWDRAttr(ISP_DEV IspDev, ISP_WDR_FS_ATTR_S *pstFSWDRAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFSWDRAttr);

    pstFSWDRAttr->bMotionComp = hi_isp_wdr_use_stitching_error_read();
    pstFSWDRAttr->u16ShortThresh = hi_isp_wdr_short_thresh_read();
    pstFSWDRAttr->u16LongThresh = hi_isp_wdr_long_thresh_read();
    pstFSWDRAttr->enFSWDRComMode = hi_isp_wdr_use_long_override_read();
    pstFSWDRAttr->enExpRatioType = hi_ext_wdr_manual_exp_ratio_enable_read();
    pstFSWDRAttr->u32ExpRatio = hi_ext_wdr_exp_ratio_target_read();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDPCalibrate(ISP_DEV IspDev, const ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDPCalibrate);
    ISP_CHECK_BOOL(pstDPCalibrate->bEnableDetect);


    if (pstDPCalibrate->u16CountMax > 0x800)
    {
        printf("Invalid u16CountMax Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPCalibrate->u16CountMin > pstDPCalibrate->u16CountMax)
    {
        printf("Invalid u16CountMin Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDPCalibrate->u16TimeLimit > 0x640)
    {
        printf("Invalid u16TimeLimit Parameter Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    if(pstDPCalibrate->enStaticDPType >= ISP_STATIC_DP_BUTT)
    {
        printf("Invalid  enStaticDPType Input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if(0 == pstDPCalibrate->u8StartThresh)
    {
        printf("u8StartThresh  should be bigger than 0!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }    
    
    hi_ext_system_dpc_static_calib_enable_write(pstDPCalibrate->bEnableDetect);// enable static calibration

    hi_ext_system_dpc_static_defect_type_write(pstDPCalibrate->enStaticDPType);
    hi_ext_system_dpc_start_thresh_write(pstDPCalibrate->u8StartThresh);
    hi_ext_system_dpc_count_max_write(pstDPCalibrate->u16CountMax);
    hi_ext_system_dpc_count_min_write(pstDPCalibrate->u16CountMin);
    hi_ext_system_dpc_trigger_time_write(pstDPCalibrate->u16TimeLimit);
    hi_ext_system_dpc_trigger_status_write(ISP_STATE_INIT);

    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_GetDPCalibrate(ISP_DEV IspDev, ISP_DP_STATIC_CALIBRATE_S *pstDPCalibrate)
{
	HI_U16 i;
    ISP_CTX_S *pstIspCtx = HI_NULL;
   
    ISP_GET_CTX(IspDev, pstIspCtx);
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDPCalibrate);

	pstDPCalibrate->bEnableDetect = hi_ext_system_dpc_static_calib_enable_read();

	pstDPCalibrate->enStaticDPType = hi_ext_system_dpc_static_defect_type_read();
	pstDPCalibrate->u8StartThresh = hi_ext_system_dpc_start_thresh_read();
	pstDPCalibrate->u16CountMax = hi_ext_system_dpc_count_max_read();
	pstDPCalibrate->u16CountMin = hi_ext_system_dpc_count_min_read();
	pstDPCalibrate->u16TimeLimit = hi_ext_system_dpc_trigger_time_read();

	pstDPCalibrate->enStatus = hi_ext_system_dpc_trigger_status_read();
	pstDPCalibrate->u8FinishThresh = hi_ext_system_dpc_finish_thresh_read();
	pstDPCalibrate->u16Count = hi_isp_dp_bpt_calib_number_read();

	if(pstDPCalibrate->enStatus == ISP_STATE_INIT)//the calibration process is still under processing
	{
		for(i=0; i<STATIC_DP_COUNT_MAX; i++)
		{		
			 pstDPCalibrate->au32Table[i] = 0;       
		}
	}
	else//the calibration process is finished
	{
		//should bypass DPC module when get defect pixel information
		pthread_mutex_lock(&pstIspCtx->stLock);
		hi_isp_dpc_enable_write(HI_FALSE);
        hi_isp_dp_bpt_raddr_write(0);
		usleep(200000);
		
		for(i=0; i<STATIC_DP_COUNT_MAX; i++)
		{
			if(i < pstDPCalibrate->u16Count)
			{
				pstDPCalibrate->au32Table[i] = hi_isp_dp_bpt_rdata_read();    
			}
			else
			{
				pstDPCalibrate->au32Table[i] = 0;
			}       
		}
		hi_isp_dpc_enable_write(HI_TRUE);
		pthread_mutex_unlock(&pstIspCtx->stLock);
		usleep(200000);
	}	
	return HI_SUCCESS;
}



HI_S32 HI_MPI_ISP_SetDPStaticAttr(ISP_DEV IspDev, const ISP_DP_STATIC_ATTR_S *pstDPStaticAttr)
{
	HI_U16 i = 0, j = 0, m = 0, u16CountIn = 0;
	HI_U32 au32DefectPixelTable[STATIC_DP_COUNT_MAX];
	ISP_CTX_S *pstIspCtx = HI_NULL;
   
    ISP_GET_CTX(IspDev, pstIspCtx);
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDPStaticAttr);	
	ISP_CHECK_BOOL(pstDPStaticAttr->bEnable);
	ISP_CHECK_BOOL(pstDPStaticAttr->bShow);
	
	
	if (pstDPStaticAttr->u16BrightCount > STATIC_DP_COUNT_MAX)
	{
		printf("StaticAttr u16BrightCount should be less  than STATIC_DP_COUNT_MAX!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	if (pstDPStaticAttr->u16DarkCount> STATIC_DP_COUNT_MAX)
	{
		printf("StaticAttr u16DarkCount should be less  than STATIC_DP_COUNT_MAX!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}


	/* merging dark talbe and bright table */
	while ((i <  pstDPStaticAttr->u16BrightCount) && (j < pstDPStaticAttr->u16DarkCount))
	{
		if (m >= STATIC_DP_COUNT_MAX)
		{
		    printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
		    return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if (pstDPStaticAttr->au32BrightTable[i] > pstDPStaticAttr->au32DarkTable[j])
		{
		    au32DefectPixelTable[m++] = pstDPStaticAttr->au32DarkTable[j++];
		}
		else if (pstDPStaticAttr->au32BrightTable[i] < pstDPStaticAttr->au32DarkTable[j])
		{
		    au32DefectPixelTable[m++] = pstDPStaticAttr->au32BrightTable[i++];
		}
		else
		{
		    au32DefectPixelTable[m++] = pstDPStaticAttr->au32BrightTable[i];
		    i++;
		    j++;
		}
	}

	if (i >=  pstDPStaticAttr->u16BrightCount)
	{
		while (j < pstDPStaticAttr->u16DarkCount)
		{
		    if (m >= STATIC_DP_COUNT_MAX)
		    {
		        printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
		        return HI_ERR_ISP_ILLEGAL_PARAM;
		    }
		    au32DefectPixelTable[m++] = pstDPStaticAttr->au32DarkTable[j++];
		}
	}
	if (j >=  pstDPStaticAttr->u16DarkCount)
	{
		while (i < pstDPStaticAttr->u16BrightCount)
		{
		    if (m >= STATIC_DP_COUNT_MAX)
		    {
		        printf("The size of merging DP table(BrightTable and DarkTable) is larger than STATIC_DP_COUNT_MAX!\n");
		        return HI_ERR_ISP_ILLEGAL_PARAM;
		    }
		    au32DefectPixelTable[m++] = pstDPStaticAttr->au32BrightTable[i++];
		}
	}

	u16CountIn = m;  
	hi_ext_system_dpc_static_cor_enable_write(pstDPStaticAttr->bEnable);
	hi_ext_system_dpc_static_dp_show_write(pstDPStaticAttr->bShow);	
	//should bypass DPC module when set defect pixel information
	pthread_mutex_lock(&pstIspCtx->stLock);
	hi_isp_dpc_enable_write(HI_FALSE);
	hi_isp_dp_bpt_waddr_write(0);
	hi_isp_dp_bpt_number_write(u16CountIn);
	usleep(200000);
	for(i = 0; i < STATIC_DP_COUNT_MAX; i++)
	{		
		if(i <u16CountIn )
		{
			hi_isp_dp_bpt_wdata_write(au32DefectPixelTable[i]);
		}
		else
		{
			hi_isp_dp_bpt_wdata_write(0);
		}
	}
	hi_isp_dpc_enable_write(HI_TRUE);
	pthread_mutex_unlock(&pstIspCtx->stLock);
	usleep(200000);
	return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_GetDPStaticAttr(ISP_DEV IspDev,  ISP_DP_STATIC_ATTR_S *pstDPStaticAttr)
{
	HI_U32 i = 0;
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDPStaticAttr);
	   
   
    ISP_GET_CTX(IspDev, pstIspCtx);
	pstDPStaticAttr->bEnable =  hi_ext_system_dpc_static_cor_enable_read();
	pstDPStaticAttr->bShow = hi_ext_system_dpc_static_dp_show_read();
	pstDPStaticAttr->u16BrightCount = hi_isp_dp_bpt_number_read();
	pstDPStaticAttr->u16DarkCount = 0;
	
	//should bypass DPC module when get defect pixel information
	pthread_mutex_lock(&pstIspCtx->stLock);
	hi_isp_dpc_enable_write(HI_FALSE);
	hi_isp_dp_bpt_raddr_write(0);
	usleep(200000);

	for(i = 0; i < STATIC_DP_COUNT_MAX; i++)
	{
		if(i < pstDPStaticAttr->u16BrightCount)
		{
			pstDPStaticAttr->au32BrightTable[i] = hi_isp_dp_bpt_rdata_read();
		}
		else
		{
			pstDPStaticAttr->au32BrightTable[i] = 0;
		}
	}
	hi_isp_dpc_enable_write(HI_TRUE);
	pthread_mutex_unlock(&pstIspCtx->stLock);
	
	usleep(200000);
	return HI_SUCCESS;
}
HI_S32 HI_MPI_ISP_SetDPDynamicAttr(ISP_DEV IspDev, const ISP_DP_DYNAMIC_ATTR_S *pstDPDynamicAttr)
{
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDPDynamicAttr);
	ISP_CHECK_BOOL(pstDPDynamicAttr->bEnable);

	if(pstDPDynamicAttr->enOpType >= OP_TYPE_BUTT)
	{
		printf("Invalid  enOpType Input!\n");
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
	
	hi_ext_system_dpc_dynamic_cor_enable_write(pstDPDynamicAttr->bEnable);

	if (OP_TYPE_AUTO == pstDPDynamicAttr->enOpType)
	{
		hi_ext_system_dpc_dynamic_manual_enable_write(pstDPDynamicAttr->enOpType);
		
		for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
		{	
			if(pstDPDynamicAttr->stAuto.au16Slope[i] > 255)
			{
				printf("Invalid Strength Input!\n");
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			if (pstDPDynamicAttr->stAuto.au16BlendRatio[i]>0x100)
			{
				printf("Invalid BlendRatio Input!\n" );
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			
			hi_ext_system_dpc_dynamic_strength_table_write(i,pstDPDynamicAttr->stAuto.au16Slope[i]);
			hi_ext_system_dpc_dynamic_blend_ratio_table_write(i,pstDPDynamicAttr->stAuto.au16BlendRatio[i]);
		}
	}
	else if(OP_TYPE_MANUAL == pstDPDynamicAttr->enOpType)
	{       
		if(pstDPDynamicAttr->stManual.u16Slope > 255)
		{
			printf("Invalid Strength Input!\n");
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		if (pstDPDynamicAttr->stManual.u16BlendRatio>0x100)
		{
			printf("Invalid BlendRatio Input!\n" );
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		hi_ext_system_dpc_dynamic_manual_enable_write(pstDPDynamicAttr->enOpType);
		hi_ext_system_dpc_dynamic_strength_write(pstDPDynamicAttr->stManual.u16Slope);
		hi_ext_system_dpc_dynamic_blend_ratio_write(pstDPDynamicAttr->stManual.u16BlendRatio);         
	}
        return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDPDynamicAttr(ISP_DEV IspDev,  ISP_DP_DYNAMIC_ATTR_S *pstDPDynamicAttr)
{
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstDPDynamicAttr);

	pstDPDynamicAttr->bEnable =hi_ext_system_dpc_dynamic_cor_enable_read ();
	pstDPDynamicAttr->enOpType = (ISP_OP_TYPE_E)hi_ext_system_dpc_dynamic_manual_enable_read();


	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
		pstDPDynamicAttr->stAuto.au16Slope[i] = hi_ext_system_dpc_dynamic_strength_table_read(i);
		pstDPDynamicAttr->stAuto.au16BlendRatio[i] = hi_ext_system_dpc_dynamic_blend_ratio_table_read(i);
	}

	pstDPDynamicAttr->stManual.u16Slope = hi_ext_system_dpc_dynamic_strength_read();
	pstDPDynamicAttr->stManual.u16BlendRatio = hi_ext_system_dpc_dynamic_blend_ratio_read();  

	return HI_SUCCESS;
}

extern HI_S32 VI_SetDISAttr(VI_DEV ViDev, const VI_DIS_ATTR_S *pstDISAttr);
HI_S32 HI_MPI_ISP_SetDISAttr(ISP_DEV IspDev, const ISP_DIS_ATTR_S *pstDISAttr)
{
    VI_DIS_ATTR_S stDisAttr = {0};
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDISAttr);
    ISP_CHECK_BOOL(pstDISAttr->bEnable);
    //hi_isp_dis_manual_control_write(!pstDISAttr->bEnable);

    stDisAttr.bEnable = pstDISAttr->bEnable;
    return VI_SetDISAttr(0, &stDisAttr);
}

extern HI_S32 VI_GetDISAttr(VI_DEV ViDev, VI_DIS_ATTR_S *pstDISAttr);
HI_S32 HI_MPI_ISP_GetDISAttr(ISP_DEV IspDev, ISP_DIS_ATTR_S *pstDISAttr)
{
    VI_DIS_ATTR_S stDisAttr = {0};
    HI_S32 s32Ret;
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDISAttr);

    //pstDISAttr->bEnable = !hi_isp_dis_manual_control_read();
    s32Ret = VI_GetDISAttr(0, &stDisAttr);
    pstDISAttr->bEnable = stDisAttr.bEnable;
    return s32Ret;
}

HI_S32 HI_MPI_ISP_SetMeshShadingAttr(ISP_DEV IspDev, const ISP_SHADING_ATTR_S *pstShadingAttr)
{
    HI_U16 i,j;
    HI_U32 u32Width, u32Height;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstShadingAttr);
    ISP_CHECK_BOOL(pstShadingAttr->bEnable);

    u32Width = hi_isp_yuv444_width_read()+1;
    u32Height = hi_isp_yuv444_height_read()+1;
    
	hi_isp_lsc_cfg_enable_write(pstShadingAttr->bEnable);

	if(pstShadingAttr->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid MeshShading type %d in %s!\n", pstShadingAttr->enOpType, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
	hi_ext_system_isp_mesh_shading_manu_mode_write(pstShadingAttr->enOpType);

    for (i = 0; i < SHADING_MESH_NUM; i++)
	{
	    hi_ext_system_isp_mesh_shading_noise_control_write(i, pstShadingAttr->au32NoiseControlGain[i] & 0x1fff);
		hi_ext_system_isp_mesh_shading_b_gain_write(i,pstShadingAttr->au32BGain[i] & 0x1fff);
		hi_ext_system_isp_mesh_shading_r_gain_write(i,pstShadingAttr->au32RGain[i] & 0x1fff);
		hi_ext_system_isp_mesh_shading_gb_gain_write(i,pstShadingAttr->au32GbGain[i] & 0x1fff);
		hi_ext_system_isp_mesh_shading_gr_gain_write(i,pstShadingAttr->au32GrGain[i] & 0x1fff);
	}

    for ( j = 0 ; j < 8 ; j++ )
    {
        if ( (pstShadingAttr->au32XGridWidth[j] > (u32Width/4)) || (pstShadingAttr->au32YGridWidth[j] > (u32Height/4)) )
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid MeshShading Block Width&Height value in %s!\n", __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        hi_ext_system_isp_mesh_shading_xgrid_write(j, pstShadingAttr->au32XGridWidth[j]);
        hi_ext_system_isp_mesh_shading_ygrid_write(j, pstShadingAttr->au32YGridWidth[j]);
    }
	hi_ext_system_isp_mesh_shading_updata_write(HI_TRUE);
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetMeshShadingAttr(ISP_DEV IspDev, ISP_SHADING_ATTR_S *pstShadingAttr)
{
	HI_U16 i, j;
    ISP_CHECK_DEV(IspDev);    
    ISP_CHECK_POINTER(pstShadingAttr);
    pstShadingAttr->bEnable = hi_isp_lsc_cfg_enable_read();
	pstShadingAttr->enOpType = hi_ext_system_isp_mesh_shading_manu_mode_read();
	
    for (i = 0; i < SHADING_MESH_NUM; i++)
	{
	    pstShadingAttr->au32NoiseControlGain[i] = hi_ext_system_isp_mesh_shading_noise_control_read(i);
		pstShadingAttr->au32BGain[i] = hi_ext_system_isp_mesh_shading_b_gain_read(i);
		pstShadingAttr->au32RGain[i] = hi_ext_system_isp_mesh_shading_r_gain_read(i);
		pstShadingAttr->au32GbGain[i]= hi_ext_system_isp_mesh_shading_gb_gain_read(i);
		pstShadingAttr->au32GrGain[i]= hi_ext_system_isp_mesh_shading_gr_gain_read(i);
	}

    for ( j = 0 ; j < 8 ; j++ )
    {
        pstShadingAttr->au32XGridWidth[j] = hi_ext_system_isp_mesh_shading_xgrid_read(j);
        pstShadingAttr->au32YGridWidth[j] = hi_ext_system_isp_mesh_shading_ygrid_read(j);
    }
    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_SetNRAttr(ISP_DEV IspDev, const ISP_NR_ATTR_S *pstNRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNRAttr);
    ISP_CHECK_BOOL(pstNRAttr->bEnable);

    if(pstNRAttr->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid NR type %d in %s!\n", pstNRAttr->enOpType, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

	if(pstNRAttr->stManual.u8LowFreqSlope > 0x10)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid NR Manual LowFreqSlope %d in %s!\n", pstNRAttr->stManual.u8LowFreqSlope, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    if (OP_TYPE_AUTO == pstNRAttr->enOpType)
    {
        for(i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
        {
        	if(pstNRAttr->stAuto.au8LowFreqSlope[i] > 0x10)
		    {
		        ISP_TRACE(HI_DBG_ERR, "Invalid NR Manual LowFreqSlope %d in %s!\n", pstNRAttr->stAuto.au8LowFreqSlope[i], __FUNCTION__);
		        return HI_ERR_ISP_ILLEGAL_PARAM;
		    }
            hi_ext_system_nr_varstrength_write(i, pstNRAttr->stAuto.au8VarStrength[i]);
            hi_ext_system_nr_fixstrength_write(i, pstNRAttr->stAuto.au8FixStrength[i]);
            hi_ext_system_nr_lowfreqslope_write(i, pstNRAttr->stAuto.au8LowFreqSlope[i]);            
            hi_ext_system_nr_threshold_write(i, pstNRAttr->stAuto.au16Threshold[i]);
        }
    }
    else if (OP_TYPE_MANUAL == pstNRAttr->enOpType)
    {        
        hi_ext_system_nr_manual_varstrength_write(pstNRAttr->stManual.u8VarStrength);
        hi_ext_system_nr_manual_fixstrength_write(pstNRAttr->stManual.u8FixStrength);
        hi_ext_system_nr_manual_lowfreqslope_write(pstNRAttr->stManual.u8LowFreqSlope);        
        hi_ext_system_nr_manual_threshold_write(pstNRAttr->stManual.u16Threshold);
    }

	hi_isp_2dnr_cfg_enable_write(pstNRAttr->bEnable);
    hi_ext_system_nr_manu_mode_write(pstNRAttr->enOpType);
   
    return HI_SUCCESS;
}



HI_S32 HI_MPI_ISP_GetNRAttr(ISP_DEV IspDev, ISP_NR_ATTR_S *pstNRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstNRAttr);

    pstNRAttr->bEnable = hi_isp_2dnr_cfg_enable_read();
    pstNRAttr->enOpType = (ISP_OP_TYPE_E)hi_ext_system_nr_manu_mode_read();

    for(i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
    {
        pstNRAttr->stAuto.au8VarStrength[i] = hi_ext_system_nr_varstrength_read(i);
        pstNRAttr->stAuto.au8FixStrength[i] = hi_ext_system_nr_fixstrength_read(i);
        pstNRAttr->stAuto.au8LowFreqSlope[i] = hi_ext_system_nr_lowfreqslope_read(i);
        pstNRAttr->stAuto.au16Threshold[i] = hi_ext_system_nr_threshold_read(i);
    }
    
    pstNRAttr->stManual.u8VarStrength = hi_ext_system_nr_manual_varstrength_read();
    pstNRAttr->stManual.u8FixStrength= hi_ext_system_nr_manual_fixstrength_read();
    pstNRAttr->stManual.u8LowFreqSlope = hi_ext_system_nr_manual_lowfreqslope_read();
    pstNRAttr->stManual.u16Threshold = hi_ext_system_nr_manual_threshold_read();
   
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetGammaAttr(ISP_DEV IspDev, const ISP_GAMMA_ATTR_S *pstGammaAttr)
{
    HI_U32 i = 0;
    HI_U32 u32WDRMode;
    const HI_U16 *pu16GammaLut; 
    static HI_U16 au16GammaDef[GAMMA_NODE_NUM] = {
         0  ,120 ,220 ,310 ,390 ,470 ,540 ,610 ,670 ,730 ,786 ,842 ,894 ,944 ,994 ,1050,    
        1096,1138,1178,1218,1254,1280,1314,1346,1378,1408,1438,1467,1493,1519,1543,1568,    
        1592,1615,1638,1661,1683,1705,1726,1748,1769,1789,1810,1830,1849,1869,1888,1907,    
        1926,1945,1963,1981,1999,2017,2034,2052,2069,2086,2102,2119,2136,2152,2168,2184,    
        2200,2216,2231,2247,2262,2277,2292,2307,2322,2337,2351,2366,2380,2394,2408,2422,    
        2436,2450,2464,2477,2491,2504,2518,2531,2544,2557,2570,2583,2596,2609,2621,2634,    
        2646,2659,2671,2683,2696,2708,2720,2732,2744,2756,2767,2779,2791,2802,2814,2825,    
        2837,2848,2859,2871,2882,2893,2904,2915,2926,2937,2948,2959,2969,2980,2991,3001,    
        3012,3023,3033,3043,3054,3064,3074,3085,3095,3105,3115,3125,3135,3145,3155,3165,    
        3175,3185,3194,3204,3214,3224,3233,3243,3252,3262,3271,3281,3290,3300,3309,3318,    
        3327,3337,3346,3355,3364,3373,3382,3391,3400,3409,3418,3427,3436,3445,3454,3463,    
        3471,3480,3489,3498,3506,3515,3523,3532,3540,3549,3557,3566,3574,3583,3591,3600,    
        3608,3616,3624,3633,3641,3649,3657,3665,3674,3682,3690,3698,3706,3714,3722,3730,    
        3738,3746,3754,3762,3769,3777,3785,3793,3801,3808,3816,3824,3832,3839,3847,3855,    
        3862,3870,3877,3885,3892,3900,3907,3915,3922,3930,3937,3945,3952,3959,3967,3974,    
        3981,3989,3996,4003,4010,4018,4025,4032,4039,4046,4054,4061,4068,4075,4082,4089,4095
    };
    static HI_U16 au16GammasRGB[GAMMA_NODE_NUM] = {
        0,    203,  347,  452,  539,  613,  679,  739,  794,  846,  894,  939,  982,  1023,  1062,  1100,  
        1136, 1171, 1204, 1237, 1268, 1299, 1329, 1358, 1386, 1414, 1441, 1467, 1493, 1519,  1543,  1568, 
        1592, 1615, 1638, 1661, 1683, 1705, 1726, 1748, 1769, 1789, 1810, 1830, 1849, 1869,  1888,  1907, 
        1926, 1945, 1963, 1981, 1999, 2017, 2034, 2052, 2069, 2086, 2102, 2119, 2136, 2152,  2168,  2184,  
        2200, 2216, 2231, 2247, 2262, 2277, 2292, 2307, 2322, 2337, 2351, 2366, 2380, 2394,  2408,  2422,  
        2436, 2450, 2464, 2477, 2491, 2504, 2518, 2531, 2544, 2557, 2570, 2583, 2596, 2609,  2621,  2634,  
        2646, 2659, 2671, 2683, 2696, 2708, 2720, 2732, 2744, 2756, 2767, 2779, 2791, 2802,  2814,  2825,  
        2837, 2848, 2859, 2871, 2882, 2893, 2904, 2915, 2926, 2937, 2948, 2959, 2969, 2980,  2991,  3001, 
        3012, 3023, 3033, 3043, 3054, 3064, 3074, 3085, 3095, 3105, 3115, 3125, 3135, 3145,  3155,  3165, 
        3175, 3185, 3194, 3204, 3214, 3224, 3233, 3243, 3252, 3262, 3271, 3281, 3290, 3300,  3309,  3318, 
        3327, 3337, 3346, 3355, 3364, 3373, 3382, 3391, 3400, 3409, 3418, 3427, 3436, 3445,  3454,  3463, 
        3471, 3480, 3489, 3498, 3506, 3515, 3523, 3532, 3540, 3549, 3557, 3566, 3574, 3583,  3591,  3600, 
        3608, 3616, 3624, 3633, 3641, 3649, 3657, 3665, 3674, 3682, 3690, 3698, 3706, 3714,  3722,  3730,  
        3738, 3746, 3754, 3762, 3769, 3777, 3785, 3793, 3801, 3808, 3816, 3824, 3832, 3839,  3847,  3855,  
        3862, 3870, 3877, 3885, 3892, 3900, 3907, 3915, 3922, 3930, 3937, 3945, 3952, 3959,  3967,  3974, 
        3981, 3989, 3996, 4003, 4010, 4018, 4025, 4032, 4039, 4046, 4054, 4061, 4068, 4075,  4082,  4089, 4095
    };   

    static HI_U16 au16GammaDefWDR[GAMMA_NODE_NUM]={
       
           0,    16,    32,    48,    64,    80,    96,   112,   128,   144,   160,   176,   192,   208,   224,   240,
         256,   272,   288,   304,   320,   336,   352,   368,   384,   400,   416,   432,   448,   464,   480,   496,
         512,   528,   544,   560,   576,   592,   608,   624,   640,   656,   672,   688,   704,   720,   736,   752,
         768,   784,   800,   816,   832,   848,   864,   880,   896,   912,   928,   944,   960,   976,   992,  1008,
        1024,  1040,  1056,  1072,  1088,  1104,  1120,  1136,  1152,  1168,  1184,  1200,  1216,  1232,  1248,  1264,
        1280,  1296,  1312,  1328,  1344,  1360,  1376,  1392,  1408,  1424,  1440,  1456,  1472,  1488,  1504,  1520,
        1536,  1552,  1568,  1584,  1600,  1616,  1632,  1648,  1664,  1680,  1696,  1712,  1728,  1744,  1760,  1776,
        1792,  1808,  1824,  1840,  1856,  1872,  1888,  1904,  1920,  1936,  1952,  1968,  1984,  2000,  2016,  2032,
        2048,  2064,  2080,  2096,  2112,  2128,  2144,  2160,  2176,  2192,  2208,  2224,  2240,  2256,  2272,  2288,
        2304,  2320,  2336,  2352,  2368,  2384,  2400,  2416,  2432,  2448,  2464,  2480,  2496,  2512,  2528,  2544,
        2560,  2576,  2592,  2608,  2624,  2640,  2656,  2672,  2688,  2704,  2720,  2736,  2752,  2768,  2784,  2800,
        2816,  2832,  2848,  2864,  2880,  2896,  2912,  2928,  2944,  2960,  2976,  2992,  3008,  3024,  3040,  3056,
        3072,  3088,  3104,  3120,  3136,  3152,  3168,  3184,  3200,  3216,  3232,  3248,  3264,  3280,  3296,  3312,
        3328,  3344,  3360,  3376,  3392,  3408,  3424,  3440,  3456,  3472,  3488,  3504,  3520,  3536,  3552,  3568,
        3584,  3600,  3616,  3632,  3648,  3664,  3680,  3696,  3712,  3728,  3744,  3760,  3776,  3792,  3808,  3824,
        3840,  3856,  3872,  3888,  3904,  3920,  3936,  3952,  3968,  3984,  4000,  4016,  4032,  4048,  4064,  4080,
        4095
    };
    static HI_U16 au16GammasRGBWDR[GAMMA_NODE_NUM]={
           0,   1,   3,   7,  13,  20,  29,  40,  52,  65,  81,  98, 116, 136, 158, 181,
         203, 226, 248, 269, 291, 312, 334, 355, 376, 396, 417, 438, 458, 478, 499, 519,
         538, 558, 578, 598, 617, 637, 656, 675, 695, 714, 733, 752, 771, 789, 808, 827,
         845, 864, 883, 901, 919, 938, 956, 974, 992,1010,1028,1046,1064,1082,1100,1118,
        1136,1153,1171,1189,1206,1224,1241,1259,1276,1293,1311,1328,1345,1362,1379,1397,
        1414,1431,1448,1465,1482,1499,1515,1532,1549,1566,1583,1599,1616,1633,1649,1666,
        1683,1699,1716,1732,1749,1765,1781,1798,1814,1830,1847,1863,1879,1896,1912,1928,
        1944,1960,1976,1992,2008,2024,2041,2056,2072,2088,2104,2120,2136,2152,2168,2184,
        2199,2215,2231,2247,2262,2278,2294,2309,2325,2341,2356,2372,2387,2403,2418,2434,
        2449,2465,2480,2496,2511,2527,2542,2557,2573,2588,2603,2619,2634,2649,2664,2680,
        2695,2710,2725,2740,2756,2771,2786,2801,2816,2831,2846,2861,2876,2891,2906,2921,
        2936,2951,2966,2981,2996,3011,3026,3041,3056,3070,3085,3100,3115,3130,3145,3159,
        3174,3189,3204,3218,3233,3248,3262,3277,3292,3306,3321,3336,3350,3365,3379,3394,
        3409,3423,3438,3452,3467,3481,3496,3510,3525,3539,3554,3568,3582,3597,3611,3626,
        3640,3654,3669,3683,3697,3712,3726,3740,3755,3769,3783,3798,3812,3826,3840,3855,
        3869,3883,3897,3911,3926,3940,3954,3968,3982,3996,4010,4025,4039,4053,4067,4081,4095
        };
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaAttr);
    ISP_CHECK_BOOL(pstGammaAttr->bEnable);

    u32WDRMode = hi_ext_system_sensor_wdr_mode_read();

    switch (pstGammaAttr->enCurveType)
    {
        case ISP_GAMMA_CURVE_DEFAULT:
            if (0 == u32WDRMode)
            {
                pu16GammaLut = au16GammaDef;
            }
            else
            {
                pu16GammaLut = au16GammaDefWDR;
            }
            break;
        case ISP_GAMMA_CURVE_SRGB:
            if (0 == u32WDRMode)
            {
                pu16GammaLut = au16GammasRGB;
            }
            else
            {
                pu16GammaLut = au16GammasRGBWDR;
            }
            break;
        case ISP_GAMMA_CURVE_USER_DEFINE:
            pu16GammaLut = pstGammaAttr->u16Table;
            break;
        default:
            pu16GammaLut = HI_NULL;
            printf("Invalid ISP Gamma Curve Type %d in %s!\n", pstGammaAttr->enCurveType, __FUNCTION__);
            return HI_ERR_ISP_ILLEGAL_PARAM;
            break;
    }
    
    hi_isp_gamma_enable_write(pstGammaAttr->bEnable);

    hi_isp_gamma_waddr_write(0);
    for(i=0; i<GAMMA_NODE_NUM; i++)
    {
		hi_isp_gamma_wdata_write((HI_U32)(pu16GammaLut[i]));
    }    

	hi_isp_gamma_lut_update_write(1);
	
    hi_ext_system_gamma_curve_type_write(pstGammaAttr->enCurveType);
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetGammaAttr(ISP_DEV IspDev, ISP_GAMMA_ATTR_S *pstGammaAttr)
{
    HI_U32 i = 0;
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaAttr);

    pstGammaAttr->bEnable = hi_isp_gamma_enable_read();

    pstGammaAttr->enCurveType = hi_ext_system_gamma_curve_type_read();

	hi_isp_gamma_raddr_write(0);
	
    for(i=0; i<GAMMA_NODE_NUM; i++)
    {
        //pstGammaAttr->u16Table[i] = hi_gamma_rgb_mem_array_data_read(i);
		pstGammaAttr->u16Table[i] = (hi_isp_gamma_rdata_read());
    }    

    return HI_SUCCESS;   
}

HI_S32 HI_MPI_ISP_SetGammaFEAttr(ISP_DEV IspDev, const ISP_GAMMAFE_ATTR_S *pstGammaFEAttr)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaFEAttr);
    ISP_CHECK_BOOL(pstGammaFEAttr->bEnable);
    
	hi_isp_gamma_fe_enable_write(pstGammaFEAttr->bEnable);
    
    hi_isp_gamma_fe_waddr1_write(0);
	for (i = 0 ; i < GAMMA_FE0_LUT_SIZE; i++)
	{
	    hi_isp_gamma_fe_wdata0_write(pstGammaFEAttr->u16Table[i]);
	}

	hi_isp_gamma_fe_waddr0_write(0);
	for (i = 0 ; i < GAMMA_FE1_LUT_SIZE; i++)
	{
	    hi_isp_gamma_fe_wdata1_write(pstGammaFEAttr->u16Table[GAMMA_FE0_LUT_SIZE + i]);
	}

    return HI_SUCCESS;

}

HI_S32 HI_MPI_ISP_GetGammaFEAttr(ISP_DEV IspDev, ISP_GAMMAFE_ATTR_S *pstGammaFEAttr)
{
    HI_U32 i = 0;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstGammaFEAttr);

    pstGammaFEAttr->bEnable = hi_isp_gamma_fe_enable_read();

	hi_isp_gamma_fe_raddr1_write(0);
	for (i = 0 ; i < GAMMA_FE0_LUT_SIZE; i++)
	{
	    pstGammaFEAttr->u16Table[i] = hi_isp_gamma_fe_rdata0_read();
	}

	hi_isp_gamma_fe_raddr0_write(0);
	for (i = 0 ; i < GAMMA_FE1_LUT_SIZE; i++)
	{
	    pstGammaFEAttr->u16Table[GAMMA_FE0_LUT_SIZE + i] = hi_isp_gamma_fe_rdata1_read();
	}
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetSharpenAttr(ISP_DEV IspDev, const ISP_SHARPEN_ATTR_S *pstSharpenAttr)
{
 
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstSharpenAttr);
    ISP_CHECK_BOOL(pstSharpenAttr->bEnable);

	

    if(pstSharpenAttr->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Sharpen Type %d in %s!\n", pstSharpenAttr->enOpType, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	
    hi_isp_sharpen_cfg_enable_write(pstSharpenAttr->bEnable); 
	
	hi_ext_system_sharpen_manu_mode_write(pstSharpenAttr->enOpType);

	if (OP_TYPE_AUTO == pstSharpenAttr->enOpType)
	{
		
		for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
		{   ISP_CHECK_BOOL(pstSharpenAttr->stAuto.abEnLowLumaShoot[i]);
			hi_ext_system_sharpen_EnLowLumaShoot_write(i, pstSharpenAttr->stAuto.abEnLowLumaShoot[i]);
			hi_ext_system_sharpen_D_write(i, pstSharpenAttr->stAuto.au8SharpenD[i]);
			hi_ext_system_sharpen_Ud_write(i, pstSharpenAttr->stAuto.au8SharpenUd[i]);
			hi_ext_system_sharpen_overshootAmt_write(i, pstSharpenAttr->stAuto.au8OverShoot[i]);
			hi_ext_system_sharpen_undershootAmt_write(i, pstSharpenAttr->stAuto.au8UnderShoot[i]);
			hi_ext_system_sharpen_TextureNoiseThd_write(i, pstSharpenAttr->stAuto.au8TextureNoiseThd[i]);
			hi_ext_system_sharpen_EdgeNoiseThd_write(i, pstSharpenAttr->stAuto.au8EdgeNoiseThd[i]);
			
		}
	}
	else if(OP_TYPE_MANUAL == pstSharpenAttr->enOpType)
	{   
    	ISP_CHECK_BOOL(pstSharpenAttr->stManual.bEnLowLumaShoot);
		hi_ext_system_manual_sharpen_EnLowLumaShoot_write(pstSharpenAttr->stManual.bEnLowLumaShoot);
		hi_ext_system_manual_sharpen_D_write(pstSharpenAttr->stManual.u8SharpenD);
		hi_ext_system_manual_sharpen_Ud_write(pstSharpenAttr->stManual.u8SharpenUd);
		hi_ext_system_manual_sharpen_overshootAmt_write(pstSharpenAttr->stManual.u8OverShoot);
		hi_ext_system_manual_sharpen_undershootAmt_write(pstSharpenAttr->stManual.u8UnderShoot);   
		hi_ext_system_manual_sharpen_TextureNoiseThd_write(pstSharpenAttr->stManual.u8TextureNoiseThd);
		hi_ext_system_manual_sharpen_EdgeNoiseThd_write(pstSharpenAttr->stManual.u8EdgeNoiseThd);  
	}
	  
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetSharpenAttr(ISP_DEV IspDev, ISP_SHARPEN_ATTR_S *pstSharpenAttr)
{
 
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstSharpenAttr);

	pstSharpenAttr->bEnable = hi_isp_sharpen_cfg_enable_read(); 
    pstSharpenAttr->enOpType = (ISP_OP_TYPE_E)hi_ext_system_sharpen_manu_mode_read();

	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
		pstSharpenAttr->stAuto.abEnLowLumaShoot  [i] = hi_ext_system_sharpen_EnLowLumaShoot_read(i);
		pstSharpenAttr->stAuto.au8SharpenD  [i] = hi_ext_system_sharpen_D_read(i);
		pstSharpenAttr->stAuto.au8SharpenUd [i] = hi_ext_system_sharpen_Ud_read(i);
		pstSharpenAttr->stAuto.au8OverShoot [i] = hi_ext_system_sharpen_overshootAmt_read(i);
		pstSharpenAttr->stAuto.au8UnderShoot[i] = hi_ext_system_sharpen_undershootAmt_read(i);
		pstSharpenAttr->stAuto.au8TextureNoiseThd [i] = hi_ext_system_sharpen_TextureNoiseThd_read(i);
		pstSharpenAttr->stAuto.au8EdgeNoiseThd[i] = hi_ext_system_sharpen_EdgeNoiseThd_read(i);
	}
	pstSharpenAttr->stManual.bEnLowLumaShoot   = hi_ext_system_manual_sharpen_EnLowLumaShoot_read();
	pstSharpenAttr->stManual.u8SharpenD   = hi_ext_system_manual_sharpen_D_read();
	pstSharpenAttr->stManual.u8SharpenUd  = hi_ext_system_manual_sharpen_Ud_read();
	pstSharpenAttr->stManual.u8OverShoot  = hi_ext_system_manual_sharpen_overshootAmt_read();
	pstSharpenAttr->stManual.u8UnderShoot = hi_ext_system_manual_sharpen_undershootAmt_read();
	pstSharpenAttr->stManual.u8TextureNoiseThd  = hi_ext_system_manual_sharpen_TextureNoiseThd_read();
	pstSharpenAttr->stManual.u8EdgeNoiseThd = hi_ext_system_manual_sharpen_EdgeNoiseThd_read();

    return HI_SUCCESS;
}

HI_S32  HI_MPI_ISP_SetUVNRAttr(ISP_DEV IspDev, ISP_UVNR_ATTR_S *pstUvnrAttr)
{
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstUvnrAttr);
	ISP_CHECK_BOOL(pstUvnrAttr->bEnable);

	if(pstUvnrAttr->enOpType >= OP_TYPE_BUTT)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid UVNR Type %d in %s!\n", pstUvnrAttr->enOpType, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	hi_ext_system_uvnr_enable_write(pstUvnrAttr->bEnable); 
	hi_ext_system_uvnr_manu_mode_write(pstUvnrAttr->enOpType);
  
	if (OP_TYPE_AUTO == pstUvnrAttr->enOpType)
	{		
		for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
		{
			if (pstUvnrAttr->stAuto.au8ColorCast[i]> 3)
    			{
       	 		printf("Invalid au8ColorCast Parameter Input!\n");
        			return HI_ERR_ISP_ILLEGAL_PARAM;
    			}
			if (pstUvnrAttr->stAuto.au8UvnrThreshold[i]> 64)
    			{
       	 		printf("Invalid au8UvnrThreshold Parameter Input!\n");
        			return HI_ERR_ISP_ILLEGAL_PARAM;
    			}
			if (pstUvnrAttr->stAuto.au8UvnrStrength[i]> 34)
    			{
       	 		printf("Invalid au8UvnrStrength Parameter Input!\n");
        			return HI_ERR_ISP_ILLEGAL_PARAM;
    			}
			
			hi_ext_system_uvnr_color_cast_write(i, pstUvnrAttr->stAuto.au8ColorCast[i]);
			hi_ext_system_uvnr_threshold_write (i, pstUvnrAttr->stAuto.au8UvnrThreshold[i]);
			hi_ext_system_uvnr_strength_write  (i, pstUvnrAttr->stAuto.au8UvnrStrength[i]);
		}
	}
	else if(OP_TYPE_MANUAL == pstUvnrAttr->enOpType)
	{       
		if (pstUvnrAttr->stManual.u8ColorCast> 3)
    		{
       	 	printf("Invalid u8ColorCast Parameter Input!\n");
        		return HI_ERR_ISP_ILLEGAL_PARAM;
    		}
		if (pstUvnrAttr->stManual.u8UvnrThreshold> 64)
    		{
       	 	printf("Invalid u8UvnrThreshold Parameter Input!\n");
        		return HI_ERR_ISP_ILLEGAL_PARAM;
    		}
		if (pstUvnrAttr->stManual.u8UvnrStrength> 34)
    		{
       	 	printf("Invalid u8UvnrStrength Parameter Input!\n");
        		return HI_ERR_ISP_ILLEGAL_PARAM;
    		}
		hi_ext_system_manual_uvnr_color_cast_write(pstUvnrAttr->stManual.u8ColorCast);
		hi_ext_system_manual_uvnr_threshold_write(pstUvnrAttr->stManual.u8UvnrThreshold);
		hi_ext_system_manual_uvnr_strength_write(pstUvnrAttr->stManual.u8UvnrStrength);     
	}

	return HI_SUCCESS;

}

HI_S32  HI_MPI_ISP_GetUVNRAttr(ISP_DEV IspDev, ISP_UVNR_ATTR_S *pstUvnrAttr)
{
	HI_U8 i;
	ISP_CHECK_DEV(IspDev);
	ISP_CHECK_POINTER(pstUvnrAttr);
	
	pstUvnrAttr->bEnable  = hi_ext_system_uvnr_enable_read(); 
	pstUvnrAttr->enOpType = (ISP_OP_TYPE_E)hi_ext_system_uvnr_manu_mode_read();
  
  for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
		pstUvnrAttr->stAuto.au8ColorCast    [i] = hi_ext_system_uvnr_color_cast_read(i);
		pstUvnrAttr->stAuto.au8UvnrThreshold[i] = hi_ext_system_uvnr_threshold_read (i);
		pstUvnrAttr->stAuto.au8UvnrStrength [i] = hi_ext_system_uvnr_strength_read  (i);
	}

	pstUvnrAttr->stManual.u8ColorCast     = hi_ext_system_manual_uvnr_color_cast_read();
	pstUvnrAttr->stManual.u8UvnrThreshold = hi_ext_system_manual_uvnr_threshold_read ();
	pstUvnrAttr->stManual.u8UvnrStrength  = hi_ext_system_manual_uvnr_strength_read  ();

	return HI_SUCCESS;
}

/* Crosstalk Removal Strength */
HI_S32 HI_MPI_ISP_SetCrosstalkAttr(ISP_DEV IspDev, const ISP_CR_ATTR_S *pstCRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCRAttr);
    ISP_CHECK_BOOL(pstCRAttr->bEnable);

	if(pstCRAttr->u8Slope > 0xE)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid Crosstalk slope %d in %s!\n", pstCRAttr->u8Slope, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstCRAttr->u8Sensitivity > 0xE)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid Crosstalk Sensitivity %d in %s!\n", pstCRAttr->u8Sensitivity, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstCRAttr->u16SensiThreshold > 0x3FFF)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid Crosstalk SensiThreshold %d in %s!\n", pstCRAttr->u16SensiThreshold, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstCRAttr->u16Threshold > 0x3FFF)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid Crosstalk Threshold %d in %s!\n", pstCRAttr->u16Threshold, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
    
    hi_ext_system_ge_enable_write(pstCRAttr->bEnable);
    hi_ext_system_ge_threshold_write(pstCRAttr->u16Threshold);
    hi_ext_system_ge_slope_write(pstCRAttr->u8Slope);
    hi_ext_system_ge_sensitivity_write   (pstCRAttr->u8Sensitivity);
    hi_ext_system_ge_sensithreshold_write(pstCRAttr->u16SensiThreshold);

    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
    {
    	if(pstCRAttr->au16Strength[i] > 0x3FFF)
		{
			ISP_TRACE(HI_DBG_ERR, "Invalid Crosstalk Strength %d in %s!\n", pstCRAttr->au16Strength[i], __FUNCTION__);
			return HI_ERR_ISP_ILLEGAL_PARAM;
		}
		
        hi_ext_system_ge_strength_write(i, pstCRAttr->au16Strength[i]);
    }
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetCrosstalkAttr(ISP_DEV IspDev, ISP_CR_ATTR_S *pstCRAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCRAttr);
     
    pstCRAttr->bEnable           = hi_ext_system_ge_enable_read();
    pstCRAttr->u16Threshold      = hi_ext_system_ge_threshold_read();
    pstCRAttr->u8Slope          = hi_ext_system_ge_slope_read();
    pstCRAttr->u8Sensitivity    = hi_ext_system_ge_sensitivity_read();
    pstCRAttr->u16SensiThreshold = hi_ext_system_ge_sensithreshold_read();

    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
    {
        pstCRAttr->au16Strength[i] = hi_ext_system_ge_strength_read(i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetAntiFalseColorAttr(ISP_DEV IspDev, const ISP_ANTI_FALSECOLOR_S *pstAntiFC)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAntiFC);
	ISP_CHECK_BOOL(pstAntiFC->bEnable);

    if(pstAntiFC->enOpType >= OP_TYPE_BUTT)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid AntiFalseColor Type %d in %s!\n", pstAntiFC->enOpType, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}

	if(pstAntiFC->stManual.u8Strength > 0xff)
	{
		ISP_TRACE(HI_DBG_ERR, "Invalid AntiFalseColor Manual Strength %d in %s!\n", pstAntiFC->stManual.u8Strength, __FUNCTION__);
		return HI_ERR_ISP_ILLEGAL_PARAM;
	}
  
	if (OP_TYPE_AUTO == pstAntiFC->enOpType)
	{		
		for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
		{
			if(pstAntiFC->stAuto.au8Strength[i] > 0xff)
			{
				ISP_TRACE(HI_DBG_ERR, "Invalid AntiFalseColor Auto Strength %d in %s!\n", pstAntiFC->stAuto.au8Strength[i], __FUNCTION__);
				return HI_ERR_ISP_ILLEGAL_PARAM;
			}
			
			hi_ext_system_fcr_auto_strength_write(i, pstAntiFC->stAuto.au8Strength[i]);
			hi_ext_system_fcr_auto_threshold_write(i, pstAntiFC->stAuto.au8Threshold[i]);
		}
	}
	else if(OP_TYPE_MANUAL == pstAntiFC->enOpType)
	{       
		hi_ext_system_manual_fcr_strength_write(pstAntiFC->stManual.u8Strength);   		
		hi_ext_system_manual_fcr_threshold_write(pstAntiFC->stManual.u8Threshold);  
	}

	hi_ext_system_fcr_enable_write(pstAntiFC->bEnable); 
	hi_ext_system_fcr_manu_mode_write(pstAntiFC->enOpType);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetAntiFalseColorAttr(ISP_DEV IspDev, ISP_ANTI_FALSECOLOR_S *pstAntiFC)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAntiFC);

    pstAntiFC->bEnable  = hi_ext_system_fcr_enable_read(); 
	pstAntiFC->enOpType = hi_ext_system_fcr_manu_mode_read();
  
	pstAntiFC->stManual.u8Strength = hi_ext_system_manual_fcr_strength_read(); 
	pstAntiFC->stManual.u8Threshold= hi_ext_system_manual_fcr_threshold_read(); 
	    
	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
	    pstAntiFC->stAuto.au8Strength[i] = hi_ext_system_fcr_auto_strength_read(i);
	    pstAntiFC->stAuto.au8Threshold[i] = hi_ext_system_fcr_auto_threshold_read(i);
	}
	   
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDemosaicAttr(ISP_DEV IspDev, const ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDemosaicAttr);

	if (pstDemosaicAttr->u16VhSlope > 0xFF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic u16VhSlope %d in %s!\n", pstDemosaicAttr->u16VhSlope, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (pstDemosaicAttr->u16UuSlope > 0x3FF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic u16UuSlope %d in %s!\n", pstDemosaicAttr->u16UuSlope, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{   
        if (pstDemosaicAttr->au16NpOffset[i] > 0x3FF)
    	{
	        ISP_TRACE(HI_DBG_ERR, "Invalid Demosaic au16NpOffset[%d] %d in %s!\n", i, pstDemosaicAttr->au16NpOffset[i], __FUNCTION__);
	        return HI_ERR_ISP_ILLEGAL_PARAM;
    	}
	}
	
	hi_ext_system_demosaic_vhSlope_write(pstDemosaicAttr->u16VhSlope);
	hi_ext_system_demosaic_uuSlope_write(pstDemosaicAttr->u16UuSlope);
	hi_ext_system_demosaic_vhLimit_write(pstDemosaicAttr->u8VhLimit);
	
	hi_ext_system_demosaic_vhOffset_write(pstDemosaicAttr->u8VhOffset);    
    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{   
        hi_ext_system_demosaic_npOffset_write(i, pstDemosaicAttr->au16NpOffset[i]);	
	}

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDemosaicAttr(ISP_DEV IspDev, ISP_DEMOSAIC_ATTR_S *pstDemosaicAttr)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDemosaicAttr);

    pstDemosaicAttr->u16VhSlope = hi_ext_system_demosaic_vhSlope_read();
    pstDemosaicAttr->u16UuSlope = hi_ext_system_demosaic_uuSlope_read();
    pstDemosaicAttr->u8VhLimit = hi_ext_system_demosaic_vhLimit_read();
    pstDemosaicAttr->u8VhOffset = hi_ext_system_demosaic_vhOffset_read();

    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
    {
        pstDemosaicAttr->au16NpOffset[i] = hi_ext_system_demosaic_npOffset_read(i);
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetBlackLevelAttr(ISP_DEV IspDev, const ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstBlackLevel);

	if (pstBlackLevel->au16BlackLevel[0] > 0x7FF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Blacklevel %d in %s!\n", pstBlackLevel->au16BlackLevel[0], __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	if (pstBlackLevel->au16BlackLevel[1] > 0x7FF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Blacklevel %d in %s!\n", pstBlackLevel->au16BlackLevel[1], __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	if (pstBlackLevel->au16BlackLevel[2] > 0x7FF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Blacklevel %d in %s!\n", pstBlackLevel->au16BlackLevel[2], __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
	if (pstBlackLevel->au16BlackLevel[3] > 0x7FF)
    {
        ISP_TRACE(HI_DBG_ERR, "Invalid Blacklevel %d in %s!\n", pstBlackLevel->au16BlackLevel[3], __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    hi_ext_system_black_level_00_write(pstBlackLevel->au16BlackLevel[0]);
    hi_ext_system_black_level_01_write(pstBlackLevel->au16BlackLevel[1]);
    hi_ext_system_black_level_10_write(pstBlackLevel->au16BlackLevel[2]);
    hi_ext_system_black_level_11_write(pstBlackLevel->au16BlackLevel[3]);
    hi_ext_system_black_level_change_write((HI_U8)HI_TRUE);
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetBlackLevelAttr(ISP_DEV IspDev, ISP_BLACK_LEVEL_S *pstBlackLevel)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstBlackLevel);

    pstBlackLevel->au16BlackLevel[0] = hi_ext_system_black_level_00_read();
    pstBlackLevel->au16BlackLevel[1] = hi_ext_system_black_level_01_read();
    pstBlackLevel->au16BlackLevel[2] = hi_ext_system_black_level_10_read();
    pstBlackLevel->au16BlackLevel[3] = hi_ext_system_black_level_11_read();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_FPNCalibrate(ISP_DEV IspDev, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrateAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstCalibrateAttr);
   
    return ISP_SetCalibrateAttr(0, pstCalibrateAttr);
}

HI_S32 HI_MPI_ISP_SetFPNAttr(ISP_DEV IspDev, const ISP_FPN_ATTR_S *pstFPNAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFPNAttr);

    return ISP_SetCorrectionAttr(0, pstFPNAttr);
}

HI_S32 HI_MPI_ISP_GetFPNAttr(ISP_DEV IspDev, ISP_FPN_ATTR_S *pstFPNAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstFPNAttr);

    return ISP_GetCorrectionAttr(0, pstFPNAttr);
}

HI_S32 HI_MPI_ISP_SetDeFogAttr(ISP_DEV IspDev, const ISP_DEFOG_ATTR_S *pstDefogAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDefogAttr);
    ISP_CHECK_BOOL(pstDefogAttr->bEnable);
    if(pstDefogAttr->enOpType >= OP_TYPE_BUTT)
    {
        printf("Invalid Defog OpType!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if((pstDefogAttr->bEnable != HI_FALSE)&&(pstDefogAttr->bEnable != HI_TRUE))
    {
       printf("Invalid Defog Enable Input\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if((pstDefogAttr->u8HorizontalBlock > 16)||(pstDefogAttr->u8HorizontalBlock < 6))
    {
        printf("HorizonBlock should between 6 and 16!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if((pstDefogAttr->u8VerticalBlock > 15)||(pstDefogAttr->u8VerticalBlock < 6))
    {
        printf("VerticalBlock should between 6 and 15 !\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    hi_isp_dehaze_en_write(pstDefogAttr->bEnable);
    isp_ext_system_manual_dehaze_hblk_write(pstDefogAttr->u8HorizontalBlock);
  	isp_ext_system_manual_dehaze_vblk_write(pstDefogAttr->u8VerticalBlock);
    
    if(pstDefogAttr->enOpType == OP_TYPE_MANUAL)
    {
        isp_ext_system_manual_dehaze_enable_write(1);
    	isp_ext_system_manual_dehaze_strength_write(pstDefogAttr->stManual.u8strength);
    }
    else if(pstDefogAttr->enOpType == OP_TYPE_AUTO)
    {
        isp_ext_system_manual_dehaze_enable_write(0);
        isp_ext_system_manual_dehaze_autostrength_write(pstDefogAttr->stAuto.u8strength);
    }	

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetDeFogAttr(ISP_DEV IspDev, ISP_DEFOG_ATTR_S *pstDefogAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstDefogAttr);
    
    pstDefogAttr->bEnable = hi_isp_dehaze_en_read();  
    pstDefogAttr->u8HorizontalBlock = isp_ext_system_manual_dehaze_hblk_read();
    pstDefogAttr->u8VerticalBlock = isp_ext_system_manual_dehaze_vblk_read();
    pstDefogAttr->enOpType = (ISP_OP_TYPE_E)isp_ext_system_manual_dehaze_enable_read();
    pstDefogAttr->stManual.u8strength = isp_ext_system_manual_dehaze_strength_read();
    pstDefogAttr->stAuto.u8strength = isp_ext_system_manual_dehaze_autostrength_read();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetAcmAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmAttr);
    
    return ISP_ACM_SetAttr(pstAcmAttr);
}
HI_S32 HI_MPI_ISP_GetAcmAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmAttr);
    
    return ISP_ACM_GetAttr(pstAcmAttr);
}

HI_S32 HI_MPI_ISP_SetAcmCoeff(ISP_DEV IspDev, ISP_ACM_LUT_S *pstAcmLUT, ISP_ACM_MODE_E enMode)
{
    HI_S32 s32Ret;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmLUT);
    if (enMode >= ISP_ACM_MODE_BUTT)
    {
        printf("Invalid ACM Mode %d in %s!\n", enMode, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    s32Ret = ISP_ACM_SetCoeff(pstAcmLUT,enMode);
    return s32Ret;
}
HI_S32 HI_MPI_ISP_GetAcmCoeff(ISP_DEV IspDev, ISP_ACM_LUT_S *pstAcmLUT, ISP_ACM_MODE_E enMode)
{
    HI_S32 s32Ret;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstAcmLUT);
    s32Ret =  ISP_ACM_GetCoeff(pstAcmLUT,enMode);
    return s32Ret;
}

HI_S32 HI_MPI_ISP_QueryInnerStateInfo(ISP_DEV IspDev, ISP_INNER_STATE_INFO_S *pstInnerStateInfo)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstInnerStateInfo);
    pstInnerStateInfo->u32DRCStrengthActual = hi_ext_system_drc_auto_strength_read();

    pstInnerStateInfo->u32SharpenStrengthDActual = (hi_isp_edge_amt_read())>> 4;
    pstInnerStateInfo->u32SharpenStrengthUdActual = (hi_isp_sharp_amt_read())>> 4;
    pstInnerStateInfo->u32DefogStrengthActual = hi_isp_dehaze_gstrth_read();

    pstInnerStateInfo->bWDRSwitchFinish = hi_ext_top_wdr_switch_read();
    pstInnerStateInfo->bResSwitchFinish = hi_ext_top_res_switch_read();

    if (1 == hi_isp_rgbir_auto_gain_en_read())
    {
        pstInnerStateInfo->u32RgbirGain = hi_isp_rgbir_gain_use_read();
    }
    else
    {
        pstInnerStateInfo->u32RgbirGain = hi_isp_rgbir_manul_gain_read();
    }
 
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetRgbirAttr(ISP_DEV IspDev, ISP_RGBIR_ATTR_S *pstRgbirAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstRgbirAttr);

    if((pstRgbirAttr->bEnable != HI_FALSE)&&(pstRgbirAttr->bEnable != HI_TRUE))
    {
       printf("Invalid Rgbir Enable Input in !\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstRgbirAttr->enIrPosType >= ISP_IRPOS_TYPE_BUTT)
    {
       printf("Invalid Rgbir enIrPosType Input !\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstRgbirAttr->u16OverExpThresh > 0xfff)
    {
       printf("Invalid Rgbir u16OverExpThresh Input !\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_isp_rgbir_en_write(pstRgbirAttr->bEnable);
    hi_isp_rgbir_ir_position_write(pstRgbirAttr->enIrPosType);
    hi_isp_rgbir_exp_value_write(pstRgbirAttr->u16OverExpThresh);
        

    return HI_SUCCESS;
    
}

HI_S32 HI_MPI_ISP_GetRgbirAttr(ISP_DEV IspDev, ISP_RGBIR_ATTR_S *pstRgbirAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstRgbirAttr);

    pstRgbirAttr->bEnable = hi_isp_rgbir_en_read();
    pstRgbirAttr->enIrPosType = hi_isp_rgbir_ir_position_read();
    pstRgbirAttr->u16OverExpThresh = hi_isp_rgbir_exp_value_read();
    

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetRgbirCtrl(ISP_DEV IspDev, ISP_RGBIR_CTRL_S *pstRgbirCtrl)
{
    HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstRgbirCtrl);

    if((pstRgbirCtrl->bIrOutEn != HI_FALSE)&&(pstRgbirCtrl->bIrOutEn != HI_TRUE))
    {
       printf("Invalid Rgbir bIrOutEn Input !\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if((pstRgbirCtrl->bIrFilterEn != HI_FALSE)&&(pstRgbirCtrl->bIrFilterEn != HI_TRUE))
    {
       printf("Invalid Rgbir bIrFilterEn Input !\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if((pstRgbirCtrl->bRemovelEn != HI_FALSE)&&(pstRgbirCtrl->bRemovelEn != HI_TRUE))
    {
       printf("Invalid Rgbir bRemovelEn Input !\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if(pstRgbirCtrl->enCompType >= OP_TYPE_BUTT)
    {
       printf("Invalid Rgbir bAutoCompEn Input !\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if((pstRgbirCtrl->u16ManuGain > 0x3ff) || (pstRgbirCtrl->u16ManuGain < 0x100))
    {
       printf("Invalid Rgbir u16ManuGain Input in !\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    for(i=0;i<RGBIR_MATRIX_NUM;i++)
    {
        if((pstRgbirCtrl->as16ScaleCoef[i] > 511) || (pstRgbirCtrl->as16ScaleCoef[i] < -512))
        {
           printf("Invalid Rgbir as16ScaleCoef Input !\n");
           return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    hi_isp_rgbir_ir_out_en_write(pstRgbirCtrl->bIrOutEn);
    hi_isp_rgbir_ir_filter_en_write(pstRgbirCtrl->bIrFilterEn);
    if (pstRgbirCtrl->bRemovelEn)
    {
        hi_isp_rgbir_scale_b_b_write(pstRgbirCtrl->as16ScaleCoef[0]);
        hi_isp_rgbir_scale_b_g_write(pstRgbirCtrl->as16ScaleCoef[1]);
        hi_isp_rgbir_scale_b_r_write(pstRgbirCtrl->as16ScaleCoef[2]);
        hi_isp_rgbir_scale_g_b_write(pstRgbirCtrl->as16ScaleCoef[3]);
        hi_isp_rgbir_scale_g_g_write(pstRgbirCtrl->as16ScaleCoef[4]);
        hi_isp_rgbir_scale_g_r_write(pstRgbirCtrl->as16ScaleCoef[5]);
        hi_isp_rgbir_scale_r_b_write(pstRgbirCtrl->as16ScaleCoef[6]);
        hi_isp_rgbir_scale_r_g_write(pstRgbirCtrl->as16ScaleCoef[7]);
        hi_isp_rgbir_scale_r_r_write(pstRgbirCtrl->as16ScaleCoef[8]);
        hi_isp_rgbir_scale_ir_b_write(pstRgbirCtrl->as16ScaleCoef[9]);
        hi_isp_rgbir_scale_ir_g_write(pstRgbirCtrl->as16ScaleCoef[10]);
        hi_isp_rgbir_scale_ir_r_write(pstRgbirCtrl->as16ScaleCoef[11]);
        hi_isp_rgbir_scale_c_b_write(pstRgbirCtrl->as16ScaleCoef[12]);
        hi_isp_rgbir_scale_c_g_write(pstRgbirCtrl->as16ScaleCoef[13]);
        hi_isp_rgbir_scale_c_r_write(pstRgbirCtrl->as16ScaleCoef[14]);
    }
    else
    {
        hi_isp_rgbir_scale_b_b_write(256);
        hi_isp_rgbir_scale_b_g_write(0);
        hi_isp_rgbir_scale_b_r_write(0);
        hi_isp_rgbir_scale_g_b_write(0);
        hi_isp_rgbir_scale_g_g_write(256);
        hi_isp_rgbir_scale_g_r_write(0);
        hi_isp_rgbir_scale_r_b_write(0);
        hi_isp_rgbir_scale_r_g_write(0);
        hi_isp_rgbir_scale_r_r_write(256);
        hi_isp_rgbir_scale_ir_b_write(0);
        hi_isp_rgbir_scale_ir_g_write(0);
        hi_isp_rgbir_scale_ir_r_write(0);
        hi_isp_rgbir_scale_c_b_write(0);
        hi_isp_rgbir_scale_c_g_write(0);
        hi_isp_rgbir_scale_c_r_write(0);
    }
    hi_isp_rgbir_removel_en_write(1);
    hi_ext_system_isp_rgbir_removel_en_write(pstRgbirCtrl->bRemovelEn);
    if(OP_TYPE_AUTO == pstRgbirCtrl->enCompType)
    {
        hi_isp_rgbir_auto_gain_en_write(1);
        hi_isp_rgbir_manul_gain_en_write(0);
    }
    else if(OP_TYPE_MANUAL == pstRgbirCtrl->enCompType)
    {
        hi_isp_rgbir_auto_gain_en_write(0);
        hi_isp_rgbir_manul_gain_en_write(1);
    }
    
    hi_isp_rgbir_manul_gain_write(pstRgbirCtrl->u16ManuGain); 
    hi_ext_system_isp_rgbir_scale_b_b_write(pstRgbirCtrl->as16ScaleCoef[0]);
    hi_ext_system_isp_rgbir_scale_b_g_write(pstRgbirCtrl->as16ScaleCoef[1]);
    hi_ext_system_isp_rgbir_scale_b_r_write(pstRgbirCtrl->as16ScaleCoef[2]);
    hi_ext_system_isp_rgbir_scale_g_b_write(pstRgbirCtrl->as16ScaleCoef[3]);
    hi_ext_system_isp_rgbir_scale_g_g_write(pstRgbirCtrl->as16ScaleCoef[4]);
    hi_ext_system_isp_rgbir_scale_g_r_write(pstRgbirCtrl->as16ScaleCoef[5]);
    hi_ext_system_isp_rgbir_scale_r_b_write(pstRgbirCtrl->as16ScaleCoef[6]);
    hi_ext_system_isp_rgbir_scale_r_g_write(pstRgbirCtrl->as16ScaleCoef[7]);
    hi_ext_system_isp_rgbir_scale_r_r_write(pstRgbirCtrl->as16ScaleCoef[8]);
    hi_ext_system_isp_rgbir_scale_ir_b_write(pstRgbirCtrl->as16ScaleCoef[9]);
    hi_ext_system_isp_rgbir_scale_ir_g_write(pstRgbirCtrl->as16ScaleCoef[10]);
    hi_ext_system_isp_rgbir_scale_ir_r_write(pstRgbirCtrl->as16ScaleCoef[11]);
    hi_ext_system_isp_rgbir_scale_c_b_write(pstRgbirCtrl->as16ScaleCoef[12]);
    hi_ext_system_isp_rgbir_scale_c_g_write(pstRgbirCtrl->as16ScaleCoef[13]);
    hi_ext_system_isp_rgbir_scale_c_r_write(pstRgbirCtrl->as16ScaleCoef[14]);

    return HI_SUCCESS;
    
}

HI_S32 HI_MPI_ISP_GetRgbirCtrl(ISP_DEV IspDev, ISP_RGBIR_CTRL_S *pstRgbirCtrl)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstRgbirCtrl);

    pstRgbirCtrl->bIrOutEn = hi_isp_rgbir_ir_out_en_read();
    pstRgbirCtrl->bIrFilterEn = hi_isp_rgbir_ir_filter_en_read();
    pstRgbirCtrl->bRemovelEn = hi_ext_system_isp_rgbir_removel_en_read();
    if((1 == hi_isp_rgbir_auto_gain_en_read()) && (0 == hi_isp_rgbir_manul_gain_en_read()))
    {
        pstRgbirCtrl->enCompType = OP_TYPE_AUTO;
    }
    else
    {
        pstRgbirCtrl->enCompType = OP_TYPE_MANUAL;
    }
    pstRgbirCtrl->u16ManuGain = hi_isp_rgbir_manul_gain_read();

    pstRgbirCtrl->as16ScaleCoef[0] = hi_ext_system_isp_rgbir_scale_b_b_read();
    pstRgbirCtrl->as16ScaleCoef[1] = hi_ext_system_isp_rgbir_scale_b_g_read();
    pstRgbirCtrl->as16ScaleCoef[2] = hi_ext_system_isp_rgbir_scale_b_r_read();
    pstRgbirCtrl->as16ScaleCoef[3] = hi_ext_system_isp_rgbir_scale_g_b_read();
    pstRgbirCtrl->as16ScaleCoef[4] = hi_ext_system_isp_rgbir_scale_g_g_read();
    pstRgbirCtrl->as16ScaleCoef[5] = hi_ext_system_isp_rgbir_scale_g_r_read();
    pstRgbirCtrl->as16ScaleCoef[6] = hi_ext_system_isp_rgbir_scale_r_b_read();
    pstRgbirCtrl->as16ScaleCoef[7] = hi_ext_system_isp_rgbir_scale_r_g_read();
    pstRgbirCtrl->as16ScaleCoef[8] = hi_ext_system_isp_rgbir_scale_r_r_read();
    pstRgbirCtrl->as16ScaleCoef[9] = hi_ext_system_isp_rgbir_scale_ir_b_read();
    pstRgbirCtrl->as16ScaleCoef[10] = hi_ext_system_isp_rgbir_scale_ir_g_read();
    pstRgbirCtrl->as16ScaleCoef[11] = hi_ext_system_isp_rgbir_scale_ir_r_read();
    pstRgbirCtrl->as16ScaleCoef[12] = hi_ext_system_isp_rgbir_scale_c_b_read();
    pstRgbirCtrl->as16ScaleCoef[13] = hi_ext_system_isp_rgbir_scale_c_g_read();
    pstRgbirCtrl->as16ScaleCoef[14] = hi_ext_system_isp_rgbir_scale_c_r_read();

    return HI_SUCCESS;
}



HI_S32 HI_MPI_ISP_SetStatisticsConfig(ISP_DEV IspDev, const ISP_STATISTICS_CFG_S *pstStatCfg)
{
    HI_U8 i;
    
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStatCfg);
   
    if (pstStatCfg->stAECfg.enAESwitch >= ISP_AE_SWITCH_BUTT)
    {
        printf("Invalid AE Switch %d in %s!\n", pstStatCfg->stAECfg.enAESwitch, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

#if 0
    if (pstStatCfg->stAECfg.enAESumSwitch >= ISP_AE_SWITCH_BUTT)
    {
        printf("Invalid AE Sum Switch %d in %s!\n", pstStatCfg->stAECfg.enAESumSwitch, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stAECfg.enHistSwitch >= ISP_AE_HIST_SWITCH_BUTT)
    {
        printf("Invalid AE 256 Histogram Switch %d in %s!\n", pstStatCfg->stAECfg.enHistSwitch, __FUNCTION__);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
#endif

    if (pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel > 0xFFF)
    {
        printf("Max value of Bayer Config u16WhiteLevel is 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16CrMax > 0xFFF)
    {
        printf("Max value of Bayer Config u16CrMax is 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16CbMax > 0xFFF)
    {
        printf("Max value of Bayer Config u16CbMax is 0xFFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    if (pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel > pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel)
    {
        printf("Bayer Config BlackLevel should not larger than WhiteLevel!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    
    if (pstStatCfg->stWBCfg.stBayerCfg.u16CrMin > pstStatCfg->stWBCfg.stBayerCfg.u16CrMax)
    {
        printf("Bayer CrMin should not larger than CrMax!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stWBCfg.stBayerCfg.u16CbMin > pstStatCfg->stWBCfg.stBayerCfg.u16CbMax)
    {
        printf("Bayer CbMin should not larger than CbMax!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

#if 0
    /*confilict with the logic default setting, delete*/
    if ((pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh > pstStatCfg->stWBCfg.stBayerCfg.u16CrMax) ||
         (pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh < pstStatCfg->stWBCfg.stBayerCfg.u16CrMin) )
    {
        printf("Bayer u16CrHigh should in range [u16CrMin, u16CrMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stBayerCfg.u16CrLow> pstStatCfg->stWBCfg.stBayerCfg.u16CrMax) ||
         (pstStatCfg->stWBCfg.stBayerCfg.u16CrLow < pstStatCfg->stWBCfg.stBayerCfg.u16CrMin) )
    {
        printf("Bayer u16CrLow should in range [u16CrMin, u16CrMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh > pstStatCfg->stWBCfg.stBayerCfg.u16CbMax) ||
         (pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh < pstStatCfg->stWBCfg.stBayerCfg.u16CbMin) )
    {
        printf("Bayer u16CbHigh should in range [u16CbMin, u16CbMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stWBCfg.stBayerCfg.u16CbLow> pstStatCfg->stWBCfg.stBayerCfg.u16CbMax) ||
         (pstStatCfg->stWBCfg.stBayerCfg.u16CbLow < pstStatCfg->stWBCfg.stBayerCfg.u16CbMin) )
    {
        printf("Bayer u16CbLow should in range [u16CbMin, u16CbMax]!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
#endif

    if ((pstStatCfg->stFocusCfg.stConfig.bEnable != HI_FALSE) && (pstStatCfg->stFocusCfg.stConfig.bEnable != HI_TRUE))
    {
        printf("Invalid AF bEnable input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i=0;i<3;i++)
    {
        if ((pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[i] != HI_FALSE) && (pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[i] != HI_TRUE))
        {
            printf("Invalid AF stHParam_IIR0.abIIREn[%d] input!\n",i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i=0;i<3;i++)
    {
        if ((pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[i] != HI_FALSE) && (pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[i] != HI_TRUE))
        {
            printf("Invalid AF stHParam_IIR1.abIIREn[%d] input!\n",i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (pstStatCfg->stFocusCfg.stConfig.enPeakMode >= ISP_AF_STA_BUTT)
    {
        printf("Invalid AF stConfig.enPeakMode input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stConfig.enSquMode >= ISP_AF_STA_SUM_BUTT) 
    {
        printf("Invalid AF stConfig.enSquMode input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stFocusCfg.stConfig.u16Hwnd > AF_ZONE_COLUMN) || (pstStatCfg->stFocusCfg.stConfig.u16Hwnd <= 0))
    {
        printf("Invalid AF stConfig.u16Hwnd input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stFocusCfg.stConfig.u16Vwnd > AF_ZONE_ROW) || (pstStatCfg->stFocusCfg.stConfig.u16Vwnd <= 0))
    {
        printf("Invalid AF stConfig.u16Vwnd input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0] > 0xFF) || (pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0] < 0))
    {
        printf("Invalid AF stHParam_IIR0.as16IIRGain[0] input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i=1;i<7;i++)
    {
        if ((pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[i] > 511) || (pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[i] < -511))
        {
            printf("Invalid AF stHParam_IIR0.as16IIRGain[%d] input!\n",i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if ((pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0] > 0xFF) || (pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0] < 0))
    {
        printf("Invalid AF stHParam_IIR1.as16IIRGain[0] input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i=1;i<7;i++)
    {
        if ((pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[i] > 511) || (pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[i] < -511))
        {
            printf("Invalid AF stHParam_IIR1.as16IIRGain[%d] input!\n",i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i=0;i<4;i++)
    {
        if (pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[i] > 0xF)
        {
            printf("Invalid AF stHParam_IIR0.au16IIRShift[%d] input!\n",i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[i] > 0xF)
        {
            printf("Invalid AF stHParam_IIR1.au16IIRShift[%d] input!\n",i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    for (i=0;i<5;i++)
    {
        if ((pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[i] > 31) || (pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[i] < -31))
        {
            printf("Invalid AF stVParam_FIR0.as16FIRH[%d] input!\n",i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[i] > 31) || (pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[i] < -31))
        {
            printf("Invalid AF stVParam_FIR1.as16FIRH[%d] input!\n",i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (pstStatCfg->stFocusCfg.stHParam_IIR0.u16IIRThd > 0x7FF)
    {
        printf("Invalid AF stHParam_IIR0.u16IIRThd input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stHParam_IIR1.u16IIRThd > 0x7FF)
    {
        printf("Invalid AF stHParam_IIR1.u16IIRThd input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stVParam_FIR0.u16FIRThd > 0x7FF)
    {
        printf("Invalid AF stVParam_FIR0.u16FIRThd input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stVParam_FIR1.u16FIRThd > 0x7FF)
    {
        printf("Invalid AF stVParam_FIR1.u16FIRThd input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i=0;i<2;i++)
    {
        if (pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[i] > 0xF)
        {
            printf("Invalid AF stFVParam.au16AccShiftH[%d] input!\n",i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[i] > 0xF)
        {
            printf("Invalid AF stFVParam.au16AccShiftV[%d] input!\n",i);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    if (pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY > 0xF)
    {
        printf("Invalid AF stFVParam.u16AccShiftY input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stConfig.u16Hsize > 0xFFF)
    {
        printf("Invalid AF stConfig.u16Hsize input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstStatCfg->stFocusCfg.stConfig.u16Vsize > 0xFFF)
    {
        printf("Invalid AF stConfig.u16Vsize input!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_statistics_ctrl_write(pstStatCfg->unKey.u32Key);

    if(pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_DG)
    {
        hi_isp_module_ae_sel_write(HI_ISP_TOP_AE_SELECT_AFTER_DG);
    }
    else if(pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_STATIC_WB)
    {
        hi_isp_module_ae_sel_write(HI_ISP_TOP_AE_SELECT_AFTER_STATIC_WB);
    }
    else if(pstStatCfg->stAECfg.enAESwitch == ISP_AE_AFTER_DRC)
    {
        hi_isp_module_ae_sel_write(HI_ISP_TOP_AE_SELECT_AFTER_DRC);
    }
    else
    {
        printf("not support!");
    }

    hi_isp_metering_max_threshold_write(pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel);
    hi_isp_metering_min_threshold_write(pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel);
    hi_isp_metering_cr_ref_max_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CrMax);
    hi_isp_metering_cr_ref_min_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CrMin);
    hi_isp_metering_cb_ref_max_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CbMax);
    hi_isp_metering_cb_ref_min_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CbMin);
#if 0
    hi_isp_metering_cr_ref_high_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh);
    hi_isp_metering_cr_ref_low_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CrLow);
    hi_isp_metering_cb_ref_high_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh);
    hi_isp_metering_cb_ref_low_awb_sum_write(pstStatCfg->stWBCfg.stBayerCfg.u16CbLow);
#endif

    hi_isp_af_enable_write(pstStatCfg->stFocusCfg.stConfig.bEnable);
    hi_isp_af_iir0_en0_write(pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[0]);
    hi_isp_af_iir0_en1_write(pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[1]);
    hi_isp_af_iir0_en2_write(pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[2]);
    hi_isp_af_iir1_en0_write(pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[0]);
    hi_isp_af_iir1_en1_write(pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[1]);
    hi_isp_af_iir1_en2_write(pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[2]);
    hi_isp_af_peak_mode_write(pstStatCfg->stFocusCfg.stConfig.enPeakMode);
    hi_isp_af_squ_mode_write(pstStatCfg->stFocusCfg.stConfig.enSquMode);
	hi_isp_af_raw_mode_write(pstStatCfg->stFocusCfg.stConfig.enStatisticsPos);
    hi_isp_af_hnum_write(pstStatCfg->stFocusCfg.stConfig.u16Hwnd);
    hi_isp_af_vnum_write(pstStatCfg->stFocusCfg.stConfig.u16Vwnd);
    hi_isp_af_iir0_g0_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0]);
    hi_isp_af_iir1_g0_write((HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0]);
	for (i = 1; i < 7; i++)
	{
	    hi_isp_af_iir0_g_write(i, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[i]);
    	hi_isp_af_iir1_g_write(i, (HI_U32)pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[i]);
	}

	for (i = 0; i < 4; i++)
	{
	    hi_isp_af_iir0_shift_write(i, pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[i]);
		hi_isp_af_iir1_shift_write(i, pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[i]);
	}

	for (i = 0; i < 5; i++)
	{
		hi_isp_af_fir0_h_write(i, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[i]);
		hi_isp_af_fir1_h_write(i, (HI_U32)pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[i]);
	}

    hi_isp_af_iir0_thre_x_write(pstStatCfg->stFocusCfg.stHParam_IIR0.u16IIRThd);
    hi_isp_af_iir1_thre_x_write(pstStatCfg->stFocusCfg.stHParam_IIR1.u16IIRThd);
    hi_isp_af_fir0_thre_y_write(pstStatCfg->stFocusCfg.stVParam_FIR0.u16FIRThd);
    hi_isp_af_fir1_thre_y_write(pstStatCfg->stFocusCfg.stVParam_FIR1.u16FIRThd);
    hi_isp_af_acc_shift_h0_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[0]);
    hi_isp_af_acc_shift_h1_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[1]);
    hi_isp_af_acc_shift_v0_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[0]);
    hi_isp_af_acc_shift_v1_write(pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[1]);
    hi_isp_af_acc_shift_y_write(pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY);
    hi_isp_af_hsize_write(pstStatCfg->stFocusCfg.stConfig.u16Hsize - 1);
    hi_isp_af_vsize_write(pstStatCfg->stFocusCfg.stConfig.u16Vsize - 1);
    
    return HI_SUCCESS;
}


HI_S32 HI_MPI_ISP_GetStatisticsConfig(ISP_DEV IspDev, ISP_STATISTICS_CFG_S *pstStatCfg)
{
	HI_U8 i;
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStatCfg);
    HI_U8  u8Tmp;
	HI_S16 s16Tmp;
    
    pstStatCfg->unKey.u32Key = hi_ext_system_statistics_ctrl_read();
   
    u8Tmp = hi_isp_module_ae_sel_read();
    if (u8Tmp == HI_ISP_TOP_AE_SELECT_AFTER_DG)
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_DG;
    }
    else if (u8Tmp == HI_ISP_TOP_AE_SELECT_AFTER_STATIC_WB)
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_STATIC_WB;
    }
    else if (u8Tmp == HI_ISP_TOP_AE_SELECT_AFTER_DRC)
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_AFTER_DRC;
    }
    else
    {
        pstStatCfg->stAECfg.enAESwitch = ISP_AE_SWITCH_BUTT;
    }

    pstStatCfg->stWBCfg.stBayerCfg.u16WhiteLevel = hi_isp_metering_max_threshold_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16BlackLevel = hi_isp_metering_min_threshold_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CrMax = hi_isp_metering_cr_ref_max_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CrMin = hi_isp_metering_cr_ref_min_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CbMax = hi_isp_metering_cb_ref_max_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CbMin = hi_isp_metering_cb_ref_min_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CrHigh = hi_isp_metering_cr_ref_high_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CrLow = hi_isp_metering_cr_ref_low_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CbHigh = hi_isp_metering_cb_ref_high_awb_sum_read();
    pstStatCfg->stWBCfg.stBayerCfg.u16CbLow = hi_isp_metering_cb_ref_low_awb_sum_read();

    pstStatCfg->stFocusCfg.stConfig.bEnable = hi_isp_af_enable_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[0] = hi_isp_af_iir0_en0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[1] = hi_isp_af_iir0_en1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR0.abIIREn[2] = hi_isp_af_iir0_en2_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[0] = hi_isp_af_iir1_en0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[1] = hi_isp_af_iir1_en1_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.abIIREn[2] = hi_isp_af_iir1_en2_read();
    pstStatCfg->stFocusCfg.stConfig.enPeakMode = hi_isp_af_peak_mode_read();
    pstStatCfg->stFocusCfg.stConfig.enSquMode = hi_isp_af_squ_mode_read();
    pstStatCfg->stFocusCfg.stConfig.u16Hwnd = hi_isp_af_hnum_read();
    pstStatCfg->stFocusCfg.stConfig.u16Vwnd = hi_isp_af_vnum_read();
	pstStatCfg->stFocusCfg.stConfig.enStatisticsPos = hi_isp_af_raw_mode_read();

    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[0] = (HI_S16)hi_isp_af_iir0_g0_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[0] = (HI_S16)hi_isp_af_iir1_g0_read();
	for (i = 1; i < 7; i++)
	{
		s16Tmp = hi_isp_af_iir0_g_read(i);
		if (s16Tmp & 0x200)
		{
		    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[i] = -(((~s16Tmp)&0x1ff) + 1);
		}
		else
		{
		    pstStatCfg->stFocusCfg.stHParam_IIR0.as16IIRGain[i] = s16Tmp;
		}

		s16Tmp = hi_isp_af_iir1_g_read(i);
		if (s16Tmp & 0x200)
		{
		    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[i] = -(((~s16Tmp)&0x1ff) + 1);
		}
		else
		{
		    pstStatCfg->stFocusCfg.stHParam_IIR1.as16IIRGain[i] = s16Tmp;
		}
	}

	for (i = 0; i < 4; i++)
	{
		pstStatCfg->stFocusCfg.stHParam_IIR0.au16IIRShift[i] = hi_isp_af_iir0_shift_read(i);
		pstStatCfg->stFocusCfg.stHParam_IIR1.au16IIRShift[i] = hi_isp_af_iir1_shift_read(i);
	}

	for (i = 0; i < 5; i++)
	{
		s16Tmp = hi_isp_af_fir0_h_read(i);
		if (s16Tmp & 0x20)
		{
		    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[i] = -(((~s16Tmp)&0x1f) + 1);
		}
		else
		{
		    pstStatCfg->stFocusCfg.stVParam_FIR0.as16FIRH[i] = s16Tmp;
		}

		s16Tmp = hi_isp_af_fir1_h_read(i);
		if (s16Tmp & 0x20)
		{
		    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[i] = -(((~s16Tmp)&0x1f) + 1);
		}
		else
		{
		    pstStatCfg->stFocusCfg.stVParam_FIR1.as16FIRH[i] = s16Tmp;
		}
	}
	
    pstStatCfg->stFocusCfg.stHParam_IIR0.u16IIRThd = hi_isp_af_iir0_thre_x_read();
    pstStatCfg->stFocusCfg.stHParam_IIR1.u16IIRThd = hi_isp_af_iir1_thre_x_read();
    pstStatCfg->stFocusCfg.stVParam_FIR0.u16FIRThd = hi_isp_af_fir0_thre_y_read();
    pstStatCfg->stFocusCfg.stVParam_FIR1.u16FIRThd = hi_isp_af_fir1_thre_y_read();
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[0] = hi_isp_af_acc_shift_h0_read();
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftH[1] = hi_isp_af_acc_shift_h1_read();
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[0] = hi_isp_af_acc_shift_v0_read();
    pstStatCfg->stFocusCfg.stFVParam.au16AccShiftV[1] = hi_isp_af_acc_shift_v1_read();
    pstStatCfg->stFocusCfg.stFVParam.u16AccShiftY = hi_isp_af_acc_shift_y_read();
    pstStatCfg->stFocusCfg.stConfig.u16Hsize = hi_isp_af_hsize_read() + 1;
    pstStatCfg->stFocusCfg.stConfig.u16Vsize = hi_isp_af_vsize_read() + 1;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetStatistics(ISP_DEV IspDev, ISP_STATISTICS_S *pstStat)
{
    HI_S32 i, j, k;
    //HI_U32 u32HighBit = 0, u32LowBit = 0;  
	//HI_U32 u32StatData;
	HI_U8 u8DataMode = 0, u8Col = 0, u8Row = 0;
    //HI_U32 u32AveMem;
    HI_S32 s32IspDevFd;
    HI_BOOL bShadowMemAccess = HI_FALSE;
    HI_U32 u32ShadowMemPhy = 0;
    ISP_STAT_S *pstIspStat = HI_NULL;
    HI_S32 s32Ret;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstStat);

    s32IspDevFd = open("/dev/isp_dev", O_RDONLY);
    if (s32IspDevFd <= 0)
    {
        printf("open isp device error!\n");
        return HI_ERR_ISP_NOT_INIT;
    }

    bShadowMemAccess = HI_TRUE;
    s32Ret = ioctl(s32IspDevFd, ISP_STAT_SHADOW_MEMSTATE_SET, &bShadowMemAccess);
    s32Ret = ioctl(s32IspDevFd, ISP_STAT_SHADOW_MEMPHY_GET, &u32ShadowMemPhy);

    pstIspStat = (ISP_STAT_S *)HI_MPI_SYS_Mmap(u32ShadowMemPhy, sizeof(ISP_STAT_S));
    if (HI_NULL == pstIspStat)
    {
        printf("mmap statistics shadow mem failed!\n");
        return HI_ERR_ISP_NOMEM;
    }

    // pstStat->unKey.bit1AeStat3
    for (i = 0; i < 256; i++)
    {
        pstStat->stAEStat.au32Hist256Value[i] = pstIspStat->stAeStat3.au32HistogramMemArray[i];
    }

    // pstStat->unKey.bit1AeStat5
    for(i = 0;i < AE_ZONE_ROW ; i++)
    {
        for(j=0;j< AE_ZONE_COLUMN ;j++)
        {           
            pstStat->stAEStat.au16ZoneAvg[i][j][0] = pstIspStat->stAeStat5.au16ZoneAvg[i][j][0];  /*R */
            pstStat->stAEStat.au16ZoneAvg[i][j][1] = pstIspStat->stAeStat5.au16ZoneAvg[i][j][1];  /*Gr*/
            pstStat->stAEStat.au16ZoneAvg[i][j][2] = pstIspStat->stAeStat5.au16ZoneAvg[i][j][2];  /*Gb*/
            pstStat->stAEStat.au16ZoneAvg[i][j][3] = pstIspStat->stAeStat5.au16ZoneAvg[i][j][3];  /*B */
        }       
    }

    // pstStat->unKey.bit1AwbStat4
    for(i = 0; i < AWB_ZONE_ROW; i++)
    {
        for(j = 0; j < AWB_ZONE_COLUMN; j++)
        {        
            pstStat->stWBStat.stBayerStatistics.au16ZoneAvgR[i][j] = pstIspStat->stAwbStat4.au16MeteringMemArrayAvgR[i*AWB_ZONE_COLUMN+j];
            pstStat->stWBStat.stBayerStatistics.au16ZoneAvgG[i][j] = pstIspStat->stAwbStat4.au16MeteringMemArrayAvgG[i*AWB_ZONE_COLUMN+j];
            pstStat->stWBStat.stBayerStatistics.au16ZoneAvgB[i][j] = pstIspStat->stAwbStat4.au16MeteringMemArrayAvgB[i*AWB_ZONE_COLUMN+j];
            pstStat->stWBStat.stBayerStatistics.au16ZoneCountAll[i][j] = pstIspStat->stAwbStat4.au16MeteringMemArrayCountAll[i*AWB_ZONE_COLUMN+j];
            pstStat->stWBStat.stBayerStatistics.au16ZoneCountMin[i][j] = pstIspStat->stAwbStat4.au16MeteringMemArrayCountMin[i*AWB_ZONE_COLUMN+j];
            pstStat->stWBStat.stBayerStatistics.au16ZoneCountMax[i][j] = pstIspStat->stAwbStat4.au16MeteringMemArrayCountMax[i*AWB_ZONE_COLUMN+j];
        }    
    }

    // AF statistics
	u8Col = hi_isp_af_hnum_read();
	u8Row = hi_isp_af_vnum_read();
    for (i = 0; i < u8Row; i++)
	{
		for (j = 0; j < u8Col; j++)
		{
			for (k = 0; k < 3; k++)
			{
				if (k == 0)
				{
					pstStat->stFocusStat.stZoneMetrics[i][j].u16v1 = pstIspStat->stAfStat.stZoneMetrics[i][j].u16v1;
					pstStat->stFocusStat.stZoneMetrics[i][j].u16h1 = pstIspStat->stAfStat.stZoneMetrics[i][j].u16h1;
				}
				else if (k == 1)
				{
					pstStat->stFocusStat.stZoneMetrics[i][j].u16v2 = pstIspStat->stAfStat.stZoneMetrics[i][j].u16v2;
					pstStat->stFocusStat.stZoneMetrics[i][j].u16h2 = pstIspStat->stAfStat.stZoneMetrics[i][j].u16h2;
				}
				else
				{
				   pstStat->stFocusStat.stZoneMetrics[i][j].u16y  = pstIspStat->stAfStat.stZoneMetrics[i][j].u16y;
				}
			}			
		}
	}

    HI_MPI_SYS_Munmap((HI_VOID *)pstIspStat, sizeof(ISP_STAT_S));
    
    bShadowMemAccess = HI_FALSE;
    s32Ret = ioctl(s32IspDevFd, ISP_STAT_SHADOW_MEMSTATE_SET, &bShadowMemAccess);
    
    close(s32IspDevFd);

    //if (pstStat->unKey.bit1AeStat4)
    {
        pstStat->stAEStat.au16GlobalAvg[0]  = hi_isp_ae_total_r_aver_read();        
        pstStat->stAEStat.au16GlobalAvg[1] = hi_isp_ae_total_gr_aver_read();
        pstStat->stAEStat.au16GlobalAvg[2] = hi_isp_ae_total_gb_aver_read();
        pstStat->stAEStat.au16GlobalAvg[3]  = hi_isp_ae_total_b_aver_read();
    }


    /*Hi3518EV200 does't support RGB Statistics*/
#if 0
    //if (pstStat->unKey.bit1AwbStat1)
    {
        pstStat->stWBStat.stRGBStatistics.u16GlobalGR = hi_isp_metering_awb_rg_read();
        pstStat->stWBStat.stRGBStatistics.u16GlobalGB = hi_isp_metering_awb_bg_read();
        pstStat->stWBStat.stRGBStatistics.u32GlobalSum = hi_isp_metering_awb_sum_read();
    }

    //if (pstStat->unKey.bit1AwbStat2)
    {
        for(i = 0; i < AWB_ZONE_ROW; i++)
        {
            for(j = 0; j < AWB_ZONE_COLUMN; j++)
            {               
                HI_U32 u32BgRg = hi_metering_mem_array_data_read_bg_rg(i*AWB_ZONE_COLUMN+j);
                HI_U32 u32Sum = hi_metering_mem_array_data_read_sum(i*AWB_ZONE_COLUMN+j);

                pstStat->stWBStat.stRGBStatistics.au16ZoneGR[i][j] = u32BgRg & 0xfff;
                pstStat->stWBStat.stRGBStatistics.au16ZoneGB[i][j] = (u32BgRg >> 16) & 0xfff;       
                pstStat->stWBStat.stRGBStatistics.au32ZoneSum[i][j] = u32Sum;
            }
        }
    }
#endif

    //if (pstStat->unKey.bit1AwbStat3)
    {
        pstStat->stWBStat.stBayerStatistics.u16GlobalR = hi_isp_metering_avg_r_read();
        pstStat->stWBStat.stBayerStatistics.u16GlobalG = hi_isp_metering_avg_g_read();
        pstStat->stWBStat.stBayerStatistics.u16GlobalB = hi_isp_metering_avg_b_read();
        pstStat->stWBStat.stBayerStatistics.u16CountAll = hi_isp_metering_count_all_read();
        pstStat->stWBStat.stBayerStatistics.u16CountMin = hi_isp_metering_count_min_read();
        pstStat->stWBStat.stBayerStatistics.u16CountMax= hi_isp_metering_count_max_read();
    }

#if 0
    //if (pstStat->unKey.bit1AwbStat4)
    {
        hi_isp_awb_stat_raddr_write(0);
		
        for(i = 0; i < AWB_ZONE_ROW; i++)
        {
            for(j = 0; j < AWB_ZONE_COLUMN; j++)
            {        
                HI_U32 u32Value = hi_isp_awb_stat_rdata_read();
                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgR[i][j] = (u32Value & 0xffff);
                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgG[i][j] = ((u32Value >> 16) & 0xffff);
                
                u32Value = hi_isp_awb_stat_rdata_read();
                pstStat->stWBStat.stBayerStatistics.au16ZoneAvgB[i][j] = (u32Value & 0xffff);
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountAll[i][j] = ((u32Value >> 16) & 0xffff);
                
                u32Value = hi_isp_awb_stat_rdata_read();
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountMin[i][j] = u32Value & 0xffff;
                pstStat->stWBStat.stBayerStatistics.au16ZoneCountMax[i][j] = (u32Value>>16) & 0xffff;
            }    
        }
    } 

    u8DataMode = hi_isp_af_data_mode_read();
	u8Col = hi_isp_af_hnum_read();
	u8Row = hi_isp_af_vnum_read();
	
	hi_isp_af_stat_ind_raddr_write(0x0);
	if (u8DataMode == 0)
	{
		for (i = 0; i < u8Row; i++)
		{
			for (j = 0; j < u8Col; j++)
			{
				for (k = 0; k < 3; k++)
				{
					u32StatData = hi_isp_af_stat_ind_rdata_read();
					if (k == 0)
					{
						pstStat->stFocusStat.stZoneMetrics[i][j].u16v1 = (HI_U16)((u32StatData & 0xFFFF0000) >> 16);
						pstStat->stFocusStat.stZoneMetrics[i][j].u16h1 = (HI_U16)(u32StatData & 0xFFFF);
					}
					else if (k == 1)
					{
						pstStat->stFocusStat.stZoneMetrics[i][j].u16v2 = (HI_U16)((u32StatData & 0xFFFF0000) >> 16);
						pstStat->stFocusStat.stZoneMetrics[i][j].u16h2 = (HI_U16)(u32StatData & 0xFFFF);
					}
					else
					{
					   pstStat->stFocusStat.stZoneMetrics[i][j].u16y  = (HI_U16)(u32StatData & 0xFFFF);
					}
				}			
			}
		}
	}
	else if (u8DataMode == 1)
	{
		for (i = 0; i < u8Row; i++)
		{
			for (j = 0; j < u8Col; j++)
			{
				for (k = 0; k < 3; k++)
				{
					u32StatData = hi_isp_af_stat_ind_rdata_read();
					if (k == 0)
					{
						pstStat->stFocusStat.stZoneMetrics[i][j].u16v1 = (HI_U16)((u32StatData & 0x03FFE000) >> 13);
						pstStat->stFocusStat.stZoneMetrics[i][j].u16h1 = (HI_U16)(u32StatData & 0x1FFF);
						u32LowBit = (u32StatData & 0xFC000000) >> 26;
					}
					else if (k == 1)
					{
						pstStat->stFocusStat.stZoneMetrics[i][j].u16v2 = (HI_U16)((u32StatData & 0x03FFE000) >> 13);
						pstStat->stFocusStat.stZoneMetrics[i][j].u16h2 = (HI_U16)(u32StatData & 0x1FFF);
						u32HighBit = (u32StatData & 0xFC000000) >> 26;
					}
					else
					{
						pstStat->stFocusStat.stZoneMetrics[i][j].u16y  = (HI_U16)((u32HighBit << 6) | u32LowBit);
					}
				}			
			}
		}
	}
#endif
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetISPRegAttr(ISP_DEV IspDev, ISP_REG_ATTR_S *pstIspRegAttr)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CTX_S *pstIspCtx = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspRegAttr);
    
    pthread_mutex_lock(&pstIspCtx->stLock);
    VReg_Munmap();

    pstIspRegAttr->u32IspRegAddr    = VReg_GetVirtAddr(ISP_REG_BASE);
    pstIspRegAttr->u32IspRegSize    = ISP_REG_SIZE;
    pstIspRegAttr->u32IspExtRegAddr = VReg_GetVirtAddr(ISP_VREG_BASE);
    pstIspRegAttr->u32IspExtRegSize = ISP_VREG_SIZE;
    pstIspRegAttr->u32AeExtRegAddr  = VReg_GetVirtAddr(AE_LIB_VREG_BASE(0));
    pstIspRegAttr->u32AeExtRegSize  = ALG_LIB_VREG_SIZE;
    pstIspRegAttr->u32AwbExtRegAddr = VReg_GetVirtAddr(AWB_LIB_VREG_BASE(0));
    pstIspRegAttr->u32AwbExtRegSize = ALG_LIB_VREG_SIZE;

    pthread_mutex_unlock(&pstIspCtx->stLock);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetDebug(ISP_DEV IspDev, const ISP_DEBUG_INFO_S *pstIspDebug)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspDebug);
    ISP_CHECK_BOOL(pstIspDebug->bDebugEn);

    return ISP_DbgSet(pstIspDebug);
}

HI_S32 HI_MPI_ISP_GetDebug(ISP_DEV IspDev, ISP_DEBUG_INFO_S * pstIspDebug)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstIspDebug);
    
    return ISP_DbgGet(pstIspDebug);
}

HI_S32 HI_MPI_ISP_SetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{	
    ISP_CHECK_POINTER(pstIspModParam);
    ISP_CHECK_OPEN(0);    
	
	return 	ioctl(g_as32IspFd[0], ISP_SET_MOD_PARAM, pstIspModParam);
}

HI_S32 HI_MPI_ISP_GetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{
    ISP_CHECK_POINTER(pstIspModParam);
    ISP_CHECK_OPEN(0);    

    return ioctl(g_as32IspFd[0], ISP_GET_MOD_PARAM, pstIspModParam);
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

