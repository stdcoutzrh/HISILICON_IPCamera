/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_regcfg.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/05/07
  Description   : 
  History       :
  1.Date        : 2013/05/07
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "isp_regcfg.h"
#include "isp_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

ISP_REGCFG_S g_astRegCfgCtx[ISP_MAX_DEV_NUM] = {{0}};
#define REGCFG_GET_CTX(dev, pstCtx)   pstCtx = &g_astRegCfgCtx[dev]

extern HI_S32 g_as32IspFd[ISP_MAX_DEV_NUM];

HI_S32 ISP_RegConfigInit(ISP_REG_CFG_S *pstRegCfgInfo)
{
    HI_S32 i, j;
	HI_U32 u32CombinWeiNum = 0;
	HI_U32 u32CombinWei = 0;

    if (pstRegCfgInfo->unKey.bit1AeCfg1)
    {
		/*modify by qlp*/
    	hi_isp_ae_mem_wei_waddr_write(0);
		
        for (i = 0; i < 15; i++)
        {
            for (j = 0; j < 17; j++)
            {
            	if (0 == u32CombinWeiNum)
            	{
            		u32CombinWei = hi_isp_ae_mem_wei_wdata_read();
            	}
	
				pstRegCfgInfo->stAeRegCfg1.au8WeightTable[i][j] = ((u32CombinWei >> (8*u32CombinWeiNum)) & 0xff);

				u32CombinWeiNum++;

				if (HI_ISP_AE_WEI_COMBIN_COUNT == u32CombinWeiNum)
				{
					u32CombinWeiNum = 0;
				}
            }
        }

        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[0] = hi_isp_metering_hist_thresh_0_1_read();
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[1] = hi_isp_metering_hist_thresh_1_2_read();
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[2] = hi_isp_metering_hist_thresh_3_4_read();
        pstRegCfgInfo->stAeRegCfg1.au8MeteringHistThresh[3] = hi_isp_metering_hist_thresh_4_5_read();
    }

    if (pstRegCfgInfo->unKey.bit1AeCfg2)
    {
        pstRegCfgInfo->stAeRegCfg2.u32IntTime[0] = 2;
        pstRegCfgInfo->stAeRegCfg2.u32IntTime[1] = 2;
        pstRegCfgInfo->stAeRegCfg2.u32IntTime[2] = 2;
        pstRegCfgInfo->stAeRegCfg2.u32IntTime[3] = 2;
        pstRegCfgInfo->stAeRegCfg2.u32IspDgain = 1;
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg1)
    {
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[0] = hi_isp_matrix_coefft_r_r_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[1] = hi_isp_matrix_coefft_r_g_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[2] = hi_isp_matrix_coefft_r_b_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[3] = hi_isp_matrix_coefft_g_r_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[4] = hi_isp_matrix_coefft_g_g_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[5] = hi_isp_matrix_coefft_g_b_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[6] = hi_isp_matrix_coefft_b_r_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[7] = hi_isp_matrix_coefft_b_g_read();
        pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[8] = hi_isp_matrix_coefft_b_b_read();

        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[0] = hi_isp_white_balance_gain_00_read();
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[1] = hi_isp_white_balance_gain_01_read();
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[2] = hi_isp_white_balance_gain_10_read();
        pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[3] = hi_isp_white_balance_gain_11_read();
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg3)
    {
        pstRegCfgInfo->stAwbRegCfg3.bAboveWhiteLevelClip = hi_isp_metering_max_clip_read();
        pstRegCfgInfo->stAwbRegCfg3.bBelowBlackLevelClip = hi_isp_metering_min_clip_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMaxAwb = hi_isp_metering_cr_ref_max_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMinAwb = hi_isp_metering_cr_ref_min_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMaxAwb = hi_isp_metering_cb_ref_max_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMinAwb = hi_isp_metering_cb_ref_min_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringWhiteLevelAwb = hi_isp_metering_max_threshold_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringBlackLevelAwb = hi_isp_metering_min_threshold_read();
#if 0		
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefHighAwb = hi_isp_metering_cr_ref_high_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefLowAwb = hi_isp_metering_cr_ref_low_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefHighAwb = hi_isp_metering_cb_ref_high_awb_sum_read();
        pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefLowAwb = hi_isp_metering_cb_ref_low_awb_sum_read();
#endif
    }

    if (pstRegCfgInfo->unKey.bit1AfCfg)
    {
        pstRegCfgInfo->stAfRegCfg.u16ThresholdWrite = hi_isp_metering_af_threshold_write_read();
        pstRegCfgInfo->stAfRegCfg.u8MetricsShift = hi_isp_metering_af_metrics_shift_read();
        pstRegCfgInfo->stAfRegCfg.u8NpOffset = hi_isp_metering_af_np_offset_read();
    }

    if (pstRegCfgInfo->unKey.bit1OffsetCfg)
    {
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[0] = (hi_isp_offset_black_00_read() >> 2);
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[1] = (hi_isp_offset_black_01_read() >> 2);
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[2] = (hi_isp_offset_black_10_read() >> 2);
        pstRegCfgInfo->stOffsetRegCfg.au16BlackLevelOffset[3] = (hi_isp_offset_black_11_read() >> 2);
    }

    if (pstRegCfgInfo->unKey.bit1DrcCfg)
    {
        //pstRegCfgInfo->stDrcRegCfg.u8Strength = hi_isp_drc_strength_read();
    }

    if (pstRegCfgInfo->unKey.bit1NrCfg)
    {

    }

    if (pstRegCfgInfo->unKey.bit1SharpenCfg)
    {      
    	//modify by qlp
		#if 1    	
		pstRegCfgInfo->stSharpenRegCfg.u16EdgeAmt = hi_isp_edge_amt_read();
		pstRegCfgInfo->stSharpenRegCfg.u16SharpAmt1 = hi_isp_sharp_amt_read();
		//pstRegCfgInfo->stSharpenRegCfg.edgeCmp = hi_isp_sharpen_comp_read();         
	    pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1 = hi_isp_sharp_thd1_read(); 
		pstRegCfgInfo->stSharpenRegCfg.u16SharpThd2 = hi_isp_sharp_thd2_read(); 
	    pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1MulCoef = hi_isp_sharp_coef_read();
	    pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1 = hi_isp_edge_thd1_read(); 
		pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd2 = hi_isp_edge_thd2_read();
	    pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1MulCoef = hi_isp_edge_coef_read(); 
		pstRegCfgInfo->stSharpenRegCfg.u8OvershootThd1 = hi_isp_sharpen_over_thd1_read(); 
		pstRegCfgInfo->stSharpenRegCfg.u8OvershootThd2 = hi_isp_sharpen_over_thd2_read(); 
		pstRegCfgInfo->stSharpenRegCfg.u8UndershootThd1 = hi_isp_sharpen_under_thd1_read(); 
		pstRegCfgInfo->stSharpenRegCfg.u8UndershootThd2 = hi_isp_sharpen_under_thd2_read(); 
	   	pstRegCfgInfo->stSharpenRegCfg.u8OvershootAmt = hi_isp_over_amt_read();   
	    pstRegCfgInfo->stSharpenRegCfg.u8UndershootAmt = hi_isp_under_amt_read();
		pstRegCfgInfo->stSharpenRegCfg.u8LineThd1 = hi_isp_sharpen_line_thd1_read();
		pstRegCfgInfo->stSharpenRegCfg.u8LineThd2 = hi_isp_sharpen_line_thd2_read();
		pstRegCfgInfo->stSharpenRegCfg.u16UndershootThdMulCoef = hi_isp_sharpen_under_coef_read();
		pstRegCfgInfo->stSharpenRegCfg.u16OvershootThdMulCoef = hi_isp_sharpen_over_coef_read();


		pstRegCfgInfo->stSharpenRegCfg.G3[0][0] = hi_isp_sharpen_mid_tmp00_read();
		pstRegCfgInfo->stSharpenRegCfg.G3[0][1] = hi_isp_sharpen_mid_tmp01_read();
		pstRegCfgInfo->stSharpenRegCfg.G3[0][2] = hi_isp_sharpen_mid_tmp02_read();
		pstRegCfgInfo->stSharpenRegCfg.G3[1][0] = hi_isp_sharpen_mid_tmp10_read();
		pstRegCfgInfo->stSharpenRegCfg.G3[1][1] = hi_isp_sharpen_mid_tmp11_read();
		pstRegCfgInfo->stSharpenRegCfg.G3[1][2] = hi_isp_sharpen_mid_tmp12_read();
		pstRegCfgInfo->stSharpenRegCfg.G3[2][0] = hi_isp_sharpen_mid_tmp20_read();
		pstRegCfgInfo->stSharpenRegCfg.G3[2][1] = hi_isp_sharpen_mid_tmp21_read();
		pstRegCfgInfo->stSharpenRegCfg.G3[2][2] = hi_isp_sharpen_mid_tmp22_read();
#endif		
    }

    if (pstRegCfgInfo->unKey.bit1GeCfg)
    {
        //pstRegCfgInfo->stGeRegCfg.u8GeStrength = hi_isp_green_equalize_ge_strength_read();
    }

    if (pstRegCfgInfo->unKey.bit1DpCfg)
    {
#if 0    
        pstRegCfgInfo->stDpRegCfg.u8DpEnable = hi_isp_defect_pixel_enable_read();
        pstRegCfgInfo->stDpRegCfg.u8GeDpEnable = hi_isp_green_equalize_dp_enable_read();
        pstRegCfgInfo->stDpRegCfg.u8DpPointerReset = hi_isp_defect_pixel_pointer_reset_read();
        pstRegCfgInfo->stDpRegCfg.u8BadPixelThreshold = hi_isp_green_equalize_dp_threshold_read();
        pstRegCfgInfo->stDpRegCfg.u8DpDetectionTrigger = hi_isp_defect_pixel_detection_trigger_read();
        pstRegCfgInfo->stDpRegCfg.u16DpCountIn = hi_isp_defect_pixel_defect_pixel_count_in_read();
        pstRegCfgInfo->stDpRegCfg.u8ShowStaticDefectPixels = hi_isp_defect_pixel_show_static_defect_pixels_read();
        pstRegCfgInfo->stDpRegCfg.u16DynamicDpSlope = hi_isp_green_equalize_dp_slope_read();
        pstRegCfgInfo->stDpRegCfg.u16DynamicDPThresh = hi_isp_green_equalize_dp_threshold_read();
#endif		
    }

    if (pstRegCfgInfo->unKey.bit1WdrCfg)
    {
        //pstRegCfgInfo->stWdrRegCfg.u8TopChannelSwitch = hi_vi_top_channel_switch_read();
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RegConfig(ISP_REG_CFG_S *pstRegCfgInfo)
{
    HI_S32 i, j;
	HI_U32 u32CombinWeight = 0;
	HI_U32 u32CombinWeightNum = 0;
	
    if (pstRegCfgInfo->unKey.bit1AeCfg1)
    {
    	/*added by qlp*/
		hi_isp_ae_mem_wei_waddr_write(0);
		
	    for (i=0; i<15; i++)
	    {
	        for (j=0; j<17; j++)
	        {
	        	u32CombinWeight |= (pstRegCfgInfo->stAeRegCfg1.au8WeightTable[i][j] << (8*u32CombinWeightNum));
				u32CombinWeightNum++;

				/*四个权重值组合成一个32位权重值*/
				if (u32CombinWeightNum == HI_ISP_AE_WEI_COMBIN_COUNT)
				{					
					hi_isp_ae_mem_wei_wdata_write(u32CombinWeight);
					u32CombinWeightNum = 0;
					u32CombinWeight = 0;
				}			
	        }
	    }
		
		if (u32CombinWeightNum != HI_ISP_AE_WEI_COMBIN_COUNT
			&&u32CombinWeightNum != 0)
		{
			hi_isp_ae_mem_wei_wdata_write(u32CombinWeight);			
		}
		//printk("####u32WriteNum == %d####\n", u32WriteNum);
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg1)
    {
#if 1  
        hi_isp_matrix_coefft_r_r_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[0])));
        hi_isp_matrix_coefft_r_g_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[1])));
        hi_isp_matrix_coefft_r_b_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[2])));
        hi_isp_matrix_coefft_g_r_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[3])));
        hi_isp_matrix_coefft_g_g_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[4])));
        hi_isp_matrix_coefft_g_b_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[5])));
        hi_isp_matrix_coefft_b_r_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[6])));
        hi_isp_matrix_coefft_b_g_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[7])));
        hi_isp_matrix_coefft_b_b_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[8])));
#endif		

#if 0    
        hi_isp_matrix_coefft_r_r_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[0]);
        hi_isp_matrix_coefft_r_g_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[1]);
        hi_isp_matrix_coefft_r_b_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[2]);
        hi_isp_matrix_coefft_g_r_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[3]);
        hi_isp_matrix_coefft_g_g_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[4]);
        hi_isp_matrix_coefft_g_b_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[5]);
        hi_isp_matrix_coefft_b_r_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[6]);
        hi_isp_matrix_coefft_b_g_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[7]);
        hi_isp_matrix_coefft_b_b_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[8]);
#endif	

#if 1
        hi_isp_white_balance_gain_00_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[0]);
        hi_isp_white_balance_gain_01_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[1]);
        hi_isp_white_balance_gain_10_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[2]);
        hi_isp_white_balance_gain_11_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[3]);
#endif
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg3)
    {
        hi_isp_metering_max_clip_write(pstRegCfgInfo->stAwbRegCfg3.bAboveWhiteLevelClip);
        hi_isp_metering_min_clip_write(pstRegCfgInfo->stAwbRegCfg3.bBelowBlackLevelClip);
        hi_isp_metering_cr_ref_max_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMaxAwb);
        hi_isp_metering_cr_ref_min_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMinAwb);
        hi_isp_metering_cb_ref_max_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMaxAwb);
        hi_isp_metering_cb_ref_min_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMinAwb);
        hi_isp_metering_max_threshold_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringWhiteLevelAwb);
        hi_isp_metering_min_threshold_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringBlackLevelAwb);
        //hi_isp_awb_top_hypotenuse_k_write(-4096);
        //hi_isp_awb_top_hypotenuse_b_write(0x400);
        //hi_isp_awb_bot_hypotenuse_k_write(-4096);
        //hi_isp_awb_bot_hypotenuse_b_write(0x40);
    }
    
    if (pstRegCfgInfo->unKey.bit1AfCfg)
    {
        hi_isp_metering_af_threshold_write_write(pstRegCfgInfo->stAfRegCfg.u16ThresholdWrite);
        hi_isp_metering_af_metrics_shift_write(pstRegCfgInfo->stAfRegCfg.u8MetricsShift); 
        hi_isp_metering_af_np_offset_write(pstRegCfgInfo->stAfRegCfg.u8NpOffset);
    }

    if (pstRegCfgInfo->unKey.bit1OffsetCfg)
    {
        /* config in isp_black_level.c */
    }

    if (pstRegCfgInfo->unKey.bit1DrcCfg)
    {
        //hi_isp_drc_exposure_write(pstRegCfgInfo->stDrcRegCfg.FrameExposure);
    }

    if (pstRegCfgInfo->unKey.bit1NrCfg)
    {
    	hi_isp_2dnr_luma_nr_enable_write(pstRegCfgInfo->stNrRegCfg.lumaNREnable);
		hi_isp_2dnr_luma_gain_write(pstRegCfgInfo->stNrRegCfg.NrLumaNrGain);
    	hi_isp_2dnr_fcr_en_write(pstRegCfgInfo->stNrRegCfg.falseColorEnable);
		hi_isp_2dnr_fcrshift_write(pstRegCfgInfo->stNrRegCfg.falseColorShift);
		hi_isp_2dnr_fcrgain_write(pstRegCfgInfo->stNrRegCfg.falseColorGain);
		hi_isp_2dnr_fcrlimit_write(pstRegCfgInfo->stNrRegCfg.falseColorLimit);

		hi_isp_2dnr_luma_low_write(pstRegCfgInfo->stNrRegCfg.rangeLumaLow);
		hi_isp_2dnr_luma_high_write(pstRegCfgInfo->stNrRegCfg.rangeLumaHig);

		hi_isp_2dnr_limit_low_write(pstRegCfgInfo->stNrRegCfg.blendLowLimit);
		hi_isp_2dnr_limit_high_write(pstRegCfgInfo->stNrRegCfg.blendHigLimit);

		for (i=0; i<17; i++)
		{
			hi_isp_2dnr_thres_low_write(i, pstRegCfgInfo->stNrRegCfg.rangeLowLUT[i]);
			hi_isp_2dnr_thres_high_write(i, pstRegCfgInfo->stNrRegCfg.rangeHigLUT[i]);
		}
    }

    if (pstRegCfgInfo->unKey.bit1SharpenCfg)
    {        
    	//modify by qlp
#if 1    	
		hi_isp_edge_amt_write(pstRegCfgInfo->stSharpenRegCfg.u16EdgeAmt);
		hi_isp_sharp_amt_write(pstRegCfgInfo->stSharpenRegCfg.u16SharpAmt1);
		hi_isp_sharpen_enpixsel_write(pstRegCfgInfo->stSharpenRegCfg.enPixSel); 
	    hi_isp_sharp_thd1_write(pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1); 
		hi_isp_sharp_thd2_write(pstRegCfgInfo->stSharpenRegCfg.u16SharpThd2); 
	    hi_isp_sharp_coef_write(pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1MulCoef);
	    hi_isp_edge_thd1_write(pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1); 
		hi_isp_edge_thd2_write(pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd2);
	    hi_isp_edge_coef_write(pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1MulCoef); 
		hi_isp_sharpen_over_thd1_write(pstRegCfgInfo->stSharpenRegCfg.u8OvershootThd1); 
		hi_isp_sharpen_over_thd2_write(pstRegCfgInfo->stSharpenRegCfg.u8OvershootThd2); 
		hi_isp_sharpen_under_thd1_write(pstRegCfgInfo->stSharpenRegCfg.u8UndershootThd1); 
		hi_isp_sharpen_under_thd2_write(pstRegCfgInfo->stSharpenRegCfg.u8UndershootThd2); 
	   	hi_isp_over_amt_write(pstRegCfgInfo->stSharpenRegCfg.u8OvershootAmt);   
	    hi_isp_under_amt_write(pstRegCfgInfo->stSharpenRegCfg.u8UndershootAmt);
		hi_isp_sharpen_line_thd1_write(pstRegCfgInfo->stSharpenRegCfg.u8LineThd1);
		hi_isp_sharpen_line_thd2_write(pstRegCfgInfo->stSharpenRegCfg.u8LineThd2);
		hi_isp_sharpen_under_coef_write(pstRegCfgInfo->stSharpenRegCfg.u16UndershootThdMulCoef);
		hi_isp_sharpen_over_coef_write(pstRegCfgInfo->stSharpenRegCfg.u16OvershootThdMulCoef);


		hi_isp_sharpen_mid_tmp00_write(pstRegCfgInfo->stSharpenRegCfg.G3[0][0]);
		hi_isp_sharpen_mid_tmp01_write(pstRegCfgInfo->stSharpenRegCfg.G3[0][1]);
		hi_isp_sharpen_mid_tmp02_write(pstRegCfgInfo->stSharpenRegCfg.G3[0][2]);
		hi_isp_sharpen_mid_tmp10_write(pstRegCfgInfo->stSharpenRegCfg.G3[1][0]);
		hi_isp_sharpen_mid_tmp11_write(pstRegCfgInfo->stSharpenRegCfg.G3[1][1]);
		hi_isp_sharpen_mid_tmp12_write(pstRegCfgInfo->stSharpenRegCfg.G3[1][2]);
		hi_isp_sharpen_mid_tmp20_write(pstRegCfgInfo->stSharpenRegCfg.G3[2][0]);
		hi_isp_sharpen_mid_tmp21_write(pstRegCfgInfo->stSharpenRegCfg.G3[2][1]);
		hi_isp_sharpen_mid_tmp22_write(pstRegCfgInfo->stSharpenRegCfg.G3[2][2]);
#endif		
    }

    if (pstRegCfgInfo->unKey.bit1GeCfg)
    {
        //hi_isp_green_equalize_ge_strength_write(pstRegCfgInfo->stGeRegCfg.u8GeStrength);
        hi_isp_ge_enable_write(pstRegCfgInfo->stGeRegCfg.ge_enable);
        hi_isp_ge_ct_th1_write(pstRegCfgInfo->stGeRegCfg.ct_th1);
		hi_isp_ge_ct_slope1_write(pstRegCfgInfo->stGeRegCfg.detail_slop);
		hi_isp_ge_ct_th2_write(pstRegCfgInfo->stGeRegCfg.detail_th);
		hi_isp_ge_ct_th3_write(pstRegCfgInfo->stGeRegCfg.ge_th);
		hi_isp_ge_ct_slope2_write(pstRegCfgInfo->stGeRegCfg.ge_th_slop);
    }

    if (pstRegCfgInfo->unKey.bit1DpCfg)
    {
    	    hi_isp_dp_in_soft_rst_write(pstRegCfgInfo->stDpRegCfg.u32DpccInSoftRst);
		    //pstRegCfgInfo->stDpRegCfg.u32DpccregsBayerPat
            hi_isp_dp_bpt_ctrl_write(pstRegCfgInfo->stDpRegCfg.u32DpccBptCtrl);
            //hi_isp_dpc_enable_write(pstRegCfgInfo->stDpRegCfg.u32DpccCfg);
            hi_isp_dpc_blend_write(pstRegCfgInfo->stDpRegCfg.u32DpccAlpha);
		    hi_isp_dp_mode_write(pstRegCfgInfo->stDpRegCfg.u32DpccMode);
		    hi_isp_dp_output_mode_write(pstRegCfgInfo->stDpRegCfg.u32DpccOutputMode);
		    hi_isp_dp_set_use_write(pstRegCfgInfo->stDpRegCfg.u32DpccSetUse);
		    hi_isp_dp_methods_set_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccMethodsSet1);
		    hi_isp_dp_methods_set_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccMethodsSet2);
		    hi_isp_dp_methods_set_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccMethodsSet3);
		    hi_isp_dp_line_thresh_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineThresh1);
		    hi_isp_dp_line_mad_fac_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineMadFac1);
		    hi_isp_dp_pg_fac_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccPgFac1);
		    hi_isp_dp_rnd_thresh_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccRndThresh1);
		    hi_isp_dp_rg_fac_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccRgFac1);                         
		    hi_isp_dp_line_thresh_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineThresh2);                         
		    hi_isp_dp_line_mad_fac_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineMadFac2);                    
		    hi_isp_dp_pg_fac_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccPgFac2);                          
		    hi_isp_dp_rnd_thresh_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccRndThresh2);                          
		    hi_isp_dp_rg_fac_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccRgFac2);                           
		    hi_isp_dp_line_thresh_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineThresh3);                          
		    hi_isp_dp_line_mad_fac_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineMadFac3);                           
		    hi_isp_dp_pg_fac_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccPgFac3);                          
		    hi_isp_dp_rnd_thresh_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccRndThresh3);    
			hi_isp_dp_rg_fac_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccRgFac3 );				
		    hi_isp_dp_ro_limits_write(pstRegCfgInfo->stDpRegCfg.u32DpccRoLimits);                           
		    hi_isp_dp_rnd_offs_write(pstRegCfgInfo->stDpRegCfg.u32DpccRndOffs);
			hi_isp_dp_bpt_thresh_write(pstRegCfgInfo->stDpRegCfg.u32DpccBadThresh);
            //pstRegCfgInfo->stDpRegCfg.u32DpccBadThresh
#if 0    
        hi_isp_defect_pixel_enable_write(pstRegCfgInfo->stDpRegCfg.u8DpEnable);
        hi_isp_green_equalize_dp_enable_write(pstRegCfgInfo->stDpRegCfg.u8GeDpEnable);
        hi_isp_defect_pixel_pointer_reset_write(pstRegCfgInfo->stDpRegCfg.u8DpPointerReset);
        hi_isp_green_equalize_dp_threshold_write(pstRegCfgInfo->stDpRegCfg.u8BadPixelThreshold);
        hi_isp_defect_pixel_detection_trigger_write(pstRegCfgInfo->stDpRegCfg.u8DpDetectionTrigger);
        hi_isp_defect_pixel_defect_pixel_count_in_write(pstRegCfgInfo->stDpRegCfg.u16DpCountIn);
        hi_isp_defect_pixel_show_static_defect_pixels_write(pstRegCfgInfo->stDpRegCfg.u8ShowStaticDefectPixels);
        hi_isp_green_equalize_dp_slope_write(pstRegCfgInfo->stDpRegCfg.u16DynamicDpSlope);
        //hi_isp_green_equalize_dp_threshold_write(pstRegCfgInfo->stDpRegCfg.u16DynamicDPThresh);
#endif        
    }

  	if (pstRegCfgInfo->unKey.bit1DehazeCfg)
    {
        
        if (pstRegCfgInfo->stDehazeRegCfg.u8DehazeEnable)
        {
            //hi_vi_dehaze_air_write(pstRegCfgInfo->stDehazeRegCfg.u32DehazeAir);
            hi_isp_dehaze_air_r_write(pstRegCfgInfo->stDehazeRegCfg.u8AirR);
			hi_isp_dehaze_air_g_write(pstRegCfgInfo->stDehazeRegCfg.u8AirG);
			hi_isp_dehaze_air_b_write(pstRegCfgInfo->stDehazeRegCfg.u8AirR);
        }
        
         //hi_vi_dehaze_gstrth_write(pstRegCfgInfo->stDehazeRegCfg.u8Strength);
         //hi_vi_dehaze_update_write(pstRegCfgInfo->stDehazeRegCfg.u32Update);
         hi_isp_dehaze_gstrth_write(pstRegCfgInfo->stDehazeRegCfg.u8Strength);
		 hi_isp_dehaze_pre_update_write(pstRegCfgInfo->stDehazeRegCfg.u32Update);
    }
    
    if (pstRegCfgInfo->unKey.bit1WdrCfg)
    {
        //hi_vi_top_channel_switch_write(pstRegCfgInfo->stWdrRegCfg.u8TopChannelSwitch);
    }
	
	if (pstRegCfgInfo->unKey.bit1LscCfg)
	{
        /* write horizontal grid info */
	    for (i = 0; i < 8; i++)
	    {
	        hi_isp_lsc_hwin_info_write(i, (2*(pstRegCfgInfo->stLscRegCfg.xgrid[i]) - 1), pstRegCfgInfo->stLscRegCfg.xinvgrid[i]);
	    }

	    /* write vertical grid info */
	    for (i = 0; i < 8; i++)
	    {
	        hi_isp_lsc_vwin_info_write(i, (2*(pstRegCfgInfo->stLscRegCfg.ygrid[i]) - 1), pstRegCfgInfo->stLscRegCfg.yinvgrid[i]);
	    }

        /* write gain data */
		hi_isp_lsc_grr_gain_waddr_write(0);
		for (i = 0; i < 289; i++)
		{
		    hi_isp_lsc_grr_gain_wdata_write(pstRegCfgInfo->stLscRegCfg.grr_gain[i]);
		}

		hi_isp_lsc_gbb_gain_waddr_write(0);
		for (i = 0; i < 289; i++)
		{
		    hi_isp_lsc_gbb_gain_wdata_write(pstRegCfgInfo->stLscRegCfg.gbb_gain[i]);
		}
	
		pstRegCfgInfo->unKey.bit1LscCfg = 0;
	}

	//if (pstRegCfgInfo->unKey.bit1UvnrCfg)
	if (0)
    {
        hi_isp_uvnr_sel_write(pstRegCfgInfo->stUVNrRegCfg.bUvnrSel);
		hi_isp_nr_en_write(pstRegCfgInfo->stUVNrRegCfg.bNrEn);
		hi_isp_midf_en_write(pstRegCfgInfo->stUVNrRegCfg.bMidfEn);
		hi_isp_uv2c_mode_write(pstRegCfgInfo->stUVNrRegCfg.bUv2cMode);
		hi_isp_uv2c_en_write(pstRegCfgInfo->stUVNrRegCfg.bUv2cEn);
		hi_isp_vcds_en_write(pstRegCfgInfo->stUVNrRegCfg.bVcdsEn);
		hi_isp_hcds_en_write(pstRegCfgInfo->stUVNrRegCfg.bHcdsEn);
		hi_isp_hcds_width_out_write(pstRegCfgInfo->stUVNrRegCfg.s32WidthOut);
		hi_isp_hcds_width_in_write(pstRegCfgInfo->stUVNrRegCfg.s32WidthIn);
		hi_isp_vcus_mode_write(pstRegCfgInfo->stUVNrRegCfg.bVcusMode);
		hi_isp_vcds_mode_write(pstRegCfgInfo->stUVNrRegCfg.bVcdsMode);
		hi_isp_hcds_mode_write(pstRegCfgInfo->stUVNrRegCfg.bHcdsMode);
		hi_isp_hcds_coef0_write(pstRegCfgInfo->stUVNrRegCfg.s32HCoef0);
		hi_isp_hcds_coef1_write(pstRegCfgInfo->stUVNrRegCfg.s32HCoef1);
		hi_isp_hcds_coef2_write(pstRegCfgInfo->stUVNrRegCfg.s32HCoef2);
		hi_isp_hcds_coef3_write(pstRegCfgInfo->stUVNrRegCfg.s32HCoef3);
		hi_isp_hcds_coef4_write(pstRegCfgInfo->stUVNrRegCfg.s32HCoef4);
		hi_isp_hcds_coef5_write(pstRegCfgInfo->stUVNrRegCfg.s32HCoef5);
		hi_isp_hcds_coef6_write(pstRegCfgInfo->stUVNrRegCfg.s32HCoef6);
		hi_isp_hcds_coef7_write(pstRegCfgInfo->stUVNrRegCfg.s32HCoef7);
		hi_isp_vcds_coef0_write(pstRegCfgInfo->stUVNrRegCfg.s32VCoef0);
		hi_isp_vcds_coef1_write(pstRegCfgInfo->stUVNrRegCfg.s32VCoef1);
		hi_isp_vcds_coef2_write(pstRegCfgInfo->stUVNrRegCfg.s32VCoef2);
		hi_isp_vcds_coef3_write(pstRegCfgInfo->stUVNrRegCfg.s32VCoef3);
		hi_isp_midfilter_width_write(pstRegCfgInfo->stUVNrRegCfg.s32MfWidth);
		hi_isp_midfilter_height_write(pstRegCfgInfo->stUVNrRegCfg.s32MfHeight);
		hi_isp_uvnr_width_write(pstRegCfgInfo->stUVNrRegCfg.s32UvnrWidth);
		hi_isp_uvnr_height_write(pstRegCfgInfo->stUVNrRegCfg.s32UvnrHeight);
		hi_isp_uvnr_diffthres1_write(pstRegCfgInfo->stUVNrRegCfg.s32Diffthres1);
		hi_isp_uvnr_diffthres2_write(pstRegCfgInfo->stUVNrRegCfg.s32Diffthres2);
		hi_isp_uvnr_diffthres3_write(pstRegCfgInfo->stUVNrRegCfg.s32Diffthres3);
		hi_isp_uvnr_diffthres4_write(pstRegCfgInfo->stUVNrRegCfg.s32Diffthres4);
		hi_isp_uvnr_diffthres5_write(pstRegCfgInfo->stUVNrRegCfg.s32Diffthres5);
		hi_isp_uvnr_diffthres6_write(pstRegCfgInfo->stUVNrRegCfg.s32Diffthres6);
		hi_isp_uvnr_ratiolimit0_write(pstRegCfgInfo->stUVNrRegCfg.s32Ratiolimit0);
		hi_isp_uvnr_ratiolimit1_write(pstRegCfgInfo->stUVNrRegCfg.s32Ratiolimit1);
		hi_isp_uvnr_ratiolimit2_write(pstRegCfgInfo->stUVNrRegCfg.s32Ratiolimit2);
		hi_isp_uvnr_ratiolimit3_write(pstRegCfgInfo->stUVNrRegCfg.s32ratiolimit3);
		hi_isp_uvnr_ratiolimit4_write(pstRegCfgInfo->stUVNrRegCfg.s32Ratiolimit4);
		hi_isp_uvnr_ratiolimit5_write(pstRegCfgInfo->stUVNrRegCfg.s32Ratiolimit5);
		hi_isp_uvnr_ratiolimit6_write(pstRegCfgInfo->stUVNrRegCfg.s32Ratiolimit6);
		hi_isp_uvnr_blendratio_write(pstRegCfgInfo->stUVNrRegCfg.s32Blendratio);
		hi_isp_uvnr_diffshift_write(pstRegCfgInfo->stUVNrRegCfg.s32Diffshift);
		hi_isp_uvnr_coring_limit_write(pstRegCfgInfo->stUVNrRegCfg.u8CoringLimit);
    }

	if (pstRegCfgInfo->unKey.bit1DemCfg)
	{
		hi_isp_demosaic_fcr_gain_write(pstRegCfgInfo->stDemRegCfg.u8FcrGain);
		hi_isp_demosaic_ahd_par1_write(pstRegCfgInfo->stDemRegCfg.u16AndPart1);
		hi_isp_demosaic_bld_limit1_write(pstRegCfgInfo->stDemRegCfg.u8hvBlendLimit1);
		hi_isp_demosaic_bld_limit2_write(pstRegCfgInfo->stDemRegCfg.u8hvBlendLimit2);
		hi_isp_demosaic_hv_ratio_write(pstRegCfgInfo->stDemRegCfg.u8hvColorRatio);
	        hi_isp_demosaic_fcr_ratio_write(pstRegCfgInfo->stDemRegCfg.u8FcrRatio);	
	        hi_isp_demosaic_fcr_limit1_write(pstRegCfgInfo->stDemRegCfg.u16FcrLimit1);
	        hi_isp_demosaic_fcr_limit2_write(pstRegCfgInfo->stDemRegCfg.u16FcrLimit2);	
	        hi_isp_demosaic_fcr_thr_write(pstRegCfgInfo->stDemRegCfg.u16FcrThr);
	}
	
    return HI_SUCCESS;
}


HI_S32 ISP_RegCfgInit(ISP_DEV IspDev, HI_VOID **ppCfg)
{
    ISP_REGCFG_S *pstRegCfg = HI_NULL;

    REGCFG_GET_CTX(IspDev, pstRegCfg);

    if (!pstRegCfg->bInit)
    {
        pstRegCfg->stRegCfg.unKey.u32Key = 0x3FFF;

        ISP_RegConfigInit(&pstRegCfg->stRegCfg);

        pstRegCfg->bInit = HI_TRUE;
    }
    
    pstRegCfg->stRegCfg.unKey.u32Key = 0;
	pstRegCfg->stRegCfg.stKernelCfg.unKey.u32Key = 0;
    
    *ppCfg = &pstRegCfg->stRegCfg;
    
    return HI_SUCCESS;
}

HI_S32 ISP_RegCfgSet(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_REGCFG_S *pstRegCfg = HI_NULL;

    REGCFG_GET_CTX(IspDev, pstRegCfg);

	pstRegCfg->stRegCfg.stKernelCfg.unKey.bit1UvnrCfg = pstRegCfg->stRegCfg.unKey.bit1UvnrCfg;
	memcpy(&pstRegCfg->stRegCfg.stKernelCfg.stUVNrRegCfg, &pstRegCfg->stRegCfg.stUVNrRegCfg, sizeof(ISP_UVNR_REG_CFG_S));
	
    ISP_RegConfig(&pstRegCfg->stRegCfg);

    if(pstRegCfg->stRegCfg.stKernelCfg.unKey.u32Key)
    {    	
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_REG_CFG_SET, &pstRegCfg->stRegCfg.stKernelCfg);
        if (HI_SUCCESS != s32Ret)
        {
            printf("Config ISP register Failed with ec %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SyncCfgSet(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_REGCFG_S *pstRegCfg = HI_NULL;
    ISP_SNS_REGS_INFO_S *pstSnsRegsInfo = NULL;

    REGCFG_GET_CTX(IspDev, pstRegCfg);

    ISP_SensorUpdateSnsReg(IspDev);
    ISP_SensorGetSnsReg(IspDev, &pstSnsRegsInfo);
    memcpy(&pstRegCfg->stSyncCfgNode.stSnsRegsInfo, pstSnsRegsInfo, sizeof(ISP_SNS_REGS_INFO_S));

    memcpy(&pstRegCfg->stSyncCfgNode.stAERegCfg, &pstRegCfg->stRegCfg.stAeRegCfg2, sizeof(ISP_AE_REG_CFG_2_S));
    pstRegCfg->stSyncCfgNode.bValid = HI_TRUE;

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_SYNC_CFG_SET, &pstRegCfg->stSyncCfgNode);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Config Sync register Failed with ec %#x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

