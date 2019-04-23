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

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif


#define _clip(l,h,v)	(MAX(l, MIN(h, v)))

#define  HI_FW_NOISE_BIT_DEPTH   (12)
#define  HI_FW_ISO_TABLE_DIVISOR (1000)
#define  HI_FW_ISO_MAX           (80000)

static HI_U32 g_au32NRIsoLut[ISP_AUTO_ISO_STENGTH_NUM] = {100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};

typedef struct hiISP_NOISE_REDUCTION_S
{
    ISP_NR_MANUAL_ATTR_S    stManual;
	ISP_NR_AUTO_ATTR_S      stAuto;
	ISP_CMOS_NOISE_TABLE_S  stCmosNRTable;
} ISP_NOISE_REDUCTION_S;

ISP_NOISE_REDUCTION_S g_astNrCtx[ISP_MAX_DEV_NUM];
#define NR_GET_CTX(dev, pstCtx)   pstCtx = &g_astNrCtx[dev] 

static HI_VOID NrExtRegsDefault(HI_VOID)
{
	hi_ext_system_nr_manu_mode_write(HI_FALSE);
    return;
}

static HI_VOID NrRegsDefault(HI_VOID) 
{    
	hi_isp_2dnr_cfg_enable_write(HI_TRUE);    
    return;
}

static HI_VOID NrExtRegsInitialize(ISP_DEV IspDev)
{
	/* initial register of noise reduction */
	//modify by qlp
	HI_U32 i, j;
	ISP_NOISE_REDUCTION_S *pstNr = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
		
    NR_GET_CTX(IspDev, pstNr);
	
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

    /*read sensor paras from cmos.c*/
	memcpy(&pstNr->stCmosNRTable, &pstSnsDft->stNoiseTbl, sizeof(ISP_CMOS_NOISE_TABLE_S));

    j = 0;
	/* initial ext auto register of noise reduction */             
    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{	    
        j=i;      
        hi_ext_system_nr_varstrength_write(i, pstNr->stCmosNRTable.stIsoParaTable[j].u8varStrength);        
        hi_ext_system_nr_fixstrength_write(i, pstNr->stCmosNRTable.stIsoParaTable[j].u8fixStrength);
		hi_ext_system_nr_threshold_write(i, (pstNr->stCmosNRTable.stIsoParaTable[j].u16Threshold));        
        hi_ext_system_nr_lowfreqslope_write(i, (pstNr->stCmosNRTable.stIsoParaTable[j].u8LowFreqSlope)); 	    	
	}

	hi_ext_system_nr_manual_threshold_write(HI_EXT_SYSTEM_NR_MANUAL_THRESHOLD_WRITE_DEFAULT);
	hi_ext_system_nr_manual_lowfreqslope_write(HI_EXT_SYSTEM_NR_MANUAL_LOWFREQSLOPE_WRITE_DEFAULT);
	hi_ext_system_nr_manual_fixstrength_write(HI_EXT_SYSTEM_NR_MANUAL_FIXSTRENGTH_WRITE_DEFAULT);
	hi_ext_system_nr_manual_varstrength_write(HI_EXT_SYSTEM_NR_MANUAL_VAR_STRENGTH_WRITE_DEFAULT);
	
    return;
}

static HI_S32 NrReadExtregs(ISP_DEV IspDev)
{
    HI_U32 i;
	ISP_NR_ATTR_S stNR;
	static ISP_NR_ATTR_S PrestNR = {0};
	HI_U8 update = 0;
	
    stNR.bEnable = hi_isp_2dnr_cfg_enable_read();
	stNR.enOpType = hi_ext_system_nr_manu_mode_read();
    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{	    
   
		stNR.stAuto.au16Threshold[i] = hi_ext_system_nr_threshold_read(i);
		stNR.stAuto.au8FixStrength[i] = hi_ext_system_nr_fixstrength_read(i);
		stNR.stAuto.au8LowFreqSlope[i] = hi_ext_system_nr_lowfreqslope_read(i);
		stNR.stAuto.au8VarStrength[i] = hi_ext_system_nr_varstrength_read(i);
	}

    stNR.stManual.u16Threshold = hi_ext_system_nr_manual_threshold_read();
	stNR.stManual.u8LowFreqSlope = 	hi_ext_system_nr_manual_lowfreqslope_read();
	stNR.stManual.u8FixStrength = hi_ext_system_nr_manual_fixstrength_read();
    stNR.stManual.u8VarStrength = hi_ext_system_nr_manual_varstrength_read();

    if(PrestNR.bEnable !=  stNR.bEnable)
    {
      update = 1;
	}
	if(PrestNR.enOpType != stNR.enOpType)
	{
		update = 1;
	}
	if(PrestNR.stManual.u16Threshold != stNR.stManual.u16Threshold)
	{
		update = 1;
	}
    if(PrestNR.stManual.u8LowFreqSlope != stNR.stManual.u8LowFreqSlope)
	{
		update = 1;
	}
    if(PrestNR.stManual.u8FixStrength != stNR.stManual.u8FixStrength)
	{
		update = 1;
	}
    if(PrestNR.stManual.u8VarStrength != stNR.stManual.u8VarStrength)
	{
		update = 1;
	}
    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{	      
		if(PrestNR.stAuto.au16Threshold[i] != stNR.stAuto.au16Threshold[i]) 
		{
          update = 1;
		  break;
		}
	   else if(PrestNR.stAuto.au8FixStrength[i] != stNR.stAuto.au8FixStrength[i])
	    {
		   update = 1;
		   break;
		}
	   else if(PrestNR.stAuto.au8LowFreqSlope[i] != stNR.stAuto.au8LowFreqSlope[i])
		{
		   update = 1;
		   break;
		}
	   else if(PrestNR.stAuto.au8VarStrength[i] != stNR.stAuto.au8VarStrength[i])
		{
		   update = 1;
		   break;
		}
	   else
	   	{
		  
	    }
	}

	PrestNR.bEnable =  stNR.bEnable;
	PrestNR.enOpType = stNR.enOpType;
	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{	      
		PrestNR.stAuto.au16Threshold[i] = stNR.stAuto.au16Threshold[i] ;
		PrestNR.stAuto.au8FixStrength[i] = stNR.stAuto.au8FixStrength[i];
		PrestNR.stAuto.au8LowFreqSlope[i] = stNR.stAuto.au8LowFreqSlope[i];
		PrestNR.stAuto.au8VarStrength[i] = stNR.stAuto.au8VarStrength[i];
	}
	 
	PrestNR.stManual.u16Threshold = stNR.stManual.u16Threshold;
	PrestNR.stManual.u8LowFreqSlope = stNR.stManual.u8LowFreqSlope;
	PrestNR.stManual.u8FixStrength = stNR.stManual.u8FixStrength;
	PrestNR.stManual.u8VarStrength = stNR.stManual.u8VarStrength;

	return update;
}

HI_S32 NrProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;

    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }


    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----NR INFO-------------------------------------------------------------------\n");
           
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8s\n",
        "En");
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d\n",
        hi_isp_2dnr_cfg_enable_read());

    pstProc->u32WriteLen += 1;
   
    return HI_SUCCESS;
}

static HI_S32 ISP_NrInit(ISP_DEV IspDev)
{
    NrRegsDefault();
    NrExtRegsDefault();
    NrExtRegsInitialize(IspDev);

    return HI_SUCCESS;
}

// z00319880: interpolate alpha and beta using calibration data
#define  EPS (0.000001f)
#define  COL_ISO      0
#define  COL_ISOLOG   1
#define  COL_ALPHA    2
#define  COL_BETA     3
static HI_VOID DMNR_Alpha_Beta(HI_FLOAT (*pRecord)[4], HI_S32 recordNum, HI_S32 iso, HI_FLOAT *pAlpha, HI_FLOAT *pBeta)
{
	HI_S32 i = 0;
	HI_FLOAT  y_diff = 0, x_diff = 0, iso_log = (HI_FLOAT)log10((HI_FLOAT)iso);

	// record: iso - log(iso) - alpha - beta
	if(iso <= pRecord[0][COL_ISO])
	{
		*pAlpha = pRecord[0][COL_ALPHA];
		*pBeta  = pRecord[0][COL_BETA];
		 return;
	}

	if(iso >= pRecord[recordNum - 1][COL_ISO])
	{
		*pAlpha = pRecord[recordNum - 1][COL_ALPHA];
		*pBeta  = pRecord[recordNum - 1][COL_BETA];
		 return;
	}

	for(i = 0; i < recordNum - 1; i++)
	{
		if(iso >= pRecord[i][COL_ISO] && iso <= pRecord[i + 1][COL_ISO])
		{
			x_diff = pRecord[i + 1][COL_ISOLOG] - pRecord[i][COL_ISOLOG];  // log(iso) diff
			y_diff = pRecord[i + 1][COL_ALPHA]  - pRecord[i][COL_ALPHA];   // alpha diff
		   *pAlpha = pRecord[i][COL_ALPHA] + y_diff * (iso_log - pRecord[i][COL_ISOLOG]) / (x_diff + EPS);

		    x_diff = pRecord[i + 1][COL_ISO]  - pRecord[i][COL_ISO];       // iso diff
		    y_diff = pRecord[i + 1][COL_BETA] - pRecord[i][COL_BETA];      // beta diff
		   *pBeta  = pRecord[i][COL_BETA] + y_diff * (iso - pRecord[i][COL_ISO]) / (x_diff + EPS);

			return;
		}
	}
}

static __inline HI_U16 NrOffsetCalculate(
    const HI_U16 u16Y2,
    const HI_U16 u16Y1,
    const HI_U32 u32X2,
    const HI_U32 u32X1,
    const HI_U32 u32Iso)    
{
    HI_U32 u32Offset;
    
	if(u32X1 == u32X2)
    {
        u32Offset = u16Y2;
    }
	else if(u16Y1 <= u16Y2)
    {
        u32Offset = u16Y1+((HI_U64)ABS(u16Y2 - u16Y1) * ABS(u32Iso - u32X1)+ ABS((u32X2 - u32X1)/2))/ABS((u32X2 - u32X1));
    }
	else if(u16Y1 > u16Y2)
    {
        u32Offset = u16Y1-((HI_U64)ABS(u16Y2 - u16Y1) * ABS(u32Iso - u32X1)+ ABS((u32X2 - u32X1)/2))/ABS((u32X2 - u32X1));
    }
    return (HI_U16)u32Offset;
}
  
static HI_VOID NrRegisterGuider(HI_U32 u32Iso, ISP_DEV IspDev, ISP_NR_REG_CFG_S *pstReg)
{  
	HI_U32 i;
	HI_FLOAT  pAlpha = 0.02f, pBeta = 28.0f;
	HI_DOUBLE v0;
	HI_DOUBLE tmp;
	HI_S32 rate    = 1;

	HI_S32 iso		 = (HI_S32)u32Iso;
	HI_S32 n;
	HI_S32 bitDepth = HI_FW_NOISE_BIT_DEPTH;
	HI_S32 msk = (1 << bitDepth)-1;
	HI_DOUBLE rangeLowCoef, rangeHighCoef;
	ISP_NOISE_REDUCTION_S *pstNr = HI_NULL; 
    
	HI_U32 eIsoLevel = 0;
    HI_U16 u16Y2;
    HI_U16 u16Y1;    
	HI_U32 u32ISO1 = 0;
    HI_U32 u32ISO2 = 0;
		
    NR_GET_CTX(IspDev, pstNr);
 
    if (u32Iso > g_au32NRIsoLut[ISP_AUTO_ISO_STENGTH_NUM-1])
    {
        eIsoLevel = 15;
        u32ISO1 = g_au32NRIsoLut[ISP_AUTO_ISO_STENGTH_NUM-1];
        u32ISO2 = g_au32NRIsoLut[ISP_AUTO_ISO_STENGTH_NUM-1]; 	
    }
    else if (u32Iso <= g_au32NRIsoLut[0])
    {
        eIsoLevel = 0;
        u32ISO1 = 0;
        u32ISO2 = g_au32NRIsoLut[0]; 	
    }	
	else 
    {
        for (i = 1; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
        {
            if (u32Iso <= g_au32NRIsoLut[i])
    		{
    			eIsoLevel = i;
    			u32ISO1 = g_au32NRIsoLut[i-1];
                u32ISO2 = g_au32NRIsoLut[i]; 
    			break;
    		}
        }
	} 

	
	DMNR_Alpha_Beta((HI_FLOAT (*)[4])pstNr->stCmosNRTable.stNrCaliPara.pCalibcoef, pstNr->stCmosNRTable.stNrCaliPara.u8CalicoefRow, iso, &pAlpha, &pBeta);

	/************************************************************/


	// range threshold 
	rate = 0x40;

	rate <<= (bitDepth - 10);


	v0 = 512 *(HI_DOUBLE) pAlpha + pBeta;
	v0 = v0*rate/64;

	pstReg->falseColorEnable = 1; 	  // 1bit
	pstReg->falseColorShift  = 0; 	  // 8bit (unit of 10bit)
	pstReg->falseColorGain   = 0x40;  // 8bit (0x40 == 100% )
	pstReg->falseColorLimit  = 0x40;  // 8bit (unit of 10bit)

	tmp =  (HI_DOUBLE)(pAlpha * 4096);
    
	pstReg->rangeGain = MIN(tmp,0x7FFFFFFF);
	pstReg->rangeLumaLow = 32;
	pstReg->rangeLumaHig = (1 << bitDepth)-1; 

	// blend ratio
	pstReg->blendLowLimit = 0x100;
	pstReg->blendHigLimit = 0x0;
	pstReg->NrLumaNrGain  = 0x0;
 
    if (OP_TYPE_MANUAL == hi_ext_system_nr_manu_mode_read())
    {
        HI_DOUBLE Threshold = (HI_DOUBLE)hi_ext_system_nr_manual_threshold_read();
        
 	    rangeLowCoef  = (Threshold*0.8)/(HI_DOUBLE)HI_FW_ISO_TABLE_DIVISOR;		
    	rangeHighCoef = (Threshold*1.2)/(HI_DOUBLE)HI_FW_ISO_TABLE_DIVISOR;
    }
    else
    {
	    u16Y1 = (eIsoLevel) ? hi_ext_system_nr_threshold_read(eIsoLevel-1):hi_ext_system_nr_threshold_read(0);
	    u16Y2 = hi_ext_system_nr_threshold_read(eIsoLevel);
        
	    rangeLowCoef  = ((HI_DOUBLE)NrOffsetCalculate(u16Y2, u16Y1, u32ISO2, u32ISO1, iso)*0.8)/(HI_DOUBLE)HI_FW_ISO_TABLE_DIVISOR;
		rangeHighCoef = ((HI_DOUBLE)NrOffsetCalculate(u16Y2, u16Y1, u32ISO2, u32ISO1, iso)*1.2)/(HI_DOUBLE)HI_FW_ISO_TABLE_DIVISOR;
    }

	tmp = v0 * rangeLowCoef;
    pstReg->rangeLowThres    = MIN(tmp,0x3FFFFFFF);

	
	tmp = v0 * rangeHighCoef;
    pstReg->rangeHigThres    = MIN(tmp,0x3FFFFFFF);

            	
    if (OP_TYPE_MANUAL == hi_ext_system_nr_manu_mode_read())
    {
        pstReg->blendLowLimit = hi_ext_system_nr_manual_varstrength_read();	
        pstReg->blendHigLimit = hi_ext_system_nr_manual_fixstrength_read();	
        pstReg->NrLumaNrGain  = hi_ext_system_nr_manual_lowfreqslope_read();            
    }
    else
    {   
		u16Y1 = (eIsoLevel)?hi_ext_system_nr_varstrength_read(eIsoLevel-1):hi_ext_system_nr_varstrength_read(0);
		u16Y2 = hi_ext_system_nr_varstrength_read(eIsoLevel);        

		pstReg->blendLowLimit = NrOffsetCalculate(u16Y2, u16Y1, u32ISO2, u32ISO1, iso); 
		u16Y1 = (eIsoLevel)?hi_ext_system_nr_fixstrength_read(eIsoLevel-1):hi_ext_system_nr_fixstrength_read(0);
		u16Y2 = hi_ext_system_nr_fixstrength_read(eIsoLevel);        
		pstReg->blendHigLimit = NrOffsetCalculate(u16Y2, u16Y1, u32ISO2, u32ISO1, iso); 
		u16Y1 = (eIsoLevel)?hi_ext_system_nr_lowfreqslope_read(eIsoLevel-1):hi_ext_system_nr_lowfreqslope_read(0);
		u16Y2 = hi_ext_system_nr_lowfreqslope_read(eIsoLevel);        
		pstReg->NrLumaNrGain = NrOffsetCalculate(u16Y2, u16Y1, u32ISO2, u32ISO1, iso);                 
    }

        pstReg->blendHigLimit = 256 - pstReg->blendHigLimit;  
        pstReg->blendLowLimit = 256 - pstReg->blendLowLimit;	
        if( pstReg->blendLowLimit > pstReg->blendHigLimit )
        {
        	pstReg->blendLowLimit = pstReg->blendHigLimit ;
        }  	

    pstReg->falseColorEnable = (iso<400)?1 :(iso<800)?1 :(iso<1600)?1 :(iso<3200)?1 :0;//pstNr->stCmosNRTable.stIsoParaTable[eIsoLevel].u8falseColorEnable;
    pstReg->falseColorShift  = (iso<400)?8 :(iso<800)?8 :(iso<1600)?12:(iso<3200)?48:0;//pstNr->stCmosNRTable.stIsoParaTable[eIsoLevel].u8falseColorShift;
    pstReg->falseColorGain   = (iso<400)?32:(iso<800)?32:(iso<1600)?48:(iso<3200)?10:0;//pstNr->stCmosNRTable.stIsoParaTable[eIsoLevel].u8falseColorGain;
    pstReg->falseColorLimit  = (iso<400)?32:(iso<800)?48:(iso<1600)?48:(iso<3200)?10:0;//pstNr->stCmosNRTable.stIsoParaTable[eIsoLevel].u8falseColorLimit;        

    pstReg->lumaNREnable = 1;//pstNr->stCmosNRTable.stNrCommPara.u8lumaNREnable;

	// LUT
	rate = 0x40;
	
	rate <<= (bitDepth - 10);

	for (n=0; n<=16; n++) 
	{
		pstReg->rangeLowLUT[n] = pstReg->rangeLowThres + (HI_S64)(n-8)*rate*pstReg->rangeGain/4096;
		pstReg->rangeHigLUT[n] = pstReg->rangeHigThres + (HI_S64)(n-8)*rate*pstReg->rangeGain/4096;

		pstReg->rangeLowLUT[n] = _clip (0, msk, pstReg->rangeLowLUT[n]);
		pstReg->rangeHigLUT[n] = _clip (0, msk, pstReg->rangeHigLUT[n]);


	}

	//return HI_SUCCESS;
}



HI_S32 ISP_NrRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	ISP_REG_CFG_S *pstReg;
    ISP_CTX_S *pstIspCtx = HI_NULL;
	static HI_U32 u32PrevIso = 0, u32PrevNrEn = 0; 
    HI_U32 u32Ret;
    ISP_GET_CTX(IspDev, pstIspCtx);

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    /* calculate every two interrupts */	
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }

	pstReg = (ISP_REG_CFG_S *)pRegCfg;

	u32Ret = NrReadExtregs(IspDev);
    if((0 == hi_isp_2dnr_cfg_enable_read())&&(u32Ret == 0))
    {
       u32PrevIso = pstIspCtx->stLinkage.u32Iso;
	   u32PrevNrEn = hi_isp_2dnr_cfg_enable_read();
	   return HI_SUCCESS;
	}
	
	if((u32PrevIso ==  pstIspCtx->stLinkage.u32Iso)&&(u32PrevNrEn == hi_isp_2dnr_cfg_enable_read()))
	{
	  if(0 == u32Ret)
	  {
	     ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1NrCfg = 0;
		 return HI_SUCCESS;
	  }
	 
	}
                
	NrRegisterGuider(pstIspCtx->stLinkage.u32Iso, IspDev, &pstReg->stNrRegCfg);	
	
	((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1NrCfg = 1;
	
	u32PrevIso = pstIspCtx->stLinkage.u32Iso;
	u32PrevNrEn = hi_isp_2dnr_cfg_enable_read();
    
    return HI_SUCCESS;
}

HI_S32 ISP_NrCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_NrInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            NrProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_NrExit(ISP_DEV IspDev)
{
	hi_isp_2dnr_cfg_enable_write(HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterNr(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_NR;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_NrInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_NrRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_NrCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_NrExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

