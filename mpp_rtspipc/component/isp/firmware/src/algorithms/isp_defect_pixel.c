/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_defect_pixel.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : qiuleping q00214668,mawenjing m00294153
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include "isp_sensor.h"
#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_DPCC_MODE (35)
#define ISP_DPCC_HOT_MODE  (12)
#define ISP_DPCC_DEAD_MODE (76)
#define ISP_DPCC_HIGHLIGHT_MODE (160)
#define ISP_CALIBRATION_PARAMS_SET (1)
#define ISP_HOTPIXEL_MAXCOUNT (1024)
#define ISP_DEADPIXEL_MAXCOUNT (1024)
#define ISP_HOT_DEV_THRESH (20)
#define ISP_DEAD_DEV_THRESH (15)
#define ISP_DPCC_BPDATA (16383)
#define ISP_DPCC_ENABLE (1)
#define ISP_DPCC_PAR_MAX_COUNT (9)//(6)
#define ISP_DPC_SLOPE_GRADE (5)

static const HI_S32 g_as32IsoLutLow[16] = {100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};   
static const HI_S32 g_as32IsoLutHigh[16] = {100,250,500,1200,2000,4000,8000,16000,32000,64000,128000,256000,512000,1024000,2048000,4096000};
static const HI_S32 g_as32DpcLineThr1[27] 		= 	{0x0808,0x0808,0x0808,0x0808,0x0808,0x0808,0x0808,0x0808,0x0808,0x0808,0x0808,0x0808,0x0707,0x0606,0x0505,0x0404,0x0404,0x0404,0x0404,0x0303,0x0202,0x0202,0x0202,0x0101,0x0101,0x0101,0x0101};
static const HI_S32 g_as32DpcLineMadFac1[27] 	= 	{0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0303,0x0303,0x0303,0x0303,0x0303};
static const HI_S32 g_as32DpcPgFac1[27] 		= 	{0x0404,0x0404,0x0404,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303,0x0303};
static const HI_S32 g_as32DpcRndThr1[27] 		= 	{0x0a0a,0x0a0a,0x0a0a,0x0a0a,0x0808,0x0808,0x0808,0x0808,0x0808,0x0808,0x0808,0x0808,0x0707,0x0606,0x0505,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404,0x0404};
static const HI_S32 g_as32DpcRgFac1[27] 		= 	{0x1f1f,0x1e1e,0x1d1d,0x1d1d,0x1d1d,0x1b1b,0x1919,0x1717,0x1515,0x1313,0x1111,0x1010,0x1010,0x1010,0x1010,0x1010,0x0d0d,0x0c0c,0x0a0a,0x0a0a,0x0a0a,0x0808,0x0808,0x0808,0x0606,0x0404,0x0202};
static const HI_S32 g_as32DpcRoLimits1[27] 		= 	{0x0dfa,0x0dfa,0x0efe,0x0efe,0x0efe,0x0efe,0x0efe,0x0efe,0x0efe,0x0efe,0x0efe,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff};
static const HI_S32 g_as32DpcRndOffs1[27] 		= 	{0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff,0x0fff};
static const HI_S32 g_as32SlopeGrade[5]			=	{0,76,99,100,127};


typedef struct hiISP_DEFECT_PIXEL_S
{
    /* Public */
	HI_BOOL bEnable;                // enable dpc module
	HI_BOOL bStaticEnable;			//enable static correction
	HI_BOOL bDynamicEnable;			//enable dynamic correction
	HI_BOOL bStaCalibrationEn;      // enable static calibration
	HI_BOOL bStaicShow;				 
	HI_BOOL bDynamicManual;

	HI_BOOL bIREn;
	HI_BOOL bIRGr;
	HI_U8 u8PixelDetectType;       //0: hot pixel detect; 1: dead pixel detect;
	HI_U8 u8FrameCnt;
	HI_U8 u8StaticDPThresh;
	HI_U8 u8TrialCount;
	HI_U8 u8TrialCntLimit;
	HI_U8 u8CalibStarted;
	HI_U8 u8StaCalibrationParamsSet;
	HI_U16  u16BlendRatio;
	HI_U16  u16Slope;	
	HI_U16  u16BadPixelsCount;
	
	HI_U32 u32DpccMode;
	HI_U32 u32DpccNormalMode;
	HI_U32 u32DpccHotMode;
	HI_U32 u32DpccDeadMode;
	HI_U32 u32DpccStaticDpShowMode;
	HI_U32 u32DpccBpData;    
	HI_U32 u32DpccBadThresh; 
	HI_U8  u8HotDevThresh;
	HI_U8  u8DeadDevThresh;
	ISP_CMOS_DPC_S stCmosDpc;
}ISP_DEFECT_PIXEL_S;

typedef struct 
{
	HI_U32 u32DpccInSoftRst;	
	HI_U32 u32DpccBptCtrl;     
	HI_U32 u32DpccOutputMode;  
	HI_U32 u32DpccSetUse;      
	HI_U32 u32DpccMethodsSet1; 
	HI_U32 u32DpccMethodsSet2; 
	HI_U32 u32DpccMethodsSet3;      
	HI_U32 u32DpccBadThresh;
	HI_U32 u32DpccCfg;
	HI_U32 u32DpccBayerPat;
	HI_U32 u32DpccMode;
	HI_U32 u32DpccAlpha;
} ISP_DPC_CFG_S;

static ISP_DPC_CFG_S g_stDpcDefCfg[ISP_DPCC_PAR_MAX_COUNT] =
{
	//0~75
	{0x1,0x3FFF0330,0x0003,0x0001 ,0x1F1F,0x0707,0x1F1F,0xff800080,0x1,0x1,0x23,0x0},
	//ori set 1  (76)
	{0x1,0x3FFF0330,0x0003,0x0003 ,0x1F1F,0x0707,0x1F1F,0xff800080,0x1,0x1,0x23,0x0},	
	//ori set 2 (99)
	{0x1,0x3FFF0330,0x0003,0x0003 ,0x1F1F,0x0707,0x1F1F,0xff800080,0x1,0x1,0x23,0x0},	
	// set 23(RB set3, G set2) (100)
	{0x1,0x3FFF0330,0x0003,0x0007 ,0x1D1F,0x0707,0x1F1F,0xff800080,0x1,0x1,0x23,0x0},
	//101 ~127
	{0x1,0x3FFF0330,0x0003,0x0001 ,0x1F1F,0x0707,0x1F1F,0xff800080,0x1,0x1,0x23,0x0},
};


typedef struct
{
  	HI_U32 u32DpccLineThrRb1; 
	HI_U32 u32DpccLineThrG1; 
	HI_U32 u32DpccLineMadFacRb1;
	HI_U32 u32DpccLineMadFacG1;
	HI_U32 u32DpccPgFacRb1; 
	HI_U32 u32DpccPgFacG1;
	HI_U32 u32DpccRndThrRb1; 
	HI_U32 u32DpccRndThrG1; 
	HI_U32 u32DpccRgFacRb1;
	HI_U32 u32DpccRgFacG1;

	HI_U32 u32DpccLineThrRb2; 
	HI_U32 u32DpccLineThrG2; 
	HI_U32 u32DpccLineMadFacRb2; 
	HI_U32 u32DpccLineMadFacG2; 
	HI_U32 u32DpccPgFacRb2; 
	HI_U32 u32DpccPgFacG2; 
	HI_U32 u32DpccRndThrRb2; 
	HI_U32 u32DpccRndThrG2; 
	HI_U32 u32DpccRgFacRb2; 
	HI_U32 u32DpccRgFacG2; 

	HI_U32 u32DpccLineThrRb3; 
	HI_U32 u32DpccLineThrG3; 
	HI_U32 u32DpccLineMadFacRb3; 
	HI_U32 u32DpccLineMadFacG3; 
	HI_U32 u32DpccPgFacRb3; 
	HI_U32 u32DpccPgFacG3; 
	HI_U32 u32DpccRndThrRb3; 
	HI_U32 u32DpccRndThrG3; 
	HI_U32 u32DpccRgFacRb3; 
	HI_U32 u32DpccRgFacG3; 

	HI_U32 u32DpccRoRb3; 
	HI_U32 u32DpccRoG3; 
	HI_U32 u32DpccRoRb2; 
	HI_U32 u32DpccRoG2; 
	HI_U32 u32DpccRoRb1; 
	HI_U32 u32DpccRoG1; 

	HI_U32 u32DpccRndOffsRb3; 
	HI_U32 u32DpccRndOffsG3; 
	HI_U32 u32DpccRndOffsRb2; 
	HI_U32 u32DpccRndOffsG2; 
	HI_U32 u32DpccRndOffsRb1; 
	HI_U32 u32DpccRndOffsG1;
}ISP_DPCC_DERIVED_PARAM_S;


static ISP_DPCC_DERIVED_PARAM_S g_stDpcDerParam[5] =
{
	//0
	{0x54,0x54,0x1B,0x1B,0x08,0x08,0x0A,0x0A,0x26,0x26,0x21,0x21,0x18,0x10,0x0B,0x0B,0x08,0x08,0x08,0x08,0x20,0x20,0x04,0x04,0x0A,0x0A,0x08,0x06,0x04,0x04,0x2,0x1,0x2,0x2,0x1,0x1,0x2,0x2,0x2,0x2,0x2,0x2},	
	//76
	{0x08,0x08,0x1B,0x1B,0x08,0x08,0x0A,0x0A,0x26,0x26,0x21,0x21,0x18,0x10,0x0B,0x0B,0x08,0x08,0x08,0x08,0x20,0x20,0x04,0x04,0x0A,0x0A,0x08,0x06,0x04,0x04,0x2,0x1,0x2,0x2,0x1,0x1,0x2,0x2,0x2,0x2,0x2,0x2},	
	//99
	{0x08,0x08,0x04,0x04,0x08,0x08,0x0A,0x0A,0x20,0x20,0x10,0x10,0x18,0x10,0x08,0x06,0x08,0x08,0x08,0x08,0x20,0x20,0x04,0x04,0x0A,0x0A,0x08,0x06,0x04,0x04,0x2,0x1,0x3,0x3,0x2,0x2,0x3,0x3,0x3,0x3,0x3,0x3},	
	//100
	{0x08,0x08,0x04,0x04,0x08,0x08,0x0A,0x0A,0x20,0x20,0x10,0x10,0x18,0x10,0x08,0x06,0x08,0x08,0x08,0x08,0x20,0x20,0x04,0x04,0x0A,0x0A,0x08,0x06,0x04,0x04,0x2,0x1,0x3,0x3,0x2,0x2,0x3,0x3,0x3,0x3,0x3,0x3},
	//127
	{0x01,0x01,0x03,0x03,0x03,0x03,0x04,0x04,0x08,0x08,0x10,0x10,0x18,0x10,0x08,0x06,0x08,0x08,0x08,0x08,0x20,0x20,0x04,0x04,0x0A,0x0A,0x08,0x06,0x04,0x04,0x2,0x1,0x3,0x3,0x2,0x2,0x3,0x3,0x3,0x3,0x3,0x3},
};


ISP_DEFECT_PIXEL_S g_astDpCtx[ISP_MAX_DEV_NUM] = {{0}};
#define DP_GET_CTX(dev, pstCtx)   pstCtx = &g_astDpCtx[dev]

static HI_VOID DpExtRegsDefault(HI_VOID)
{
    /* initial register of defect pixel calibration */
	hi_ext_system_dpc_manual_mode_write(HI_FALSE);
	hi_ext_system_dpc_static_cor_enable_write(HI_EXT_SYSTEM_DPC_STATIC_COR_ENABLE_DEFAULT);
	hi_ext_system_dpc_static_calib_enable_write(HI_EXT_SYSTEM_DPC_STATIC_CALIB_ENABLE_DEFAULT);
	hi_ext_system_dpc_count_max_write(HI_EXT_SYSTEM_DPC_COUNT_MAX_DEFAULT);
	hi_ext_system_dpc_count_min_write(HI_EXT_SYSTEM_DPC_COUNT_MIN_DEFAULT);
	hi_ext_system_dpc_start_thresh_write(HI_EXT_SYSTEM_DPC_START_THRESH_DEFAULT);
	hi_ext_system_dpc_trigger_status_write(HI_EXT_SYSTEM_DPC_TRIGGER_STATUS_DEFAULT);
	hi_ext_system_dpc_trigger_time_write(HI_EXT_SYSTEM_DPC_TRIGGER_TIME_DEFAULT);
	hi_ext_system_dpc_static_defect_type_write(HI_EXT_SYSTEM_DPC_STATIC_DEFECT_TYPE_DEFAULT);
	hi_ext_system_dpc_static_dp_show_write(HI_EXT_SYSTEM_DPC_STATIC_DP_SHOW_DEFAULT);
	
	hi_ext_system_dpc_dynamic_cor_enable_write(HI_EXT_SYSTEM_DPC_DYNAMIC_COR_ENABLE_DEFAULT);
	hi_ext_system_dpc_dynamic_manual_enable_write(HI_EXT_SYSTEM_DPC_DYNAMIC_MANUAL_ENABLE_DEFAULT);
	hi_ext_system_dpc_dynamic_strength_write(HI_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_DEFAULT);
	hi_ext_system_dpc_dynamic_blend_ratio_write(HI_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_DEFAULT);
   	return;
}

static HI_VOID DpRegsDefault(HI_VOID) 
{
	hi_isp_dpc_enable_write(HI_TRUE);
    return;
}

static HI_VOID DpExtRegsInitialize(ISP_DEV IspDev)
{
	HI_U8 i;
	ISP_DEFECT_PIXEL_S *pstDp = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;      

	DP_GET_CTX(IspDev, pstDp);
	ISP_SensorGetDefault(IspDev, &pstSnsDft);

	memcpy(&pstDp->stCmosDpc, &pstSnsDft->stDpc, sizeof(ISP_CMOS_DPC_S));

	for (i = 0;i<16;i++)
	{
		hi_ext_system_dpc_dynamic_strength_table_write(i,pstDp->stCmosDpc.au16Slope[i]);
		hi_ext_system_dpc_dynamic_blend_ratio_table_write(i,pstDp->stCmosDpc.au16BlendRatio[i]);
	}
	
	return;
}

static HI_VOID DpRegsInitialize(ISP_DEV IspDev)
{
	ISP_CTX_S *pstIspCtx = HI_NULL;    
	HI_U8 u8WDRMode;
  
	ISP_GET_CTX(IspDev, pstIspCtx);

    u8WDRMode = pstIspCtx->u8SnsWDRMode;


	if (IS_LINEAR_MODE(u8WDRMode))
	{
		/* Bypass pregamma block */
		hi_isp_module_dcg_sel_write(0);
	}
	else if (IS_BUILT_IN_WDR_MODE(u8WDRMode)
			|| IS_FS_WDR_MODE(u8WDRMode))
	{					 
		hi_isp_module_dcg_sel_write(1);
	}
	else
	{
		hi_isp_module_dcg_sel_write(0);
	}

    return;
}

static HI_S32 DpReadExtregs(ISP_DEV IspDev)
{
	HI_U8 i;
	ISP_DEFECT_PIXEL_S *pstDp = HI_NULL;

	DP_GET_CTX(IspDev, pstDp);

	pstDp->bIREn = hi_isp_rgbir_en_read();
	if(pstDp->bIREn)
	{
		pstDp->bIRGr = !((HI_BOOL)hi_isp_rgbir_ir_position_read());
	}
	else
	{
		pstDp->bIRGr =0;
	}

	pstDp->bStaticEnable	= hi_ext_system_dpc_static_cor_enable_read();
	pstDp->bStaCalibrationEn= hi_ext_system_dpc_static_calib_enable_read();
	pstDp->bStaicShow		= hi_ext_system_dpc_static_dp_show_read();
	pstDp->u8PixelDetectType= hi_ext_system_dpc_static_defect_type_read();
	pstDp->u8TrialCntLimit	= (HI_U8)(hi_ext_system_dpc_trigger_time_read() >> 3);

	pstDp->bDynamicEnable	= hi_ext_system_dpc_dynamic_cor_enable_read();
	pstDp->bDynamicManual	= hi_ext_system_dpc_dynamic_manual_enable_read();
		
	for (i = 0;i<16;i++)
	{
		pstDp->stCmosDpc.au16Slope[i] 		= hi_ext_system_dpc_dynamic_strength_table_read(i);
		pstDp->stCmosDpc.au16BlendRatio[i]	= hi_ext_system_dpc_dynamic_blend_ratio_table_read(i);
	}

	pstDp->u16BlendRatio= hi_ext_system_dpc_dynamic_blend_ratio_read();
	pstDp->u16Slope		= hi_ext_system_dpc_dynamic_strength_read();
	
	return HI_SUCCESS;
}

static HI_S32 DpInit(ISP_DEV IspDev,ISP_DEFECT_PIXEL_S *pstDp)
{
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	
	ISP_SensorGetDefault(IspDev, &pstSnsDft);
 	
	pstDp->bIREn= pstSnsDft->stRgbir.stRgbirAttr.bEnable;
	pstDp->bIRGr = (pstSnsDft->stRgbir.stRgbirAttr.enIrPosType == 0) ? HI_TRUE : HI_FALSE;
	
	pstDp->u32DpccMode	= (pstDp->bIREn<<9)+(pstDp->bIREn<<8)+ISP_DPCC_MODE;

	
	pstDp->bEnable          = HI_TRUE;
	pstDp->bStaticEnable	= HI_TRUE;
	pstDp->bDynamicEnable	= HI_TRUE;
	pstDp->bStaCalibrationEn= HI_FALSE;
	pstDp->bStaicShow		= HI_FALSE;
	pstDp->bDynamicManual	= HI_FALSE;
	pstDp->u8PixelDetectType= ISP_STATIC_DP_BRIGHT;
	
	pstDp->u8TrialCount		= 0;
	pstDp->u8CalibStarted	= 0;
	pstDp->u32DpccBpData 	= ISP_DPCC_BPDATA;
	pstDp->u8HotDevThresh	= ISP_HOT_DEV_THRESH;
	pstDp->u8DeadDevThresh	= ISP_DEAD_DEV_THRESH;
	pstDp->u16BadPixelsCount= 0;
	pstDp->u8FrameCnt 		= 0;
	pstDp->u32DpccBadThresh = 0xff800080;
	pstDp->u16BlendRatio	= 0x0;
	pstDp->u16Slope			= 0x0;
	pstDp->u8StaCalibrationParamsSet = ISP_CALIBRATION_PARAMS_SET;
	return HI_SUCCESS;
}

HI_S32 ISP_DpInit(ISP_DEV IspDev)
{
    ISP_DEFECT_PIXEL_S *pstDp = HI_NULL;
    
    DP_GET_CTX(IspDev, pstDp);

    DpRegsDefault();
    DpExtRegsDefault();
    DpRegsInitialize(IspDev);
    DpExtRegsInitialize(IspDev);

    DpInit(IspDev,pstDp);

    return HI_SUCCESS;
}

static HI_S32 DpEnter(ISP_DEV IspDev, ISP_DEFECT_PIXEL_S *pstDp)
{    
	ISP_SensorSetPixelDetect(IspDev, HI_TRUE);

	/*Should bypass digital gain when enter defect pixel calibration*/
	hi_isp_dg_enable_write(0);

	pstDp->u8StaticDPThresh	= hi_ext_system_dpc_start_thresh_read();
	pstDp->u8CalibStarted	= 1;

	return HI_SUCCESS;
}

static HI_S32 DpExit(ISP_DEV IspDev, ISP_DEFECT_PIXEL_S *pstDp)
{    
	DpInit(IspDev,pstDp);

	ISP_SensorSetPixelDetect(IspDev, HI_FALSE);
	pstDp->u8CalibStarted = 0;

	/*Not bypass digital gain when finish defect pixel calibration*/
	hi_isp_dg_enable_write(1);

	return HI_SUCCESS;
}
HI_S32 ISP_DPC_Linear_Interpol(HI_S32 xm,HI_S32 x0,HI_S32 y0,HI_S32 x1,HI_S32 y1)
{
	HI_S32 ym;

    if ( xm <= x0 ) return y0;
    if ( xm >= x1 ) return y1;

    ym = (y1 - y0) * (xm - x0) / (x1 - x0) + y0;
    return ym;
}

static HI_VOID set_dpcc_parameters_inter(ISP_DP_REG_CFG_S *pstIspDpccHwCfg,HI_U32 u32DpccStat)
{
	HI_S32 s32StatIdxUp,s32StatIdxLow;
	HI_S32 i;
	ISP_DPCC_DERIVED_PARAM_S stDpcDerParam;
	HI_S32 s32StatUpper,s32StatLower;

	s32StatIdxUp = ISP_DPC_SLOPE_GRADE - 1;
	for (i = 0; i < ISP_DPC_SLOPE_GRADE;i++)
	{
		if ((HI_S32)u32DpccStat < g_as32SlopeGrade[i])
		{
			s32StatIdxUp = i;
			break;
		}
	}
	s32StatIdxLow = MAX2(s32StatIdxUp - 1, 0);

	s32StatUpper = g_as32SlopeGrade[s32StatIdxUp];
	s32StatLower = g_as32SlopeGrade[s32StatIdxLow];

	pstIspDpccHwCfg->u32DpccInSoftRst	= g_stDpcDefCfg[s32StatIdxLow].u32DpccInSoftRst;
	pstIspDpccHwCfg->u32DpccBptCtrl		= g_stDpcDefCfg[s32StatIdxLow].u32DpccBptCtrl;
	pstIspDpccHwCfg->u32DpccOutputMode	= g_stDpcDefCfg[s32StatIdxLow].u32DpccOutputMode;
	pstIspDpccHwCfg->u32DpccSetUse		= g_stDpcDefCfg[s32StatIdxLow].u32DpccSetUse;
	pstIspDpccHwCfg->u32DpccMethodsSet1 = g_stDpcDefCfg[s32StatIdxLow].u32DpccMethodsSet1;
	pstIspDpccHwCfg->u32DpccMethodsSet2 = g_stDpcDefCfg[s32StatIdxLow].u32DpccMethodsSet2;
	pstIspDpccHwCfg->u32DpccMethodsSet3 = g_stDpcDefCfg[s32StatIdxLow].u32DpccMethodsSet3;
	pstIspDpccHwCfg->u32DpccBadThresh	= g_stDpcDefCfg[s32StatIdxLow].u32DpccBadThresh;
	pstIspDpccHwCfg->u32DpccCfg			= g_stDpcDefCfg[s32StatIdxLow].u32DpccCfg;
	pstIspDpccHwCfg->u32DpccregsBayerPat= g_stDpcDefCfg[s32StatIdxLow].u32DpccBayerPat;
	pstIspDpccHwCfg->u32DpccMode		= g_stDpcDefCfg[s32StatIdxLow].u32DpccMode;
	pstIspDpccHwCfg->u32DpccAlpha		= g_stDpcDefCfg[s32StatIdxLow].u32DpccAlpha;

	//LineThr1 (0~0xFF)
	stDpcDerParam.u32DpccLineThrRb1 = (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineThrRb1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineThrRb1);
	stDpcDerParam.u32DpccLineThrG1	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineThrG1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineThrG1);
	//lineMadFac1(0~0x3F)
	stDpcDerParam.u32DpccLineMadFacRb1	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineMadFacRb1,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineMadFacRb1);
	stDpcDerParam.u32DpccLineMadFacG1	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineMadFacG1,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineMadFacG1);
	//Pg1 (0~0x3F)
	stDpcDerParam.u32DpccPgFacRb1 	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccPgFacRb1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccPgFacRb1);
	stDpcDerParam.u32DpccPgFacG1 	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccPgFacG1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccPgFacG1);
	//Rnd1 (0~0xFF)
	stDpcDerParam.u32DpccRndThrRb1	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndThrRb1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndThrRb1);
	stDpcDerParam.u32DpccRndThrG1	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndThrG1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndThrG1);
	//Rg1 (0~0x3F)
	stDpcDerParam.u32DpccRgFacRb1	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRgFacRb1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRgFacRb1);
	stDpcDerParam.u32DpccRgFacG1 	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRgFacG1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRgFacG1);
	//Rnd offset1 (0~0x3)
	stDpcDerParam.u32DpccRndOffsRb1	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndOffsRb1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndOffsRb1);
	stDpcDerParam.u32DpccRndOffsG1	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndOffsG1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndOffsG1);
	//Ro limit 1 (0~0x3)
	stDpcDerParam.u32DpccRoRb1		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRoRb1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRoRb1);
	stDpcDerParam.u32DpccRoG1		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRoG1,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRoG1);


	//LineThr2 (0~0xFF)
	stDpcDerParam.u32DpccLineThrRb2	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineThrRb2,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineThrRb2);
	stDpcDerParam.u32DpccLineThrG2	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineThrG2,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineThrG2);
	//lineMadFac2(0~0x3F)
	stDpcDerParam.u32DpccLineMadFacRb2	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineMadFacRb2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineMadFacRb2);
	stDpcDerParam.u32DpccLineMadFacG2	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineMadFacG2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineMadFacG2);
	//Pg2 (0~0x3F)
	stDpcDerParam.u32DpccPgFacRb2 		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccPgFacRb2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccPgFacRb2);
	stDpcDerParam.u32DpccPgFacG2		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccPgFacG2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccPgFacG2);

	//Rnd2 (0~0xFF)
	stDpcDerParam.u32DpccRndThrRb2		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndThrRb2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndThrRb2);
	stDpcDerParam.u32DpccRndThrG2 		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndThrG2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndThrG2);
	//Rg2 (0~0x3F)
	stDpcDerParam.u32DpccRgFacRb2		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRgFacRb2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRgFacRb2);
	stDpcDerParam.u32DpccRgFacG2		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRgFacG2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRgFacG2);
	//Rnd offset2 (0~0x3)
	stDpcDerParam.u32DpccRndOffsRb2		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndOffsRb2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndOffsRb2);
	stDpcDerParam.u32DpccRndOffsG2		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndOffsG2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndOffsG2);
	//Ro limit 2 (0~0x3)
	stDpcDerParam.u32DpccRoRb2			= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRoRb2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRoRb2);
	stDpcDerParam.u32DpccRoG2			= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRoG2,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRoG2);

	//LineThr3 (0~0xFF)
	stDpcDerParam.u32DpccLineThrRb3		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineThrRb3,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineThrRb3);
	stDpcDerParam.u32DpccLineThrG3		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineThrG3,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineThrG3);
	//lineMadFac3(0~0x3F)
	stDpcDerParam.u32DpccLineMadFacRb3	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineMadFacRb3,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineMadFacRb3);
	stDpcDerParam.u32DpccLineMadFacG3	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																		  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccLineMadFacG3,
																		  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccLineMadFacG3);
	//Pg3 (0~0x3F)
	stDpcDerParam.u32DpccPgFacRb3 = (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccPgFacRb3,
																	s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccPgFacRb3);
	stDpcDerParam.u32DpccPgFacG3 = (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccPgFacG3,
																	s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccPgFacG3);

	//Rnd3 (0~0xFF)
	stDpcDerParam.u32DpccRndThrRb3	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndThrRb3,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndThrRb3);
	stDpcDerParam.u32DpccRndThrG3 	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndThrG3,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndThrG3);
	//Rg3 (0~0x3F)
	stDpcDerParam.u32DpccRgFacRb3	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																      s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRgFacRb3,
																      s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRgFacRb3);
	stDpcDerParam.u32DpccRgFacG3 	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRgFacG3,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRgFacG3);
	//Rnd offset3 (0~0x3)
	stDpcDerParam.u32DpccRndOffsRb3	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndOffsRb3,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndOffsRb3);
	stDpcDerParam.u32DpccRndOffsG3	= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRndOffsG3,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRndOffsG3);
	//Ro limit 3 (0~0x3)
	stDpcDerParam.u32DpccRoRb3		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRoRb3,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRoRb3);
	stDpcDerParam.u32DpccRoG3		= (HI_U32)ISP_DPC_Linear_Interpol(u32DpccStat,
																	  s32StatLower,g_stDpcDerParam[s32StatIdxLow].u32DpccRoG3,
																	  s32StatUpper,g_stDpcDerParam[s32StatIdxUp].u32DpccRoG3);
	//method set 1
	pstIspDpccHwCfg->u32DpccLineThresh1	= ((stDpcDerParam.u32DpccLineThrRb1&0xFF)<<8)+(stDpcDerParam.u32DpccLineThrG1 & 0xFF);
	pstIspDpccHwCfg->u32DpccLineMadFac1	= ((stDpcDerParam.u32DpccLineMadFacRb1 &0x3F)<<8)+(stDpcDerParam.u32DpccLineMadFacG1 & 0x3F);
	pstIspDpccHwCfg->u32DpccPgFac1		= ((stDpcDerParam.u32DpccPgFacRb1 &0x3F)<<8)+(stDpcDerParam.u32DpccPgFacG1 & 0x3F);
	pstIspDpccHwCfg->u32DpccRndThresh1	= ((stDpcDerParam.u32DpccRndThrRb1 &0xFF)<<8)+(stDpcDerParam.u32DpccRndThrG1 & 0xFF);
	pstIspDpccHwCfg->u32DpccRgFac1		= ((stDpcDerParam.u32DpccRgFacRb1 &0x3F)<<8)+(stDpcDerParam.u32DpccRgFacG1 & 0x3F);
	//method set 2
	pstIspDpccHwCfg->u32DpccLineThresh2 = ((stDpcDerParam.u32DpccLineThrRb2 &0xFF)<<8)+(stDpcDerParam.u32DpccLineThrG2 & 0xFF);
	pstIspDpccHwCfg->u32DpccLineMadFac2 = ((stDpcDerParam.u32DpccLineMadFacRb2 &0x3F)<<8)+(stDpcDerParam.u32DpccLineMadFacG2 & 0x3F);
	pstIspDpccHwCfg->u32DpccPgFac2		= ((stDpcDerParam.u32DpccPgFacRb2 &0x3F)<<8)+(stDpcDerParam.u32DpccPgFacG2 & 0x3F);
	pstIspDpccHwCfg->u32DpccRndThresh2	= ((stDpcDerParam.u32DpccRndThrRb2 &0xFF)<<8)+(stDpcDerParam.u32DpccRndThrG2 & 0xFF);
	pstIspDpccHwCfg->u32DpccRgFac2		= ((stDpcDerParam.u32DpccRgFacRb2 &0x3F)<<8)+(stDpcDerParam.u32DpccRgFacG2 & 0x3F);
	//method set 3
	pstIspDpccHwCfg->u32DpccLineThresh3 	= ((stDpcDerParam.u32DpccLineThrRb3 &0xFF)<<8)+(stDpcDerParam.u32DpccLineThrG3 & 0xFF);
	pstIspDpccHwCfg->u32DpccLineMadFac3	= ((stDpcDerParam.u32DpccLineMadFacRb3 &0x3F)<<8)+(stDpcDerParam.u32DpccLineMadFacG3 & 0x3F);
	pstIspDpccHwCfg->u32DpccPgFac3		= ((stDpcDerParam.u32DpccPgFacRb3 &0x3F)<<8)+(stDpcDerParam.u32DpccPgFacG3 & 0x3F);
	pstIspDpccHwCfg->u32DpccRndThresh3	= ((stDpcDerParam.u32DpccRndThrRb3&0xFF)<<8)+(stDpcDerParam.u32DpccRndThrG3 & 0xFF);
	pstIspDpccHwCfg->u32DpccRgFac3		= ((stDpcDerParam.u32DpccRgFacRb3&0x3F)<<8)+(stDpcDerParam.u32DpccRgFacG3 & 0x3F);

	pstIspDpccHwCfg->u32DpccRoLimits 	= ((stDpcDerParam.u32DpccRoRb3&0x3)<<10)+((stDpcDerParam.u32DpccRoG3&0x3)<<8)+((stDpcDerParam.u32DpccRoRb2&0x3)<<6)+
		((stDpcDerParam.u32DpccRoG2&0x3)<<4)+((stDpcDerParam.u32DpccRoRb1&0x3)<<2)+(stDpcDerParam.u32DpccRoG1&0x3);
	pstIspDpccHwCfg->u32DpccRndOffs		= ((stDpcDerParam.u32DpccRndOffsRb3&0x3)<<10)+((stDpcDerParam.u32DpccRndOffsG3&0x3)<<8)+((stDpcDerParam.u32DpccRndOffsRb2&0x3)<<6)+
		((stDpcDerParam.u32DpccRndOffsG2&0x3)<<4)+((stDpcDerParam.u32DpccRndOffsRb1&0x3)<<2)+(stDpcDerParam.u32DpccRndOffsG1&0x3);
}


	
HI_S32 ISP_Dynamic_set(HI_S32 s32Iso,ISP_DP_REG_CFG_S *pstDpcHwCfg,ISP_DEFECT_PIXEL_S *pstDpcFwCfg)
{
	HI_U8 i = 0;  
	ISP_CMOS_DPC_S *pstDpc = &pstDpcFwCfg->stCmosDpc;
	HI_U32 u32DpccMode = pstDpcFwCfg->u32DpccMode;
	
	HI_U8  u8Alpha0RB=0;
	HI_U8  u8Alpha0G =0;	//the blend ratio of 5 & 9
	HI_U8  u8Alpha1RB=0;
	HI_U8  u8Alpha1G =0;	//the blend ratio of input data and filtered result
	HI_U16 u16BlendRatio = 0x0;	
	HI_S32 s32IsoIndexUpper,s32IsoIndexLower;
	static HI_U32 u32DpccStat = 0;

	s32IsoIndexUpper = ISP_AUTO_ISO_STENGTH_NUM - 1;
	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
    {
        if (s32Iso <= g_as32IsoLutLow[i])
        {
            s32IsoIndexUpper = i;
            break;
        }
    }
	
    s32IsoIndexLower = MAX2(s32IsoIndexUpper - 1, 0);


	if (pstDpcFwCfg->bDynamicManual)
	{
		if(pstDpcFwCfg->u16Slope> 255)
		{
			printf("Invalid Strength Input!");
			return HI_FAILURE;
		}
		u32DpccStat = pstDpcFwCfg->u16Slope;
		u16BlendRatio = pstDpcFwCfg->u16BlendRatio;
	}
	else
	{
		u32DpccStat		= (HI_U32)ISP_DPC_Linear_Interpol(s32Iso,
													  	  g_as32IsoLutLow[s32IsoIndexLower],(HI_S32)pstDpc->au16Slope[s32IsoIndexLower],
													  	  g_as32IsoLutLow[s32IsoIndexUpper],(HI_S32)pstDpc->au16Slope[s32IsoIndexUpper]);
		u16BlendRatio 	= (HI_U16)ISP_DPC_Linear_Interpol(s32Iso,
														  g_as32IsoLutLow[s32IsoIndexLower],(HI_S32)pstDpc->au16BlendRatio[s32IsoIndexLower],
														  g_as32IsoLutLow[s32IsoIndexUpper],(HI_S32)pstDpc->au16BlendRatio[s32IsoIndexUpper]);
	}

	u32DpccStat >>=1;
	set_dpcc_parameters_inter(pstDpcHwCfg,u32DpccStat);

	if (u32DpccStat == 0)
	{
		u32DpccMode &= 0xFFFFFFFC;
	}
	else if (u32DpccStat > 100)
	{
		pstDpcHwCfg->u32DpccSetUse		= 0x1;
		pstDpcHwCfg->u32DpccMethodsSet1 = 0x1f1f;
		pstDpcHwCfg->u32DpccLineThresh1 = g_as32DpcLineThr1[u32DpccStat-101];
		pstDpcHwCfg->u32DpccLineMadFac1 = g_as32DpcLineMadFac1[u32DpccStat-101];
		pstDpcHwCfg->u32DpccPgFac1		= g_as32DpcPgFac1[u32DpccStat-101];
		pstDpcHwCfg->u32DpccRndThresh1	= g_as32DpcRndThr1[u32DpccStat-101];
		pstDpcHwCfg->u32DpccRgFac1		= g_as32DpcRgFac1[u32DpccStat-101];
		pstDpcHwCfg->u32DpccRoLimits	= g_as32DpcRoLimits1[u32DpccStat-101];
		pstDpcHwCfg->u32DpccRndOffs		= g_as32DpcRndOffs1[u32DpccStat-101];
	}
	
	if(!((u32DpccMode & 0x2)>>1))
	{
		u16BlendRatio = 0;
	}
	
	u8Alpha0RB = (u16BlendRatio>0x80)? u16BlendRatio-0x80:0x0; 
	u8Alpha1RB = (u16BlendRatio>0x80)? 0x80: u16BlendRatio;
	pstDpcHwCfg->u32DpccAlpha = (u8Alpha0RB << 24) + (u8Alpha0G << 16) + (u8Alpha1RB << 8) + u8Alpha1G;
	
	pstDpcHwCfg->u32DpccMode = u32DpccMode;
	return HI_SUCCESS;

}

HI_VOID ISP_Dpcc_Fw(HI_S32 s32Iso, ISP_DP_REG_CFG_S *pstDpcHwCfg, ISP_DEFECT_PIXEL_S *pstDpcFwCfg)
{
	HI_U8 	u8DpccIRChannel ;
	HI_U8 	u8DpccGrayscaleMode;
	HI_U8 	u8ParamSet		= pstDpcFwCfg->u8StaCalibrationParamsSet;
	HI_U32 	u32DpccMode 	= pstDpcFwCfg->u32DpccMode;
	HI_U32 	u32DpccBpData 	= pstDpcFwCfg->u32DpccBpData;
	HI_U32 	u32DpccBadThresh= pstDpcFwCfg->u32DpccBadThresh;
	
	HI_BOOL bDpccBptDetEn 		= ((u32DpccMode       & 0x8) >> 3);
	HI_BOOL bDpccDpHighlightEn 	= ((u32DpccMode  & 0x80)>>7);
	
	if (bDpccBptDetEn == HI_TRUE)
	{
		set_dpcc_parameters_inter(pstDpcHwCfg, u8ParamSet);
		pstDpcHwCfg->u32DpccBadThresh	= u32DpccBadThresh;
		pstDpcHwCfg->u32DpccMode		= u32DpccMode;
	}
	else
	{
		ISP_Dynamic_set(s32Iso,pstDpcHwCfg,pstDpcFwCfg);	
	}

	if (bDpccDpHighlightEn)
	{
		pstDpcHwCfg->u32DpccBptCtrl = (pstDpcHwCfg->u32DpccBptCtrl & 0x0000FFFF)+ (u32DpccBpData << 16);
	}
	
	//grayscale mode, clear bit 9 of dpccmode 
	u8DpccIRChannel = (u32DpccMode & 0x200)>>9;
	u8DpccGrayscaleMode = (u32DpccMode& 0x10)>>4;
	if(u8DpccIRChannel && u8DpccGrayscaleMode)
	{
		pstDpcHwCfg->u32DpccMode &= ~(1 << 9);
	}
	return;
}

HI_VOID ISP_Dpc_StaticCalibration(ISP_DEV IspDev, ISP_DEFECT_PIXEL_S  *pstDp)
{
	if(0 == pstDp->u8PixelDetectType)
	{
		if(pstDp->u8FrameCnt < 9)
		{
			if(pstDp->u8FrameCnt == 1)
			{			 
				hi_ext_system_dpc_trigger_status_write(ISP_STATE_INIT);
				DpEnter(IspDev, pstDp);    
			}
			
			pstDp->u8FrameCnt++;
			
			 if(pstDp->u8FrameCnt == 4)
			{
				pstDp->u32DpccBadThresh = (pstDp->u8StaticDPThresh<<24)+(((50+0x80*pstDp->u8HotDevThresh)/100)<<16)+0x00000080;
				pstDp->u32DpccMode = pstDp->u32DpccHotMode;
				hi_ext_system_dpc_dynamic_cor_enable_write(HI_FALSE);
				hi_ext_system_dpc_static_cor_enable_write(HI_FALSE);
			}

			if(pstDp->u8FrameCnt == 6)
			{
				pstDp->u32DpccMode = pstDp->u32DpccNormalMode;
				hi_ext_system_dpc_dynamic_cor_enable_write(HI_TRUE);
				hi_ext_system_dpc_static_cor_enable_write(HI_TRUE);
			}

			 if (pstDp->u8FrameCnt == 8)
			{
				pstDp->u16BadPixelsCount = hi_isp_dp_bpt_calib_number_read();

				if (pstDp->u8TrialCount >= pstDp->u8TrialCntLimit)
				{
					printf("BAD PIXEL CALIBRATION TIME OUT  %x\n", pstDp->u8TrialCntLimit);
					DpExit(IspDev, pstDp);
					pstDp->bStaCalibrationEn = HI_FALSE;
					hi_ext_system_dpc_static_calib_enable_write(HI_FALSE);
					hi_ext_system_dpc_finish_thresh_write(pstDp->u8StaticDPThresh);
					hi_ext_system_dpc_trigger_status_write(0x2);
				}
				else if (pstDp->u16BadPixelsCount > hi_ext_system_dpc_count_max_read())
				{
					printf("BAD_PIXEL_COUNT_UPPER_LIMIT %x, %x\n", pstDp->u8StaticDPThresh, pstDp->u16BadPixelsCount );
					pstDp->u8FrameCnt = 2;
					pstDp->u8StaticDPThresh++;
					pstDp->u8TrialCount ++;
				}
				else if (pstDp->u16BadPixelsCount < hi_ext_system_dpc_count_min_read())
				{
					printf("BAD_PIXEL_COUNT_LOWER_LIMIT %x, %x\n", pstDp->u8StaticDPThresh, pstDp->u16BadPixelsCount);
					pstDp->u8FrameCnt = 2;

					if(pstDp->u8StaticDPThresh == 1 )
					{
						pstDp->u8TrialCount = pstDp->u8TrialCntLimit;
					}
					pstDp->u8StaticDPThresh--;                    
					pstDp->u8TrialCount ++;
				}
				else
				{
					printf("trial: %x, findshed: %x\n", pstDp->u8TrialCount, pstDp->u16BadPixelsCount);
					DpExit(IspDev, pstDp);  
					pstDp->bStaCalibrationEn = HI_FALSE;
					hi_ext_system_dpc_static_calib_enable_write(HI_FALSE);
					hi_ext_system_dpc_finish_thresh_write(pstDp->u8StaticDPThresh);
					hi_ext_system_dpc_trigger_status_write(0x1);
				}	
			}				
		}
	}
	else if(1 == pstDp->u8PixelDetectType)
	{
		if(pstDp->u8FrameCnt < 9)
		{
			if(pstDp->u8FrameCnt == 1)
			{			 
				hi_ext_system_dpc_trigger_status_write(ISP_STATE_INIT);
				pstDp->u8CalibStarted = 1;
				pstDp->u8StaticDPThresh = hi_ext_system_dpc_start_thresh_read();
			}
			
			pstDp->u8FrameCnt++;
			
			 if(pstDp->u8FrameCnt == 4)
			{
				pstDp->u32DpccBadThresh = 0xFF800000+(pstDp->u8StaticDPThresh<<8)+((0x80*pstDp->u8DeadDevThresh)/100);
				pstDp->u32DpccMode = pstDp->u32DpccDeadMode;
				hi_ext_system_dpc_dynamic_cor_enable_write(HI_FALSE);
				hi_ext_system_dpc_static_cor_enable_write(HI_FALSE);
			}

			if(pstDp->u8FrameCnt == 6)
			{
				pstDp->u32DpccMode = pstDp->u32DpccNormalMode;
				hi_ext_system_dpc_dynamic_cor_enable_write(HI_TRUE);
				hi_ext_system_dpc_static_cor_enable_write(HI_TRUE);
			}

			 if (pstDp->u8FrameCnt == 8)
			{
				pstDp->u16BadPixelsCount = hi_isp_dp_bpt_calib_number_read();

				if (pstDp->u8TrialCount >= pstDp->u8TrialCntLimit)
				{
					printf("BAD PIXEL CALIBRATION TIME OUT  %x\n", pstDp->u8TrialCntLimit);
					DpInit(IspDev,pstDp);
					pstDp->u8CalibStarted = 0;
					pstDp->bStaCalibrationEn = HI_FALSE;
					hi_ext_system_dpc_static_calib_enable_write(HI_FALSE);
					hi_ext_system_dpc_trigger_status_write(0x2);
					hi_ext_system_dpc_finish_thresh_write(pstDp->u8StaticDPThresh);
				}
				else if (pstDp->u16BadPixelsCount > hi_ext_system_dpc_count_max_read())
				{
					printf("BAD_PIXEL_COUNT_UPPER_LIMIT %x, %x\n", pstDp->u8StaticDPThresh, pstDp->u16BadPixelsCount );
					pstDp->u8FrameCnt = 2;
					pstDp->u8StaticDPThresh++;
					pstDp->u8TrialCount ++;
				}
				else if (pstDp->u16BadPixelsCount < hi_ext_system_dpc_count_min_read())
				{
					printf("BAD_PIXEL_COUNT_LOWER_LIMIT %x, %x\n", pstDp->u8StaticDPThresh, pstDp->u16BadPixelsCount);
					pstDp->u8FrameCnt = 2;

					if(pstDp->u8StaticDPThresh == 1 )
					{
						pstDp->u8TrialCount = pstDp->u8TrialCntLimit;
					}
					pstDp->u8StaticDPThresh--;                    
					pstDp->u8TrialCount ++;
				}
				else
				{
					printf("trial: %x, findshed: %x\n", pstDp->u8TrialCount, pstDp->u16BadPixelsCount);
					DpInit(IspDev,pstDp);
					pstDp->u8CalibStarted = 0;
					pstDp->bStaCalibrationEn = HI_FALSE;
					hi_ext_system_dpc_static_calib_enable_write(HI_FALSE);
					hi_ext_system_dpc_trigger_status_write(0x1);
					hi_ext_system_dpc_finish_thresh_write(pstDp->u8StaticDPThresh);
				}	
			}	
		}
	}
	else
	{
		printf("invalid static defect pixel detect type!\n");
	}
	return ;
}

HI_S32 ISP_DpRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	ISP_CTX_S           *pstIspCtx  = HI_NULL;
	ISP_DEFECT_PIXEL_S  *pstDp      = HI_NULL;
	ISP_REG_CFG_S       *pstRegCfg  = (ISP_REG_CFG_S *)pRegCfg;

	DP_GET_CTX(IspDev, pstDp);    
	ISP_GET_CTX(IspDev, pstIspCtx); 
	DpReadExtregs(IspDev);
	pstDp->u32DpccNormalMode		= ( pstDp->bIREn<<9 )+( pstDp->bIRGr<<8 ) + ISP_DPCC_MODE;
	pstDp->u32DpccHotMode			= ( pstDp->bIREn<<9 )+( pstDp->bIRGr<<8 ) + ISP_DPCC_HOT_MODE;
	pstDp->u32DpccDeadMode			= ( pstDp->bIREn<<9 )+( pstDp->bIRGr<<8 ) + ISP_DPCC_DEAD_MODE;
	pstDp->u32DpccStaticDpShowMode	= ( pstDp->bIREn<<9 )+( pstDp->bIRGr<<8 ) + ISP_DPCC_HIGHLIGHT_MODE;
	pstDp->u32DpccMode 				= pstDp->u32DpccNormalMode;
	
	if((!pstDp->bStaCalibrationEn) &&(pstDp->u8CalibStarted == 1))
	{
		DpExit(IspDev, pstDp); 
	}

	if (pstDp->bStaCalibrationEn)//calibration mode
	{	
		pstIspCtx->stLinkage.bDefectPixel = HI_TRUE;	
		ISP_Dpc_StaticCalibration(IspDev,pstDp);
	}
	else if(pstDp->bStaicShow) //highlight static defect pixels mode
	{
		//under highlight mode,should enble satic correction moudle and disable dynamic correction moudle
		pstIspCtx->stLinkage.bDefectPixel = HI_FALSE;	
		pstDp->u32DpccMode = pstDp->u32DpccStaticDpShowMode;
		hi_ext_system_dpc_static_cor_enable_write(HI_TRUE);
		hi_ext_system_dpc_dynamic_cor_enable_write(HI_FALSE);	
	}
	else//normal detection and correction mode
	{
		pstIspCtx->stLinkage.bDefectPixel = HI_FALSE;
		pstDp->u32DpccMode =(pstDp->u32DpccNormalMode &0x3df)+(pstDp->bStaticEnable<<5);
		pstDp->u32DpccMode =(pstDp->u32DpccMode &0x3fd)+(pstDp->bDynamicEnable<<1);
	}

	ISP_Dpcc_Fw((HI_S32)pstIspCtx->stLinkage.u32Iso, &pstRegCfg->stDpRegCfg, pstDp);

	
	if (!hi_ext_system_dpc_manual_mode_read())
	{
		pstRegCfg->unKey.bit1DpCfg = 1;
	}

	if (pstDp->bEnable != pstRegCfg->stDpRegCfg.u32DpccCfg)
	{
		pstDp->bEnable = pstRegCfg->stDpRegCfg.u32DpccCfg;
		hi_isp_dpc_enable_write(pstRegCfg->stDpRegCfg.u32DpccCfg);		
	}
	
    return HI_SUCCESS;
}

HI_S32 ISP_DpCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
	HI_U8 u8Mode = *((HI_U8 *)pValue);

    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET:
			
            if (IS_LINEAR_MODE(u8Mode))
            {
                /* Bypass pregamma block */
				hi_isp_module_dcg_sel_write(0);
            }
            else if (IS_BUILT_IN_WDR_MODE(u8Mode)
					|| IS_FS_WDR_MODE(u8Mode))
            {                    
				hi_isp_module_dcg_sel_write(1);
            }
            else
            {
                /* unknow u8Mode */
				hi_isp_module_dcg_sel_write(0);
            }
            break;
        default :
            break;
    }
	
    return HI_SUCCESS;
}

HI_S32 ISP_DpExit(ISP_DEV IspDev)
{
	hi_isp_dpc_enable_write(HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterDp(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_DP;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_DpInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_DpRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_DpCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_DpExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

