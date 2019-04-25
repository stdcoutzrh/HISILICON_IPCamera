/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_noise_reduction.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"
#include "hi_math.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


#define RGBIR_EN_DEFAULT                (0)
#define RGBIR_IR_OUT_EN_DEFAULT         (0)
#define RGBIR_IR_FILTER_EN_DEFAULT      (1)
#define RGBIR_BLC_OFF_EN_DEFAULT        (0)
#define RGBIR_REMOVEL_EN_DEFAULT        (1)
#define RGBIR_AUTO_GAIN_EN__DEFAULT     (1)
#define RGBIR_MANU_GAIN_EN_DEFAULT      (0)
#define RGBIR_IR_POSITION_DEFAULT       (0)
#define RGBIR_MANU_GAIN_DEFAULT         (0x100)
#define RGBIR_G_MIN_DEFAULT             (5)
#define RGBIR_RB_MAX_DEFAULT            (1000)
#define RGBIR_BLACK_LEVEL_DEFAULT       (0x40)
#define RGBIR_FILTER_COEF02_DEFAULT     (1)
#define RGBIR_FILTER_COEF01_DEFAULT     (1)
#define RGBIR_FILTER_COEF00_DEFAULT     (1)
#define RGBIR_FILTER_COEF12_DEFAULT     (1)
#define RGBIR_FILTER_COEF11_DEFAULT     (8)
#define RGBIR_FILTER_COEF10_DEFAULT     (1)
#define RGBIR_FILTER_COEF22_DEFAULT     (1)
#define RGBIR_FILTER_COEF21_DEFAULT     (1)
#define RGBIR_FILTER_COEF20_DEFAULT     (1)
#define RGBIR_THRE2_DEFAULT             (0x5)
#define RGBIR_THRE1_DEFAULT             (0xa)
#define RGBIR_EXP_VALULE_DEFAULT        (4028)
#define RGBIR_SCALE_B_B_DEFAULT         (264)
#define RGBIR_SCALE_B_G_DEFAULT         (-1)
#define RGBIR_SCALE_B_R_DEFAULT         (21)
#define RGBIR_SCALE_G_B_DEFAULT         (8)
#define RGBIR_SCALE_G_G_DEFAULT         (258)
#define RGBIR_SCALE_G_R_DEFAULT         (9)
#define RGBIR_SCALE_R_B_DEFAULT         (-20)
#define RGBIR_SCALE_R_G_DEFAULT         (26)
#define RGBIR_SCALE_R_R_DEFAULT         (238)
#define RGBIR_SCALE_IR_B_DEFAULT        (-261)
#define RGBIR_SCALE_IR_G_DEFAULT        (-255)
#define RGBIR_SCALE_IR_R_DEFAULT        (-229)
#define RGBIR_SCALE_C_B_DEFAULT         (19)
#define RGBIR_SCALE_C_G_DEFAULT         (12)
#define RGBIR_SCALE_C_R_DEFAULT         (15)

#ifndef CLIP3
#define CLIP3(x,min,max)    ((x)< (min) ? (min) : ((x)>(max)?(max):(x)))
#endif

typedef struct hiISP_RGBIR_S
{
	ISP_CMOS_RGBIR_S  stCmosRgbir;
} ISP_RGBIR_S;

ISP_RGBIR_S g_astRgbirCtx[ISP_MAX_DEV_NUM];
#define RGBIR_GET_CTX(dev, pstCtx)   pstCtx = &g_astRgbirCtx[dev]

static HI_VOID RgbirExtRegsDefault(HI_VOID)
{
    hi_ext_system_isp_rgbir_removel_en_write(RGBIR_REMOVEL_EN_DEFAULT);
    hi_ext_system_isp_rgbir_scale_b_b_write(RGBIR_SCALE_B_B_DEFAULT);
    hi_ext_system_isp_rgbir_scale_b_g_write(RGBIR_SCALE_B_G_DEFAULT);
    hi_ext_system_isp_rgbir_scale_b_r_write(RGBIR_SCALE_B_R_DEFAULT);
    hi_ext_system_isp_rgbir_scale_g_b_write(RGBIR_SCALE_G_B_DEFAULT);
    hi_ext_system_isp_rgbir_scale_g_g_write(RGBIR_SCALE_G_G_DEFAULT);
    hi_ext_system_isp_rgbir_scale_g_r_write(RGBIR_SCALE_G_R_DEFAULT);
    hi_ext_system_isp_rgbir_scale_r_b_write(RGBIR_SCALE_R_B_DEFAULT);
    hi_ext_system_isp_rgbir_scale_r_g_write(RGBIR_SCALE_R_G_DEFAULT);
    hi_ext_system_isp_rgbir_scale_r_r_write(RGBIR_SCALE_R_R_DEFAULT);
    hi_ext_system_isp_rgbir_scale_ir_b_write(RGBIR_SCALE_IR_B_DEFAULT);
    hi_ext_system_isp_rgbir_scale_ir_g_write(RGBIR_SCALE_IR_G_DEFAULT);
    hi_ext_system_isp_rgbir_scale_ir_r_write(RGBIR_SCALE_IR_R_DEFAULT);
    hi_ext_system_isp_rgbir_scale_c_b_write(RGBIR_SCALE_C_B_DEFAULT);
    hi_ext_system_isp_rgbir_scale_c_g_write(RGBIR_SCALE_C_G_DEFAULT);
    hi_ext_system_isp_rgbir_scale_c_r_write(RGBIR_SCALE_C_R_DEFAULT);
    return;
}

static HI_VOID RgbirRegsDefault(HI_VOID) 
{   
    hi_isp_rgbir_en_write(RGBIR_EN_DEFAULT);
    hi_isp_rgbir_ir_out_en_write(RGBIR_IR_OUT_EN_DEFAULT);
    hi_isp_rgbir_ir_filter_en_write(RGBIR_IR_FILTER_EN_DEFAULT);
    hi_isp_rgbir_blc_off_en_write(RGBIR_BLC_OFF_EN_DEFAULT);
    hi_isp_rgbir_removel_en_write(RGBIR_REMOVEL_EN_DEFAULT);
    hi_isp_rgbir_auto_gain_en_write(RGBIR_AUTO_GAIN_EN__DEFAULT);
    hi_isp_rgbir_manul_gain_en_write(RGBIR_MANU_GAIN_EN_DEFAULT);
    hi_isp_rgbir_ir_position_write(RGBIR_IR_POSITION_DEFAULT);
    hi_isp_rgbir_manul_gain_write(RGBIR_MANU_GAIN_DEFAULT);
    hi_isp_rgbir_g_min_write(RGBIR_G_MIN_DEFAULT);
    hi_isp_rgbir_rb_max_write(RGBIR_RB_MAX_DEFAULT);
    hi_isp_rgbir_black_level_write(RGBIR_BLACK_LEVEL_DEFAULT);
    hi_isp_rgbir_filter_coef02_write(RGBIR_FILTER_COEF02_DEFAULT);
    hi_isp_rgbir_filter_coef01_write(RGBIR_FILTER_COEF01_DEFAULT);
    hi_isp_rgbir_filter_coef00_write(RGBIR_FILTER_COEF00_DEFAULT);
    hi_isp_rgbir_filter_coef12_write(RGBIR_FILTER_COEF12_DEFAULT);
    hi_isp_rgbir_filter_coef11_write(RGBIR_FILTER_COEF11_DEFAULT);
    hi_isp_rgbir_filter_coef10_write(RGBIR_FILTER_COEF10_DEFAULT);
    hi_isp_rgbir_filter_coef22_write(RGBIR_FILTER_COEF22_DEFAULT);
    hi_isp_rgbir_filter_coef21_write(RGBIR_FILTER_COEF21_DEFAULT);
    hi_isp_rgbir_filter_coef20_write(RGBIR_FILTER_COEF20_DEFAULT);
    hi_isp_rgbir_thre2_write(RGBIR_THRE2_DEFAULT);
    hi_isp_rgbir_thre1_write(RGBIR_THRE1_DEFAULT);
    hi_isp_rgbir_exp_value_write(RGBIR_EXP_VALULE_DEFAULT);
    hi_isp_rgbir_scale_b_b_write(RGBIR_SCALE_B_B_DEFAULT);
    hi_isp_rgbir_scale_b_g_write(RGBIR_SCALE_B_G_DEFAULT);
    hi_isp_rgbir_scale_b_r_write(RGBIR_SCALE_B_R_DEFAULT);
    hi_isp_rgbir_scale_g_b_write(RGBIR_SCALE_G_B_DEFAULT);
    hi_isp_rgbir_scale_g_g_write(RGBIR_SCALE_G_G_DEFAULT);
    hi_isp_rgbir_scale_g_r_write(RGBIR_SCALE_G_R_DEFAULT);
    hi_isp_rgbir_scale_r_b_write(RGBIR_SCALE_R_B_DEFAULT);
    hi_isp_rgbir_scale_r_g_write(RGBIR_SCALE_R_G_DEFAULT);
    hi_isp_rgbir_scale_r_r_write(RGBIR_SCALE_R_R_DEFAULT);
    hi_isp_rgbir_scale_ir_b_write(RGBIR_SCALE_IR_B_DEFAULT);
    hi_isp_rgbir_scale_ir_g_write(RGBIR_SCALE_IR_G_DEFAULT);
    hi_isp_rgbir_scale_ir_r_write(RGBIR_SCALE_IR_R_DEFAULT);
    hi_isp_rgbir_scale_c_b_write(RGBIR_SCALE_C_B_DEFAULT);
    hi_isp_rgbir_scale_c_g_write(RGBIR_SCALE_C_G_DEFAULT);
    hi_isp_rgbir_scale_c_r_write(RGBIR_SCALE_C_R_DEFAULT);
    
    return;
}

static HI_VOID RgbirExtRegsInitialize(ISP_DEV IspDev)
{
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    if (pstSnsDft->stRgbir.bValid)
    {
        hi_ext_system_isp_rgbir_removel_en_write(pstSnsDft->stRgbir.stRgbirCtrl.bRemovelEn);
        hi_ext_system_isp_rgbir_scale_b_b_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[0],-512,511));
        hi_ext_system_isp_rgbir_scale_b_g_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[1],-512,511));
        hi_ext_system_isp_rgbir_scale_b_r_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[2],-512,511));
        hi_ext_system_isp_rgbir_scale_g_b_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[3],-512,511));
        hi_ext_system_isp_rgbir_scale_g_g_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[4],-512,511));
        hi_ext_system_isp_rgbir_scale_g_r_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[5],-512,511));
        hi_ext_system_isp_rgbir_scale_r_b_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[6],-512,511));
        hi_ext_system_isp_rgbir_scale_r_g_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[7],-512,511));
        hi_ext_system_isp_rgbir_scale_r_r_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[8],-512,511));
        hi_ext_system_isp_rgbir_scale_ir_b_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[9],-512,511));
        hi_ext_system_isp_rgbir_scale_ir_g_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[10],-512,511));
        hi_ext_system_isp_rgbir_scale_ir_r_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[11],-512,511));
        hi_ext_system_isp_rgbir_scale_c_b_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[12],-512,511));
        hi_ext_system_isp_rgbir_scale_c_g_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[13],-512,511));
        hi_ext_system_isp_rgbir_scale_c_r_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[14],-512,511));
    }

    return;
}

static HI_VOID RgbirRegsInitialize(ISP_DEV IspDev)
{
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    if (pstSnsDft->stRgbir.bValid)
    {
        hi_isp_rgbir_en_write(pstSnsDft->stRgbir.stRgbirAttr.bEnable); 
        hi_isp_rgbir_ir_out_en_write(pstSnsDft->stRgbir.stRgbirCtrl.bIrOutEn);
        hi_isp_rgbir_ir_filter_en_write(pstSnsDft->stRgbir.stRgbirCtrl.bIrFilterEn);
        hi_isp_rgbir_removel_en_write(1);
        if (OP_CMOS_TYPE_AUTO == pstSnsDft->stRgbir.stRgbirCtrl.enCompType)
        {
            hi_isp_rgbir_auto_gain_en_write(1);
            hi_isp_rgbir_manul_gain_en_write(0);
        }
        else if (OP_CMOS_TYPE_MANUAL == pstSnsDft->stRgbir.stRgbirCtrl.enCompType)
        {
            hi_isp_rgbir_auto_gain_en_write(0);
            hi_isp_rgbir_manul_gain_en_write(1);
        }
        hi_isp_rgbir_ir_position_write(pstSnsDft->stRgbir.stRgbirAttr.enIrPosType);
        hi_isp_rgbir_manul_gain_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.u16ManuGain,0x100,0x3ff));
        hi_isp_rgbir_exp_value_write(pstSnsDft->stRgbir.stRgbirAttr.u16OverExpThresh);
        if (pstSnsDft->stRgbir.stRgbirCtrl.bRemovelEn)
        {
            hi_isp_rgbir_scale_b_b_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[0],-512,511));
            hi_isp_rgbir_scale_b_g_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[1],-512,511));
            hi_isp_rgbir_scale_b_r_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[2],-512,511));
            hi_isp_rgbir_scale_g_b_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[3],-512,511));
            hi_isp_rgbir_scale_g_g_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[4],-512,511));
            hi_isp_rgbir_scale_g_r_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[5],-512,511));
            hi_isp_rgbir_scale_r_b_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[6],-512,511));
            hi_isp_rgbir_scale_r_g_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[7],-512,511));
            hi_isp_rgbir_scale_r_r_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[8],-512,511));
            hi_isp_rgbir_scale_ir_b_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[9],-512,511));
            hi_isp_rgbir_scale_ir_g_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[10],-512,511));
            hi_isp_rgbir_scale_ir_r_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[11],-512,511));
            hi_isp_rgbir_scale_c_b_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[12],-512,511));
            hi_isp_rgbir_scale_c_g_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[13],-512,511));
            hi_isp_rgbir_scale_c_r_write(CLIP3(pstSnsDft->stRgbir.stRgbirCtrl.as16ScaleCoef[14],-512,511));
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

        /* RGBIR enable GE must be disable */
        if (pstSnsDft->stRgbir.stRgbirAttr.bEnable)
        {
            hi_ext_system_ge_enable_write(HI_FALSE);
        }
    }

    return;
}

static HI_S32 RgbirReadExtregs(ISP_DEV IspDev)
{	
    ISP_RGBIR_S *pstRgbir = HI_NULL;
    RGBIR_GET_CTX(IspDev,pstRgbir);
    
    pstRgbir->stCmosRgbir.stRgbirAttr.bEnable = hi_isp_rgbir_en_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.bIrOutEn = hi_isp_rgbir_ir_out_en_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.bIrFilterEn = hi_isp_rgbir_ir_filter_en_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.bRemovelEn = hi_ext_system_isp_rgbir_removel_en_read();
    if ((1 == hi_isp_rgbir_auto_gain_en_read()) && (0 == hi_isp_rgbir_manul_gain_en_read()))
    {
        pstRgbir->stCmosRgbir.stRgbirCtrl.enCompType = OP_CMOS_TYPE_AUTO;
    }
    else
    {
        pstRgbir->stCmosRgbir.stRgbirCtrl.enCompType = OP_CMOS_TYPE_MANUAL;
    }
    pstRgbir->stCmosRgbir.stRgbirAttr.enIrPosType = hi_isp_rgbir_ir_position_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.u16ManuGain = hi_isp_rgbir_manul_gain_read();
    pstRgbir->stCmosRgbir.stRgbirAttr.u16OverExpThresh = hi_isp_rgbir_exp_value_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[0] = hi_ext_system_isp_rgbir_scale_b_b_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[1] = hi_ext_system_isp_rgbir_scale_b_g_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[2] = hi_ext_system_isp_rgbir_scale_b_r_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[3] = hi_ext_system_isp_rgbir_scale_g_b_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[4] = hi_ext_system_isp_rgbir_scale_g_g_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[5] = hi_ext_system_isp_rgbir_scale_g_r_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[6] = hi_ext_system_isp_rgbir_scale_r_b_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[7] = hi_ext_system_isp_rgbir_scale_r_g_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[8] = hi_ext_system_isp_rgbir_scale_r_r_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[9] = hi_ext_system_isp_rgbir_scale_ir_b_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[10] = hi_ext_system_isp_rgbir_scale_ir_g_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[11] = hi_ext_system_isp_rgbir_scale_ir_r_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[12] = hi_ext_system_isp_rgbir_scale_c_b_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[13] = hi_ext_system_isp_rgbir_scale_c_g_read();
    pstRgbir->stCmosRgbir.stRgbirCtrl.as16ScaleCoef[14] = hi_ext_system_isp_rgbir_scale_c_r_read();

	return HI_SUCCESS;
}

HI_S32 RgbirProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_RGBIR_S *pstRgbir = HI_NULL;
    HI_U16 u16RgbirGain;
    HI_BOOL bAutoCompEn;

    RGBIR_GET_CTX(IspDev,pstRgbir);

    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    if (OP_CMOS_TYPE_AUTO == pstRgbir->stCmosRgbir.stRgbirCtrl.enCompType)
    {
        u16RgbirGain = hi_isp_rgbir_gain_use_read();
        bAutoCompEn = 1;
    }
    else
    {
        u16RgbirGain = pstRgbir->stCmosRgbir.stRgbirCtrl.u16ManuGain;
        bAutoCompEn = 0;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----RGBIR INFO----------------------------------------------------------------\n");
           
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8s\t%8s\t%8s\n",
        "En","AutoEn","AutoGain");
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d\t%8d\t%8d\n",
        pstRgbir->stCmosRgbir.stRgbirAttr.bEnable,bAutoCompEn,u16RgbirGain);

    pstProc->u32WriteLen += 1;
   
    return HI_SUCCESS;
}

static HI_S32 ISP_RgbirInit(ISP_DEV IspDev)
{
    RgbirRegsDefault();
    RgbirExtRegsDefault();
    RgbirExtRegsInitialize(IspDev);
    RgbirRegsInitialize(IspDev);

    return HI_SUCCESS;
}

HI_S32 ISP_RgbirRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	//ISP_REG_CFG_S *pstReg;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    RgbirReadExtregs(IspDev);
     
    return HI_SUCCESS;
}

HI_S32 ISP_RgbirCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_RgbirInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            RgbirProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_RgbirExit(ISP_DEV IspDev)
{
	hi_isp_rgbir_en_write(HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterRgbir(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_RGBIR;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_RgbirInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_RgbirRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_RgbirCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_RgbirExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

