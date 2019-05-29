/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_defaults.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_sensor.h"
#include "isp_defaults.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_VOID ISP_RegsDefault(HI_VOID) 
{
    /* Statistics after static WB */
//#define HI_ISP_TOP_AE_SWITCH_AFTER_STATIC_WB (0)
//#define HI_ISP_TOP_AE_SWITCH_IMMEDIATELY_FROM_SENSOR_CHANNEL_1_FOR_WDR_MODES_ (1)
//#define HI_ISP_TOP_AE_SWITCH_AFTER_SHADING (2)
//#define HI_ISP_TOP_AE_SWITCH_AFTER_FRONTEND_LUT_CHANNEL_1_FOR_WDR_MODES_ (3)
//#define HI_ISP_TOP_AE_SWITCH_AFTER_DRC (4)
//#define HI_ISP_TOP_AE_SWITCH_IMMEDIATELY_FROM_SENSOR_CHANNEL_2_FOR_WDR_MODES_ (5)
//#define HI_ISP_TOP_AE_SWITCH_AFTER_WDR_STITCH (6)
//#define HI_ISP_TOP_AE_SWITCH_AFTER_FRONTEND_LUT_CHANNEL_2_FOR_WDR_MODES_ (7)
    hi_isp_top_ae_switch_write(HI_ISP_TOP_AE_SWITCH_AFTER_STATIC_WB);
    
    /* top bypass functions, added by d00181140,2012.04.23 */ 
    hi_isp_top_bypass_green_equalize_write(HI_ISP_TOP_BYPASS_GREEN_EQUALIZE_DEFAULT);
    hi_isp_top_bypass_color_matrix_write(HI_ISP_TOP_BYPASS_COLOR_MATRIX_DEFAULT);
    hi_isp_top_bypass_debuger_write(HI_ISP_TOP_BYPASS_DEBUGER_DEFAULT);

    /* Set low_power_consumption_config */
    hi_isp_low_power_consumption_config_write(HI_ISP_LOW_POWER_CONSUMPTION_CONFIG_DEFAULT);

    /* Set interrupt */
    hi_isp_interrupts_interrupt0_source_write(HI_ISP_INTERRUPTS_INTERRUPT0_SOURCE_VALUE);
    hi_isp_interrupts_interrupt1_source_write(0);
    
    /* Select config double-buffering mode: 2 = Local (module config updates during local vertical blanking) */
    hi_isp_top_config_buffer_mode_write(2);
    
    return;
}

HI_VOID ISP_RegsInitialize(ISP_DEV IspDev)
{    
    return;
}

/* The ext regs is a block of memory, not real register, need a default value. */
HI_VOID ISP_ExtRegsDefault(HI_VOID)
{
    hi_ext_system_set_1001_rate_divider_write(HI_EXT_SYSTEM_SET_1001_RATE_DIVIDER_DEFAULT);
    hi_ext_system_half_pixel_clock_write(HI_EXT_SYSTEM_HALF_PIXEL_CLOCK_DEFAULT);
    hi_ext_system_freeze_firmware_write(HI_EXT_SYSTEM_FREEZE_FIRMWARE_DEFAULT);
    hi_ext_system_low_light_enable_write(HI_EXT_SYSTEM_LOW_LIGHT_ENABLE_DEFAULT);

    hi_ext_system_sys_debug_enable_write(HI_EXT_SYSTEM_SYS_DEBUG_ENABLE_DEFAULT);
    hi_ext_system_sys_debug_addr_write(HI_EXT_SYSTEM_SYS_DEBUG_ADDR_DEFAULT);
    hi_ext_system_sys_debug_size_write(HI_EXT_SYSTEM_SYS_DEBUG_SIZE_DEFAULT);
    hi_ext_system_sys_debug_depth_write(HI_EXT_SYSTEM_DEBUG_DEPTH_DEFAULT);

	hi_ext_system_isp_balancefe_enable_write(HI_EXT_SYSTEM_ISP_BALANCEFE_ENABLE_WRITE_DEFAULT);
	hi_ext_system_top_bit_reserve3_write(HI_EXT_SYSTEM_TOP_BIT_RESERVE3_WRITE_DEFAULT);
	hi_ext_system_top_bit_reserve4_write(HI_EXT_SYSTEM_TOP_BIT_RESERVE4_WRITE_DEFAULT);
	hi_ext_system_top_bit_reserve5_write(HI_EXT_SYSTEM_TOP_BIT_RESERVE5_WRITE_DEFAULT);
	hi_ext_system_isp_chnswitch_write(HI_EXT_SYSTEM_ISP_CHNSWITCH_WRITE_DEFAULT);
	hi_ext_system_gammafe_position_write(HI_EXT_SYSTEM_GAMMAFE_POSITION_WRITE_DEFAULT);
	hi_ext_system_fswdr_enable_write(HI_EXT_SYSTEM_FSWDR_ENABLE_WRITE_DEFAULT);
	hi_ext_system_isp_bypassmode_write(HI_EXT_SYSTEM_ISP_BYPASSMODE_WRITE_DEFAULT);
	hi_ext_system_isp_bypassall_write(HI_EXT_SYSTEM_ISP_BYPASSALL_WRITE_DEFAULT);

    hi_ext_system_statistics_ctrl_write(HI_EXT_SYSTEM_STATISTICS_DEFAULT);

    return;
}

HI_VOID ISP_ExtRegsInitialize(ISP_DEV IspDev)
{
    ISP_CMOS_SENSOR_MAX_RESOLUTION_S *pstSnsMaxResolution = HI_NULL;
    
    ISP_SensorGetMaxResolution(IspDev,  &pstSnsMaxResolution);

    hi_ext_system_sensor_max_resolution_width_write(pstSnsMaxResolution->u32MaxWidth);
    hi_ext_system_sensor_max_resolution_height_write(pstSnsMaxResolution->u32MaxHeight);
    
    return;
}

HI_VOID ISP_GlobalInitialize(ISP_DEV IspDev)
{
    HI_U32 u32Value = 0;
    HI_VOID *pValue = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstIspCtx->stSnsImageMode.u16Width = hi_isp_top_active_width_read();
    pstIspCtx->stSnsImageMode.u16Height = hi_isp_top_active_height_read();
    u32Value = hi_ext_system_fps_base_read();
    pValue = (HI_VOID *)&u32Value;
    pstIspCtx->stSnsImageMode.f32Fps = *(HI_FLOAT *)pValue;
    
    pstIspCtx->stPreSnsImageMode.u16Width = pstIspCtx->stSnsImageMode.u16Width;
    pstIspCtx->stPreSnsImageMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
    pstIspCtx->stPreSnsImageMode.f32Fps = pstIspCtx->stSnsImageMode.f32Fps;

    pstIspCtx->u8SnsWDRMode = hi_ext_system_sensor_wdr_mode_read();
    pstIspCtx->u8PreSnsWDRMode = pstIspCtx->u8SnsWDRMode;
    
    pstIspCtx->u32FrameCnt = 0;
    pstIspCtx->u8IspImageModeFlag = 0;
    
    pstIspCtx->stLinkage.bDefectPixel = HI_TRUE;    
    pstIspCtx->bFreezeFw = HI_FALSE;

    return;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

