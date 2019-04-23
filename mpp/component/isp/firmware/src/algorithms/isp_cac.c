/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_green_equalization.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/07/09
  Description   : 
  History       :
  1.Date        : 2014/07/09
    Author      : q00214668
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_config.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/* CAC */
#define CAC_MAX_LUT_SIZE 64 // Max size of LUT
#define CAC_INTER_B      1

// Shift table
HI_S16 g_lut_r_hor[CAC_MAX_LUT_SIZE] = {-81,-79,-77,-74,-72,-69,-67,-64,-61,-59,-56,-53,-50,-47,-44,-41,-38,-35,-32,-29,-26,-23,-20,-16,-13,-10,-7,-4,-1,0,3,6,9,12,15,18,20,23,26,28,31,33,36,38,40,42,44,46,48,50,52,53,55,56,57,58,59,60,61,61}; 
HI_S16 g_lut_r_ver[CAC_MAX_LUT_SIZE] = {-60,-55,-51,-46,-42,-37,-33,-29,-25,-21,-17,-13,-9,-5,-1,0,4,7,10,13,16,19,21,24,26,28,30,32,34,35,37,38,39,40};
HI_S16 g_lut_b_hor[CAC_MAX_LUT_SIZE] = {-48,-47,-46,-45,-44,-43,-42,-41,-39,-38,-37,-36,-34,-33,-32,-30,-29,-27,-26,-24,-23,-21,-20,-18,-17,-15,-13,-11,-10,-8,-6,-4,-2,-1,0,1,3,5,7,9,11,13,15,18,20,22,24,26,29,31,33,36,38,40,43,45,48,50,53,55};
HI_S16 g_lut_b_ver[CAC_MAX_LUT_SIZE] = {-24,-23,-22,-22,-21,-19,-18,-17,-16,-14,-12,-11,-9,-7,-5,-3,-1,0,1,4,6,8,10,12,14,17,19,21,23,25,26,28,30,31};

typedef struct hiISP_CAC_REG_CFG_S
{
	HI_U16 u16Width;
	HI_U16 u16Height;
    HI_S16 lut_r_hor[CAC_MAX_LUT_SIZE];    // Shift table  // 64 X 8-bit unsigned integer  // 64 X U1.7
    HI_S16 lut_r_ver[CAC_MAX_LUT_SIZE];    // Shift table  // 64 X 8-bit unsigned integer  // 64 X U1.7
    HI_S16 lut_b_hor[CAC_MAX_LUT_SIZE];    // Shift table  // 64 X 8-bit unsigned integer  // 64 X U1.7
    HI_S16 lut_b_ver[CAC_MAX_LUT_SIZE];    // Shift table  // 64 X 8-bit unsigned integer  // 64 X U1.7

    HI_U8 lut_sample_ratio_h;   // Horizontal sampling ratio of LUT. [0, 10], Power number. // 4-bit unsigned integer  // U4.0  
    HI_U8 lut_sample_ratio_v;   // Vertical sampling ratio of LUT. [0, 10] ,Power number.   // 4-bit unsigned integer  // U4.0  

    HI_BOOL bEnInter;              // Interplation enable. TRUE : interplation when using LUT          // 1-bit unsigned integer   // U1.0
} ISP_CAC_CFG_S;

ISP_CAC_CFG_S g_astCacCtx[ISP_MAX_DEV_NUM] = {{0}};
#define CAC_GET_CTX(dev, pstCtx)   pstCtx = &g_astCacCtx[dev]


static HI_VOID CacExtRegsDefault(HI_VOID)
{
    return;
}

static HI_VOID CacRegsDefault(HI_VOID) 
{   	
    return;
}

static HI_VOID CacExtRegsInitialize(ISP_DEV IspDev)
{	
    return;
}

static HI_VOID CacRegsInitialize(ISP_DEV IspDev)
{    
	HI_U32 i = 0;
	ISP_CAC_CFG_S *pstCac = HI_NULL;    
    CAC_GET_CTX(IspDev, pstCac);

	pstCac->bEnInter = CAC_INTER_B;
	/*写寄存器的时候，高宽都减一*/
	pstCac->u16Width  = (hi_isp_top_active_width_read() + 1);
	pstCac->u16Height = (hi_isp_top_active_height_read() + 1);

    while (((pstCac->u16Width + CAC_MAX_LUT_SIZE - 1) / CAC_MAX_LUT_SIZE) > (1 << i))
    {
        i++;
    }
	
    pstCac->lut_sample_ratio_h = i;


    i = 0;
    while (((pstCac->u16Height + CAC_MAX_LUT_SIZE - 1) / CAC_MAX_LUT_SIZE) > (1 << i))
    {
        i++;
    }
	
    pstCac->lut_sample_ratio_v = i;

	//printf("hi_isp_cac_info:sample_ratio_h %d, sample_ratio_v %d \n", pstCac->lut_sample_ratio_h, pstCac->lut_sample_ratio_v);

	for (i = 0; i < CAC_MAX_LUT_SIZE; i++)
    {
        pstCac->lut_r_hor[i] = g_lut_r_hor[i];
        pstCac->lut_r_ver[i] = g_lut_r_ver[i];
        pstCac->lut_b_hor[i] = g_lut_b_hor[i];
        pstCac->lut_b_ver[i] = g_lut_b_ver[i];
    }

	/*CAC is off by default*/
	//hi_isp_cac_enable_write(HI_TRUE);
	hi_isp_cac_inter_enable_write(pstCac->bEnInter);

	hi_isp_cac_sample_ratio_v_write(pstCac->lut_sample_ratio_v);
	hi_isp_cac_sample_ratio_h_write(pstCac->lut_sample_ratio_h);

	hi_isp_cac_h_waddr_write(0);
	hi_isp_cac_v_waddr_write(0);
	
	for (i = 0; i < CAC_MAX_LUT_SIZE; i++)
	{
		hi_isp_cac_h_wdata_write((pstCac->lut_r_hor[i] & 0x1ff) | ((pstCac->lut_b_hor[i] & 0x1ff) << 16));
		hi_isp_cac_v_wdata_write((pstCac->lut_r_ver[i] & 0x1ff) | ((pstCac->lut_b_ver[i] & 0x1ff) << 16));
	}
		
    return;
}

static HI_S32 CacReadExtregs(ISP_DEV IspDev)
{
    return 0;
}

HI_S32 ISP_CacInit(ISP_DEV IspDev)
{
    CacRegsDefault();
    CacExtRegsDefault();
    CacRegsInitialize(IspDev);
    CacExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_CacRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    CacReadExtregs(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_CacCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_CacExit(ISP_DEV IspDev)
{
	hi_isp_cac_enable_write(HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterCac(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_CAC;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_CacInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_CacRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_CacCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_CacExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */



