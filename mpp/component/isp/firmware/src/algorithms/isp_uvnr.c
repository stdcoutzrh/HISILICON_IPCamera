/******************************************************************************

  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_uvnr.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/12/01
  Description   : 
  History       :
  1.Date        : 2014/12/01
    Author      : q00214668
    Modification: Created file

******************************************************************************/
#include <math.h>
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_proc.h"
#include "isp_ext_config.h"
#include "hi_math.h"
#include "isp_math_utils.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_ISP_UVNR_ISO_COUNT    (16)

#define UVNR_nrStrength 	(2)
#define UVNR_sigmaSP 	(3)
#define ISO_Med2UVNR_THD 	(500)
#define ISO_UVNR2Med_THD (600)
#define HI_ISP_UVNR_ENFILTERH    (1)
#define HI_ISP_UVNR_ENFILTERV    (1)
/*1:Enable the color Median Filter£»0£ºDisable the color Median Filter*/
#define HI_ISP_UVNR_ENMIDFILTERH (0)
/*1£ºEnable the UVNR in high ISO£¬0: Disable the UVNR*/
#define HI_ISP_UVNR_ENUVNR       (1)
/*   Enable the UV2C, divide tje UV data into single C data. 0: close; 1: open   */
#define HI_ISP_UVNR_ENUV2C       (1)
/*  UV2C bypass mode. 0: output U data; 1: output V data*/
#define HI_ISP_UVNR_UV2CMODE     (0)

/* UVNR strength : 0-32*/
#define HI_ISP_UVNR_BLENDRATIO     (32)

static const HI_S32 Coeff_Filter_8tap_H[2][8]={{-16,0,145,254,145,0,-16,0},{0,0,128,128,128,128,0,0}};											
static const HI_S32 Coeff_Discard_8pixel_H[8]={0,0,0,511,0,0,0,0};

static const HI_S32 Coeff_Filer_4tap_V[2][4]={{4,4,6,6},{3,3,3,3}};	//0: multiply 1£»1: multiply 2£»2: multiply 4£»3: multiply 8£»4: multiply 16£»5: multiply 32£»6: multiply 0£»7: invalid£»
static const HI_S32 Coeff_Discard_4tap_V[4]={5,6,6,6};

static const HI_S32 Coeff_Filter_Y_V[4]={1,1,1,1};

static const HI_S32 g_UvnrLutIso  [ISP_AUTO_ISO_STENGTH_NUM] ={100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};

/*  range: 0-100, float, */
//static const HI_S32 g_UvnrLutSigma[ISP_AUTO_ISO_STENGTH_NUM] ={8000,10000,12000,16000,18000,20000,22000,24000,26000};
//static const HI_S32 g_CoringIsoLut[ISP_AUTO_ISO_STENGTH_NUM] ={10000,20000,40000,80000,100000,150000,200000,300000,400000};
//static const HI_S8  g_CoringLimitLut[ISP_AUTO_ISO_STENGTH_NUM] ={0,0,0,1,1,1,2,2,2};

typedef enum {
	UVNR_2UP_SCALE = 0,
	UVNR_4UP_SCALE = 1
}UVNR_UP_SCALE_MODE;

typedef enum {
	UVNR_PIXEL_OUT_FORMAT_420 = 0,
	UVNR_PIXEL_OUT_FORMAT_422 = 1,
	UVNR_PIXEL_OUT_FORMAT_BUTT
}UVNR_PIXEL_OUT_FORMAT;

typedef struct hiHI_ISP_UVNR_ATTR
{
	/*common*/
	HI_BOOL bUvnrMedSel;        //0: med; 1: uvnr
	HI_BOOL bEnFilterH;
	HI_BOOL bEnFilterV;
	HI_U32  u32Width;
	HI_U32  u32Height;
	UVNR_PIXEL_OUT_FORMAT enPixelFmt;
	HI_U32 u32Iso_UVNR2Med_Thd;
	HI_U32 u32Iso_Med2UVNR_Thd;
	/*MF*/
	HI_BOOL bEnMedianFilter;
	HI_U32 u32DsTimesV;
	/*UVNR*/
	HI_BOOL bEnUVNR;
	HI_BOOL bEnUV2C;
	HI_U8   u8Uv2cMode;

	HI_U8   au8UvnrStrength[ISP_AUTO_ISO_STENGTH_NUM];
	
	ISP_CMOS_UVNR_S stCmosUvnr;
}ISP_UVNR_ATTR;


ISP_UVNR_ATTR g_astUvnrCtx[ISP_MAX_DEV_NUM];

#define UVNR_GET_CTX(dev, pstCtx)   pstCtx = &g_astUvnrCtx[dev]

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#ifndef ABS
#define ABS(a)         ((a)<0?-(a):(a))
#endif

static HI_VOID UvnrRegsDefault(HI_VOID) 
{    
	hi_isp_midfilter_timing_write(HI_ISP_MIDFILTER_TIMING_DEFAULT);  
	hi_isp_midfilter_auto_timing_en_write(HI_FALSE);
    
    /* init uvnr param */
    hi_isp_uvnr_sel_write(0);
    hi_isp_nr_en_write(0);
    hi_isp_midf_en_write(0);
    hi_isp_uv2c_mode_write(0);
    hi_isp_uv2c_en_write(1);

    hi_isp_vcds_en_write(1);
    hi_isp_hcds_en_write(1);
    hi_isp_vcus_mode_write(0);
    hi_isp_vcds_mode_write(0);
    hi_isp_hcds_mode_write(0);

	//-16,0,145,254,145,0,-16,0
    //4,4,6,6
	hi_isp_hcds_coef0_write(-16);
	hi_isp_hcds_coef1_write(0);
	hi_isp_hcds_coef2_write(145);
	hi_isp_hcds_coef3_write(254);
	hi_isp_hcds_coef4_write(145);
	hi_isp_hcds_coef5_write(0);
	hi_isp_hcds_coef6_write(-16);
	hi_isp_hcds_coef7_write(0);
	hi_isp_vcds_coef0_write(4);
	hi_isp_vcds_coef1_write(4);
	hi_isp_vcds_coef2_write(6);
	hi_isp_vcds_coef3_write(6);

	hi_isp_midfilter_width_write(0);
	hi_isp_midfilter_height_write(0);
	hi_isp_uvnr_width_write(0);
	hi_isp_uvnr_height_write(0);
	hi_isp_uvnr_diffthres1_write(0);
	hi_isp_uvnr_diffthres2_write(0);
	hi_isp_uvnr_diffthres3_write(0);
	hi_isp_uvnr_diffthres4_write(0);
	hi_isp_uvnr_diffthres5_write(0);
	hi_isp_uvnr_diffthres6_write(0);
	hi_isp_uvnr_ratiolimit0_write(0);
	hi_isp_uvnr_ratiolimit1_write(0);
	hi_isp_uvnr_ratiolimit2_write(0);
	hi_isp_uvnr_ratiolimit3_write(0);
	hi_isp_uvnr_ratiolimit4_write(0);
	hi_isp_uvnr_ratiolimit5_write(0);
	hi_isp_uvnr_ratiolimit6_write(0);
	hi_isp_uvnr_blendratio_write(0);
	hi_isp_uvnr_diffshift_write(0);
	hi_isp_uvnr_coring_limit_write(0);
    return;
}

static HI_VOID UvnrRegsInitialize(ISP_DEV IspDev)
{
	HI_U32 i;
	ISP_UVNR_ATTR *pstUvnrCtx     = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;   
    
    UVNR_GET_CTX(IspDev, pstUvnrCtx);

	ISP_SensorGetDefault(IspDev, &pstSnsDft);			

	/*get default para from sensor*/
	memcpy(&pstUvnrCtx->stCmosUvnr, &pstSnsDft->stUvnr, sizeof(ISP_CMOS_UVNR_S));

	hi_ext_system_uvnr_manu_mode_write(OP_TYPE_BUTT);
    hi_ext_system_uvnr_enable_write(HI_TRUE);

	pstUvnrCtx->bUvnrMedSel     = HI_FALSE;                   //0: med; 1: uvnr
	pstUvnrCtx->bEnFilterH      = HI_ISP_UVNR_ENFILTERH;
	pstUvnrCtx->bEnFilterV      = HI_ISP_UVNR_ENFILTERV;
	pstUvnrCtx->bEnMedianFilter = HI_ISP_UVNR_ENMIDFILTERH;
	pstUvnrCtx->bEnUVNR         = HI_ISP_UVNR_ENUVNR;
	pstUvnrCtx->bEnUV2C         = HI_ISP_UVNR_ENUV2C;
	pstUvnrCtx->u8Uv2cMode      = HI_ISP_UVNR_UV2CMODE;
	pstUvnrCtx->enPixelFmt      = UVNR_PIXEL_OUT_FORMAT_420;
	pstUvnrCtx->u32Iso_Med2UVNR_Thd = ISO_Med2UVNR_THD;
	pstUvnrCtx->u32Iso_UVNR2Med_Thd = ISO_UVNR2Med_THD;

	/*write to extern register*/
	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
		hi_ext_system_uvnr_color_cast_write(i, pstUvnrCtx->stCmosUvnr.Coring_lutLimit[i]);
		
		hi_ext_system_uvnr_threshold_write(i, pstUvnrCtx->stCmosUvnr.UVNR_lutSigma[i]);

		if((pstUvnrCtx->bEnMedianFilter == 0) && (pstUvnrCtx->bEnUVNR == 0))
		{
			pstUvnrCtx->au8UvnrStrength[i] = 0;
		}		
 		else 
 		{
 			if(g_UvnrLutIso[i] < pstUvnrCtx->u32Iso_Med2UVNR_Thd)
 			{
 				pstUvnrCtx->au8UvnrStrength[i] = pstUvnrCtx->bEnMedianFilter;
 			} 			
 			else
 			{
 				pstUvnrCtx->au8UvnrStrength[i] = pstUvnrCtx->stCmosUvnr.UVNR_blendRatio[i];
 			} 			
 		}	

		hi_ext_system_uvnr_strength_write(i, pstUvnrCtx->au8UvnrStrength[i]);
	}

	hi_ext_system_manual_uvnr_color_cast_write(HI_EXT_SYSTEM_MANUAL_UVNR_COLOR_CAST_WRITE_DEFAULT);
	hi_ext_system_manual_uvnr_strength_write(HI_EXT_SYSTEM_MANUAL_UVNR_STRENGTH_WRITE_DEFAULT);
	hi_ext_system_manual_uvnr_threshold_write(HI_EXT_SYSTEM_MANUAL_UVNR_THRESHOLD_WRITE_DEFAULT);
	
		
    return;
}

static inline HI_U8 UvnrGetIsoIndex(HI_U32 u32Iso)
{   
	HI_U8 u8Index;
	
    for (u8Index = 0; u8Index < ISP_AUTO_ISO_STENGTH_NUM; u8Index++)
    {
        if (u32Iso <= g_UvnrLutIso[u8Index])
        {
            return u8Index;
        }
    }
	
    return (ISP_AUTO_ISO_STENGTH_NUM - 1);
}

static HI_S32 UvnrReadExtregs(ISP_DEV IspDev)
{   
	HI_U32 i;
    ISP_OP_TYPE_E enUvnrType = OP_TYPE_BUTT;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_UVNR_ATTR *pstUvnrCtx;

    UVNR_GET_CTX(IspDev, pstUvnrCtx); 
	
	pstUvnrCtx->enPixelFmt = hi_isp_pix_format_read();//hi_ext_system_isp_pixel_format_read();
	pstUvnrCtx->u32Width   = (hi_isp_top_active_width_read() + 1);
	pstUvnrCtx->u32Height  = (hi_isp_top_active_height_read() + 1);

	if ((enUvnrType = hi_ext_system_uvnr_manu_mode_read()) >= OP_TYPE_BUTT)
	{
		return HI_SUCCESS;
	}

    if (!hi_ext_system_uvnr_enable_read())
    {
        pstUvnrCtx->bEnMedianFilter = 0;
    	pstUvnrCtx->bEnUVNR = 0;

        return HI_SUCCESS;
    }

    ISP_GET_CTX(IspDev, pstIspCtx);

	/*write to extern register*/
    if (enUvnrType == OP_TYPE_AUTO)
    {
        for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
    	{
    		pstUvnrCtx->stCmosUvnr.Coring_lutLimit[i] = hi_ext_system_uvnr_color_cast_read(i);
    		pstUvnrCtx->stCmosUvnr.UVNR_lutSigma  [i] = hi_ext_system_uvnr_threshold_read(i);
            pstUvnrCtx->au8UvnrStrength           [i] = hi_ext_system_uvnr_strength_read(i);
    	}
    }
    else
    {
          for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
    	{
			pstUvnrCtx->stCmosUvnr.Coring_lutLimit[i] = hi_ext_system_manual_uvnr_color_cast_read();
			pstUvnrCtx->stCmosUvnr.UVNR_lutSigma  [i] = hi_ext_system_manual_uvnr_threshold_read();
			pstUvnrCtx->au8UvnrStrength           [i] = hi_ext_system_manual_uvnr_strength_read();
         }
    }

    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
        if(pstUvnrCtx->au8UvnrStrength[i] < 35
            && pstUvnrCtx->au8UvnrStrength[i] >= 2)
        {
        	pstUvnrCtx->stCmosUvnr.UVNR_blendRatio[i] = pstUvnrCtx->au8UvnrStrength[i] - 2;
        }
        else
        {
        	pstUvnrCtx->stCmosUvnr.UVNR_blendRatio[i] = 32;
        }
	}    

    i = UvnrGetIsoIndex(pstIspCtx->stLinkage.u32Iso);
    
    if(pstUvnrCtx->au8UvnrStrength[i]  == 0)
    {
    	pstUvnrCtx->bEnMedianFilter = 0;
    	pstUvnrCtx->bEnUVNR = 0;
    }
    else if(pstUvnrCtx->au8UvnrStrength[i]  == 1)
    {
    	if(pstUvnrCtx->bEnUVNR == 1)
    		pstUvnrCtx->u32Iso_UVNR2Med_Thd = g_UvnrLutIso[i];
    	pstUvnrCtx->bEnMedianFilter = 1;
    	pstUvnrCtx->bEnUVNR = 0;
    }
    else if(pstUvnrCtx->au8UvnrStrength[i]  < 35)
    {
    	if(pstUvnrCtx->bEnMedianFilter == 1)
    		pstUvnrCtx->u32Iso_Med2UVNR_Thd = g_UvnrLutIso[i];
    	pstUvnrCtx->bEnMedianFilter = 0;
    	pstUvnrCtx->bEnUVNR = 1;
    }
    else
    {
    	if(pstUvnrCtx->bEnMedianFilter == 1)
    		pstUvnrCtx->u32Iso_Med2UVNR_Thd = g_UvnrLutIso[i];
    	pstUvnrCtx->bEnMedianFilter = 0;
    	pstUvnrCtx->bEnUVNR = 1;
    }

    return 0;
}

HI_S32 UvnrProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
	ISP_UVNR_ATTR *pstUvnrCtx = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;
	HI_U32  i;

    UVNR_GET_CTX(IspDev, pstUvnrCtx); 
    ISP_GET_CTX(IspDev, pstIspCtx);

	i = UvnrGetIsoIndex(pstIspCtx->stLinkage.u32Iso);

    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }
		
    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----UVNR INFO-------------------------------------------------------------\n");
	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
		"%8s"         "%8s"   "%8s\n",
		"colorcast" , "thd",  "Sth");

	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
		"%8d"       "%8d"    "%8d\n",
		pstUvnrCtx->stCmosUvnr.Coring_lutLimit[i],
		pstUvnrCtx->stCmosUvnr.UVNR_lutSigma  [i],
		pstUvnrCtx->au8UvnrStrength           [i]);

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}


HI_S32 ISP_UvnrInit(ISP_DEV IspDev)
{
	UvnrRegsDefault();
    UvnrRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

//------------------------------------------------------
// function for gaussian 
//  * only for FW
//------------------------------------------------------
static const float EXP = 2.7182818;
static HI_FLOAT _gauss (HI_FLOAT x, HI_FLOAT sigma) 
{
    HI_FLOAT val = x / sigma;
    return powf(EXP, -val*val/2.);
}

//------------------------------------------------------
// Interpolated value from LUT 
//  * only for FW
//------------------------------------------------------
static HI_FLOAT ISP_Uvnr_GetValueFromLut (HI_S32 x, const HI_S32 * xLut, HI_FLOAT * yLut, HI_S32 length)
{
    HI_S32 n;
	if (x <= xLut[0]) return yLut[0];

    for (n=1; n<length; n++) {
        if (x < xLut[n]) {
            return yLut[n-1] + (yLut[n] - yLut[n-1]) * (x - xLut[n-1]) / (xLut[n] - xLut[n-1]);
        }
    }

    return yLut[length-1];
}

HI_U8 HiISP_UNR_Linear_Interp(HI_S32 x, HI_S8 yLut[ISP_AUTO_ISO_STENGTH_NUM])
{
	int n;

	if (x <= g_UvnrLutIso[0]) return yLut[0];

	for (n=1; n<ISP_AUTO_ISO_STENGTH_NUM; n++) {
		if (x < g_UvnrLutIso[n]) {
			return yLut[n-1] + (yLut[n] - yLut[n-1]) * (x - g_UvnrLutIso[n-1]) / (g_UvnrLutIso[n] - g_UvnrLutIso[n-1]);
		}
	}

	return yLut[ISP_AUTO_ISO_STENGTH_NUM-1];

}


HI_U8 CrCbCoring_fw(HI_S32 iso, ISP_UVNR_ATTR *pstUVNRCfg)
{
	return HiISP_UNR_Linear_Interp(iso, pstUVNRCfg->stCmosUvnr.Coring_lutLimit);
}


HI_S32 ISP_MedFilterFw(ISP_UVNR_ATTR *pstUVNRCfg, ISP_UVNR_REG_CFG_S * isp_uvnr_reg)
{
	HI_BOOL EnFilterH,EnFilterV;
		
	EnFilterH = pstUVNRCfg->bEnFilterH;
	EnFilterV = pstUVNRCfg->bEnFilterV;

	if (pstUVNRCfg->enPixelFmt == UVNR_PIXEL_OUT_FORMAT_422)
	{
		pstUVNRCfg->u32DsTimesV=1;
	} 
	else
	{
		pstUVNRCfg->u32DsTimesV=2;
	}
	

	//choose filter mode or discard pixel mode. Different coefficients will be chosed in different mode
	if (HI_FALSE == isp_uvnr_reg->bUvnrSel)//MedFilter Select
	{
		if (EnFilterH == 1)
		{
			isp_uvnr_reg->s32HCoef0 = Coeff_Filter_8tap_H[0][0];
			isp_uvnr_reg->s32HCoef1 = Coeff_Filter_8tap_H[0][1];
			isp_uvnr_reg->s32HCoef2 = Coeff_Filter_8tap_H[0][2];
			isp_uvnr_reg->s32HCoef3 = Coeff_Filter_8tap_H[0][3];
			isp_uvnr_reg->s32HCoef4 = Coeff_Filter_8tap_H[0][4];
			isp_uvnr_reg->s32HCoef5 = Coeff_Filter_8tap_H[0][5];
			isp_uvnr_reg->s32HCoef6 = Coeff_Filter_8tap_H[0][6];
			isp_uvnr_reg->s32HCoef7 = Coeff_Filter_8tap_H[0][7];
		}
		else
		{
			isp_uvnr_reg->s32HCoef0 = Coeff_Discard_8pixel_H[0];
			isp_uvnr_reg->s32HCoef1 = Coeff_Discard_8pixel_H[1];
			isp_uvnr_reg->s32HCoef2 = Coeff_Discard_8pixel_H[2];
			isp_uvnr_reg->s32HCoef3 = Coeff_Discard_8pixel_H[3];
			isp_uvnr_reg->s32HCoef4 = Coeff_Discard_8pixel_H[4];
			isp_uvnr_reg->s32HCoef5 = Coeff_Discard_8pixel_H[5];
			isp_uvnr_reg->s32HCoef6 = Coeff_Discard_8pixel_H[6];
			isp_uvnr_reg->s32HCoef7 = Coeff_Discard_8pixel_H[7];
		}
		
		if (EnFilterV == 1)
		{
			isp_uvnr_reg->s32VCoef0 = Coeff_Filer_4tap_V[0][0];
			isp_uvnr_reg->s32VCoef1 = Coeff_Filer_4tap_V[0][1];
			isp_uvnr_reg->s32VCoef2 = Coeff_Filer_4tap_V[0][2];
			isp_uvnr_reg->s32VCoef3 = Coeff_Filer_4tap_V[0][3];
		}
		else
		{
			isp_uvnr_reg->s32VCoef0 = Coeff_Discard_4tap_V[0];
			isp_uvnr_reg->s32VCoef1 = Coeff_Discard_4tap_V[1];
			isp_uvnr_reg->s32VCoef2 = Coeff_Discard_4tap_V[2];
			isp_uvnr_reg->s32VCoef3 = Coeff_Discard_4tap_V[3];
		}
	}

	return HI_SUCCESS;
}


HI_S32 ISP_UvnrFw(HI_U32 u32iso, ISP_UVNR_ATTR *pstUVNRCfg, ISP_UVNR_REG_CFG_S * isp_uvnr_reg)
{
	HI_S32 iso;
	// kernel 7x13
	const HI_S32 xradius = 6;		// const, HW area 
	//const HI_S32 yradius = 3;		// const, HW area 

	// sigma
	HI_FLOAT sigmaDF;	// get this value from calibration
	HI_FLOAT sigmaSP;	// set this value by FW or user prefer
	HI_S32 n;
	HI_FLOAT rt;
	HI_U8 diffThres [16];         // u8
    HI_U8 ratioLimit[16];         // u7
	//------------------------------------------------------
	// iso -> sigma 
	//------------------------------------------------------
	HI_FLOAT lutSigma[ISP_AUTO_ISO_STENGTH_NUM] = {0};

	//------------------------------------------------------
	// NR strength
	//		   0.0	 -> disable 
	//	 (0.0, 1.0]  -> weak 
	//	 (1.0, 2.0]  -> normal 
	//	 (2.0, 10.0) -> strong 
	//------------------------------------------------------
	HI_FLOAT nrStrength = 2.0;
	//------------------------------------------------------
	// Filter strength 
	//------------------------------------------------------

	//user define
	nrStrength = UVNR_nrStrength;
	sigmaSP = UVNR_sigmaSP;

	lutSigma[0] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[0]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[1] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[1]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[2] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[2]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[3] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[3]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[4] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[4]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[5] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[5]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[6] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[6]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[7] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[7]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[8] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[8]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[9] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[9]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[10] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[10]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[11] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[11]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[12] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[12]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[13] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[13]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[14] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[14]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;
	lutSigma[15] = (HI_FLOAT)pstUVNRCfg->stCmosUvnr.UVNR_lutSigma[15]/(HI_FLOAT)HI_ISP_UVNR_SIGMA_PRECISION;	

	//------------------------------------------------------
	// pre-check
	//------------------------------------------------------
	iso = (HI_S32)u32iso;
	sigmaDF = ISP_Uvnr_GetValueFromLut (iso, (const HI_S32 *)g_UvnrLutIso, lutSigma, ISP_AUTO_ISO_STENGTH_NUM);

	//BlendRatio
	isp_uvnr_reg->s32Blendratio = HiISP_UNR_Linear_Interp(iso, pstUVNRCfg->stCmosUvnr.UVNR_blendRatio);
	
	//new add

	if (pstUVNRCfg->enPixelFmt == UVNR_PIXEL_OUT_FORMAT_422)
	{
		isp_uvnr_reg->bVcusMode=UVNR_4UP_SCALE;
	} 
	else
	{
		isp_uvnr_reg->bVcusMode=UVNR_2UP_SCALE;
	}

	if (HI_TRUE == isp_uvnr_reg->bUvnrSel) //UVNR
	{
		if (pstUVNRCfg->bEnFilterH == 1)
		{
			isp_uvnr_reg->s32HCoef0 = Coeff_Filter_8tap_H[1][0];
			isp_uvnr_reg->s32HCoef1 = Coeff_Filter_8tap_H[1][1];
			isp_uvnr_reg->s32HCoef2 = Coeff_Filter_8tap_H[1][2];
			isp_uvnr_reg->s32HCoef3 = Coeff_Filter_8tap_H[1][3];
			isp_uvnr_reg->s32HCoef4 = Coeff_Filter_8tap_H[1][4];
			isp_uvnr_reg->s32HCoef5 = Coeff_Filter_8tap_H[1][5];
			isp_uvnr_reg->s32HCoef6 = Coeff_Filter_8tap_H[1][6];
			isp_uvnr_reg->s32HCoef7 = Coeff_Filter_8tap_H[1][7];
		}
		else
		{
			isp_uvnr_reg->s32HCoef0 = Coeff_Discard_8pixel_H[0];
			isp_uvnr_reg->s32HCoef1 = Coeff_Discard_8pixel_H[1];
			isp_uvnr_reg->s32HCoef2 = Coeff_Discard_8pixel_H[2];
			isp_uvnr_reg->s32HCoef3 = Coeff_Discard_8pixel_H[3];
			isp_uvnr_reg->s32HCoef4 = Coeff_Discard_8pixel_H[4];
			isp_uvnr_reg->s32HCoef5 = Coeff_Discard_8pixel_H[5];
			isp_uvnr_reg->s32HCoef6 = Coeff_Discard_8pixel_H[6];
			isp_uvnr_reg->s32HCoef7 = Coeff_Discard_8pixel_H[7];
		}

		if (pstUVNRCfg->bEnFilterV==1)
		{
			isp_uvnr_reg->s32VCoef0 = Coeff_Filer_4tap_V[1][0];
			isp_uvnr_reg->s32VCoef1 = Coeff_Filer_4tap_V[1][1];
			isp_uvnr_reg->s32VCoef2 = Coeff_Filer_4tap_V[1][2];
			isp_uvnr_reg->s32VCoef3 = Coeff_Filer_4tap_V[1][3];
		} 
		else
		{
			isp_uvnr_reg->s32VCoef0 = Coeff_Discard_4tap_V[0];
			isp_uvnr_reg->s32VCoef1 = Coeff_Discard_4tap_V[1];
			isp_uvnr_reg->s32VCoef2 = Coeff_Discard_4tap_V[2];
			isp_uvnr_reg->s32VCoef3 = Coeff_Discard_4tap_V[3];
		}
	}	

	//------------------------------------------------------
	// integer version bilateral coefficient 
	//------------------------------------------------------
	isp_uvnr_reg->s32Diffshift = (sigmaDF <  4.0) ? 3 
					: (sigmaDF <  8.0) ? 2 
					: (sigmaDF < 16.0) ? 1 
					:					 0
					;

	for (n=0; n<= xradius; n++) {
		rt = _gauss(n, sigmaSP) / _gauss(0, sigmaSP);

		diffThres[n]	= (int) ( nrStrength * sigmaDF * 2.0 * rt + 0.5);
		ratioLimit[n] = (int) ( 127 * rt + 0.5);
	}

	isp_uvnr_reg->s32Diffthres1 = diffThres[1];
	isp_uvnr_reg->s32Diffthres2 = diffThres[2];
	isp_uvnr_reg->s32Diffthres3 = diffThres[3];
	isp_uvnr_reg->s32Diffthres4 = diffThres[4];
	isp_uvnr_reg->s32Diffthres5 = diffThres[5];
	isp_uvnr_reg->s32Diffthres6 = diffThres[6];

	isp_uvnr_reg->s32Ratiolimit0 = ratioLimit[0];
	isp_uvnr_reg->s32Ratiolimit1 = ratioLimit[1];
	isp_uvnr_reg->s32Ratiolimit2 = ratioLimit[2];
	isp_uvnr_reg->s32ratiolimit3 = ratioLimit[3];
	isp_uvnr_reg->s32Ratiolimit4 = ratioLimit[4];
	isp_uvnr_reg->s32Ratiolimit5 = ratioLimit[5];
	isp_uvnr_reg->s32Ratiolimit6 = ratioLimit[6];

	return HI_SUCCESS;
}


HI_S32 ISP_UvnrRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	ISP_CTX_S     *pstIspCtx  = HI_NULL;
	ISP_UVNR_ATTR *pstUvnrCtx = HI_NULL;
	ISP_REG_CFG_S *pstReg     = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);
    UVNR_GET_CTX(IspDev, pstUvnrCtx);
	
    /* calculate every two interrupts */	
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }
	
    UvnrReadExtregs(IspDev);

    pstReg = (ISP_REG_CFG_S *)pRegCfg;
	
	if (pstUvnrCtx->bUvnrMedSel == HI_FALSE)		//Med Filter
	{
		if (pstIspCtx->stLinkage.u32Iso > pstUvnrCtx->u32Iso_Med2UVNR_Thd)	//0: med; 1: uvnr
		{
			pstUvnrCtx->bUvnrMedSel = HI_TRUE;
		}	
	}
	else //UVNR
	{
		if (pstIspCtx->stLinkage.u32Iso < pstUvnrCtx->u32Iso_UVNR2Med_Thd)	//0: med; 1: uvnr
		{
			pstUvnrCtx->bUvnrMedSel = HI_FALSE;
		}
	}

	pstReg->stUVNrRegCfg.bUvnrSel = (pstUvnrCtx->bUvnrMedSel & pstUvnrCtx->bEnUVNR);
	pstReg->stUVNrRegCfg.bMidfEn  = ((!(pstUvnrCtx->bUvnrMedSel & pstUvnrCtx->bEnUVNR)) & pstUvnrCtx->bEnMedianFilter);
	pstReg->stUVNrRegCfg.bNrEn    = (pstUvnrCtx->bUvnrMedSel & pstUvnrCtx->bEnUVNR);

	ISP_UvnrFw(pstIspCtx->stLinkage.u32Iso, pstUvnrCtx, &pstReg->stUVNrRegCfg);

	ISP_MedFilterFw(pstUvnrCtx, &pstReg->stUVNrRegCfg);

	pstReg->stUVNrRegCfg.u8CoringLimit = CrCbCoring_fw(pstIspCtx->stLinkage.u32Iso, pstUvnrCtx);

	pstReg->stUVNrRegCfg.bUv2cEn   = pstUvnrCtx->bEnUV2C;
	pstReg->stUVNrRegCfg.bUv2cMode = pstUvnrCtx->u8Uv2cMode;
	pstReg->stUVNrRegCfg.bHcdsEn   = HI_TRUE;
	/*Using Medfilter channel and pixel format was 420*/
	pstReg->stUVNrRegCfg.bVcdsEn   = \
		((pstReg->stUVNrRegCfg.bUvnrSel == 0) && (pstUvnrCtx->u32DsTimesV==1)) ? HI_FALSE : HI_TRUE;
	pstReg->stUVNrRegCfg.bVcdsMode = (pstReg->stUVNrRegCfg.bUvnrSel) ? 1 : 0;
	pstReg->stUVNrRegCfg.bHcdsMode = (pstReg->stUVNrRegCfg.bUvnrSel) ? 1 : 0;

	pstReg->stUVNrRegCfg.s32WidthIn  = (pstUvnrCtx->u32Width - 1);
	pstReg->stUVNrRegCfg.s32WidthOut = pstReg->stUVNrRegCfg.bUvnrSel ? (((pstUvnrCtx->u32Width) >>2)-1) : (((pstUvnrCtx->u32Width) >>1)-1);
	//printf("width_out %d bUvnrMedSel %d\n", pstReg->stUVNrRegCfg.width_out, pstUvnrCtx->bUvnrMedSel);
	
	pstReg->stUVNrRegCfg.s32UvnrHeight = ((pstUvnrCtx->u32Height) >>2)-1;
	pstReg->stUVNrRegCfg.s32UvnrWidth = ((pstUvnrCtx->u32Width) >>1)-1;

	pstReg->stUVNrRegCfg.s32MfHeight = (pstUvnrCtx->u32DsTimesV==1) ? (pstUvnrCtx->u32Height -1) : ((pstUvnrCtx->u32Height>>1) -1);
	pstReg->stUVNrRegCfg.s32MfWidth = pstUvnrCtx->u32Width -1;

	pstReg->stUVNrRegCfg.s32SysHeightIn = pstUvnrCtx->u32Height -1;
	pstReg->stUVNrRegCfg.s32SysWidthIn  = pstUvnrCtx->u32Width -1;

	pstReg->unKey.bit1UvnrCfg = 1;
	
    return HI_SUCCESS;
}

HI_S32 ISP_UvnrCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_UvnrInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            UvnrProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_UvnrExit(ISP_DEV IspDev)
{
	hi_ext_system_uvnr_manu_mode_write(HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterUvnr(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_UVNR;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_UvnrInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_UvnrRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_UvnrCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_UvnrExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


