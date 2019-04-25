/******************************************************************************

  Copyright (C), 2001-2014, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mkp_acm.c
  Version       : Initial Draft
  Author        : y00246723
  Created       : 2014/04/24
  Last Modified :
  Description   : acm alg
  Function List :
              
  History       :
  1.Date        : 2014/04/24
    Author      : y00246723
    Modification: Created file

******************************************************************************/

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/file.h>
#include <linux/miscdevice.h>

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <asm/errno.h>

#include <asm/uaccess.h>
#include <asm/io.h>

#include "hi_common.h"
#include "hi_comm_isp.h"
#include "mkp_isp.h"
#include "mm_ext.h"
#include "acm_ext.h"
#include "isp_acm_config.h"
#include "isp_config_ex.h"

//#include "isp_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define ACM_DBG_LVL HI_DGB_ERR

#define VouBitValue(a)	(a) 

//Default ACM Look Up Table for Y S H
extern HI_S16 s16AcmLUTDeflt[3][5][7][29] ;

//Temp ACM Look Up Table
static HI_S16 s16AcmLUT[3][5][7][29] = {{{{0}}}};

//Temp ACM Lut for logic format
static HI_S16 s_s16AcmLUTDeflt0_Luma[180]={0};
static HI_S16 s_s16AcmLUTDeflt0_Sat[180] ={0};
static HI_S16 s_s16AcmLUTDeflt0_Hue[180] ={0};

static HI_S16 s_s16AcmLUTDeflt1_Luma[120]={0};
static HI_S16 s_s16AcmLUTDeflt1_Sat[120] ={0};
static HI_S16 s_s16AcmLUTDeflt1_Hue[120] ={0};

static HI_S16 s_s16AcmLUTDeflt2_Luma[135]={0};
static HI_S16 s_s16AcmLUTDeflt2_Sat[135] ={0};
static HI_S16 s_s16AcmLUTDeflt2_Hue[135] ={0};

static HI_S16 s_s16AcmLUTDeflt3_Luma[90]={0};
static HI_S16 s_s16AcmLUTDeflt3_Sat[90] ={0};
static HI_S16 s_s16AcmLUTDeflt3_Hue[90] ={0};

static HI_S16 s_s16AcmLUTDeflt4_Luma[168]={0};
static HI_S16 s_s16AcmLUTDeflt4_Sat[168] ={0};
static HI_S16 s_s16AcmLUTDeflt4_Hue[168] ={0};

static HI_S16 s_s16AcmLUTDeflt5_Luma[112]={0};
static HI_S16 s_s16AcmLUTDeflt5_Sat[112] ={0};
static HI_S16 s_s16AcmLUTDeflt5_Hue[112] ={0};

static HI_S16 s_s16AcmLUTDeflt6_Luma[126]={0};
static HI_S16 s_s16AcmLUTDeflt6_Sat[126] ={0};
static HI_S16 s_s16AcmLUTDeflt6_Hue[126] ={0};
    
static HI_S16 s_s16AcmLUTDeflt7_Luma[84]={0};
static HI_S16 s_s16AcmLUTDeflt7_Sat[84] ={0};
static HI_S16 s_s16AcmLUTDeflt7_Hue[84] ={0};

static ACM_COEF_BITARRAY_S  stArray;

static ISP_ACM_COEF_S g_stAcmCoef;

spinlock_t g_stAcmLock;

/* */
static ISP_ACM_CTX_S               g_stAcmCtx;
static HI_BOOL                     g_bInited = HI_FALSE;

#define ACM_CHECK_NULL(p) \
{\
    if(HI_NULL == (p)) { return HI_FAILURE;}\
}\

HI_S32 ISP_ACM_DRV_GetCoeff(ISP_ACM_COEF_S *pstAcmCoef);
HI_S32 ISP_ACM_DRV_SetCoeff(ISP_ACM_COEF_S *pstAcmCoef);
HI_S32 ISP_ACM_DRV_GetAttr(ISP_ACM_ATTR_S *pstAcmAttr);
HI_S32 ISP_ACM_DRV_SetAttr(ISP_ACM_ATTR_S *pstAcmAttr);



#define ACM_CHECK_INIT() \
{\
    if(HI_TRUE != g_bInited) \
    {\
        ISP_TRACE(HI_DBG_ERR, "Error acm not initialized\n");\
        return HI_DEF_ERR(HI_ID_ISP,EN_ERR_LEVEL_ERROR,EN_ERR_SYS_NOTREADY);\
    }\
}\

/*****************************************************************************
 Prototype    : HI_U32 ACM_DRV_SplitArray
 Description  : 
 Input        :   
 Output       : none
 Return Value : static 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
static HI_U32 ACM_DRV_SplitArray(const HI_S16 * pstAcmCoefArray)
{
    HI_S32 i, j, k;
    HI_S16 *pLUT0, *pLUT1, *pLUT2, *pLUT3, *pLUT4, *pLUT5, *pLUT6, *pLUT7;

    memcpy(&s16AcmLUT[0][0][0][0],pstAcmCoefArray,3*29*7*5*sizeof(HI_S16));
    
	pLUT0 = s_s16AcmLUTDeflt0_Luma; 
	pLUT1 = s_s16AcmLUTDeflt1_Luma;
	pLUT2 = s_s16AcmLUTDeflt2_Luma;
	pLUT3 = s_s16AcmLUTDeflt3_Luma;
	pLUT4 = s_s16AcmLUTDeflt4_Luma;
	pLUT5 = s_s16AcmLUTDeflt5_Luma;
	pLUT6 = s_s16AcmLUTDeflt6_Luma;
	pLUT7 = s_s16AcmLUTDeflt7_Luma;

	for(k = 0; k < 29; k++)
	{
		for(j = 0; j < 7; j++)
		{
			for(i = 0; i < 5; i++)
			{
				switch((i&1) + ((j&1)<<1) + ((k&1)<<2))//Reorder
				{
				case 0:
					*pLUT0 = s16AcmLUT[0][i][j][k]; 
					pLUT0++;
					break;						
				case 1:
					*pLUT1 = s16AcmLUT[0][i][j][k];
					pLUT1++;
					break;
				case 2:
					*pLUT2 = s16AcmLUT[0][i][j][k];
					pLUT2++;
					break;
				case 3:
					*pLUT3 = s16AcmLUT[0][i][j][k];
					pLUT3++;
					break;
				case 4:
					*pLUT4 = s16AcmLUT[0][i][j][k];
					pLUT4++;
					break;
				case 5:
					*pLUT5 = s16AcmLUT[0][i][j][k];
					pLUT5++;
					break;
				case 6:
					*pLUT6 = s16AcmLUT[0][i][j][k];
					pLUT6++;
					break;
				case 7:
					*pLUT7 = s16AcmLUT[0][i][j][k];
					pLUT7++;
					break;
				}
			}
		}
	}

	pLUT0 = s_s16AcmLUTDeflt0_Hue;
	pLUT1 = s_s16AcmLUTDeflt1_Hue;
	pLUT2 = s_s16AcmLUTDeflt2_Hue;
	pLUT3 = s_s16AcmLUTDeflt3_Hue;
	pLUT4 = s_s16AcmLUTDeflt4_Hue;
	pLUT5 = s_s16AcmLUTDeflt5_Hue;
	pLUT6 = s_s16AcmLUTDeflt6_Hue;
	pLUT7 = s_s16AcmLUTDeflt7_Hue;

	for(k = 0; k < 29; k++)
	{
		for(j = 0; j < 7; j++)
		{
			for(i = 0; i < 5; i++)
			{
				switch((i&1) + ((j&1)<<1) + ((k&1)<<2))
				{
				case 0:
					*pLUT0 = s16AcmLUT[1][i][j][k];
					pLUT0++;
					break;						
				case 1:
					*pLUT1 = s16AcmLUT[1][i][j][k];
					pLUT1++;
					break;
				case 2:
					*pLUT2 = s16AcmLUT[1][i][j][k];
					pLUT2++;
					break;
				case 3:
					*pLUT3 = s16AcmLUT[1][i][j][k];
					pLUT3++;
					break;
				case 4:
					*pLUT4 = s16AcmLUT[1][i][j][k];
					pLUT4++;
					break;
				case 5:
					*pLUT5 = s16AcmLUT[1][i][j][k];
					pLUT5++;
					break;
				case 6:
					*pLUT6 = s16AcmLUT[1][i][j][k];
					pLUT6++;
					break;
				case 7:
					*pLUT7 = s16AcmLUT[1][i][j][k];
					pLUT7++;
					break;
				}
			}
		}
	}

	pLUT0 = s_s16AcmLUTDeflt0_Sat;
	pLUT1 = s_s16AcmLUTDeflt1_Sat;
	pLUT2 = s_s16AcmLUTDeflt2_Sat;
	pLUT3 = s_s16AcmLUTDeflt3_Sat;
	pLUT4 = s_s16AcmLUTDeflt4_Sat;
	pLUT5 = s_s16AcmLUTDeflt5_Sat;
	pLUT6 = s_s16AcmLUTDeflt6_Sat;
	pLUT7 = s_s16AcmLUTDeflt7_Sat;

	for(k = 0; k < 29; k++)
	{
		for(j = 0; j < 7; j++)
		{
			for(i = 0; i < 5; i++)
			{
				switch((i&1) + ((j&1)<<1) + ((k&1)<<2))
				{
				case 0:
					*pLUT0 = s16AcmLUT[2][i][j][k];
					pLUT0++;
					break;						
				case 1:
					*pLUT1 = s16AcmLUT[2][i][j][k];
					pLUT1++;
					break;
				case 2:
					*pLUT2 = s16AcmLUT[2][i][j][k];
					pLUT2++;
					break;
				case 3:
					*pLUT3 = s16AcmLUT[2][i][j][k];
					pLUT3++;
					break;
				case 4:
					*pLUT4 = s16AcmLUT[2][i][j][k];
					pLUT4++;
					break;
				case 5:
					*pLUT5 = s16AcmLUT[2][i][j][k];
					pLUT5++;
					break;
				case 6:
					*pLUT6 = s16AcmLUT[2][i][j][k];
					pLUT6++;
					break;
				case 7:
					*pLUT7 = s16AcmLUT[2][i][j][k];
					pLUT7++;
					break;
				}
			}
		}
	}
	return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : HI_U32 ACM_DRV_TransCoefAlign
 Description  : 
 Input        :   
 Output       : none
 Return Value : static 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
static HI_U32 ACM_DRV_TransCoefAlign(const HI_S16 * pstAcmCoefArray, ACM_COEF_BITARRAY_S *pBitCoef)
{
    HI_S16 i, s16Tmp;
    ACM_PSCoef stACM_PSCoef;
    ACM_PSCoef *pACM_PSCoef = &stACM_PSCoef;
    
    ACM_DRV_SplitArray(pstAcmCoefArray);

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt0_Luma[0];
    pACM_PSCoef->ps16Sat  = &s_s16AcmLUTDeflt0_Sat[0];
    pACM_PSCoef->ps16Hue  = &s_s16AcmLUTDeflt0_Hue[0];

    for (i = 0; i < 45; i++) // 180/4
    {
        pBitCoef->stBit[i].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++); //repeat luma   
        pBitCoef->stBit[i].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++); //repeat saturation
        pBitCoef->stBit[i].bits_35 = s16Tmp;
        pBitCoef->stBit[i].bits_34 = (s16Tmp >> 5); //handle it as bit move to the right 5 bit
        
        pBitCoef->stBit[i].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++); //repeat
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i].bits_75 = s16Tmp;
        pBitCoef->stBit[i].bits_74 = s16Tmp >> 5;
        
        pBitCoef->stBit[i].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i].bits_113 = s16Tmp;
        pBitCoef->stBit[i].bits_114 = s16Tmp >> 3;
        
        pBitCoef->stBit[i].bits_12 = 0; //apply last 28bit to 0
    }
	
    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt1_Luma[0];
    pACM_PSCoef->ps16Sat  = &s_s16AcmLUTDeflt1_Sat[0];
    pACM_PSCoef->ps16Hue  = &s_s16AcmLUTDeflt1_Hue[0];
    
    for (i = 0; i < 30; i++)  //120/4
    {
        pBitCoef->stBit[i+45].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
        pBitCoef->stBit[i+45].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
        pBitCoef->stBit[i+45].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+45].bits_35 = s16Tmp;
        pBitCoef->stBit[i+45].bits_34 = (s16Tmp >> 5); 
        
        pBitCoef->stBit[i+45].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+45].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+45].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i+45].bits_75 = s16Tmp;
        pBitCoef->stBit[i+45].bits_74 = s16Tmp >> 5;
        
        pBitCoef->stBit[i+45].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+45].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+45].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+45].bits_113 = s16Tmp;
        pBitCoef->stBit[i+45].bits_114 = s16Tmp >> 3;
        
        pBitCoef->stBit[i+45].bits_12 = 0; 
    }
	
    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt2_Luma[0];
    pACM_PSCoef->ps16Sat  = &s_s16AcmLUTDeflt2_Sat[0];
    pACM_PSCoef->ps16Hue  = &s_s16AcmLUTDeflt2_Hue[0];
    
    for (i = 0; i < 34; i++) //135/4=33.75
    {
        if(i==33)  //0.75 is different from other data
        {
            pBitCoef->stBit[i+75].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
            pBitCoef->stBit[i+75].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
            pBitCoef->stBit[i+75].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+75].bits_35 = s16Tmp;
            pBitCoef->stBit[i+75].bits_34 = (s16Tmp >> 5); 
            pBitCoef->stBit[i+75].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+75].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            
            pBitCoef->stBit[i+75].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
            
            s16Tmp = 0;
            pBitCoef->stBit[i+75].bits_75 = s16Tmp;
            pBitCoef->stBit[i+75].bits_74 = s16Tmp >> 5;
            
            pBitCoef->stBit[i+75].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+75].bits_9 = 0;
            pBitCoef->stBit[i+75].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
            
            s16Tmp = 0;
            pBitCoef->stBit[i+75].bits_113 = s16Tmp;
            pBitCoef->stBit[i+75].bits_114 = s16Tmp >> 3;
        }
        else
        {
            pBitCoef->stBit[i+75].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
            pBitCoef->stBit[i+75].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
            pBitCoef->stBit[i+75].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+75].bits_35 = s16Tmp;
            pBitCoef->stBit[i+75].bits_34 = (s16Tmp >> 5); 
            
            pBitCoef->stBit[i+75].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+75].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+75].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+75].bits_75 = s16Tmp;
            pBitCoef->stBit[i+75].bits_74 = s16Tmp >> 5;
            
            pBitCoef->stBit[i+75].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+75].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+75].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+75].bits_113 = s16Tmp;
            pBitCoef->stBit[i+75].bits_114 = s16Tmp >> 3;        
        }
    pBitCoef->stBit[i+75].bits_12 = 0; 
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt3_Luma[0];
    pACM_PSCoef->ps16Sat = &s_s16AcmLUTDeflt3_Sat[0];
    pACM_PSCoef->ps16Hue = &s_s16AcmLUTDeflt3_Hue[0];

    for (i = 0; i < 23; i++)  //90/4=22.5
    {
        if(i==22)
        {
            pBitCoef->stBit[i+109].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+109].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
            pBitCoef->stBit[i+109].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+109].bits_35 = s16Tmp;
            pBitCoef->stBit[i+109].bits_34 = (s16Tmp >> 5);
            
            pBitCoef->stBit[i+109].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+109].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            /*		
            pBitCoef->stBit[i+109].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+109].bits_75 = s16Tmp;
            pBitCoef->stBit[i+109].bits_74 = s16Tmp >> 5;
            
            pBitCoef->stBit[i+109].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+109].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+109].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+109].bits_113 = s16Tmp;
            pBitCoef->stBit[i+109].bits_114 = s16Tmp >> 3;
            */
        }
        else
        {
            pBitCoef->stBit[i+109].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
            pBitCoef->stBit[i+109].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
            pBitCoef->stBit[i+109].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+109].bits_35 = s16Tmp;
            pBitCoef->stBit[i+109].bits_34 = (s16Tmp >> 5); 
            
            pBitCoef->stBit[i+109].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+109].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+109].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+109].bits_75 = s16Tmp;
            pBitCoef->stBit[i+109].bits_74 = s16Tmp >> 5;
            
            pBitCoef->stBit[i+109].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+109].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+109].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+109].bits_113 = s16Tmp;
            pBitCoef->stBit[i+109].bits_114 = s16Tmp >> 3;
        }
        pBitCoef->stBit[i+109].bits_12 = 0; 
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt4_Luma[0];
    pACM_PSCoef->ps16Sat = &s_s16AcmLUTDeflt4_Sat[0];
    pACM_PSCoef->ps16Hue = &s_s16AcmLUTDeflt4_Hue[0];

    for (i = 0; i < 42; i++)   //168/4=42
    {
        pBitCoef->stBit[i+132].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
        pBitCoef->stBit[i+132].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
        pBitCoef->stBit[i+132].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+132].bits_35 = s16Tmp;
        pBitCoef->stBit[i+132].bits_34 = (s16Tmp >> 5); 
        
        pBitCoef->stBit[i+132].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+132].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+132].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i+132].bits_75 = s16Tmp;
        pBitCoef->stBit[i+132].bits_74 = s16Tmp >> 5;
        
        pBitCoef->stBit[i+132].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+132].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+132].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+132].bits_113 = s16Tmp;
        pBitCoef->stBit[i+132].bits_114 = s16Tmp >> 3;
        
        pBitCoef->stBit[i+132].bits_12 = 0; 
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt5_Luma[0];
    pACM_PSCoef->ps16Sat = &s_s16AcmLUTDeflt5_Sat[0];
    pACM_PSCoef->ps16Hue = &s_s16AcmLUTDeflt5_Hue[0];

    for (i = 0; i < 28; i++) //112/4=28
    {
        pBitCoef->stBit[i+174].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
        pBitCoef->stBit[i+174].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
        pBitCoef->stBit[i+174].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+174].bits_35 = s16Tmp;
        pBitCoef->stBit[i+174].bits_34 = (s16Tmp >> 5);
        
        pBitCoef->stBit[i+174].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+174].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+174].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i+174].bits_75 = s16Tmp;
        pBitCoef->stBit[i+174].bits_74 = s16Tmp >> 5;
        
        pBitCoef->stBit[i+174].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+174].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+174].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+174].bits_113 = s16Tmp;
        pBitCoef->stBit[i+174].bits_114 = s16Tmp >> 3;
        
        pBitCoef->stBit[i+174].bits_12 = 0;
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt6_Luma[0];
    pACM_PSCoef->ps16Sat = &s_s16AcmLUTDeflt6_Sat[0];
    pACM_PSCoef->ps16Hue = &s_s16AcmLUTDeflt6_Hue[0];

    for (i = 0; i < 32; i++)   //126/4=31.5
    {
        if(i==31)
        {
            pBitCoef->stBit[i+202].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+202].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
            pBitCoef->stBit[i+202].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+202].bits_35 = s16Tmp;
            pBitCoef->stBit[i+202].bits_34 = (s16Tmp >> 5);
            pBitCoef->stBit[i+202].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+202].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            /*
            pBitCoef->stBit[i+202].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+202].bits_75 = s16Tmp;
            pBitCoef->stBit[i+202].bits_74 = s16Tmp >> 5;
            
            pBitCoef->stBit[i+202].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+202].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+202].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+202].bits_113 = s16Tmp;
            pBitCoef->stBit[i+202].bits_114 = s16Tmp >> 3;
            */
        }
        else
        {
            pBitCoef->stBit[i+202].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+202].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
            pBitCoef->stBit[i+202].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+202].bits_35 = s16Tmp;
            pBitCoef->stBit[i+202].bits_34 = (s16Tmp >> 5);
            
            pBitCoef->stBit[i+202].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+202].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+202].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+202].bits_75 = s16Tmp;
            pBitCoef->stBit[i+202].bits_74 = s16Tmp >> 5;
            
            pBitCoef->stBit[i+202].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+202].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+202].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+202].bits_113 = s16Tmp;
            pBitCoef->stBit[i+202].bits_114 = s16Tmp >> 3;
        }
        pBitCoef->stBit[i+202].bits_12 = 0;
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt7_Luma[0];
    pACM_PSCoef->ps16Sat =  &s_s16AcmLUTDeflt7_Sat[0];
    pACM_PSCoef->ps16Hue =  &s_s16AcmLUTDeflt7_Hue[0];

    for (i = 0; i < 21; i++)   //84/4=21
    {
        pBitCoef->stBit[i+234].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i+234].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
        pBitCoef->stBit[i+234].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+234].bits_35 = s16Tmp;
        pBitCoef->stBit[i+234].bits_34 = (s16Tmp >> 5);
        
        pBitCoef->stBit[i+234].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+234].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+234].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i+234].bits_75 = s16Tmp;
        pBitCoef->stBit[i+234].bits_74 = s16Tmp >> 5;
        
        pBitCoef->stBit[i+234].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+234].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+234].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+234].bits_113 = s16Tmp;
        pBitCoef->stBit[i+234].bits_114 = s16Tmp >> 3;
        
        pBitCoef->stBit[i+234].bits_12 = 0;
    }

    pBitCoef->u32Size = COEFACMCNT * sizeof(ACM_COEF_BIT_S); 
    
    return HI_SUCCESS;
}

HI_VOID* ACM_DRV_GetLutAddr(const ISP_ACM_LUTS_S* pstAcmCoef,ISP_ACM_MODE_E enAcmMode)
{
    HI_VOID* pAcmCoefAddr = HI_NULL;
    switch(enAcmMode)
    {
        case ISP_ACM_MODE_BLUE:
            pAcmCoefAddr = (HI_VOID*)&pstAcmCoef->stPqAcmModeBlue;
            break;
        case ISP_ACM_MODE_GREEN:
            pAcmCoefAddr = (HI_VOID*)&pstAcmCoef->stPqAcmModeGreen;
            break;
        case ISP_ACM_MODE_BG:
            pAcmCoefAddr = (HI_VOID*)&pstAcmCoef->stPqAcmModeBG;
            break;
        case ISP_ACM_MODE_SKIN:
            pAcmCoefAddr = (HI_VOID*)&pstAcmCoef->stPqAcmModeSkin;
            break;
        case ISP_ACM_MODE_VIVID:
            pAcmCoefAddr = (HI_VOID*)&pstAcmCoef->stPqAcmModeVivid;
            break;
        default:
            pAcmCoefAddr = (HI_VOID*)&pstAcmCoef->stPqAcmModeVivid;
            break;
    }
    return pAcmCoefAddr;
}

/*****************************************************************************
 Prototype    : ACM_DRV_TransCoef
 Description  : Write ACM Lut to Ram
 Input        :   
 Output       : none
 Return Value : HI_VOID 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_VOID ACM_DRV_TransCoef(HI_VOID* pCoefMem, const ISP_ACM_LUTS_S* pstAcmCoef, ISP_ACM_MODE_E enAcmMode)
{
    HI_S16* ps16AcmCoef = HI_NULL;
    memset(&stArray,0,sizeof(stArray));
    stArray.u32Size = 255*16;

    ps16AcmCoef = (HI_S16*)ACM_DRV_GetLutAddr(pstAcmCoef,enAcmMode);
    HI_ASSERT(ps16AcmCoef != HI_NULL);

    HI_ASSERT(pCoefMem != HI_NULL);
    
    ACM_DRV_TransCoefAlign(ps16AcmCoef, &stArray); //transform the table to struct
    memcpy((HI_U8*)pCoefMem, stArray.stBit, stArray.u32Size); //memcpy copy to suppositional address
    
}

/*****************************************************************************
 Prototype    : ACM_DRV_UpdateRegister
 Description  : 
 Input        :   
 Output       : none
 Return Value : HI_VOID 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_VOID ACM_DRV_UpdateRegister(HI_U32 u32AcmCoefAddr)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
    unsigned long u32Flags;

    HI_ASSERT(u32AcmCoefAddr != 0);

    spin_lock_irqsave(&g_stAcmLock, u32Flags);

    hi_isp_acm_enable_write((HI_BOOL)pstAcmCtx->stAcmCtrl.bEnable);
    hi_isp_acm_dbg_enable_write((HI_BOOL)pstAcmCtx->stAcmCtrl.bDemoEnable);
    //hi_isp_acm_dbg_enable_write(HI_FALSE);
    hi_isp_acm_luma_gain_write(pstAcmCtx->stAcmCtrl.u32GainLuma);
    hi_isp_acm_sat_gain_write(pstAcmCtx->stAcmCtrl.u32GainSat);
    hi_isp_acm_hue_gain_write(pstAcmCtx->stAcmCtrl.u32GainHue);
    hi_isp_acm_cbcrthr_write(pstAcmCtx->stAcmCtrl.u32CbcrThr);
    hi_isp_acm_cliprange_write(pstAcmCtx->stAcmCtrl.u32ClipRange);
    hi_isp_acm_stretch_write(pstAcmCtx->stAcmCtrl.u32Stretch);
    hi_isp_acm_cliporwrap_write(pstAcmCtx->stAcmCtrl.u32AcmClipOrWrap);
	
    hi_isp_acm_coeff_addr_write(u32AcmCoefAddr);
    /* In chip power up, there maybe a wrong interrput!!! */
    hi_isp_acm_coeff_up_write(1);

    spin_unlock_irqrestore(&g_stAcmLock, u32Flags);
}

/*****************************************************************************
 Prototype    : ACM_DRV_InitDefault
 Description  : 
 Input        :   
 Output       : none
 Return Value : HI_VOID 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ACM_DRV_InitDefault(ISP_ACM_CTX_S* pstAcmCtx)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32AcmCoefSize; 
    
    if(down_interruptible(&pstAcmCtx->stAcmSem))
    {
        return -ERESTARTSYS;
    }

    pstAcmCtx->stAcmCtrl.bEnable = HI_FALSE;
    pstAcmCtx->stAcmCtrl.bDemoEnable = HI_FALSE;
    pstAcmCtx->stAcmCtrl.u32GainLuma = 64;
    pstAcmCtx->stAcmCtrl.u32GainSat = 64;
    pstAcmCtx->stAcmCtrl.u32GainHue = 64;
    
    pstAcmCtx->stAcmCtrl.u32CbcrThr = 0;
    pstAcmCtx->stAcmCtrl.u32ClipRange = 1;
    pstAcmCtx->stAcmCtrl.u32Stretch = 1;
    pstAcmCtx->stAcmCtrl.u32AcmClipOrWrap = 0;
    
    pstAcmCtx->stAcmCtrl.enMode = ISP_ACM_MODE_VIVID;
    
    s32AcmCoefSize = sizeof(HI_S16)*LUTDEFNUM;
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeBlue.as16Y) ,&s16AcmLUTDeflt[0][0][0][0],s32AcmCoefSize);
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeBlue.as16H) ,&s16AcmLUTDeflt[1][0][0][0],s32AcmCoefSize);
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeBlue.as16S) ,&s16AcmLUTDeflt[2][0][0][0],s32AcmCoefSize);
    
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeGreen.as16Y) ,&s16AcmLUTDeflt[0][0][0][0],s32AcmCoefSize);
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeGreen.as16H) ,&s16AcmLUTDeflt[1][0][0][0],s32AcmCoefSize);
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeGreen.as16S) ,&s16AcmLUTDeflt[2][0][0][0],s32AcmCoefSize);
    
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeBG.as16Y) ,&s16AcmLUTDeflt[0][0][0][0],s32AcmCoefSize);
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeBG.as16H) ,&s16AcmLUTDeflt[1][0][0][0],s32AcmCoefSize);
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeBG.as16S) ,&s16AcmLUTDeflt[2][0][0][0],s32AcmCoefSize);
    
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeSkin.as16Y) ,&s16AcmLUTDeflt[0][0][0][0],s32AcmCoefSize);
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeSkin.as16H) ,&s16AcmLUTDeflt[1][0][0][0],s32AcmCoefSize);
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeSkin.as16S) ,&s16AcmLUTDeflt[2][0][0][0],s32AcmCoefSize);
    
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeVivid.as16Y) ,&s16AcmLUTDeflt[0][0][0][0],s32AcmCoefSize);
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeVivid.as16H) ,&s16AcmLUTDeflt[1][0][0][0],s32AcmCoefSize);
    memcpy(&(pstAcmCtx->pstCoefLut->stPqAcmModeVivid.as16S) ,&s16AcmLUTDeflt[2][0][0][0],s32AcmCoefSize);

    ACM_DRV_TransCoef((HI_VOID*)pstAcmCtx->stCoefBuff.pVirAddr, pstAcmCtx->pstCoefLut, pstAcmCtx->stAcmCtrl.enMode);        
    
    up(&pstAcmCtx->stAcmSem);
    
    ACM_DRV_UpdateRegister(pstAcmCtx->stCoefBuff.u32PhyAddr);
    
    return s32Ret;
}

/*****************************************************************************
 Prototype    : ISP_ACM_SetAttr
 Description  : Set ACM attribute
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_SetAttr(ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
    ISP_ACM_MODE_E enOldMode;

    ACM_CHECK_NULL(pstAcmAttr);
    ACM_CHECK_INIT();

    if(down_interruptible(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }

    enOldMode = pstAcmCtx->stAcmCtrl.enMode;
    memcpy((HI_VOID*)&pstAcmCtx->stAcmCtrl, (HI_VOID*)pstAcmAttr, sizeof(ISP_ACM_ATTR_S));
    
    HI_ASSERT(pstAcmCtx->stCoefBuff.pVirAddr != HI_NULL);
    if(enOldMode != pstAcmCtx->stAcmCtrl.enMode)
    {
        ACM_DRV_TransCoef((HI_VOID*)pstAcmCtx->stCoefBuff.pVirAddr, pstAcmCtx->pstCoefLut, pstAcmCtx->stAcmCtrl.enMode);    
    }
    up(&pstAcmCtx->stAcmSem);

    ACM_DRV_UpdateRegister(pstAcmCtx->stCoefBuff.u32PhyAddr);


    return HI_SUCCESS;
}

HI_S32 ISP_ACM_Status_Update(HI_VOID)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
    ACM_CHECK_INIT();
    
    pstAcmCtx->stAcmCtrl.bEnable = hi_isp_acm_enable_read();
    pstAcmCtx->stAcmCtrl.bDemoEnable = hi_isp_acm_dbg_enable_read();
    pstAcmCtx->stAcmCtrl.u32GainLuma = hi_isp_acm_luma_gain_read();
    pstAcmCtx->stAcmCtrl.u32GainSat = hi_isp_acm_sat_gain_read();
    pstAcmCtx->stAcmCtrl.u32GainHue = hi_isp_acm_hue_gain_read();
    pstAcmCtx->stAcmCtrl.u32CbcrThr = hi_isp_acm_cbcrthr_read();
    pstAcmCtx->stAcmCtrl.u32ClipRange = hi_isp_acm_cliprange_read();
    pstAcmCtx->stAcmCtrl.u32Stretch = hi_isp_acm_stretch_read();
    pstAcmCtx->stAcmCtrl.u32AcmClipOrWrap =  hi_isp_acm_cliporwrap_read();
    
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_ACM_GetAttr
 Description  : Get ACM attribute
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_GetAttr(ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;

    ACM_CHECK_NULL(pstAcmAttr);
    ACM_CHECK_INIT();
    ISP_ACM_Status_Update();
	
    if(down_interruptible(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }
    memcpy((HI_VOID*)pstAcmAttr, (HI_VOID*)&pstAcmCtx->stAcmCtrl, sizeof(ISP_ACM_ATTR_S));
    up(&pstAcmCtx->stAcmSem);
    
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_ACM_SetCoeff
 Description  : Set ACM coeff
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_SetCoeff(ISP_ACM_COEF_S *pstAcmCoef)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
    HI_VOID* pCoefAddr = HI_NULL;
    ACM_CHECK_NULL(pstAcmCoef);
    ACM_CHECK_INIT();

    if(down_interruptible(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }

    pCoefAddr = ACM_DRV_GetLutAddr(pstAcmCtx->pstCoefLut,pstAcmCoef->enMode);
    HI_ASSERT(pCoefAddr != HI_NULL);
    memcpy((HI_VOID*)pCoefAddr, (HI_VOID*)&pstAcmCoef->stLut, sizeof(ISP_ACM_LUT_S));
    pstAcmCtx->stAcmCtrl.enMode = pstAcmCoef->enMode;
    ACM_DRV_TransCoef((HI_VOID*)pstAcmCtx->stCoefBuff.pVirAddr, pstAcmCtx->pstCoefLut, pstAcmCtx->stAcmCtrl.enMode); 
    up(&pstAcmCtx->stAcmSem);
    ACM_DRV_UpdateRegister(pstAcmCtx->stCoefBuff.u32PhyAddr);

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_ACM_GetCoeff
 Description  : Get ACM Coeff
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_GetCoeff(ISP_ACM_COEF_S *pstAcmCoef)
{
    HI_VOID* pCoefAddr = HI_NULL;
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;

    ACM_CHECK_NULL(pstAcmCoef);
    ACM_CHECK_INIT();

    if(down_interruptible(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }
    pCoefAddr = ACM_DRV_GetLutAddr(pstAcmCtx->pstCoefLut,pstAcmCoef->enMode);
    HI_ASSERT(pCoefAddr != HI_NULL);

    memcpy((HI_VOID*)&pstAcmCoef->stLut, (HI_VOID*)pCoefAddr, sizeof(ISP_ACM_LUT_S));
    up(&pstAcmCtx->stAcmSem);

    return HI_SUCCESS;
}

/********************* INTERFACE****************************/

/*****************************************************************************
 Prototype    : ISP_AcmCtrl
 Description  : ACM interface Ctrl
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_Ioctrl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int __user *argp = (unsigned int __user *)arg;
    ISP_DEV IspDev;

    ACM_CHECK_INIT();
    IspDev = ISP_GET_DEV(file);

    switch (cmd)
    {
        case ISP_ACM_ATTR_GET:
        {
            ISP_ACM_ATTR_S       stAcmAttr;

            ISP_ACM_DRV_GetAttr(&stAcmAttr);

            if (copy_to_user(argp, &stAcmAttr, sizeof(ISP_ACM_ATTR_S)))
            {
                printk(KERN_INFO "copy proc mem to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_ACM_ATTR_SET:
        {
            ISP_ACM_ATTR_S       stTempAcmAttr;
            
            if (copy_from_user(&stTempAcmAttr, argp, sizeof(ISP_ACM_ATTR_S)))
            {
                printk(KERN_INFO "copy Acm attribute from user failed!\n");
                return -EFAULT;
            }

            ISP_ACM_DRV_SetAttr(&stTempAcmAttr);

            return 0;
        }
        case ISP_ACM_COEF_GET:
        {
            if (copy_from_user(&g_stAcmCoef, argp,  sizeof(ISP_ACM_COEF_S)))
            {
                printk(KERN_INFO "copy proc mem to user failed!\n");
                return -EFAULT;
            }
            ISP_ACM_DRV_GetCoeff(&g_stAcmCoef);
            if (copy_to_user(argp, &g_stAcmCoef, sizeof(ISP_ACM_COEF_S)))
            {
                printk(KERN_INFO "copy proc mem to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_ACM_COEF_SET:
        {
            if (copy_from_user(&g_stAcmCoef, argp, sizeof(ISP_ACM_COEF_S)))
            {
                printk(KERN_INFO "copy Acm attribute from user failed!\n");
                return -EFAULT;
            }

            ISP_ACM_DRV_SetCoeff(&g_stAcmCoef);
            return 0;
        }
        default :
            break;
    }
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_ACM_Init
 Description  : ACM ALG initialize
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_Init(HI_VOID)
{   
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;
    HI_U32 u32CoefPhyAddr;
    HI_U8  *pu8CoefVirAddr;
    HI_U32 u32Size;
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
    HI_S32 s32Ret = HI_SUCCESS;

    if(HI_TRUE == g_bInited)
    {
        ISP_TRACE(HI_DBG_ERR, "err acm reinitialized\n");
        return HI_FAILURE;
    }

    spin_lock_init(&g_stAcmLock);
    sema_init(&pstAcmCtx->stAcmSem,1);

    u32Size = sizeof(ISP_ACM_LUTS_S);
    s32Ret = CMPI_MmzMallocNocache(HI_NULL, "ACM LUTS", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc acm luts buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    u32Size = ACMADDROFFSET;
    s32Ret = CMPI_MmzMallocCached(HI_NULL, "ACM Coef", &u32CoefPhyAddr, (HI_VOID**)&pu8CoefVirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc acm coef buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    pstAcmCtx->stMmzBuff.pVirAddr   = pu8VirAddr;
    pstAcmCtx->stMmzBuff.u32PhyAddr = u32PhyAddr;
    pstAcmCtx->pstCoefLut           = (ISP_ACM_LUTS_S*)(pu8VirAddr);
    pstAcmCtx->stCoefBuff.u32PhyAddr= u32CoefPhyAddr;
    pstAcmCtx->stCoefBuff.pVirAddr  = pu8CoefVirAddr;
    
    ACM_DRV_InitDefault(pstAcmCtx);

    g_bInited = HI_TRUE;
    
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_ACM_Exit
 Description  : ACM interface Exit
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_Exit(HI_VOID)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
    unsigned long u32Flags;

    HI_U32 u32PhyAddr; 
    HI_U8* pu8VirAddr; 
    HI_U32 u32CoefPhyAddr; 
    HI_U8* pu8CoefVirAddr; 

    ACM_CHECK_INIT();
    g_bInited = HI_FALSE;

    u32PhyAddr = pstAcmCtx->stMmzBuff.u32PhyAddr;
    pu8VirAddr = (HI_U8 *)pstAcmCtx->stMmzBuff.pVirAddr;
    u32CoefPhyAddr = pstAcmCtx->stCoefBuff.u32PhyAddr;
    pu8CoefVirAddr = (HI_U8 *)pstAcmCtx->stCoefBuff.pVirAddr;


    spin_lock_irqsave(&g_stAcmLock, u32Flags);
    hi_vi_acm_write_coeff_addr(0);
    spin_unlock_irqrestore(&g_stAcmLock, u32Flags);

    if(down_interruptible(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }
    pstAcmCtx->stMmzBuff.u32PhyAddr = 0;
    pstAcmCtx->stMmzBuff.pVirAddr   = HI_NULL;
    pstAcmCtx->stCoefBuff.u32PhyAddr = 0;
    pstAcmCtx->stCoefBuff.pVirAddr   = HI_NULL;
    up(&pstAcmCtx->stAcmSem);

    if (0 != u32PhyAddr)
    {
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

    if (0 != u32CoefPhyAddr)
    {
        CMPI_MmzFree(u32CoefPhyAddr, pu8CoefVirAddr);
    }
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

