/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_lsc.c
  Version       : Initial Draft
  Author        : 
  Created       : 2014/11/20
  Last Modified :
  Description   : Lens Shading Correction Algorithms
  Function List :
  History       :
  1.Date        : 2014/11/20
    Author      : l00307048
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_sensor.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define LSC_GRID_ROWS		17
#define LSC_GRID_COLS		17
#define LSC_GRID_POINTS		(LSC_GRID_ROWS*LSC_GRID_COLS)

#define WINDOWS_INFO		1.0f
#define LSC_GRID_Q_VALUE	1024
#define LSC_GRID_MAX_VALUE  8191

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/
HI_U8 g_u8PreLight[2] = {0};
HI_FLOAT g_f16PreWeight[2] = {0};

typedef enum hiISP_LSC_MODE
{
	HI_SINGLE_LIGHT_MODE,
	HI_TRIPLE_LIGHT_MODE,
}HI_ISP_LSC_MODE_E;

/* light source calibration mode*/
#define HI_ISP_LSC_LIGHT_MODE		HI_TRIPLE_LIGHT_MODE
#define HI_ISP_LSC_DEFAULT_GAIN     1024
#define HI_ISP_LSC_DEFAULT_GAINL    0x00800400

typedef enum hiISP_LSC_LIGHT
{
    HI_LSC_GRID_LIGHT1 = 0,
	HI_LSC_GRID_LIGHT2,
	HI_LSC_GRID_LIGHT3,
   	HI_LSC_GRID_LIGHT_NUM,             
}HI_ISP_LSC_LIGHT_E;

typedef struct hiISP_LSC
{
	HI_BOOL bLscEnable;
	HI_ISP_LSC_MODE_E eLscMode;
	HI_U16 au16Wbgain[2];
	HI_BOOL bLightChaged;
	HI_U8 u8LscDefLight;
	HI_BOOL bLscCoefUpdata;

	HI_U8 u8LscGridRows;
	HI_U8 u8LscGridCols;
	ISP_BAYER_FORMAT_E enBayerFormat;

    HI_U32 noise_control[LSC_GRID_POINTS];
	HI_U32 grr_gain[LSC_GRID_POINTS];
	HI_U32 gbb_gain[LSC_GRID_POINTS];
	HI_U32 delta_x[(LSC_GRID_COLS-1)/2];	// grid width
	HI_U32 delta_y[(LSC_GRID_ROWS-1)/2];	// grid hight
	HI_U32 inv_dx[(LSC_GRID_COLS-1)/2];		// grid width inv
	HI_U32 inv_dy[(LSC_GRID_ROWS-1)/2];		// grid hight inv      

	ISP_CMOS_LSC_S stCmosLsc;
}ISP_LSC_S;

ISP_LSC_S g_astLscCtx[ISP_MAX_DEV_NUM] = {{0}};
#define LSC_GET_CTX(dev, pstCtx)   pstCtx = &g_astLscCtx[dev]

HI_U16 GetWeightedGain(HI_FLOAT f32Weight1, HI_FLOAT f32Weight2, HI_U32 u32X1,
			HI_U32 u32X2, HI_FLOAT f32X3, HI_U32 u32BoundUp, HI_U32 u32BoundBtm)
{
	HI_FLOAT f32Tmp1, f32Tmp2, f32tmp;
	HI_U16 u16Result;
	HI_U32 u32Q = 10000000;

	f32Tmp1 = ((HI_FLOAT)u32X1)/((HI_FLOAT)u32Q);
	f32Tmp2 = ((HI_FLOAT)u32X2)/((HI_FLOAT)u32Q);
	f32tmp  = (f32Weight1*f32Tmp1 + f32Weight2*f32Tmp2)*f32X3*1024.0f;
    if ( f32tmp > u32BoundUp )
    {
        f32tmp = u32BoundUp;
    }
	u16Result = (HI_U16)f32tmp;
    

	return u16Result;
}


HI_VOID geometricGridSize(ISP_LSC_S *pstLsc)
{

   HI_S32 i;

   for ( i = 0 ; i < ((LSC_GRID_COLS-1)/2); i++ )
   {
       if ( pstLsc->delta_x[i] != 0  )  
       {
         pstLsc->inv_dx[i] = (4095*1024/pstLsc->delta_x[i] + (1024>>1))/1024;   
       }else{
         pstLsc->inv_dx[i] = 0;
       }

       if ( pstLsc->delta_y[i] != 0  )  
       {
         pstLsc->inv_dy[i] = (4095*1024/pstLsc->delta_y[i] + (1024>>1))/1024;   
       }else{
         pstLsc->inv_dy[i] = 0;
       }
   }

}


HI_VOID ISP_Lsc_Fw(ISP_LSC_S *pstLsc)
{
	HI_U32 i = 0;
    //HI_U32 j;
	HI_U32 dist[HI_LSC_GRID_LIGHT_NUM];
	HI_U32 dist_min1, dist_min2;
	HI_FLOAT w1, w2;

	HI_U16  r_gain[LSC_GRID_POINTS];   //13bit	17*17  
	HI_U16  gr_gain[LSC_GRID_POINTS];  //13bit   
	HI_U16  gb_gain[LSC_GRID_POINTS];  //13bit	
	HI_U16  b_gain[LSC_GRID_POINTS];   //13bit	

	HI_ISP_LSC_LIGHT_E lsc_light1, lsc_light2;
	HI_S32 dif1, dif2;
	
	HI_FLOAT dec_gain_grid_r[LSC_GRID_POINTS];
    HI_FLOAT dec_gain_grid_g[LSC_GRID_POINTS];
    HI_FLOAT dec_gain_grid_b[LSC_GRID_POINTS];
	ISP_LSC_CABLI_TABLE_S lsc_grid_info[2];
	HI_U16 bound_u = LSC_GRID_MAX_VALUE;
    HI_U16 bound_d = 0;
    HI_U16 light_num[2];
	HI_U16 au16Wb[2];

	for (i = 0; i < LSC_GRID_POINTS; i++)
	{
	    dec_gain_grid_r[i] = (HI_FLOAT)pstLsc->noise_control[i]/1024;
        dec_gain_grid_g[i] = (HI_FLOAT)pstLsc->noise_control[i]/1024;
        dec_gain_grid_b[i] = (HI_FLOAT)pstLsc->noise_control[i]/1024;
	}

	if (pstLsc->eLscMode == HI_SINGLE_LIGHT_MODE)
	{
	    au16Wb[0] = pstLsc->stCmosLsc.stLscParaTable[0].u32RGain;
		au16Wb[1] = pstLsc->stCmosLsc.stLscParaTable[0].u32BGain;
	}
	else
	{
		au16Wb[0] = pstLsc->au16Wbgain[0];
		au16Wb[1] = pstLsc->au16Wbgain[1];
	}

	lsc_light1 = HI_LSC_GRID_LIGHT1;
	lsc_light2 = lsc_light1;
	dist_min1 = 1<<31;
	dist_min2 = 1<<31;

	for (i = 0; i < HI_LSC_GRID_LIGHT_NUM; i++)
	{
	    dif1 = au16Wb[0] - pstLsc->stCmosLsc.stLscParaTable[i].u32RGain;
		dif2 = au16Wb[1] - pstLsc->stCmosLsc.stLscParaTable[i].u32BGain;
		dist[i] = dif1*dif1 + dif2*dif2;
		if (dist[i] < dist_min1)
		{
		    dist_min1 = dist[i];
			lsc_light1 = (HI_ISP_LSC_LIGHT_E)i;
		}
	}

	dist[(HI_S32)lsc_light1] = dist_min2;

	for (i = 0; i < HI_LSC_GRID_LIGHT_NUM; i++)
	{
	    if (dist[i] < dist_min2)
	    {
	    	dist_min2 = dist[i];
			lsc_light2= (HI_ISP_LSC_LIGHT_E)i;
	    }
	}
    
    if ( dist_min1 + dist_min2 != 0 )
    {
        w1 = (HI_FLOAT)dist_min2 / (dist_min1 + dist_min2);
	    w2 = (HI_FLOAT)dist_min1 / (dist_min1 + dist_min2);
    }else{
        w1 = 1;
        w2 = 0;
    }
	light_num[0] = lsc_light1;
	light_num[1] = lsc_light2;
   	
	if (pstLsc->eLscMode == HI_TRIPLE_LIGHT_MODE)
	{
		if(((w1 - g_f16PreWeight[0]) > 0.1) || ((w2 - g_f16PreWeight[1])>0.1))
		{
			pstLsc->bLightChaged = HI_TRUE;
			g_f16PreWeight[0] = w1;
			g_f16PreWeight[1] = w2;
		}
		else if((lsc_light1 != g_u8PreLight[0]) || (lsc_light2 != g_u8PreLight[1]))
        {
   			pstLsc->bLightChaged = HI_TRUE;
			g_u8PreLight[0] = lsc_light1;
			g_u8PreLight[1] = lsc_light2;
        }
        else
		{
			pstLsc->bLightChaged = HI_FALSE;
		}
	}

	lsc_grid_info[0] = pstLsc->stCmosLsc.stLscParaTable[light_num[0]];
	lsc_grid_info[1] = pstLsc->stCmosLsc.stLscParaTable[light_num[1]];

	for (i = 0; i < LSC_GRID_POINTS; i++)
	{
		r_gain[i] = GetWeightedGain(w1, w2, lsc_grid_info[0].au32R_Gain[i],\
			lsc_grid_info[1].au32R_Gain[i], dec_gain_grid_r[i], bound_u, bound_d);
		gr_gain[i] = GetWeightedGain(w1, w2, lsc_grid_info[0].au32Gr_Gain[i],\
			lsc_grid_info[1].au32Gr_Gain[i], dec_gain_grid_g[i], bound_u, bound_d);
		gb_gain[i] = GetWeightedGain(w1, w2, lsc_grid_info[0].au32Gb_Gain[i],\
			lsc_grid_info[1].au32Gb_Gain[i], dec_gain_grid_g[i], bound_u, bound_d);
		b_gain[i] = GetWeightedGain(w1, w2, lsc_grid_info[0].au32B_Gain[i],\
			lsc_grid_info[1].au32B_Gain[i], dec_gain_grid_b[i], bound_u, bound_d);
	}

	/* merge gr and r for write reg*/
	for (i = 0; i < LSC_GRID_POINTS; i++)
	{
	    pstLsc->grr_gain[i] = (gr_gain[i] << 13) + r_gain[i];
	}

	/* merge gb and b for write reg*/
	for (i = 0; i < LSC_GRID_POINTS; i++)
	{
	    pstLsc->gbb_gain[i] = (gb_gain[i] << 13) + b_gain[i];
	}
}

static HI_VOID LscRegsDefault(HI_VOID)
{
	HI_S32 i;
    ISP_BAYER_FORMAT_E enBayerFormat;

    enBayerFormat = hi_isp_yuv444_rggb_start_read();
	
	hi_isp_lsc_cfg_enable_write(HI_FALSE);
    hi_isp_lsc_rggb_cfg_write(enBayerFormat);
	hi_isp_lsc_win_num_write(LSC_GRID_ROWS - 2, LSC_GRID_COLS - 2);

	/* write horizontal grid info */
	for (i = 0; i < (LSC_GRID_COLS - 1)/2; i++)
	{
	    hi_isp_lsc_hwin_info_write(i, (2*60 - 1), 68);
	}

	/* write vertical grid info */
	for (i = 0; i < (LSC_GRID_ROWS - 1)/2; i++)
	{
	    hi_isp_lsc_vwin_info_write(i, (2*34 - 1), 120);
	}

	/* write gain data */
	hi_isp_lsc_grr_gain_waddr_write(0);
	for (i = 0; i < LSC_GRID_POINTS; i++)
	{
	    hi_isp_lsc_grr_gain_wdata_write(HI_ISP_LSC_DEFAULT_GAINL);
	}

	hi_isp_lsc_gbb_gain_waddr_write(0);
	for (i = 0; i < LSC_GRID_POINTS; i++)
	{
	    hi_isp_lsc_gbb_gain_wdata_write(HI_ISP_LSC_DEFAULT_GAINL);
	}
	
	return;
}

static HI_VOID LscExtRegsDefault(HI_VOID)
{
    HI_U16 i;
        
	hi_ext_system_isp_mesh_shading_updata_write(HI_FALSE);
	hi_ext_system_isp_mesh_shading_manu_mode_write(OP_TYPE_AUTO);

    for (i = 0; i < LSC_GRID_POINTS; i++)
	{
	    hi_ext_system_isp_mesh_shading_noise_control_write(i, HI_ISP_LSC_DEFAULT_GAIN);
		hi_ext_system_isp_mesh_shading_b_gain_write(i, HI_ISP_LSC_DEFAULT_GAIN);
		hi_ext_system_isp_mesh_shading_r_gain_write(i, HI_ISP_LSC_DEFAULT_GAIN);
		hi_ext_system_isp_mesh_shading_gb_gain_write(i, HI_ISP_LSC_DEFAULT_GAIN);
		hi_ext_system_isp_mesh_shading_gr_gain_write(i, HI_ISP_LSC_DEFAULT_GAIN);
	}
    return;
}


static HI_VOID LscExtRegsInitialize(ISP_DEV IspDev)
{
	ISP_LSC_S *pstLsc = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;  
	HI_U16 i;

	LSC_GET_CTX(IspDev, pstLsc);
	ISP_SensorGetDefault(IspDev, &pstSnsDft);
	/*get default calibration para from sensor*/
    
    memcpy(&pstLsc->stCmosLsc.stLscUniParaTable, &pstSnsDft->stLsc.stLscUniParaTable, sizeof(ISP_LSC_CABLI_UNI_TABLE_S));
	memcpy(&pstLsc->stCmosLsc.stLscParaTable[0], &pstSnsDft->stLsc.stLscParaTable[0], sizeof(ISP_LSC_CABLI_TABLE_S)*HI_LSC_GRID_LIGHT_NUM);

    
	pstLsc->eLscMode = HI_ISP_LSC_LIGHT_MODE;
	pstLsc->u8LscDefLight = 0;
	pstLsc->u8LscGridCols = LSC_GRID_COLS;
	pstLsc->u8LscGridRows = LSC_GRID_ROWS;
	pstLsc->enBayerFormat = hi_isp_yuv444_rggb_start_read();

	pstLsc->bLscEnable = hi_isp_lsc_cfg_enable_read();

    for ( i = 0 ; i < LSC_GRID_POINTS; i++ )
    {
        pstLsc->noise_control[i] = 1024;
    }

    memcpy(&pstLsc->delta_x, &pstSnsDft->stLsc.stLscUniParaTable.u32XGrid, sizeof(HI_U32)*((LSC_GRID_COLS-1)/2));
    memcpy(&pstLsc->delta_y, &pstSnsDft->stLsc.stLscUniParaTable.u32YGrid, sizeof(HI_U32)*((LSC_GRID_ROWS-1)/2));
    
    for ( i = 0 ; i < 8 ; i++ )
    {
        hi_ext_system_isp_mesh_shading_xgrid_write(i, pstLsc->delta_x[i]);
        hi_ext_system_isp_mesh_shading_ygrid_write(i, pstLsc->delta_y[i]);
    }
}


HI_S32 ISP_LscInit(ISP_DEV IspDev)
{
    LscRegsDefault(); 
	LscExtRegsDefault();
	LscExtRegsInitialize(IspDev);
	
    return HI_SUCCESS;
}

static HI_VOID LscReadExtRegs(ISP_DEV IspDev)
{
	ISP_LSC_S *pstLsc = HI_NULL;
	HI_U16 u16Wb_RGain, u16Wb_GGain, u16Wb_BGain;
	HI_U16 i;

	LSC_GET_CTX(IspDev, pstLsc);

	u16Wb_RGain = hi_isp_white_balance_gain_00_read();
	u16Wb_GGain = (hi_isp_white_balance_gain_01_read()+hi_isp_white_balance_gain_10_read())>>1;
	u16Wb_BGain = hi_isp_white_balance_gain_11_read();

	pstLsc->bLscCoefUpdata = hi_ext_system_isp_mesh_shading_updata_read();

	for (i = 0; i < LSC_GRID_POINTS; i++)
	{
	    pstLsc->noise_control[i] = hi_ext_system_isp_mesh_shading_noise_control_read(i);
		pstLsc->gbb_gain[i] = (hi_ext_system_isp_mesh_shading_gb_gain_read(i) << 13) + hi_ext_system_isp_mesh_shading_b_gain_read(i);
		pstLsc->grr_gain[i] = (hi_ext_system_isp_mesh_shading_gr_gain_read(i) << 13) + hi_ext_system_isp_mesh_shading_r_gain_read(i);
	}

    for ( i = 0 ; i < 8 ; i++ )
    {
        pstLsc->delta_x[i] = hi_ext_system_isp_mesh_shading_xgrid_read(i);
        pstLsc->delta_y[i] = hi_ext_system_isp_mesh_shading_ygrid_read(i);
    }
    geometricGridSize(pstLsc);
    
	pstLsc->au16Wbgain[0] = (HI_FLOAT)u16Wb_RGain / DIV_0_TO_1(u16Wb_GGain) * 256;
	pstLsc->au16Wbgain[1] = (HI_FLOAT)u16Wb_BGain / DIV_0_TO_1(u16Wb_GGain) * 256;

}

HI_S32 ISP_LscRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{	
	HI_S32 i;

	ISP_CTX_S *pstIspCtx = HI_NULL;

	ISP_LSC_S *pstLsc = HI_NULL;
    ISP_REG_CFG_S *pstRegCfg  = (ISP_REG_CFG_S *)pRegCfg;
	ISP_GET_CTX(IspDev, pstIspCtx);
	LSC_GET_CTX(IspDev, pstLsc);
    
    pstLsc->bLscEnable = hi_isp_lsc_cfg_enable_read();
    if ( pstLsc->bLscEnable == 0 )    
    {
        return HI_SUCCESS;
    }
    

	if (hi_ext_system_isp_mesh_shading_manu_mode_read())
	{
	    LscReadExtRegs(IspDev);
        
		if (pstLsc->bLscCoefUpdata == HI_TRUE)
		{
		    for (i = 0; i < LSC_GRID_POINTS; i++)
			{
	   			pstRegCfg->stLscRegCfg.grr_gain[i] = pstLsc->grr_gain[i];
				pstRegCfg->stLscRegCfg.gbb_gain[i] = pstLsc->gbb_gain[i];
			}
            for ( i = 0 ; i < 8 ; i++ )
            {
                pstRegCfg->stLscRegCfg.xgrid[i] = pstLsc->delta_x[i];
                pstRegCfg->stLscRegCfg.ygrid[i] = pstLsc->delta_y[i];
                pstRegCfg->stLscRegCfg.xinvgrid[i] = pstLsc->inv_dx[i];
                pstRegCfg->stLscRegCfg.yinvgrid[i] = pstLsc->inv_dy[i];
            }
			pstRegCfg->unKey.bit1LscCfg = 1;
			hi_ext_system_isp_mesh_shading_updata_write(HI_FALSE);

			return HI_SUCCESS;
		}
	}
    else if (pstLsc->eLscMode == HI_TRIPLE_LIGHT_MODE)
	{
    	if ((pstLsc->stCmosLsc.stLscParaTable[0].u32RGain == 0) && (pstLsc->stCmosLsc.stLscParaTable[0].u32BGain == 0))
        {
            return HI_SUCCESS;
        }
		if ((pstIspCtx->u32FrameCnt % 25) == 0)
		{
		    
			LscReadExtRegs(IspDev);
           
			ISP_Lsc_Fw(pstLsc);
		    if ((pstLsc->bLightChaged == HI_TRUE) || (pstLsc->bLscCoefUpdata == HI_TRUE))
			{
				
				for (i = 0; i < LSC_GRID_POINTS; i++)
				{

		   			pstRegCfg->stLscRegCfg.grr_gain[i] = pstLsc->grr_gain[i];
					pstRegCfg->stLscRegCfg.gbb_gain[i] = pstLsc->gbb_gain[i];

                    hi_ext_system_isp_mesh_shading_noise_control_write(i, pstLsc->noise_control[i] & 0x1fff);
					hi_ext_system_isp_mesh_shading_b_gain_write(i, pstLsc->gbb_gain[i] & 0x1fff);
					hi_ext_system_isp_mesh_shading_r_gain_write(i, pstLsc->grr_gain[i] & 0x1fff);
					hi_ext_system_isp_mesh_shading_gb_gain_write(i, (pstLsc->gbb_gain[i] >> 13) & 0x1fff);
					hi_ext_system_isp_mesh_shading_gr_gain_write(i, (pstLsc->grr_gain[i] >> 13) & 0x1fff);
				}
                for ( i = 0 ; i < 8 ; i++ )
                {
                    pstRegCfg->stLscRegCfg.xgrid[i] = pstLsc->delta_x[i];
                    pstRegCfg->stLscRegCfg.ygrid[i] = pstLsc->delta_y[i];
                    pstRegCfg->stLscRegCfg.xinvgrid[i] = pstLsc->inv_dx[i];
                    pstRegCfg->stLscRegCfg.yinvgrid[i] = pstLsc->inv_dy[i];
                }
				pstRegCfg->unKey.bit1LscCfg = 1;
                hi_ext_system_isp_mesh_shading_updata_write(HI_FALSE);
			}
		}
	}
	
	return HI_SUCCESS;
}

HI_S32 ISP_LscCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
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

HI_S32 ISP_LscExit(ISP_DEV IspDev)
{
	hi_isp_lsc_cfg_enable_write(HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterLsc(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_LSC;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_LscInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_LscRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_LscCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_LscExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

