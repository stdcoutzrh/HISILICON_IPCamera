/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_green_equalization.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/24
  Description   : 
  History       :
  1.Date        : 2013/04/24
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include <math.h>
#include "isp_alg.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_sensor.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define MAX_BLK_NUM   256
#define MAX_BLK_NUM_X 17
#define MAX_BLK_NUM_Y 15

#ifndef CLIP3
#define CLIP3(x,min,max) ((x<min) ? min : ((x>max) ? max : x))
#endif

static HI_U32 g_GeScale = MAX2(HI_MINIISP_BITDEPTH-10,0);
static HI_U32 g_au32GeIsoLut[ISP_AUTO_ISO_STENGTH_NUM] = {100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};
typedef struct hiISP_CMOS_GREEN_EQUALIZATION_S
{
    HI_U16 au16Strength[ISP_AUTO_ISO_STENGTH_NUM];     
    HI_U16 u16Threshold;                          
    HI_U8  u8Slope;                              
    HI_U8  u8Sensitivity;                         
    HI_U16 u16SensiThreshold;     /*edge detail*/

}ISP_CMOS_GREEN_EQUALIZATION_S;
typedef struct hiISP_GREEN_EQUALIZATION_S
{
    HI_BOOL bEnable;
	HI_S32 grgb_w;
    HI_S32 grgb_h;
	HI_U32 bitDepth;
	HI_U32 gDiffBuff_w;
	HI_U32 gDiffBuff_h;
	HI_U32 *pGdiffBuf;
	HI_U16 pGrAvg[MAX_BLK_NUM];
    HI_U16 pGbAvg[MAX_BLK_NUM];
	
    HI_U16 au16Strength[ISP_AUTO_ISO_STENGTH_NUM];     
    HI_U16 u16Threshold;                          
    HI_U8  u8Slope;                              
    HI_U8  u8Sensitivity;                         
    HI_U16 u16SensiThreshold;     /*edge detail*/	
} ISP_GREEN_EQUALIZATION_S;

#define Filt_NUM  8

typedef struct hiISP_GE_REG_CFG_BUFFER_S
{
    HI_U16  au16ge_th[Filt_NUM];          //u14.0
    HI_U8   au8ge_th_slop[Filt_NUM];    //u4.0
    HI_U16  au16detail_th[Filt_NUM];      //u14.0
    HI_U8   au8detail_slop[Filt_NUM];   //u4.0 
}ISP_GE_REG_CFG_BUFFER_S;

ISP_GE_REG_CFG_BUFFER_S  g_GeRegBuffer = {{0}};

ISP_CMOS_GREEN_EQUALIZATION_S g_astCmosGeCtx[ISP_MAX_DEV_NUM] = {{0}};

ISP_GREEN_EQUALIZATION_S g_astGeCtx[ISP_MAX_DEV_NUM] = {{0}};

#define GE_GET_CTX(dev, pstCtx)   pstCtx = &g_astGeCtx[dev]

#define CMOS_GE_GET_CTX(dev, pstCtx)  pstCtx = &g_astCmosGeCtx[dev]

static HI_VOID GeExtRegsDefault(HI_VOID)
{
    return;
}

static HI_VOID GeRegsDefault(HI_VOID) 
{
	hi_isp_ge_hnum_write(MAX_BLK_NUM_X);
	hi_isp_ge_vnum_write(MAX_BLK_NUM_Y);
	
 	hi_isp_ge_enable_write(HI_FALSE);   
	hi_isp_ge_aver_enable_write(HI_TRUE);
	hi_isp_ge_correct_enable_write(HI_TRUE);
	
    return;
}

static HI_VOID GeExtRegsInitialize(ISP_DEV IspDev)
{	
    HI_U32 i;
    ISP_GREEN_EQUALIZATION_S *pstGe = HI_NULL;
    GE_GET_CTX(IspDev, pstGe);	
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	ISP_CMOS_GE_S *pstCmosGe = HI_NULL;
    CMOS_GE_GET_CTX(IspDev, pstCmosGe);
	
    ISP_SensorGetDefault(IspDev, &pstSnsDft);			
   
	/*get default para from sensor*/
	memcpy(pstCmosGe, &pstSnsDft->stGe, sizeof(ISP_CMOS_GE_S));
    /* initial ext register of Ge */ 

    hi_ext_system_ge_threshold_write(pstCmosGe->u16Threshold);
    hi_ext_system_ge_slope_write    (pstCmosGe->u8Slope);
    hi_ext_system_ge_sensitivity_write   (pstCmosGe->u8Sensitivity);
    hi_ext_system_ge_sensithreshold_write(pstCmosGe->u16SensiThreshold);

	hi_ext_system_ge_enable_write(pstCmosGe->bEnable);
	
	pstGe->u16Threshold = pstCmosGe->u16Threshold;
	pstGe->u8Slope = pstCmosGe->u8Slope;
	pstGe->u8Sensitivity =pstCmosGe->u8Sensitivity;
	pstGe->u16SensiThreshold = pstCmosGe->u16SensiThreshold;
	pstGe->bEnable = pstCmosGe->bEnable;
	
    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
    {
        hi_ext_system_ge_strength_write(i, pstCmosGe->au16Strength[i]);	
		pstGe->au16Strength[i] = pstCmosGe->au16Strength[i];
    }

    return;
}

static HI_VOID GeRegsInitialize(ISP_DEV IspDev)
{    
	HI_U32 i;
	ISP_GREEN_EQUALIZATION_S *pstGe = HI_NULL;
	
    GE_GET_CTX(IspDev, pstGe);	
    
	pstGe->grgb_w     = MAX_BLK_NUM_X;
	pstGe->grgb_h     = MAX_BLK_NUM_Y;
	pstGe->bitDepth   = HI_MINIISP_BITDEPTH;
	pstGe->gDiffBuff_w= pstGe->grgb_w + 2;
	pstGe->gDiffBuff_h= pstGe->grgb_h + 2;
	
	memset(&pstGe->pGbAvg, 0, MAX_BLK_NUM*sizeof(HI_U16));
	memset(&pstGe->pGrAvg, 0, MAX_BLK_NUM*sizeof(HI_U16));

	pstGe->pGdiffBuf = (HI_U32 *)malloc(pstGe->gDiffBuff_w*pstGe->gDiffBuff_h*sizeof(HI_U32));
	if(NULL == pstGe->pGdiffBuf)
	{
		printf("hi_isp_ge_info:malloc mem for ge failure!\n");
		return;
	}
	
	memset(pstGe->pGdiffBuf,0,pstGe->gDiffBuff_w*pstGe->gDiffBuff_h*sizeof(HI_U32));

    pstGe->u16Threshold      = (1<<(pstGe->bitDepth-1));
	pstGe->u8Slope          =     (pstGe->bitDepth>>1);
	pstGe->u8Sensitivity    =     (pstGe->bitDepth>>1);
	pstGe->u16SensiThreshold = (1<<(pstGe->bitDepth-1));
	pstGe->bEnable = HI_FALSE;

    for(i=0; i<ISP_AUTO_ISO_STENGTH_NUM; i++)
    {	
	    pstGe->au16Strength[i]   = (g_au32GeIsoLut[i]<=256)?64<<g_GeScale
	                              :(g_au32GeIsoLut[i]<=720)?64<<g_GeScale
	                              :(g_au32GeIsoLut[i]<=2000)?128<<g_GeScale
								  :                     128<<g_GeScale;	
    }  			
	
    return;
}

static HI_VOID GeGetStat(ISP_DEV IspDev)
{
	HI_U32 i, aver_data;
	ISP_GREEN_EQUALIZATION_S *pstGe = HI_NULL;
    GE_GET_CTX(IspDev, pstGe);	
	hi_isp_ge_aver_raddr_write(0);
	for(i = 0; i < MAX_BLK_NUM; i++)
	{
		aver_data = hi_isp_ge_aver_rdata_read();
		pstGe->pGrAvg[i] = (aver_data & 0x0fffffff) >> 14;
		pstGe->pGbAvg[i] = (aver_data & 0x0003fff);
	}
    return;
}

static HI_S32 _linearinter (HI_S32 v, HI_S32 x0, HI_S32 x1, HI_S32 y0, HI_S32 y1)
{
    if (v <= x0)    return y0;
    if (v >= x1)    return y1;

    if(y1 >= y0)
    {
        return  (y0 + (y1-y0)*(v-x0)/(x1-x0));  // 0 protect
    }
    else
    {
        return  (y1 + (y0-y1)*(x1-v)/(x1-x0));  // 0 protect
    }
}

static HI_U8 GeGetIsoIndex(HI_U32 u32Iso)
{   
	HI_U8 u8Index;
	
    for (u8Index = 0; u8Index < ISP_AUTO_ISO_STENGTH_NUM-1; u8Index++)
    {
        if (u32Iso <= g_au32GeIsoLut[u8Index])
        {
            break;
        }
    }
	
    return u8Index;
}

static HI_U16 GeGetStrength(HI_U32 u32Iso, ISP_GREEN_EQUALIZATION_S *pstGe)
{
    HI_U8 u8Index = GeGetIsoIndex(u32Iso);
    
    if (0 == u8Index
        || (ISP_AUTO_ISO_STENGTH_NUM - 1) == u8Index)
    {
        return pstGe->au16Strength[u8Index];
    }

    return _linearinter(u32Iso, g_au32GeIsoLut[u8Index-1], g_au32GeIsoLut[u8Index], pstGe->au16Strength[u8Index-1], pstGe->au16Strength[u8Index]);
}

static HI_VOID Ge_GetRegCfg(ISP_DEV IspDev, ISP_GE_REG_CFG_S* pstGeReg, HI_U32 iso)
{
	ISP_GREEN_EQUALIZATION_S *pstGe = HI_NULL;
    GE_GET_CTX(IspDev, pstGe);
	
	HI_U32 *pGdiffT;
	HI_U32 *pGdiffL;
	HI_S32 x,y,i,j;
	HI_U32 ge_num = 0;
	HI_U32 max_g_diff = 0;
	HI_U32 isGeTh = 15; 
	HI_U32 lumaTh= 50;
	HI_U32 hKeyNum = 0;
	HI_U32 maxLuma;
	HI_U32 ge_num_th = 8;
	HI_U32 length = pstGe->grgb_w*pstGe->grgb_h;
	HI_U32 region_th;
	HI_U32 region_num;
	HI_U32 pos;
	

	HI_U16 ge_test_inp=((pstGe->u16SensiThreshold)&0x2)>>1;		
	HI_U16 ge_test_yxw=(pstGe->u16SensiThreshold)&0x1;

    HI_U8  non_print_pre_register= (pstGe->u8Sensitivity)&0x1;

	{
		isGeTh		 <<= g_GeScale; 
		lumaTh		 <<= g_GeScale; 
		ge_num_th = 8;

		//caculate gdiff
		pGdiffL = pstGe->pGdiffBuf + pstGe->gDiffBuff_w + 1;
		for(y = 0 ; y < pstGe->grgb_h; y++)
		{
			for(x = 0 ; x < pstGe->grgb_w; x++)
			{
				pos = y * pstGe->grgb_w+ x;
				maxLuma = MAX2(pstGe->pGrAvg[pos] , pstGe->pGbAvg[pos]) ;
				pGdiffL[x] = abs(pstGe->pGrAvg[pos] - pstGe->pGbAvg[pos])  ;

				if(maxLuma >  lumaTh)
				{
					hKeyNum++;
				}
			}

			pGdiffL += pstGe->gDiffBuff_w;
		}

		//change ge_num_th depand on valid blocks number
		ge_num_th = MAX2(ge_num_th * hKeyNum / length , ge_num_th/2);

		//gdiff adjustments based on the number of similar blocks around
		pGdiffT = pstGe->pGdiffBuf + pstGe->gDiffBuff_w + 1;
		for(y = 0 ; y < pstGe->grgb_h; y++)
		{
			for(x = 0 ; x < pstGe->grgb_w; x++)
			{
				pGdiffL 	= &pGdiffT[x];
				region_th = pGdiffL[0] >> 1;

				//compare with blocks around
				region_num = 0; 
				for(j = -1 ; j <= 1; j++)
				{
					for(i = -1 ; i <= 1; i++)
					{
						if(pGdiffL[j * pstGe->gDiffBuff_w + i] > region_th)
						{
							region_num++;
						}
					}
				}

				if(region_num <= 3) 
				{
					pGdiffT[x] >>= 1;
				}
			}

			pGdiffT += pstGe->gDiffBuff_w;
		}
		
		//find max gdiff and geblock number
		max_g_diff = 0;
		ge_num		= 0;
		pGdiffL 	   = pstGe->pGdiffBuf + pstGe->gDiffBuff_w + 1;
		for(y = 0 ; y < pstGe->grgb_h; y++)
		{
			for(x = 0 ; x < pstGe->grgb_w; x++)
			{
				max_g_diff = MAX2(max_g_diff, pGdiffL[x]);
			
				if(pGdiffL[x] > isGeTh)
				{
					ge_num++;
				}
			}

			pGdiffL += pstGe->gDiffBuff_w;
		}
	}


	//parameter set
	if(ge_num < ge_num_th) 
	{
		//maybe there is very small crosstalk
	    if(ge_test_yxw==0){
		pstGeReg->detail_th    = 16<<g_GeScale; 
		pstGeReg->detail_slop = pstGe->bitDepth>>1;
	    }else{
		pstGeReg->detail_th    = 8<<g_GeScale; 	   
		pstGeReg->detail_slop = (pstGe->bitDepth>>1)-1;		 
	    }	    
	    	
		pstGeReg->ge_th = MIN2(max_g_diff>>1, isGeTh);
		pstGeReg->ge_th_slop = pstGe->bitDepth>>2;
	}
	else
	{
		if(max_g_diff > (300<< g_GeScale))
		{
			//very strong cross talk
			pstGeReg->detail_th    = 512<<g_GeScale; 
			pstGeReg->detail_slop = pstGe->bitDepth;

			pstGeReg->ge_th = (HI_U32)(max_g_diff*1.414); //multiply sqrt(2) to get peak value
			pstGeReg->ge_th_slop = pstGe->bitDepth-1;
		}
		else if(max_g_diff > (200<< g_GeScale))
		{
			//very strong cross talk
		    if(ge_test_inp==0)	{
			pstGeReg->detail_th    = 256<<g_GeScale; 
		    }else{
	                pstGeReg->detail_th    =_linearinter(iso, 200,300, 256, 512)<<g_GeScale;
	            }
			pstGeReg->detail_slop =  pstGe->bitDepth-2;

			pstGeReg->ge_th = (HI_U32)(max_g_diff*1.414); //multiply sqrt(2) to get peak value
			pstGeReg->ge_th_slop =	pstGe->bitDepth-2;
		}
		else if(max_g_diff > (100<< g_GeScale))
		{
			// strong cross talk
		    if(ge_test_inp==0)	{
			pstGeReg->detail_th    = 128<<g_GeScale; 
		    }else{
	                pstGeReg->detail_th    =_linearinter(iso, 100,200, 128, 256)<<g_GeScale;
	            }
			pstGeReg->detail_slop =  pstGe->bitDepth-3;
		    if(ge_test_inp==0)	{
			pstGeReg->ge_th = (HI_U32)(max_g_diff*1.2);
		    }else{
			pstGeReg->ge_th = (HI_U32)((max_g_diff*_linearinter(iso, 100,200, 1200, 1414))/1000);
	            }
			pstGeReg->ge_th_slop =	pstGe->bitDepth-3;
		}
		else 
		{
			//cross talk
		  if(ge_test_yxw==0){	
		    if(ge_test_inp==0)	{
			pstGeReg->detail_th    = 64<<g_GeScale; 
   		    }else{
	                pstGeReg->detail_th    =_linearinter(iso, 0,100, 64, 128)<<g_GeScale;
	            }			
		  }else{
		    if(ge_test_inp==0)	{
			pstGeReg->detail_th    = 32<<g_GeScale; 		
		    }else{
	                pstGeReg->detail_th    =_linearinter(iso, 0,100, 32, 128)<<g_GeScale;
	            }			
	          }
			pstGeReg->detail_slop =  pstGe->bitDepth-5;

			pstGeReg->ge_th = (HI_U32)(max_g_diff*1.2) ;
			pstGeReg->ge_th_slop =	pstGe->bitDepth-5;
		}
	}

if(non_print_pre_register==0){	
	//printf("pre: max_g_diff=%d  detail_th=%d  detail_slop=%d  ge_th=%d ge_th_slop=%d ct_th1=%d\r\n",max_g_diff,pstGeReg->detail_th,pstGeReg->detail_slop,pstGeReg->ge_th,pstGeReg->ge_th_slop,pstGeReg->ct_th1);
} 

   //printf("max_g_diff=%d  detail_th=%d  detail_slop=%d  ge_th=%d ge_th_slop=%d \r\n",max_g_diff,pstGeReg->detail_th,pstGeReg->detail_slop,pstGeReg->ge_th,pstGeReg->ge_th_slop);

#if 0	
	{
	
		for(i=1;i<Filt_NUM;i++)
		{
		  g_GeRegBuffer.au16detail_th[i] = g_GeRegBuffer.au16detail_th[i-1];
		  g_GeRegBuffer.au8detail_slop[i] = g_GeRegBuffer.au8detail_slop[i-1];
		  g_GeRegBuffer.au16ge_th[i] =	g_GeRegBuffer.au16ge_th[i-1];
		  g_GeRegBuffer.au8ge_th_slop[i] = g_GeRegBuffer.au8ge_th_slop[i-1];
		}

		g_GeRegBuffer.au16detail_th[0] = pstGeReg->detail_th;
		g_GeRegBuffer.au8detail_slop[0] =  pstGeReg->detail_slop;
		g_GeRegBuffer.au16ge_th[0] =	pstGeReg->ge_th;
		g_GeRegBuffer.au8ge_th_slop[0] = pstGeReg->ge_th_slop;
			
		pstGeReg->detail_th = 0;
		pstGeReg->detail_slop = 0;
		pstGeReg->ge_th = 0;
		pstGeReg->ge_th_slop = 0;

		for(i=0;i<Filt_NUM;i++)
		{
			pstGeReg->detail_th = pstGeReg->detail_th + g_GeRegBuffer.au16detail_th[i];
			pstGeReg->detail_slop = pstGeReg->detail_slop + g_GeRegBuffer.au8detail_slop[i];
			pstGeReg->ge_th = pstGeReg->ge_th + g_GeRegBuffer.au16ge_th[i];
			pstGeReg->ge_th_slop = pstGeReg->ge_th_slop + g_GeRegBuffer.au8ge_th_slop[i];
		}

       
	
		pstGeReg->detail_th = pstGeReg->detail_th /Filt_NUM;
		pstGeReg->detail_slop = pstGeReg->detail_slop /Filt_NUM;
		pstGeReg->ge_th  = pstGeReg->ge_th /Filt_NUM;
		pstGeReg->ge_th_slop = pstGeReg->ge_th_slop /Filt_NUM;


	}
#endif

	//--------------------------------------------------------------------------
	//add offset
    pstGeReg->detail_th   = CLIP3((HI_S32)pstGeReg->detail_th   + (HI_S32)pstGe->u16SensiThreshold - (HI_S32)(1<<(pstGe->bitDepth-1)),0,(1<<pstGe->bitDepth)-1);        
    pstGeReg->detail_slop = CLIP3((HI_S32)pstGeReg->detail_slop + (HI_S32)pstGe->u8Sensitivity    -     (HI_S32)(pstGe->bitDepth>>1),0,    pstGe->bitDepth) ;  
    pstGeReg->ge_th       = CLIP3((HI_S32)pstGeReg->ge_th       + (HI_S32)pstGe->u16Threshold      - (HI_S32)(1<<(pstGe->bitDepth-1)),0,(1<<pstGe->bitDepth)-1);
    pstGeReg->ge_th_slop  = CLIP3((HI_S32)pstGeReg->ge_th_slop  + (HI_S32)pstGe->u8Slope          -     (HI_S32)(pstGe->bitDepth>>1),0,    pstGe->bitDepth) ;  

	pstGeReg->ct_th1 = GeGetStrength(iso, pstGe);
	pstGeReg->ge_enable = pstGe->bEnable;
	
        
	return;
}

static HI_S32 GeReadExtregs(ISP_DEV IspDev)
{
    HI_U32 i;
    ISP_GREEN_EQUALIZATION_S *pstGe = HI_NULL;
	
    GE_GET_CTX(IspDev, pstGe);	
    
    /* initial ext register of Ge */ 
    pstGe->bEnable = hi_ext_system_ge_enable_read();
	
  
    pstGe->u16Threshold = hi_ext_system_ge_threshold_read();
    pstGe->u8Slope     = hi_ext_system_ge_slope_read    ();
    pstGe->u8Sensitivity    = hi_ext_system_ge_sensitivity_read   ();
    pstGe->u16SensiThreshold = hi_ext_system_ge_sensithreshold_read();
    
    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
    {
        pstGe->au16Strength[i] = hi_ext_system_ge_strength_read(i);	
    }
    
    return 0;
}

HI_S32 ISP_GeInit(ISP_DEV IspDev)
{
    GeRegsDefault();
    GeExtRegsDefault();
    GeRegsInitialize(IspDev);
    GeExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_GeRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_REG_CFG_S *pstReg = HI_NULL;
	ISP_GET_CTX(IspDev, pstIspCtx);

	/* calculate every two interrupts */	
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }

	pstReg = (ISP_REG_CFG_S *)pRegCfg;

	if((0 == hi_ext_system_ge_enable_read()) && (0 == hi_isp_ge_enable_read()))
	{
	  pstReg->unKey.bit1GeCfg = 0;
      return HI_SUCCESS;
	}

	GeReadExtregs(IspDev);
	GeGetStat(IspDev);
	Ge_GetRegCfg(IspDev, &pstReg->stGeRegCfg, pstIspCtx->stLinkage.u32Iso);

	pstReg->unKey.bit1GeCfg = 1;

	
	
    return HI_SUCCESS;
}

HI_S32 ISP_GeCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
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

HI_S32 ISP_GeExit(ISP_DEV IspDev)
{
	ISP_GREEN_EQUALIZATION_S *pstGe = HI_NULL;
    GE_GET_CTX(IspDev, pstGe);

	hi_isp_ge_aver_enable_write(HI_FALSE);
	hi_isp_ge_correct_enable_write(HI_FALSE);
	hi_isp_ge_enable_write(HI_FALSE);   

	if (pstGe->pGdiffBuf)
	{
		free(pstGe->pGdiffBuf);
		pstGe->pGdiffBuf = HI_NULL;
	}
	
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterGe(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_GE;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_GeInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_GeRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_GeCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_GeExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */



