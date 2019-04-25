/******************************************************************************

  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_Compander.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/09/02
  Description   : 
  History       :
  1.Date        : 2014/09/02
    Author      : q00214668
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"

#include "isp_config.h"

#define COMPANDER_BITSIN  12
#define COMPANDER_BITSOUT 20

#define COMPANDER_BITSOUT_X0 64
#define COMPANDER_BITSOUT_Y0 64
#define COMPANDER_BITSOUT_X1 95
#define COMPANDER_BITSOUT_Y1 2048
#define COMPANDER_BITSOUT_X2 125
#define COMPANDER_BITSOUT_Y2 32768
#define COMPANDER_BITSOUT_X3 125
#define COMPANDER_BITSOUT_Y3 32768
#define COMPANDER_BITSOUT_XMAX 129
#define COMPANDER_BITSOUT_YMAX 32768

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiISP_COMPANDER_S
{
	HI_U32 u32BitDepthIn;
	HI_U32 u32BitDepthOut;
	HI_U32 u32compander_x0;
	HI_U32 u32compander_y0;
	HI_U32 u32compander_x1;
	HI_U32 u32compander_y1;
	HI_U32 u32compander_x2;
	HI_U32 u32compander_y2;
	HI_U32 u32compander_x3;
	HI_U32 u32compander_y3;
	HI_U32 u32compander_x_max;
	HI_U32 u32compander_y_max;

	HI_U32 u32Compander_LUT[COMPANDER_BITSOUT_XMAX];	
} ISP_COMPANDER_S;

ISP_COMPANDER_S g_astCompanderCtx[ISP_MAX_DEV_NUM] = {{0}};
#define COMPANDER_GET_CTX(dev, pstCtx)   pstCtx = &g_astCompanderCtx[dev]


static HI_VOID CompanderExtRegsDefault(HI_VOID)
{
    return;
}

static HI_VOID CompanderRegsDefault(HI_VOID) 
{
    return;
}

static HI_VOID CompanderExtRegsInitialize(ISP_DEV IspDev)
{
    return;
}

static HI_VOID CompanderRegsInitialize(ISP_DEV IspDev)
{
	HI_U32 _i,_v;
	HI_U32 X0, Y0, X1, Y1, X2, Y2, X3, Y3, X_max, Y_max;
	ISP_COMPANDER_S *pstCompander = HI_NULL;
	ISP_CMOS_BLACK_LEVEL_S  *pstSnsBlackLevel = HI_NULL;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	
	ISP_CTX_S *pstIspCtx = HI_NULL;    
	HI_U8 u8WDRMode;
    COMPANDER_GET_CTX(IspDev, pstCompander);
    ISP_SensorGetDefault(IspDev, &pstSnsDft);

	ISP_GET_CTX(IspDev, pstIspCtx);

    u8WDRMode = pstIspCtx->u8SnsWDRMode;	

	hi_isp_compander_enable_write(HI_FALSE);
	

	pstCompander->u32BitDepthIn = pstSnsDft->stCompander.u32BitDepthIn;
	pstCompander->u32BitDepthOut= pstSnsDft->stCompander.u32BitDepthOut;

	pstCompander->u32compander_x0	 = pstSnsDft->stCompander.u32X0;
	pstCompander->u32compander_y0	 = pstSnsDft->stCompander.u32Y0;
	pstCompander->u32compander_x1	 = pstSnsDft->stCompander.u32X1;
	pstCompander->u32compander_y1	 = pstSnsDft->stCompander.u32Y1;
	pstCompander->u32compander_x2	 = pstSnsDft->stCompander.u32X2;
	pstCompander->u32compander_y2	 = pstSnsDft->stCompander.u32Y2;
	pstCompander->u32compander_x3	 = pstSnsDft->stCompander.u32X3;
	pstCompander->u32compander_y3	 = pstSnsDft->stCompander.u32Y3;
	pstCompander->u32compander_x_max = pstSnsDft->stCompander.u32Xmax;
	pstCompander->u32compander_y_max = pstSnsDft->stCompander.u32Ymax;

#if 0
	pstCompander->u32BitDepthIn = COMPANDER_BITSIN;
    pstCompander->u32BitDepthOut= COMPANDER_BITSOUT;

	pstCompander->u32compander_x0    = COMPANDER_BITSOUT_X0;
	pstCompander->u32compander_y0    = COMPANDER_BITSOUT_Y0;
	pstCompander->u32compander_x1    = COMPANDER_BITSOUT_X1;
	pstCompander->u32compander_y1    = COMPANDER_BITSOUT_Y1;
	pstCompander->u32compander_x2    = COMPANDER_BITSOUT_X2;
	pstCompander->u32compander_y2    = COMPANDER_BITSOUT_Y2;
	pstCompander->u32compander_x3    = COMPANDER_BITSOUT_X3;
	pstCompander->u32compander_y3    = COMPANDER_BITSOUT_Y3;
	pstCompander->u32compander_x_max = COMPANDER_BITSOUT_XMAX;
	pstCompander->u32compander_y_max = COMPANDER_BITSOUT_YMAX;
#endif

	X0       = pstCompander->u32compander_x0;
	Y0       = pstCompander->u32compander_y0;
	X1       = pstCompander->u32compander_x1;
	Y1       = pstCompander->u32compander_y1;
	X2       = pstCompander->u32compander_x2;
	Y2       = pstCompander->u32compander_y2;
	X3       = pstCompander->u32compander_x3;
	Y3       = pstCompander->u32compander_y3;
	X_max    = pstCompander->u32compander_x_max;
	Y_max    = pstCompander->u32compander_y_max;

	for(_i=0;_i<X0;_i++)
	{
		_v = (((_i*Y0)/X0) >> 0);
		pstCompander->u32Compander_LUT[_i]=_v;
	
		
	}
	for(;_i<X1;_i++)
	{
		_v = ((((_i-X0)*(Y1-Y0))/(X1-X0)+Y0)  >> 0);
		pstCompander->u32Compander_LUT[_i]=_v;
		
	}
	for(;_i<X2;_i++)
	{
		_v = ((((_i-X1)*(Y2-Y1))/(X2-X1)+Y1)  >> 0);
		pstCompander->u32Compander_LUT[_i]=_v;
		
	}
	for(;_i<X3;_i++)
	{
		_v = ((((_i-X2)*(Y3-Y2))/(X3-X2)+Y2)  >> 0);
		pstCompander->u32Compander_LUT[_i]=_v;
		
	}
	for(;_i<X_max;_i++)
	{
		_v = (Y_max  >> 0);
		pstCompander->u32Compander_LUT[_i]=_v;		
	}

	hi_isp_compander_waddr_write(0);

	for (_i = 0; _i < COMPANDER_BITSOUT_XMAX; _i++)
	{
		hi_isp_compander_wdata_write(pstCompander->u32Compander_LUT[_i]);
	}	
    
    ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel);

	hi_isp_compander_bitw_in_write    (pstCompander->u32BitDepthIn);
	hi_isp_compander_bitw_out_write   (pstCompander->u32BitDepthOut);
	hi_isp_compander_OFFSETR_in_write (pstSnsBlackLevel->au16BlackLevel[0]);
	hi_isp_compander_OFFSETGR_in_write(pstSnsBlackLevel->au16BlackLevel[1]);
	hi_isp_compander_OFFSETGB_in_write(pstSnsBlackLevel->au16BlackLevel[2]);
	hi_isp_compander_OFFSETB_in_write (pstSnsBlackLevel->au16BlackLevel[3]);

	if (IS_LINEAR_MODE(u8WDRMode))
	{             
		hi_isp_compander_enable_write(HI_FALSE);
		hi_isp_blc_enable_write(HI_TRUE);
	}
	else if (IS_BUILT_IN_WDR_MODE(u8WDRMode)
			|| IS_FS_WDR_MODE(u8WDRMode))
	{                    
		hi_isp_compander_enable_write(HI_TRUE);
		hi_isp_blc_enable_write(HI_FALSE);
	}
	else
	{
	    
	}

    return;
}

static HI_S32 CompanderReadExtregs(ISP_DEV IspDev)
{
    return 0;
}

HI_S32 ISP_CompanderInit(ISP_DEV IspDev)
{
    CompanderRegsDefault();
    CompanderExtRegsDefault();
    CompanderRegsInitialize(IspDev);
    CompanderExtRegsInitialize(IspDev);

    return HI_SUCCESS;
}

HI_S32 ISP_CompanderRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{   
	ISP_CTX_S *pstIspCtx  = HI_NULL;
	ISP_CMOS_BLACK_LEVEL_S  *pstSnsBlackLevel = HI_NULL;

	ISP_GET_CTX(IspDev, pstIspCtx);

	/* calculate every two interrupts */
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }
	CompanderReadExtregs(IspDev);

	ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel);

	hi_isp_compander_OFFSETR_in_write (pstSnsBlackLevel->au16BlackLevel[0]);
	hi_isp_compander_OFFSETGR_in_write(pstSnsBlackLevel->au16BlackLevel[1]);
	hi_isp_compander_OFFSETGB_in_write(pstSnsBlackLevel->au16BlackLevel[2]);
	hi_isp_compander_OFFSETB_in_write (pstSnsBlackLevel->au16BlackLevel[3]);
	
    return HI_SUCCESS;
}

HI_S32 ISP_CompanderCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    HI_U8 u8Mode = *((HI_U8 *)pValue);

    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET:
            if (IS_LINEAR_MODE(u8Mode))
            {             
                CompanderRegsInitialize(IspDev);
            	
            }
            else if (IS_BUILT_IN_WDR_MODE(u8Mode)
					|| IS_FS_WDR_MODE(u8Mode))
            {  
               CompanderRegsInitialize(IspDev);
				hi_isp_compander_enable_write(HI_TRUE);
				hi_isp_blc_enable_write(HI_FALSE);
            }
            else
            {
                
            }
            break;
        default :
            break;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_CompanderExit(ISP_DEV IspDev)
{
	hi_isp_compander_enable_write(HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterCompander(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_COMPANDER;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_CompanderInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_CompanderRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_CompanderCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_CompanderExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

