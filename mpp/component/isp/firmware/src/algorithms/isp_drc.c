/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_drc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include <math.h>
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define DRC_Y_SFT1 (0)
#define DRC_Y_VAL1 (127)
#define DRC_Y_SFT2 (0)
#define DRC_Y_VAL2 (0)

#define DRC_C_SFT1 (0)
#define DRC_C_VAL1 (127)
#define DRC_C_SFT2 (0)
#define DRC_C_VAL2 (0)

#define DRC_B_SFT1 (0)
#define DRC_B_VAL1 (127)

#define DRC_ASYMMETRY  (0x1)
#define DRC_SECONDPOLE (192)
#define DRC_STRETCH    (60)

#define DRC_H_BLK_NUM  (36)
#define DRC_V_BLK_NUM  (21)

#define DRC_BIN_NUM_Z  (8)

#define DRC_LOCAL_EDGELMT  (155)

#define DRC_STRLENGTH_LINEAR     (128)
#define DRC_STRLENGTH_WDR         (255)

#define LUT_MAX_NODE       (257)


#define DRC_K1   (1232567)
#define DRC_G1   (8213)
#define DRC_F2   (2250000)
#define DRC_G2   (4169)

#define LUT_MIX_CTRL (255)
#define DETAIL_MIXING_BRIGHT (54)
#define DETAIL_MIXING_DARK (32)
#define DETAIL_MIXING_THRES (2)

#define VARSPATIAL  (10)
#define VARRANGE  (0)
#define RANGE_FLT_COEF (4)
#define GAIN_CLIP_KNEE (0xF)
#define GAIN_CLIP_STEP (0xF)

#define DRC_STRLENGTH_TARGET_LINEAR (0x80)
#define DRC_STRLENGTH_TARGET_WDR    (0xff)

#define BRIGHT_GAIN_LMT (0x0)
#define DARK_GAIN_LMT_C (0x0)
#define DARK_GAIN_LMT_Y (0x0)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#define DRC_CLIP1(high, x) (MAX( MIN((x), high), 0))
#define DRC_CLIP3(low, high, x) (MAX( MIN((x), high), low))


/***********************************************************/

typedef struct hiDRC_HIST_WEIGHT_CALC_S
{
	HI_U16 au8IndexTh[7];
	HI_U16 au8Weight[7];
	
} DRC_HIST_WEIGHT_CALC_S;


typedef struct hiISP_DRC_HIST_S
{
    HI_S32  s32HistDark;
    HI_U16  u16ExpRatio;
	
	DRC_HIST_WEIGHT_CALC_S pstHistWeightCalc;
	
} ISP_DRC_HIST_S;
HI_U16 au16DarkGainLmt[134] = {	0x007f,0x8416,0x8415,0x8415,0x8414,0x8414,0x8413,0x8413,0x8412,0x8412,0x8411,0x8411,0x8410,0x8410,0x840f,0x840f,0x840e,0x840e,0x840d,
								0x840d,0x840c,0x840c,0x840b,0x840b,0x8494,0x8494,0x8493,0x8493,0x8492,0x8492,0x8491,0x8491,0x8490,0x8490,0x848f,0x848f,0x851d,0x851c,
								0x851c,0x851b,0x851b,0x851a,0x851a,0x8519,0x8519,0x8518,0x8518,0x8517,0x8517,0x8516,0x8516,0x8515,0x8515,0x85a8,0x85a8,0x85a7,0x85a7,
								0x85a6,0x85a6,0x85a5,0x85a5,0x85a4,0x85a4,0x85a3,0x85a3,0x85a2,0x85a2,0x85a1,0x85a1,0x85a0,0x85a0,0x859f,0x859f,0x859e,0x859e,0x859d,
								0x859d,0x8638,0x8637,0x8636,0x8635,0x8634,0x8633,0x8632,0x8631,0x8630,0x862f,0x862e,0x862d,0x862c,0x862b,0x862a,0x8629,0x86d0,0x86ce,
								0x86cc,0x86ca,0x86c8,0x86c6,0x86c4,0x86c2,0x86c0,0x86be,0x86bc,0x86ba,0x8771,0x876d,0x8769,0x8765,0x8761,0x875d,0x8759,0x8755,0x8751,
								0x874d,0x8749,0x8745,0x8741,0x873d,0x8739,0x8735,0x8731,0x872d,0x8729,0x8725,0x8721,0x871d,0x8719,0x8715,0x8711,0x870d,0x8709,0x8705,
								0x8701
					  		  };

HI_U16 au16BrightGainLmt[161]= {	0x007f,0x0017,0x0017,0x0016,0x0016,0x0015,0x0015,0x0014,0x0014,0x0013,0x0013,0x0012,0x0012,0x0011,0x0011,0x0010,0x0010,0x000f,0x000f,
								0x000e,0x000e,0x000d,0x000d,0x000c,0x000c,0x000b,0x000b,0x0094,0x0094,0x0093,0x0093,0x0092,0x0092,0x0091,0x0091,0x0090,0x0090,0x008f,
								0x008f,0x011d,0x011c,0x011c,0x011b,0x011b,0x011a,0x011a,0x0119,0x0119,0x0118,0x0118,0x0117,0x0117,0x0116,0x0116,0x0115,0x0115,0x01a8,
								0x01a8,0x01a7,0x01a7,0x01a6,0x01a6,0x01a5,0x01a5,0x01a4,0x01a4,0x01a3,0x01a3,0x01a2,0x01a2,0x01a1,0x01a1,0x01a0,0x01a0,0x019f,0x019f,
								0x019e,0x019e,0x019d,0x019d,0x0239,0x0238,0x0238,0x0237,0x0237,0x0236,0x0236,0x0235,0x0235,0x0234,0x0234,0x0233,0x0233,0x0232,0x0232,
								0x0231,0x0231,0x0230,0x0230,0x022f,0x022f,0x022e,0x022e,0x022d,0x022d,0x022c,0x022c,0x022b,0x022b,0x022a,0x022a,0x0229,0x0229,0x02d1,
								0x02d0,0x02d0,0x02cf,0x02cf,0x02ce,0x02ce,0x02cd,0x02cd,0x02cc,0x02cc,0x02cb,0x02cb,0x02ca,0x02ca,0x02c9,0x02c9,0x02c8,0x02c8,0x02c7,
								0x02c7,0x02c6,0x02c6,0x02c5,0x02c5,0x02c4,0x02c4,0x02c3,0x02c3,0x02c2,0x02c2,0x02c1,0x02c1,0x02c0,0x02c0,0x02bf,0x02bf,0x02be,0x02be,
								0x02bd,0x02bd,0x02bc,0x02bc,0x02bb,0x02bb,0x02ba,0x02ba,0x02b9
							   };


/***********************************************************/


typedef struct hiISP_DRC_S
{
	HI_BOOL bEnable;
	HI_BOOL bDrcDitherEn;
	
	HI_BOOL bUpdateLut;
    
	HI_U8   u8Asymmetry;
	HI_U8   u8SecondPole;
	HI_U8   u8Stretch;
    
    HI_U8   u8SpatialVar;             
	HI_U8   u8RangeVar;               
	
	HI_U8   u8LocalMixingBrigtht;     
	HI_U8   u8LocalMixingDark;        
	HI_U8   u8LocalMixingThres;       

	HI_U16  u16DarkGainLmtY;          
	HI_U16  u16DarkGainLmtC;          
	HI_U16  u16BrightGainLmt;         

	HI_U16 ToneMappingValue0[200];
    HI_U16 ToneMappingDiff0[200];

    HI_U16 ToneMappingValue1[200];
    HI_U16 ToneMappingDiff1[200];
	
    HI_DOUBLE LutValue0[201];
    HI_DOUBLE LutValue1[201];    
    HI_U32    K1;
    HI_U32    G1;
    HI_U32    F2;
    HI_U32    G2;
	
	HI_U32    u32Strength;
	HI_U8     u8StrengthTarget;
	HI_U8     u8BinNumZ;
    HI_U16    au16ColorCorrectionLut[33];
    
    ISP_DRC_HIST_S   stHist;
	
} ISP_DRC_S;

ISP_DRC_S g_astDrcCtx[ISP_MAX_DEV_NUM] = {{0}};
#define DRC_GET_CTX(dev, pstCtx)   pstCtx = &g_astDrcCtx[dev]

static HI_DOUBLE g_DrcLutPoint[201] = {
	1.00001525902190	,0.994614600098015	,0.989243107845890	,0.983900624748680	,0.978586994140223, 
	0.973302060200447	,0.968045667950797	,0.962817663249692	,0.957617892788002	,0.952446204084559, 
	0.947302445481677	,0.942186466140712	,0.937098116037634	,0.932037245958629	,0.927003707495726, 
	0.921997353042439	,0.917018035789446	,0.912065609720277	,0.907139929607036	,0.902240851006141, 
	0.897368230254090	,0.892521924463247	,0.887701791517648	,0.882907690068841	,0.878139479531735, 
	0.873397020080482	,0.868680172644370	,0.863988798903754	,0.859322761285990	,0.854681922961409, 
	0.850066147839301	,0.845475300563922	,0.840909246510528	,0.836367851781427	,0.831850983202052, 
	0.827358508317055	,0.822890295386421	,0.818446213381611	,0.814026131981713	,0.809629921569624, 
	0.805257453228247	,0.800908598736713	,0.796583230566619	,0.792281221878290	,0.788002446517055, 
	0.783746779009556	,0.779514094560059	,0.775304269046801	,0.771117179018348	,0.766952701689974, 
	0.762810714940063	,0.758691097306525	,0.754593727983235	,0.750518486816491	,0.746465254301490, 
	0.742433911578826	,0.738424340430999	,0.734436423278954	,0.730470043178629	,0.726525083817529, 
	0.722601429511313	,0.718698965200401	,0.714817576446602	,0.710957149429758	,0.707117570944405, 
	0.699500509799884	,0.691965499537512	,0.684511656306270	,0.677138105775969	,0.669843983034693, 
	0.662628432487349	,0.655490607755298	,0.648429671577085	,0.641444795710222	,0.634535160834039, 
	0.627699956453576	,0.620938380804516	,0.614249640759135	,0.607632951733272	,0.601087537594296, 
	0.594612630570064	,0.588207471158868	,0.581871308040340	,0.575603397987325	,0.569403005778703, 
	0.563269404113144	,0.557201873523800	,0.551199702293909	,0.545262186373315	,0.539388629295877, 
	0.533578342097782	,0.527830643236725	,0.522144858511963	,0.516520320985239	,0.510956370902542, 
	0.505452355616726	,0.500007629510948	,0.494621553922945	,0.489293497070112	,0.484022833975399, 
	0.478808946394001	,0.473651222740838	,0.468549058018817	,0.463501853747863	,0.458509017894723, 
	0.453569964803518	,0.448684115127045	,0.443850895758824	,0.439069739765868	,0.434340086322185, 
	0.429661380642995	,0.425033073919650	,0.420454623255264	,0.415925491601026	,0.411445147693211, 
	0.407013065990857	,0.402628726614123	,0.398291615283310	,0.394001223258528	,0.389757047280029, 
	0.385558589509174	,0.381405357470032	,0.377296863991617	,0.373232627150745	,0.369212170215499, 
	0.365235021589314	,0.361300714755656	,0.357408788223301	,0.353558785472202	,0.345982749768756, 
	0.338569052887984	,0.331314216243674	,0.324214835788543	,0.317267580417019	,0.310469190402258, 
	0.303816475866636	,0.297306315285032	,0.290935654020170	,0.284701502889351	,0.278600936761900, 
	0.272631093186657	,0.266789171048891	,0.261072429255982	,0.255478185451271	,0.250003814755474, 
	0.244646748535056	,0.239404473197001	,0.234274529009408	,0.229254508947361	,0.224342057563523, 
	0.219534869882934	,0.214830690321498	,0.210227311627632	,0.205722573846605	,0.201314363307062, 
	0.197000611629264	,0.192779294754587	,0.188648431995809	,0.184606085107750	,0.180650357377828, 
	0.176779392736101	,0.169284526443992	,0.162107417894271	,0.155234595201129	,0.148653157642516, 
	0.142350751444676	,0.136315546593329	,0.130536214627991	,0.125001907377737	,0.119702236598500, 
	0.114627254473681	,0.109767434941467	,0.105113655813816	,0.100657181653531	,0.0963896473772935,	
	0.0923030425538749	,0.0883896963680506 ,0.0810537089471357 ,0.0743265788212580 ,0.0681577732966643,	
	0.0625009536888685	,0.0573136272368404 ,0.0525568279069080 ,0.0481948236886467 ,0.0441948481840253,	
	0.0371632894106290	,0.0312504768444343 ,0.0262784139534540 ,0.0220974240920127 ,0.0156252384222171,	
	0.0110487120460063	,0.00781261921110857	,0.00390630960555428	,0.00195315480277714	,0.000976577401388571,	
	0.000488288700694286,0.000244144350347143	,0.000122072175173571	,0.00006103608758678	,0.000030518043793392,	
	0.000015259021896696
}; 


static void GenerateAsymmetry(ISP_DRC_S *pstDrc)
{
   HI_DOUBLE x[201];
   HI_DOUBLE xi;
   HI_DOUBLE Temp,Temp1,Temp2;
   HI_S8	ai;
   HI_DOUBLE as,dp;
   HI_U8	LutMaxOut = 1;
   HI_U8	Asy,Sec;
   HI_U8	i;
   HI_DOUBLE LutValueTemp;
   HI_DOUBLE Stretch;

   Asy = pstDrc->u8Asymmetry;
   Sec = pstDrc->u8SecondPole;
   Stretch = (HI_DOUBLE)pstDrc->u8Stretch/100;
  
   
   xi = (HI_DOUBLE)(Asy + 1)/257*2 - 1;
   Temp = 255*((1-1/(1000*xi*xi*xi)) + xi - ((xi>=0)*2));
   ai = (HI_S8)Temp;
   as = ABS((HI_DOUBLE)ai/255);
   dp = (HI_DOUBLE)Sec/255;
   

   if(ai >= 0)
   {
		for(i = 0;i<201;i++)
		{
			x[i] = pow(*(g_DrcLutPoint+i),Stretch); 
		}
   }
   else
   {
		for(i = 0;i<201;i++)
		{
			x[i] = pow((1 - *(g_DrcLutPoint+i)),Stretch);
		}
   }

   for(i = 0;i<201;i++)
   {
		Temp1 = dp + (1-dp)*(ABS((1-dp-x[i])/dp)*ABS((1-dp-x[i])/dp)*ABS((1-dp-x[i])/dp)) ;
		Temp2 = x[i]*(as+1)/(as+x[i]);
		LutValueTemp = Temp1*Temp2*LutMaxOut;
		pstDrc->LutValue1[i]= DRC_CLIP3(LutValueTemp,0,1);
        pstDrc->LutValue1[i] = pow(pstDrc->LutValue1[i],1/Stretch);
   }

   if(ai<0)
   {
		for(i = 0;i<201;i++)
		{
			pstDrc->LutValue1[i] = (HI_FLOAT)LutMaxOut - pstDrc->LutValue1[i];			
		}
   }

}

HI_S32 CameraResponseFunction(ISP_DRC_S *pstDrc)
{
  HI_U8 FlipLR;
  HI_U8 i;
  HI_U32 M,L;
  HI_DOUBLE Gamma1[201];
  HI_DOUBLE Gamma2[201];
  HI_DOUBLE GammaCurve[201];
  HI_DOUBLE Diff[201];
  HI_DOUBLE X[201] = {0.0f},Smooth[201];
  HI_DOUBLE K1,G1,F2,G2;
  HI_U8 index = 0;
  HI_U8 index_pre=0;
  HI_U8 J;
  HI_DOUBLE T,W1,W2,S;
  HI_DOUBLE temp;

  K1 = (HI_DOUBLE)pstDrc->K1/10000;
  G1 = (HI_DOUBLE)pstDrc->G1/10000;
  F2 = (HI_DOUBLE)pstDrc->F2/10000;
  G2 = (HI_DOUBLE)pstDrc->G2/10000;  

  /*K1 = 123.2567;
  G1 = 0.8213;
  F2 = 225;
  G2 = 0.4169;*/
  
   if(*g_DrcLutPoint > *(g_DrcLutPoint+200))
   {
       for(i = 0;i<201;i++)
       {
            X[i] = *(g_DrcLutPoint+200-i);
            X[i] = X[i]*256;
       }
       FlipLR = 1;
   }
   else
   {
        FlipLR = 0;
   }

   M = 4096;
   L = 256;
   
   for(i = 0;i<201;i++)
   {
        Gamma1[i] = K1*pow(X[i],G1);
        Gamma2[i] = (M+F2)*pow((X[i]/L),G2) - F2;

        if(Gamma1[i]<Gamma2[i])
        {
            index_pre = i;            
        }

        if(index_pre > index)
        {
            index = index_pre;
        }
   //curve difference
        Diff[i] = Gamma1[i] - Gamma2[i];
   }
   
   //Junction index by searching
   J = index;
   
   T = ((HI_DOUBLE)74/13)*J;

   W1 = ((HI_DOUBLE)125/13)*J;

   W2 = ((HI_DOUBLE)375/13)*J;

   S = 0.5;

   for(i = 0;i<201;i++)
   {
        if(Diff[i]<0)
        {
            temp = MAX(MIN((Diff[i]+W1)/W1*T,T),0);
        }
        else
        {
            temp =  MAX(MIN((Diff[i]+W1)/W1*T,(1-pow(Diff[i]/W2,S))*T),0);
        }
        Smooth[i] = (MIN(X[i],8)/8)* temp;

        if(i<J-3)
        {
            GammaCurve[i] =  Gamma1[i] - Smooth[i];
        }
        else if(i>=J+2)
        {
            GammaCurve[i] =  Gamma2[i] - Smooth[i];
        }
        else
        {
            GammaCurve[i] =  MIN(Gamma1[i],Gamma2[i]) - Smooth[i];
        }

		GammaCurve[i] = DRC_CLIP3(0,M,GammaCurve[i]);

       
   }

  if (1 == FlipLR)
  {
   
       for(i = 0;i<201;i++)
       {   
            pstDrc->LutValue0[i] = GammaCurve[200-i]/4096;
       }
  }
  else
  {
        for(i = 0;i<201;i++)
        {
            pstDrc->LutValue0[i] = GammaCurve[i]/4096;
        }
  }
       
   
    return HI_SUCCESS;

}

static HI_BOOL DrcJudge(HI_U16 *a,HI_U32 n)
{
     HI_U32 i= 0;

	 for (i=1;i<n;i++)
 	{
		if(a[i]>a[i-1])
		{
		 	return 0;
		}
 	}
	 return 1;
}


static HI_VOID DrcUpdateAsyLUT(ISP_DRC_S *pstDrc)
{
	HI_U32 i;
	HI_U16 ToneMappingValue1[201];
    HI_U16 ToneMappingDiff1[200]; 
    HI_U16 ToneMappingValue0[201];
    HI_U16 ToneMappingDiff0[200]; 
    HI_DOUBLE Temp0,Temp1;

    CameraResponseFunction(pstDrc);
    GenerateAsymmetry(pstDrc);

	for (i = 0;i<201;i++)
	{
		Temp0 = (HI_DOUBLE)(((HI_DOUBLE)log(pstDrc->LutValue0[i]))/((HI_DOUBLE)log(2))) + 16;
		
		ToneMappingValue0[i] = (HI_U32)(Temp0/16*65535 + 0.5);		

        Temp1 = (HI_DOUBLE)log(pstDrc->LutValue1[i])/log(2) + 16;		
		
        ToneMappingValue1[i] = (HI_U32)(Temp1/16*65535 + 0.5);     
	}

	for(i=0;i<200;i++)
	{		
        ToneMappingDiff0[i] = ToneMappingValue0[i] - ToneMappingValue0[i+1];
        pstDrc->ToneMappingValue0[i] = MIN(65535,ToneMappingValue0[i]);
        pstDrc->ToneMappingDiff0[i]  = MIN(4095,ToneMappingDiff0[i]);

        ToneMappingDiff1[i] = ToneMappingValue1[i] - ToneMappingValue1[i+1];
        pstDrc->ToneMappingValue1[i] = MIN(65535,ToneMappingValue1[i]);
        pstDrc->ToneMappingDiff1[i]  = MIN(4095,ToneMappingDiff1[i]);
	}

	hi_isp_drc_stat_ind_waddr0_write(0);	
    hi_isp_drc_stat_ind_waddr1_write(0);
	
     for (i = 0; i < 200; i++)
     {
     	hi_isp_drc_stat_ind_wdata0_write((((HI_U32)pstDrc->ToneMappingValue0[i]) << 12) | ((HI_U32)pstDrc->ToneMappingDiff0[i]));
     }
	
	if (DrcJudge(pstDrc->ToneMappingValue1,200))
	{
	     
		  for (i = 0; i < 200; i++)
         {    
                   
			hi_isp_drc_stat_ind_wdata1_write((((HI_U32)pstDrc->ToneMappingValue1[i]) << 12) | ((HI_U32)pstDrc->ToneMappingDiff1[i]));
		
         }
	}
	
	return;
	
    
}


static HI_VOID DrcExtRegsDefault(HI_VOID)
{
    /* initial register of drc */
	hi_ext_system_drc_manual_mode_write(OP_TYPE_AUTO);
    hi_ext_system_drc_asymmetry_write(DRC_ASYMMETRY);
    hi_ext_system_drc_secondpole_write(DRC_SECONDPOLE);
    hi_ext_system_drc_stretch_write(DRC_STRETCH);	
    hi_ext_system_drc_bright_gain_lmt_write(BRIGHT_GAIN_LMT);
	hi_ext_system_drc_dark_gain_lmt_c_write(DARK_GAIN_LMT_C);
	hi_ext_system_drc_dark_gain_lmt_y_write(DARK_GAIN_LMT_Y);
	   
    return;
}

static HI_VOID DrcSetRegsVal(HI_VOID) 
{
	HI_U32 Temp,u32BinScale, u32DivZ;
	HI_U32 u32Width, u32Height, u32HBlkSize, u32VBlkSize;
	HI_U32 u32HBlkNum, u32VBlkNum, u32BinNumZ;
	HI_DOUBLE Delta;
    HI_U32  Chk_Y,Chk_X;
    HI_FLOAT hblksizetemp,vblksizetemp;
    HI_U32  Div_x0,Div_x1,Div_y0,Div_y1;

	u32HBlkNum = hi_isp_drc_hnum_read();
	u32VBlkNum = hi_isp_drc_vnum_read();
	u32BinNumZ = hi_isp_drc_bin_read();


	u32Width  = (hi_isp_top_active_width_read() + 1);
	u32Height = (hi_isp_top_active_height_read() + 1);
	
    u32HBlkSize = u32Width/u32HBlkNum;
    u32VBlkSize = u32Height/u32VBlkNum;

    Chk_X = u32Width%u32HBlkNum;
    Chk_Y = u32Height%u32VBlkNum;

    
    hblksizetemp = ((HI_FLOAT)u32Width)/u32HBlkNum;
    
    vblksizetemp = ((HI_FLOAT)u32Height)/u32VBlkNum;

    if(hblksizetemp != u32HBlkSize)
    {
        hblksizetemp = (HI_U32)(u32HBlkSize + 1);
    }
    if(vblksizetemp != u32VBlkSize)
    {
        vblksizetemp = (HI_U32)(u32VBlkSize + 1);
    }

    
    Div_x0 = (HI_U32)(32768.0/u32HBlkSize + 0.5);//reciprocal value of block height
    
    Div_x1 = (HI_U32)(32768.0/hblksizetemp + 0.5);
    
    Div_y0 = (HI_U32)(32768.0/u32VBlkSize + 0.5);//reciprocal value of block width
    
    Div_y1 = (HI_U32)(32768.0/vblksizetemp + 0.5);
    
    u32DivZ = (HI_U32)(32768.0/(u32BinNumZ) + 0.5); 
    

	Temp = u32HBlkSize*u32VBlkSize;

    Delta = (log(u32Width*u32Height/DRC_H_BLK_NUM/DRC_V_BLK_NUM)/log(2)) -  (HI_U32)(log(u32Width*u32Height/DRC_H_BLK_NUM/DRC_V_BLK_NUM)/log(2));
	if(Delta>0)
	{
		Temp = (HI_U32)(log(u32Width*u32Height/DRC_H_BLK_NUM/DRC_V_BLK_NUM)/log(2))+1;
	}
	else
	{
		 Temp = (HI_U32)(log(u32Width*u32Height/DRC_H_BLK_NUM/DRC_V_BLK_NUM)/log(2));
	}
    
    u32BinScale = 17 - Temp;//3-bit bin counter shift control (0~4 supported)

	hi_isp_drc_hsize_write(u32HBlkSize-1);
	hi_isp_drc_vsize_write(u32VBlkSize-1);
    hi_isp_drc_chk_x_write(Chk_X);
	hi_isp_drc_chk_y_write(Chk_Y);
    
	hi_isp_drc_div_x0_write(Div_x0);
    hi_isp_drc_div_x1_write(Div_x1);
    hi_isp_drc_div_y0_write(Div_y0);
    hi_isp_drc_div_y1_write(Div_y1);
	hi_isp_drc_div_z_write(u32DivZ);
	
	hi_isp_drc_bin_scale_write(u32BinScale);
}

static HI_VOID DrcRegsDefault( ISP_DEV IspDev) 
{   	 
       HI_U8 i;
	ISP_CTX_S *pstIspCtx   = HI_NULL;       
       
       ISP_GET_CTX(IspDev, pstIspCtx);
	   
	
	HI_U8 u8WDRMode = pstIspCtx->u8SnsWDRMode;
	HI_U16 ColorCorrectionLut[33] = { 				
	    1024,        1024,        1024,        1024,        1024,        1024,        1024,        1024,
        1001,        1037,        1100,        1122,        1135,        1137,        1142,        1144,
        1148,        1159,        1167,        1179,        1193,        1207,        1221,        1219,
        1193,        1159,        1102,        1067,        1044,        1038,        1033,        1029,        1024
	};

	if(IS_BUILT_IN_WDR_MODE(u8WDRMode))
	{
		hi_isp_drc_strength_write(DRC_STRLENGTH_WDR);
	}
	else
	{
		hi_isp_drc_strength_write(DRC_STRLENGTH_LINEAR);
	}	
    
       hi_isp_drc_hnum_write(DRC_H_BLK_NUM);
	hi_isp_drc_vnum_write(DRC_V_BLK_NUM);
	
	hi_isp_drc_bin_write(DRC_BIN_NUM_Z);
	
	hi_isp_drc_local_edge_lmt_write(DRC_LOCAL_EDGELMT);
	//hi_isp_drc_strength_write(DRC_STRLENGTH);

	hi_isp_drc_y_val1_write(DRC_Y_VAL1);
	hi_isp_drc_y_sft1_write(DRC_Y_SFT1);
	hi_isp_drc_y_val2_write(DRC_Y_VAL2);
	hi_isp_drc_y_sft2_write(DRC_Y_SFT2);

	hi_isp_drc_c_val1_write(DRC_C_VAL1);
	hi_isp_drc_c_sft1_write(DRC_C_SFT1);
	hi_isp_drc_c_val2_write(DRC_C_VAL2);
	hi_isp_drc_c_sft2_write(DRC_C_SFT2);

	hi_isp_drc_bright_gain_val_write(DRC_B_VAL1);
	hi_isp_drc_bright_gain_sft_write(DRC_B_SFT1);

    
	hi_isp_drc_detail_mixing_bright_write(DETAIL_MIXING_BRIGHT);
	hi_isp_drc_detail_mixing_dark_write(DETAIL_MIXING_DARK);
	hi_isp_drc_detail_mixing_thres_write(DETAIL_MIXING_THRES);   
	hi_isp_drc_lut_mix_ctrl_write(LUT_MIX_CTRL);

	hi_isp_drc_var_spatial_write(VARSPATIAL);
	hi_isp_drc_var_range_write(VARRANGE);

	hi_isp_drc_stat_ind_waddr2_write(0);

	for (i = 0; i < 33; i++)
	{    
	    hi_isp_drc_stat_ind_wdata2_write(ColorCorrectionLut[i]); 
	}

	hi_isp_drc_range_flt_coef_write(RANGE_FLT_COEF);
	hi_isp_drc_gain_clip_knee_write(GAIN_CLIP_KNEE);
	hi_isp_drc_gain_clip_step_write(GAIN_CLIP_STEP);
    
	DrcSetRegsVal();    
    return;
}

static HI_VOID DrcExtRegsInitialize(ISP_DEV IspDev)
{	
	HI_U8 u8WDRMode;
    ISP_CTX_S *pstIspCtx = HI_NULL;
	ISP_DRC_S *pstDrc = HI_NULL;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

	ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_SensorGetDefault(IspDev, &pstSnsDft);			
    DRC_GET_CTX(IspDev, pstDrc);
    
	u8WDRMode = pstIspCtx->u8SnsWDRMode;

    pstDrc->u16BrightGainLmt      = pstSnsDft->stDrc.u16BrightGainLmt;
    pstDrc->u16DarkGainLmtC       = pstSnsDft->stDrc.u16DarkGainLmtC;
    pstDrc->u16DarkGainLmtY       = pstSnsDft->stDrc.u16DarkGainLmtY;
    pstDrc->u8SecondPole             = pstSnsDft->stDrc.u8SecondPole;
    pstDrc->u8Stretch                   = pstSnsDft->stDrc.u8Stretch;
    pstDrc->u8Asymmetry             = pstSnsDft->stDrc.u8Asymmetry;
    

	hi_ext_system_drc_bright_gain_lmt_write(pstDrc->u16BrightGainLmt);
	hi_ext_system_drc_dark_gain_lmt_c_write(pstDrc->u16DarkGainLmtC);
	hi_ext_system_drc_dark_gain_lmt_y_write(pstDrc->u16DarkGainLmtY);
    hi_ext_system_drc_stretch_write(pstDrc->u8Stretch);
    hi_ext_system_drc_secondpole_write(pstDrc->u8SecondPole);
    hi_ext_system_drc_asymmetry_write(pstDrc->u8Asymmetry);
    hi_ext_system_drc_manual_localmixingbrigtht_write(DETAIL_MIXING_BRIGHT);
    hi_ext_system_drc_manual_localmixingdark_write(DETAIL_MIXING_DARK);
    hi_ext_system_drc_auto_localmixingbrigtht_write(DETAIL_MIXING_BRIGHT);
    hi_ext_system_drc_auto_localmixingdark_write(DETAIL_MIXING_DARK);
    

	if (IS_BUILT_IN_WDR_MODE(u8WDRMode))
	{
		hi_ext_system_drc_strength_target_write(DRC_STRLENGTH_TARGET_WDR);
		hi_ext_system_drc_manual_strength_write(DRC_STRLENGTH_TARGET_WDR);
        hi_ext_system_drc_auto_strength_write(DRC_STRLENGTH_TARGET_WDR);
	}
	else
	{
		hi_ext_system_drc_strength_target_write(DRC_STRLENGTH_TARGET_LINEAR);
		hi_ext_system_drc_manual_strength_write(DRC_STRLENGTH_TARGET_LINEAR);
        hi_ext_system_drc_auto_strength_write(DRC_STRLENGTH_TARGET_LINEAR);
	}

    return;
}

static HI_VOID DrcRegsInitialize(ISP_DEV IspDev)
{   
	HI_U8 i;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
	ISP_DRC_S *pstDrc = HI_NULL;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);			
    DRC_GET_CTX(IspDev, pstDrc);
    
	pstDrc->bUpdateLut    = HI_FALSE;	
    pstDrc->G1            = DRC_G1;
    pstDrc->F2            = DRC_F2;
    pstDrc->G2            = DRC_G2;
    pstDrc->K1            = DRC_K1;
    

    pstDrc->bEnable               = pstSnsDft->stDrc.bEnable;
    pstDrc->bDrcDitherEn          = (pstDrc->bEnable) ? HI_FALSE : HI_TRUE;
    //pstDrc->u8LocalMixingBrigtht  = pstSnsDft->stDrc.u8LocalMixingBrigtht;
    //pstDrc->u8LocalMixingDark     = pstSnsDft->stDrc.u8LocalMixingDark;
    //pstDrc->u8LocalMixingThres    = pstSnsDft->stDrc.u8LocalMixingThres;
    pstDrc->u8SpatialVar          = pstSnsDft->stDrc.u8SpatialVar;
    pstDrc->u8RangeVar            = pstSnsDft->stDrc.u8RangeVar;

    for (i = 0; i < 33; i++)
    {    
        pstDrc->au16ColorCorrectionLut[i] = pstSnsDft->stDrc.au16ColorCorrectionLut[i];
    }    

   	pstDrc->u8SecondPole   = hi_ext_system_drc_secondpole_read();
	pstDrc->u8Stretch      = hi_ext_system_drc_stretch_read();
    pstDrc->u8Asymmetry    = hi_ext_system_drc_asymmetry_read();
	pstDrc->u32Strength    = hi_isp_drc_strength_read();
    
	hi_isp_stat_en_write(HI_TRUE);
	hi_isp_drc_moudle_dither_en_write(!(pstDrc->bEnable));
    
	DrcUpdateAsyLUT(pstDrc);
    
    hi_isp_drc_enable_write(pstDrc->bEnable);
    hi_isp_drc_moudle_dither_en_write(pstDrc->bDrcDitherEn);    
    //hi_isp_drc_detail_mixing_bright_write(pstDrc->u8LocalMixingBrigtht);
    //hi_isp_drc_detail_mixing_dark_write(pstDrc->u8LocalMixingDark);
    //hi_isp_drc_detail_mixing_thres_write(pstDrc->u8LocalMixingThres);
    hi_isp_drc_var_spatial_write(pstDrc->u8SpatialVar);
    hi_isp_drc_var_range_write(pstDrc->u8RangeVar);
    
    //hi_isp_drc_bin_write(0x7);
    
    hi_isp_drc_stat_ind_waddr2_write(0);
    for (i = 0; i < 33; i++)
    {    
    	hi_isp_drc_stat_ind_wdata2_write(pstDrc->au16ColorCorrectionLut[i]);
    }

    return;
}

HI_VOID DrcHistDarkWeightDef(DRC_HIST_WEIGHT_CALC_S *pstWeightCalc, HI_U8 u8WDRMode)
{
	if(IS_LINEAR_MODE(u8WDRMode))
    {
         pstWeightCalc->au8IndexTh[0] = 0;
         pstWeightCalc->au8IndexTh[1] = 4;
         pstWeightCalc->au8IndexTh[2] = 8;
	     pstWeightCalc->au8IndexTh[3] = 16;	
	     pstWeightCalc->au8IndexTh[4] = 32;
	     pstWeightCalc->au8IndexTh[5] = 64;
	     pstWeightCalc->au8IndexTh[6] = 255;

	     pstWeightCalc->au8Weight[0] = 32;
         pstWeightCalc->au8Weight[1] = 16;
         pstWeightCalc->au8Weight[2] = 8;
         pstWeightCalc->au8Weight[3] = 4;	
         pstWeightCalc->au8Weight[4] = 0;
         pstWeightCalc->au8Weight[5] = 0;
         pstWeightCalc->au8Weight[6] = 0;
	}
	else if(IS_BUILT_IN_WDR_MODE(u8WDRMode))
	{
	     pstWeightCalc->au8IndexTh[0] = 0;
         pstWeightCalc->au8IndexTh[1] = 4;
         pstWeightCalc->au8IndexTh[2] = 8;
	     pstWeightCalc->au8IndexTh[3] = 16;	
	     pstWeightCalc->au8IndexTh[4] = 32;
	     pstWeightCalc->au8IndexTh[5] = 64;
	     pstWeightCalc->au8IndexTh[6] = 255;

         pstWeightCalc->au8Weight[0] = 32;
         pstWeightCalc->au8Weight[1] = 16;
         pstWeightCalc->au8Weight[2] = 8;
         pstWeightCalc->au8Weight[3] = 4;  
         pstWeightCalc->au8Weight[4] = 2;
         pstWeightCalc->au8Weight[5] = 1;
         pstWeightCalc->au8Weight[6] = 0;
	}
	else
	{
	}
          
     return;
}

static __inline HI_S16 DrcHistDarkWeightCalc(DRC_HIST_WEIGHT_CALC_S *pstWeightCalc, HI_S8 s8HistIndex)
{
	HI_S16 as16HistIndexTh[7] = {0};
	HI_S16 as16HistIndexSlope[6] = {0};
	HI_S16 s16Weight;
	HI_S16 as16HistWeight[7] = {0};

    as16HistIndexTh[0] = pstWeightCalc->au8IndexTh[0];
    as16HistIndexTh[1] = pstWeightCalc->au8IndexTh[1];
    as16HistIndexTh[2] = pstWeightCalc->au8IndexTh[2];
    as16HistIndexTh[3] = pstWeightCalc->au8IndexTh[3];
    as16HistIndexTh[4] = pstWeightCalc->au8IndexTh[4];
    as16HistIndexTh[5] = pstWeightCalc->au8IndexTh[5];
    as16HistIndexTh[6] = pstWeightCalc->au8IndexTh[6];
    as16HistWeight[0]  = pstWeightCalc->au8Weight[0];
    as16HistWeight[1]  = pstWeightCalc->au8Weight[1];
    as16HistWeight[2]  = pstWeightCalc->au8Weight[2];
    as16HistWeight[3]  = pstWeightCalc->au8Weight[3];
    as16HistWeight[4]  = pstWeightCalc->au8Weight[4];
    as16HistWeight[5]  = pstWeightCalc->au8Weight[5];
    as16HistWeight[6]  = pstWeightCalc->au8Weight[6];

    as16HistIndexSlope[0] = (as16HistWeight[1] - as16HistWeight[0]) * 32
        / DIV_0_TO_1(as16HistIndexTh[1] - as16HistIndexTh[0]);
    as16HistIndexSlope[1] = (as16HistWeight[2] - as16HistWeight[1]) * 32
        / DIV_0_TO_1(as16HistIndexTh[2] - as16HistIndexTh[1]);
    as16HistIndexSlope[2] = (as16HistWeight[3] - as16HistWeight[2]) * 32
        / DIV_0_TO_1(as16HistIndexTh[3] - as16HistIndexTh[2]);
    as16HistIndexSlope[3] = (as16HistWeight[4] - as16HistWeight[3]) * 32
        / DIV_0_TO_1(as16HistIndexTh[4] - as16HistIndexTh[3]);
    as16HistIndexSlope[4] = (as16HistWeight[5] - as16HistWeight[4]) * 32
        / DIV_0_TO_1(as16HistIndexTh[5] - as16HistIndexTh[4]);
    as16HistIndexSlope[5] = (as16HistWeight[6] - as16HistWeight[5]) * 32
        / DIV_0_TO_1(as16HistIndexTh[6] - as16HistIndexTh[5]);

	if (s8HistIndex < as16HistIndexTh[0])
	{
		s16Weight = as16HistWeight[0];	
	}
	else if ((s8HistIndex >= as16HistIndexTh[0]) && (s8HistIndex < as16HistIndexTh[1]))
	{
	    s16Weight = as16HistWeight[0] + ((as16HistIndexSlope[0] * (s8HistIndex - as16HistIndexTh[0])) / 32);	
	}
	else if ((s8HistIndex >= as16HistIndexTh[1]) && (s8HistIndex < as16HistIndexTh[2]))
	{
	    s16Weight = as16HistWeight[1] + ((as16HistIndexSlope[1] * (s8HistIndex - as16HistIndexTh[1])) / 32);
	}
	else if ((s8HistIndex >= as16HistIndexTh[2]) && (s8HistIndex < as16HistIndexTh[3]))
	{
	    s16Weight = as16HistWeight[2] + ((as16HistIndexSlope[2] * (s8HistIndex - as16HistIndexTh[2])) / 32);
	}
	else if ((s8HistIndex >= as16HistIndexTh[3]) && (s8HistIndex < as16HistIndexTh[4]))
	{
	    s16Weight = as16HistWeight[3] + ((as16HistIndexSlope[3] * (s8HistIndex - as16HistIndexTh[3])) / 32);
	}
	else if ((s8HistIndex >= as16HistIndexTh[4]) && (s8HistIndex < as16HistIndexTh[5]))
	{
	    s16Weight = as16HistWeight[4] + ((as16HistIndexSlope[4] * (s8HistIndex - as16HistIndexTh[4])) / 32);
	}
	else if ((s8HistIndex >= as16HistIndexTh[5]) && (s8HistIndex < as16HistIndexTh[6]))
	{
	    s16Weight = as16HistWeight[5] + ((as16HistIndexSlope[5] * (s8HistIndex - as16HistIndexTh[5])) / 32);
	}
	else
	{
	    s16Weight = as16HistWeight[6];
	}

    s16Weight = MAX2(MIN2(s16Weight, 128), 0);

    return s16Weight;
}


static HI_U32  DrcCalHist(ISP_DRC_HIST_S *pstHist, ISP_STAT_S *pStatInfo, HI_U8 u8SnsWDRMode)
{
    HI_S32 i;
    HI_U32 u32ExpRatio;
	HI_U32 u32Stat3HistAcc = pStatInfo->stAeStat3.u32PixelWeight;
	HI_U32 u32HistDark   = 0;
		
    /* calculate histdark */
	
    DrcHistDarkWeightDef(&pstHist->pstHistWeightCalc, u8SnsWDRMode);
	
	for (i = 0; i < 256; i++)
	{
        u32HistDark  += (DrcHistDarkWeightCalc(&pstHist->pstHistWeightCalc, i) * pStatInfo->stAeStat3.au32HistogramMemArray[i] + 8) >> 4;
	}

	pstHist->s32HistDark = u32HistDark;

	


    /*calculate exposure_ratio for drc*/
     u32ExpRatio = (0xFF * ( pstHist->s32HistDark ) + u32Stat3HistAcc/2) / DIV_0_TO_1(u32Stat3HistAcc);/* Exposure ratio modulation: */
	
     if (u32ExpRatio > 0xFF)
     {
         u32ExpRatio = 0xFF;
     }
     pstHist->u16ExpRatio = u32ExpRatio;
     return HI_SUCCESS;
}



HI_S32 DrcCalStrength(ISP_DRC_S *pstDrc, HI_U32 u32Iso, HI_U8 u8SnsWDRMode, ISP_STAT_S *pStatInfo)
{
    HI_U32 u32Strength = 0;
	
	DrcCalHist(&pstDrc->stHist, pStatInfo, u8SnsWDRMode);

    /* Limit Drc strength for well balanced scenes */
    if(IS_LINEAR_MODE(u8SnsWDRMode))
    {
        u32Strength = pstDrc->stHist.u16ExpRatio;
    }
    else if(IS_BUILT_IN_WDR_MODE(u8SnsWDRMode))
    {   
        u32Strength = (105220000 + 679400 * pstDrc->stHist.u16ExpRatio  + 500 * pstDrc->stHist.u16ExpRatio * pstDrc->stHist.u16ExpRatio - 3 * pstDrc->stHist.u16ExpRatio * pstDrc->stHist.u16ExpRatio * pstDrc->stHist.u16ExpRatio + 500000) / 1000000;
    }
    else
    {
    }
    
    if (u32Strength > pstDrc->u32Strength)
    {
        u32Strength = pstDrc->u32Strength + 1;
    }
    else if (u32Strength < pstDrc->u32Strength)
    {
        u32Strength = pstDrc->u32Strength - 1;
    }
    else
    {
        
    }    

	if (u32Strength > pstDrc->u8StrengthTarget)
	{
		u32Strength = pstDrc->u8StrengthTarget;
	}
    
    if (u32Strength > 255)
    {
        u32Strength = 255;
    }

    pstDrc->u8LocalMixingBrigtht = (HI_U8)((53 * u32Strength + 146) / 255);
    pstDrc->u8LocalMixingDark = (HI_U8)((21784 - 53 * u32Strength) / 255);
    
    pstDrc->u32Strength = u32Strength;
    pstDrc->u8LocalMixingBrigtht = DRC_CLIP3(32, 64, pstDrc->u8LocalMixingBrigtht);
    pstDrc->u8LocalMixingDark= DRC_CLIP3(32, 64, pstDrc->u8LocalMixingDark);
    return HI_SUCCESS;
}




static HI_S32 DrcReadExtregs(ISP_DEV IspDev)
{   
	ISP_DRC_S *pstDrc = HI_NULL;

    DRC_GET_CTX(IspDev, pstDrc);	

	if (pstDrc->u8Asymmetry     != hi_ext_system_drc_asymmetry_read()
		|| pstDrc->u8SecondPole != hi_ext_system_drc_secondpole_read()
		|| pstDrc->u8Stretch    != hi_ext_system_drc_stretch_read())
	{
		pstDrc->u8Asymmetry  = hi_ext_system_drc_asymmetry_read();
		pstDrc->u8SecondPole = hi_ext_system_drc_secondpole_read();		
		pstDrc->u8Stretch    = hi_ext_system_drc_stretch_read();   	
        pstDrc->bUpdateLut   = HI_TRUE;	
	}	

	pstDrc->u16BrightGainLmt = au16BrightGainLmt[hi_ext_system_drc_bright_gain_lmt_read()];
	pstDrc->u16DarkGainLmtC  = au16DarkGainLmt[hi_ext_system_drc_dark_gain_lmt_c_read()];
	pstDrc->u16DarkGainLmtY  = au16DarkGainLmt[hi_ext_system_drc_dark_gain_lmt_y_read()];
	
    return 0;
}

HI_S32 DrcProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_DRC_S *pstDrc = HI_NULL;

    DRC_GET_CTX(IspDev, pstDrc);
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----DRC INFO------------------------------------------------------------------\n");
           
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
               "%8s" "%8s"     "%8s\n",
               "En", "ManuEn", "DrcSt");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d" "%8d"     "%8d\n",
        hi_isp_drc_enable_read(),
        hi_ext_system_drc_manual_mode_read(),
        pstDrc->u32Strength);
     
    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}

HI_S32 ISP_DrcInit(ISP_DEV IspDev)
{
    DrcRegsDefault(IspDev);
    DrcExtRegsDefault();
    DrcExtRegsInitialize(IspDev);
    DrcRegsInitialize(IspDev);

    return HI_SUCCESS;
}

HI_S32 ISP_DrcRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    ISP_DRC_S *pstDrc = HI_NULL;
    ISP_CTX_S *pstIspCtx   = HI_NULL;    
    
    DRC_GET_CTX(IspDev, pstDrc);
    ISP_GET_CTX(IspDev, pstIspCtx);
	HI_U8 u8WDRMode = pstIspCtx->u8SnsWDRMode;

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    /* calculate every two interrupts */
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }    

	DrcReadExtregs(IspDev);
	
	if (0 == hi_isp_drc_enable_read())
	{
       return HI_SUCCESS;   
	}

	if (pstDrc->bUpdateLut)
	{
		DrcUpdateAsyLUT(pstDrc);
		pstDrc->bUpdateLut = HI_FALSE;
	}		

	DrcSetRegsVal();

	hi_isp_drc_y_val1_write(pstDrc->u16DarkGainLmtY&0X7F);
	hi_isp_drc_y_sft1_write((pstDrc->u16DarkGainLmtY>>7)&0X7);
	hi_isp_drc_y_val2_write((pstDrc->u16DarkGainLmtY>>10)&0XF);
	hi_isp_drc_y_sft2_write((pstDrc->u16DarkGainLmtY>>14)&0X3);

	hi_isp_drc_c_val1_write((pstDrc->u16DarkGainLmtC)&0X7F);
	hi_isp_drc_c_sft1_write((pstDrc->u16DarkGainLmtC>>7)&0X7);
	hi_isp_drc_c_val2_write((pstDrc->u16DarkGainLmtC>>10)&0XF);
	hi_isp_drc_c_sft2_write((pstDrc->u16DarkGainLmtC>>14)&0X3);

	hi_isp_drc_bright_gain_val_write(pstDrc->u16BrightGainLmt&0X7F);
	hi_isp_drc_bright_gain_sft_write((pstDrc->u16BrightGainLmt>>7)&0X7);

	if (OP_TYPE_AUTO == hi_ext_system_drc_manual_mode_read())
	{   
	    pstDrc->u8StrengthTarget = hi_ext_system_drc_strength_target_read();
	    DrcCalStrength(pstDrc, pstIspCtx->stLinkage.u32Iso, u8WDRMode ,(ISP_STAT_S *)pStatInfo);
		hi_ext_system_drc_auto_strength_write(pstDrc->u32Strength);
		hi_ext_system_drc_auto_localmixingbrigtht_write(pstDrc->u8LocalMixingBrigtht);
		hi_ext_system_drc_auto_localmixingdark_write(pstDrc->u8LocalMixingDark);			
	}
	else
	//for manual	
	{
		pstDrc->u32Strength = hi_ext_system_drc_manual_strength_read();
        pstDrc->u8LocalMixingBrigtht = hi_ext_system_drc_manual_localmixingbrigtht_read();
        pstDrc->u8LocalMixingDark = hi_ext_system_drc_manual_localmixingdark_read();
	}
    
       hi_isp_drc_detail_mixing_bright_write(pstDrc->u8LocalMixingBrigtht);
       hi_isp_drc_detail_mixing_dark_write(pstDrc->u8LocalMixingDark);
    
	if(IS_BUILT_IN_WDR_MODE(u8WDRMode))
	{
		hi_isp_drc_lut_mix_ctrl_write(pstDrc->u32Strength);
	}
	else
	{
		hi_isp_drc_strength_write(pstDrc->u32Strength);		
	}

	if (hi_isp_drc_enable_read())
	{			
		hi_isp_drc_enable_write(HI_TRUE);
		hi_isp_drc_moudle_dither_en_write(HI_FALSE);
	}
	else
	{
		hi_isp_drc_enable_write(HI_FALSE);		
		hi_isp_drc_moudle_dither_en_write(HI_TRUE);
	}
    
	((ISP_REG_CFG_S *)pRegCfg)->stDrcRegCfg.FrameExposure = pstIspCtx->stLinkage.u32Inttime;

	((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1DrcCfg = 1;

    return HI_SUCCESS;
}

HI_S32 ISP_DrcCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{   
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET : 
			ISP_DrcInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            DrcProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_DrcExit(ISP_DEV IspDev)
{
	hi_isp_drc_enable_write(HI_FALSE);	
	hi_isp_stat_en_write(HI_FALSE);
	hi_isp_drc_rd_en_write(HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterDrc(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_DRC;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_DrcInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_DrcRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_DrcCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_DrcExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

