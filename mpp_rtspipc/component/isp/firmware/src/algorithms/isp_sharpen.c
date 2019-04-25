/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_sharpen.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2014/01/16
    Author      : q00214668
    Modification: Created file

******************************************************************************/
#include "isp_config.h"
#include "isp_sharpen_mem_config.h"
#include "hi_isp_debug.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef MAX2
#define MAX2(a, b)              ((a) > (b) ? (a) : (b))
#endif

static const HI_S32 g_SharpenLutIso[ISP_AUTO_ISO_STENGTH_NUM] ={100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};
static const HI_U16 SharpThdDelta[ISP_AUTO_ISO_STENGTH_NUM]={50,60,80,150,200,250,300,350,400,500,600,700,700,700,700,700};
static const HI_U16 EdgeThdDelta[ISP_AUTO_ISO_STENGTH_NUM]={30,50,80,100,150,200,250,300,350,400,450,500,500,500,500,500};

static const HI_U16 SharpThdDeltaWDR[ISP_AUTO_ISO_STENGTH_NUM]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
static const HI_U16 EdgeThdDeltaWDR[ISP_AUTO_ISO_STENGTH_NUM]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};


typedef struct hiISP_SHARPEN_S
{
    HI_BOOL abEnPixSel[ISP_AUTO_ISO_STENGTH_NUM];

    HI_U8  au8MaxSharpAmt1[ISP_AUTO_ISO_STENGTH_NUM];
    HI_U8  au8MaxEdgeAmt[ISP_AUTO_ISO_STENGTH_NUM];

    HI_U8  au8SharpThd2[ISP_AUTO_ISO_STENGTH_NUM]; 
    HI_U8  au8EdgeThd2[ISP_AUTO_ISO_STENGTH_NUM];

    HI_U8  au8OvershootAmt[ISP_AUTO_ISO_STENGTH_NUM]; 
    HI_U8  au8UndershootAmt[ISP_AUTO_ISO_STENGTH_NUM];
} ISP_SHARPEN_S;

#define SHARPEN_EN               (1)
#define SHARPEN_EN_PIXSEL        (1)
#define SHARPEN_LINE_THD1		(8)
#define SHARPEN_LINE_THD2		(4)

ISP_SHARPEN_S g_astSharpenCtx[ISP_MAX_DEV_NUM] = {{{0}}};
#define SHARPEN_GET_CTX(dev, pstCtx)   pstCtx = &g_astSharpenCtx[dev]

static HI_VOID SharpenExtRegsDefault(HI_VOID)
{
    return;
}

static HI_VOID SharpenRegsDefault(HI_VOID) 
{
	hi_isp_sharpen_cfg_enable_write(SHARPEN_EN);
	hi_isp_sharpen_enpixsel_write(SHARPEN_EN_PIXSEL);
    return;
}

static HI_VOID SharpenExtRegsInitialize(ISP_DEV IspDev)
{
	HI_S32 i;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	ISP_SHARPEN_S *pstSharpen = HI_NULL;
    
    ISP_SensorGetDefault(IspDev, &pstSnsDft);			
    SHARPEN_GET_CTX(IspDev, pstSharpen);

	/*get default para from sensor*/
	memcpy(pstSharpen, &pstSnsDft->stRgbSharpen, sizeof(ISP_SHARPEN_S));

	hi_ext_system_sharpen_manu_mode_write(HI_EXT_SYSTEM_SHARPEN_MANU_MODE_WRITE_DEFAULT);

	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
		hi_ext_system_sharpen_EnLowLumaShoot_write(i, pstSharpen->abEnPixSel[i]);		
		hi_ext_system_sharpen_D_write(i, pstSharpen->au8MaxEdgeAmt[i]);
		hi_ext_system_sharpen_Ud_write(i, pstSharpen->au8MaxSharpAmt1[i]);
		hi_ext_system_sharpen_overshootAmt_write(i, pstSharpen->au8OvershootAmt[i]);
		hi_ext_system_sharpen_undershootAmt_write(i, pstSharpen->au8UndershootAmt[i]);  
		hi_ext_system_sharpen_TextureNoiseThd_write(i, pstSharpen->au8SharpThd2[i]);
		hi_ext_system_sharpen_EdgeNoiseThd_write(i, pstSharpen->au8EdgeThd2[i]);
		
	}	
	
	hi_ext_system_manual_sharpen_EnLowLumaShoot_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_ENLOWLUMASHOOT_WRITE_DEFAULT);
	hi_ext_system_manual_sharpen_D_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_D_WRITE_DEFAULT);
	hi_ext_system_manual_sharpen_Ud_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_UD_WRITE_DEFAULT);
	hi_ext_system_manual_sharpen_overshootAmt_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_OVERSHOOTAMT_WRITE_DEFAULT);
	hi_ext_system_manual_sharpen_undershootAmt_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_UNDERSHOOTAMT_WRITE_DEFAULT);
	hi_ext_system_manual_sharpen_TextureNoiseThd_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTURENOISETHD_WRITE_DEFAULT);
	hi_ext_system_manual_sharpen_EdgeNoiseThd_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGENOISETHD_WRITE_DEFAULT);
	
	return;
}

static HI_VOID SharpenRegsInitialize(ISP_DEV IspDev)
{   
    return;
}

static inline HI_U8 SharpenGetIsoIndex(HI_U32 u32Iso)
{   
	HI_U8 u8Index;
	
    for (u8Index = 0; u8Index < ISP_AUTO_ISO_STENGTH_NUM; u8Index++)
    {
        if (u32Iso <= g_SharpenLutIso[u8Index])
        {
            return u8Index;
        }
    }
	
    return (ISP_AUTO_ISO_STENGTH_NUM - 1);
}


static HI_S32 SharpenReadExtregs(ISP_DEV IspDev)
{	
	HI_U32 i;
	ISP_OP_TYPE_E enSharpenType = OP_TYPE_AUTO;
	ISP_SHARPEN_S *pstSharpen = HI_NULL;
	static ISP_SHARPEN_S PrestSharpen = {{0}};
	HI_U8 bUpdate = 0;
	static HI_U8 preMode = 0;

	SHARPEN_GET_CTX(IspDev, pstSharpen);
	
	enSharpenType = hi_ext_system_sharpen_manu_mode_read();
	if(preMode != enSharpenType )
	{
      bUpdate = 1;
	}
	

	if (OP_TYPE_MANUAL == enSharpenType)
	{

		for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
		{

			pstSharpen->abEnPixSel[i] = hi_ext_system_manual_sharpen_EnLowLumaShoot_read();
			pstSharpen->au8MaxEdgeAmt[i] = hi_ext_system_manual_sharpen_D_read();
			pstSharpen->au8MaxSharpAmt1[i] = hi_ext_system_manual_sharpen_Ud_read();
			pstSharpen->au8OvershootAmt[i] = hi_ext_system_manual_sharpen_overshootAmt_read();
			pstSharpen->au8UndershootAmt[i] = hi_ext_system_manual_sharpen_undershootAmt_read();
			pstSharpen->au8SharpThd2[i] = hi_ext_system_manual_sharpen_TextureNoiseThd_read();
			pstSharpen->au8EdgeThd2[i] = hi_ext_system_manual_sharpen_EdgeNoiseThd_read();
		}
		
	}
	else
	{
	  
		for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
		{
			pstSharpen->abEnPixSel[i] = hi_ext_system_sharpen_EnLowLumaShoot_read(i);
			pstSharpen->au8MaxEdgeAmt[i] = hi_ext_system_sharpen_D_read(i);
			pstSharpen->au8MaxSharpAmt1[i] = hi_ext_system_sharpen_Ud_read(i);
			pstSharpen->au8OvershootAmt[i] = hi_ext_system_sharpen_overshootAmt_read(i);
			pstSharpen->au8UndershootAmt[i] = hi_ext_system_sharpen_undershootAmt_read(i);
			pstSharpen->au8SharpThd2[i] = hi_ext_system_sharpen_TextureNoiseThd_read(i);
			pstSharpen->au8EdgeThd2[i] = hi_ext_system_sharpen_EdgeNoiseThd_read(i);
			
		}
	}				

	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
	
      if(PrestSharpen.abEnPixSel[i] != pstSharpen->abEnPixSel[i])
	   	{
          bUpdate = 1;
		  break;
	    }
	  else if(PrestSharpen.au8MaxEdgeAmt[i] != pstSharpen->au8MaxEdgeAmt[i])
		{
          bUpdate = 1;
		  break;
	    }
	  else if(PrestSharpen.au8MaxSharpAmt1[i] != pstSharpen->au8MaxSharpAmt1[i])
		{
			bUpdate = 1;
			break;
		}
	  else if(PrestSharpen.au8OvershootAmt[i] != pstSharpen->au8OvershootAmt[i])
		{
			bUpdate = 1;
			break;
		}
	  else if(PrestSharpen.au8UndershootAmt[i] != pstSharpen->au8UndershootAmt[i])
		{
			bUpdate = 1;
			break;
		}
	  else if(PrestSharpen.au8SharpThd2[i] != pstSharpen->au8SharpThd2[i])
		{
			bUpdate = 1;
			break;
		}
	  else if(PrestSharpen.au8EdgeThd2[i] != pstSharpen->au8EdgeThd2[i])
		{
			bUpdate = 1;
			break;
		}
	   else
		{
		
		}
	}
	
	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
        preMode = enSharpenType;
		PrestSharpen.abEnPixSel[i] = pstSharpen->abEnPixSel[i];
		PrestSharpen.au8MaxEdgeAmt[i] =	pstSharpen->au8MaxEdgeAmt[i] ;
		PrestSharpen.au8MaxSharpAmt1[i] = pstSharpen->au8MaxSharpAmt1[i];
		PrestSharpen.au8OvershootAmt[i] = pstSharpen->au8OvershootAmt[i] ;
		PrestSharpen.au8UndershootAmt[i] = pstSharpen->au8UndershootAmt[i] ;
		PrestSharpen.au8SharpThd2[i] = pstSharpen->au8SharpThd2[i] ;
		PrestSharpen.au8EdgeThd2[i] = pstSharpen->au8EdgeThd2[i];
		
	}
	
    return bUpdate;
}

HI_S32 SharpenProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
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
        "-----SHARPEN INFO--------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%15s"    "%15s"             "%15s"          "%15s"     ,
        "En"  ,   "LowLumaShoot"  , "SharpenD" ,     "SharpenUD\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%15d"    "%15d"    "%15d"   "%15d   \n",
        hi_isp_sharpen_cfg_enable_read(),
		!(hi_isp_sharpen_enpixsel_read()),
	   (hi_isp_edge_amt_read()>>4),
	   (hi_isp_sharp_amt_read()>>4));

       pstProc->u32WriteLen += 1;

    return HI_SUCCESS;
}


HI_S32 ISP_SharpenInit(ISP_DEV IspDev)
{
    SharpenRegsDefault();
    SharpenExtRegsDefault();
    SharpenRegsInitialize(IspDev);
    SharpenExtRegsInitialize(IspDev);
	
    return HI_SUCCESS;
}

HI_S32 LinearInter(HI_S32 v, HI_S32 x0, HI_S32 y0, HI_S32 x1, HI_S32 y1)
{
    HI_S32 res;

    if ( v <= x0 ) return y0;
    if ( v >= x1 ) return y1;

    res = (y1 - y0) * (v - x0) / (x1 - x0) + y0;
    return res;
}

HI_S32 ISP_Sharpen_GetRegCfg(HI_U32 iso, HI_U32 expTime, ISP_DEV IspDev, ISP_SHARPEN_REG_CFG_S *pstSharpenReg)
{
	HI_U32 i;
	HI_S32 index, index2;
	
	
	ISP_SHARPEN_S *pstSharpenParamsIn = HI_NULL;
	ISP_CTX_S *pstIspCtx = HI_NULL;
	HI_U8 u8WDRMode;
	ISP_GET_CTX(IspDev, pstIspCtx);
	

	HI_S32 bpCoef[3][3] = {{-16, -16, -16},
                          {-16,  28,  28},
                          {-16,  28,  32}};

	SHARPEN_GET_CTX(IspDev, pstSharpenParamsIn);
	u8WDRMode = pstIspCtx->u8SnsWDRMode;

	if (HI_NULL == pstSharpenParamsIn)
	{
		return HI_FAILURE;
	}


	index = ISP_AUTO_ISO_STENGTH_NUM - 1;
    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
    {
        if (iso <= g_SharpenLutIso[i])
        {
            index = i;
            break;
        }
    }
	
    index2 = MAX2(index - 1, 0);

    //expTimeThd_in  = pstSharpenParamsIn->u32ExpTimeThdIn;
    //expTimeThd_out = pstSharpenParamsIn->u32ExpTimeThdOut;
	
    pstSharpenReg->G3[0][0] = bpCoef[0][0];
    pstSharpenReg->G3[0][1] = bpCoef[0][1];
    pstSharpenReg->G3[0][2] = bpCoef[0][2];
    pstSharpenReg->G3[1][0] = bpCoef[1][0];
    pstSharpenReg->G3[1][1] = bpCoef[1][1];
    pstSharpenReg->G3[1][2] = bpCoef[1][2];
    pstSharpenReg->G3[2][0] = bpCoef[2][0];
    pstSharpenReg->G3[2][1] = bpCoef[2][1];
    pstSharpenReg->G3[2][2] = bpCoef[2][2];

	pstSharpenReg->enPixSel =  !(pstSharpenParamsIn->abEnPixSel[index]);
    pstSharpenReg->u8LineThd1 = SHARPEN_LINE_THD1;
    pstSharpenReg->u8LineThd2 = SHARPEN_LINE_THD2;


	 // Calc sharpAmt1
    pstSharpenReg->u16SharpAmt1 = LinearInter(iso, 
                                           g_SharpenLutIso[index2], ((pstSharpenParamsIn->au8MaxSharpAmt1[index2])<<4),
                                           g_SharpenLutIso[index],  ((pstSharpenParamsIn->au8MaxSharpAmt1[index])<<4));

    // Calc edgeAmt
    pstSharpenReg->u16EdgeAmt = LinearInter(iso, 
                                         g_SharpenLutIso[index2], ((pstSharpenParamsIn->au8MaxEdgeAmt[index2])<<4),
                                         g_SharpenLutIso[index],  ((pstSharpenParamsIn->au8MaxEdgeAmt[index])<<4));
    
   // Over/Under shoot
		pstSharpenReg->u8OvershootAmt = LinearInter(iso, 
                                         g_SharpenLutIso[index2], pstSharpenParamsIn->au8OvershootAmt[index2],
                                         g_SharpenLutIso[index],  pstSharpenParamsIn->au8OvershootAmt[index]);

		pstSharpenReg->u8UndershootAmt = LinearInter(iso, 
                                         g_SharpenLutIso[index2], pstSharpenParamsIn->au8UndershootAmt[index2],
                                         g_SharpenLutIso[index],  pstSharpenParamsIn->au8UndershootAmt[index]);	

    // mid band
   pstSharpenReg->u16SharpThd2  = LinearInter(iso, 
                                    g_SharpenLutIso[index2], ((pstSharpenParamsIn->au8SharpThd2[index2])<<2),
                                    g_SharpenLutIso[index],  ((pstSharpenParamsIn->au8SharpThd2[index])<<2));
		if(1==u8WDRMode)
		{
			pstSharpenReg->u16SharpThd1  = LinearInter(iso, 
															g_SharpenLutIso[index2], (((pstSharpenParamsIn->au8SharpThd2[index2])<<2)+SharpThdDeltaWDR[index2]),
															g_SharpenLutIso[index],  (((pstSharpenParamsIn->au8SharpThd2[index])<<2)+SharpThdDeltaWDR[index]));


		}
		else
		{
			pstSharpenReg->u16SharpThd1  = LinearInter(iso, 
												g_SharpenLutIso[index2], (((pstSharpenParamsIn->au8SharpThd2[index2])<<2)+SharpThdDelta[index2]),
												g_SharpenLutIso[index],  (((pstSharpenParamsIn->au8SharpThd2[index])<<2)+SharpThdDelta[index]));

		}
        

		if(pstSharpenReg->u16SharpThd1 > 1023)
			pstSharpenReg->u16SharpThd1 = 1023;

        // high band
        pstSharpenReg->u16EdgeThd2  = LinearInter(iso, 
                                    g_SharpenLutIso[index2], ((pstSharpenParamsIn->au8EdgeThd2[index2])<<2),
                                    g_SharpenLutIso[index],  ((pstSharpenParamsIn->au8EdgeThd2[index])<<2));
		if(1==u8WDRMode)
		{
			pstSharpenReg->u16EdgeThd1	= LinearInter(iso, 
											g_SharpenLutIso[index2], (((pstSharpenParamsIn->au8EdgeThd2[index2])<<2)+EdgeThdDeltaWDR[index2]),
											g_SharpenLutIso[index],  (((pstSharpenParamsIn->au8EdgeThd2[index])<<2)+EdgeThdDeltaWDR[index]));
			

		}
		else
		{
			pstSharpenReg->u16EdgeThd1	= LinearInter(iso, 
											g_SharpenLutIso[index2], (((pstSharpenParamsIn->au8EdgeThd2[index2])<<2)+EdgeThdDelta[index2]),
											g_SharpenLutIso[index],  (((pstSharpenParamsIn->au8EdgeThd2[index])<<2)+EdgeThdDelta[index]));
		}



	if(pstSharpenReg->u16EdgeThd1 > 1023)
			pstSharpenReg->u16EdgeThd1 = 1023;
  
    
    // Over/Under shoot Thd
    pstSharpenReg->u8OvershootThd1   = 0;
    pstSharpenReg->u8OvershootThd2   = 0;

    pstSharpenReg->u8UndershootThd1  = 0;
    pstSharpenReg->u8UndershootThd2  = 0;
	
	// Calc sharpThd1MulCoef
	if (pstSharpenReg->u16SharpThd1 > pstSharpenReg->u16SharpThd2)
	{
		pstSharpenReg->u16SharpThd1MulCoef = 4096 / (pstSharpenReg->u16SharpThd1 - pstSharpenReg->u16SharpThd2);
	}
	else
	{
		printf("Input Params Error @Sharpen: sharpThd1 should not be less than or equal to sharpThd2!\n");
		return HI_FAILURE;
	}

	// Calc edgeThd1MulCoef
	if (pstSharpenReg->u16EdgeThd1 > pstSharpenReg->u16EdgeThd2)
	{
		pstSharpenReg->u16EdgeThd1MulCoef = 4096 / (pstSharpenReg->u16EdgeThd1 - pstSharpenReg->u16EdgeThd2);
	}
	else
	{
		printf("Input Params Error @Sharpen: edgeThd1 should not be less than or equal to edgeThd2!\n");
		return HI_FAILURE;
	}

	// Over shoot overshootThdMulCoef
	if (pstSharpenReg->u8OvershootThd1 > pstSharpenReg->u8OvershootThd2)
	{
		pstSharpenReg->u16OvershootThdMulCoef = (256 - pstSharpenReg->u8OvershootAmt) / (pstSharpenReg->u8OvershootThd1 - pstSharpenReg->u8OvershootThd2);
	}
	else if (pstSharpenReg->u8OvershootThd1 == pstSharpenReg->u8OvershootThd2)
	{
		pstSharpenReg->u16OvershootThdMulCoef = 0;
	}
	else
	{
		printf("Input Params Error @Sharpen: overshootThd1 should not be less than overshootThd2!\n");
		return HI_FAILURE;
	}

	// Under shoot undershootThdMulCoef
	if (pstSharpenReg->u8UndershootThd1 > pstSharpenReg->u8UndershootThd2)
	{
		pstSharpenReg->u16UndershootThdMulCoef = (256 - pstSharpenReg->u8UndershootAmt) / (pstSharpenReg->u8UndershootThd1 - pstSharpenReg->u8UndershootThd2);
	}
	else if (pstSharpenReg->u8UndershootThd1 == pstSharpenReg->u8UndershootThd2)
	{
		pstSharpenReg->u16UndershootThdMulCoef = 0;
	}
	else
	{
		printf("Input Params Error @Sharpen: undershootThd1 should not be less than undershootThd2!\n");
		return HI_FAILURE;
	}
	
	return HI_SUCCESS;

}


HI_S32 ISP_SharpenRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{

	HI_U32 u32Iso, u32ExpTime;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;
	static HI_U32 u32PrevIso = 0, u32PreSharpenEn = 0; 
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

    u32Ret = SharpenReadExtregs(IspDev);

	u32Iso     = pstIspCtx->stLinkage.u32Iso;
	u32ExpTime = pstIspCtx->stLinkage.u32Inttime;
	
	if((0 == hi_isp_sharpen_cfg_enable_read())&& (u32Ret == 0))
	{
	   u32PrevIso = u32Iso;
       u32PreSharpenEn = hi_isp_sharpen_cfg_enable_read();
       return HI_SUCCESS;
	}

	if((u32PrevIso == u32Iso)&& (u32PreSharpenEn == hi_isp_sharpen_cfg_enable_read()))
	{
	  if(u32Ret == 0)
	  {
	  	 pstRegCfg->unKey.bit1SharpenCfg = 0;
	  	 return HI_SUCCESS;
	  }
	}


    /* sharpen strength linkage with the iso calculated by ae */
	if (HI_SUCCESS == ISP_Sharpen_GetRegCfg(u32Iso, u32ExpTime, IspDev, &pstRegCfg->stSharpenRegCfg))
	{
		pstRegCfg->unKey.bit1SharpenCfg = 1;
	}
	
	u32PrevIso = u32Iso;
	u32PreSharpenEn = hi_isp_sharpen_cfg_enable_read();

    return HI_SUCCESS;
}

HI_S32 ISP_SharpenCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_SharpenInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            SharpenProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_SharpenExit(ISP_DEV IspDev)
{
	hi_isp_sharpen_cfg_enable_write(HI_FALSE);
	hi_isp_sharpen_enpixsel_write(HI_FALSE);
	hi_ext_system_sharpen_manu_mode_write(HI_FALSE);
	
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterSharpen(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_SHARPEN;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_SharpenInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_SharpenRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_SharpenCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_SharpenExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

