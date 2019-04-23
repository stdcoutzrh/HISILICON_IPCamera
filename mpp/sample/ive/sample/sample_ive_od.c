#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <pthread.h>

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "hi_comm_ive.h"
#include "hi_comm_vgs.h"
#include "hi_comm_vi.h"
#include "hi_comm_vo.h"


#include "mpi_vb.h"
#include "mpi_sys.h"
#include "mpi_ive.h"
#include "mpi_vgs.h"
#include "mpi_vi.h"
#include "mpi_vo.h"


#include "sample_comm_ive.h"

typedef struct hiSAMPLE_IVE_OD_S
{
	IVE_SRC_IMAGE_S stSrc;
	IVE_DST_IMAGE_S stInteg;	
	IVE_INTEG_CTRL_S stIntegCtrl;
	HI_U32 u32W;
	HI_U32 u32H;
}SAMPLE_IVE_OD_S;

static HI_BOOL s_bStopSignal = HI_FALSE;

static HI_VOID SAMPLE_IVE_Od_Uninit(SAMPLE_IVE_OD_S *pstOd)
{
    IVE_MMZ_FREE(pstOd->stSrc.u32PhyAddr[0],pstOd->stSrc.pu8VirAddr[0]);
    IVE_MMZ_FREE(pstOd->stInteg.u32PhyAddr[0],pstOd->stInteg.pu8VirAddr[0]);

}

static HI_S32 SAMPLE_IVE_Od_Init(SAMPLE_IVE_OD_S *pstOd,HI_U16 u16Width,HI_U16 u16Height)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
   	memset(pstOd,0,sizeof(SAMPLE_IVE_OD_S));

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstOd->stSrc,IVE_IMAGE_TYPE_U8C1,u16Width,u16Height);
    if(s32Ret != HI_SUCCESS)
    {
       SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
       goto OD_INIT_FAIL;
    }
	s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstOd->stInteg,IVE_IMAGE_TYPE_U64C1,u16Width,u16Height);
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
	   goto OD_INIT_FAIL;
	}
    
    pstOd->stIntegCtrl.enOutCtrl = IVE_INTEG_OUT_CTRL_COMBINE;

    pstOd->u32W = u16Width/IVE_CHAR_CALW;
    pstOd->u32H = u16Height/IVE_CHAR_CALH;


OD_INIT_FAIL:
    
    if(HI_SUCCESS != s32Ret)
	{
        SAMPLE_IVE_Od_Uninit(pstOd);
	}
	return s32Ret;    
    
}

static HI_S32 SAMPLE_IVE_Linear2DClassifer(POINT_S *pstChar, HI_S32 s32CharNum, 
                                            POINT_S *pstLinearPoint, HI_S32 s32Linearnum )
{
	HI_S32 s32ResultNum;
	HI_S32 i,j;
	HI_BOOL bTestFlag;
	POINT_S *pstNextLinearPoint;
	
	s32ResultNum = 0;
	pstNextLinearPoint = &pstLinearPoint[1];	
	for(i = 0;i < s32CharNum;i++)
	{
		bTestFlag = HI_TRUE;
		for(j = 0;j < (s32Linearnum-1);j++)
		{  
			if( ( (pstChar[i].s32Y - pstLinearPoint[j].s32Y) * (pstNextLinearPoint[j].s32X - pstLinearPoint[j].s32X)>
				  (pstChar[i].s32X - pstLinearPoint[j].s32X) * (pstNextLinearPoint[j].s32Y - pstLinearPoint[j].s32Y) 
				   && (pstNextLinearPoint[j].s32X != pstLinearPoint[j].s32X))
			   || ( (pstChar[i].s32X>pstLinearPoint[j].s32X) && (pstNextLinearPoint[j].s32X == pstLinearPoint[j].s32X) ))	
			{
				bTestFlag = HI_FALSE;
				break;
			}          
           
		}
		if(HI_TRUE == bTestFlag)
		{
			s32ResultNum++;
		}
	}
	return s32ResultNum;
}


static HI_VOID * SAMPLE_IVE_OdProc(HI_VOID * pArgs)
{
    HI_S32 s32Ret;
	HI_U32 i,j;	   
	SAMPLE_IVE_OD_S *pstOd;
	VIDEO_FRAME_INFO_S stFrameInfo;
	VI_CHN viChn = 0;
	HI_S32 s32GetFrameMilliSec = 2000;
    HI_S32 s32SetFrameMilliSec = 2000;
	IVE_DATA_S stSrc;
	IVE_DATA_S stDst;
    IVE_HANDLE IveHandle;
    HI_BOOL bFinish = HI_FALSE;
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bInstant = HI_TRUE;
    VO_LAYER voLayer = 0;
	VO_CHN voChn = 0;
 
    POINT_S stChar[IVE_CHAR_NUM];
    POINT_S astPoints[10] = {{0,0}};
	IVE_LINEAR_DATA_S stIveLinerData;
    HI_U64 *pu64VirData = HI_NULL;
	IVE_DMA_CTRL_S stDmaCtrl = {IVE_DMA_MODE_DIRECT_COPY,0,0,0,0};

	HI_U64 u64TopLeft, u64TopRight, u64BtmLeft, u64BtmRight;
	HI_U64 *pu64TopRow, *pu64BtmRow;
	HI_U64 u64BlockSum,u64BlockSq;
	HI_FLOAT fSqVar;

	pstOd = (SAMPLE_IVE_OD_S *)(pArgs);
    pu64VirData = (HI_U64 *)pstOd->stInteg.pu8VirAddr[0];
	
    stIveLinerData.pstLinearPoint = &astPoints[0];
    stIveLinerData.s32LinearNum = 2;
	stIveLinerData.s32ThreshNum = IVE_CHAR_NUM/2;
	stIveLinerData.pstLinearPoint[0].s32X = 80;
	stIveLinerData.pstLinearPoint[0].s32Y = 0;
	stIveLinerData.pstLinearPoint[1].s32X = 80;
	stIveLinerData.pstLinearPoint[1].s32Y = 20;

    while (HI_FALSE == s_bStopSignal)
    {
       
		s32Ret = HI_MPI_VI_GetFrame(viChn, &stFrameInfo, s32GetFrameMilliSec);
		if (HI_SUCCESS != s32Ret)
		{
            SAMPLE_PRT("HI_MPI_VI_GetFrame fail,Error(%#x)\n",s32Ret);
            continue;
        }
        stSrc.pu8VirAddr = (HI_U8*)stFrameInfo.stVFrame.pVirAddr[0];
	    stSrc.u32PhyAddr = stFrameInfo.stVFrame.u32PhyAddr[0];
	    stSrc.u16Stride = (HI_U16)stFrameInfo.stVFrame.u32Stride[0];
	    stSrc.u16Width = (HI_U16)stFrameInfo.stVFrame.u32Width;
	    stSrc.u16Height = stFrameInfo.stVFrame.u32Height;
	   
	    stDst.pu8VirAddr = pstOd->stSrc.pu8VirAddr[0];
	    stDst.u32PhyAddr = pstOd->stSrc.u32PhyAddr[0];
	    stDst.u16Stride = (HI_U16)stFrameInfo.stVFrame.u32Stride[0];
	    stDst.u16Width = (HI_U16)stFrameInfo.stVFrame.u32Width;
	    stDst.u16Height = stFrameInfo.stVFrame.u32Height;
		
        bInstant = HI_FALSE;        
        s32Ret = HI_MPI_IVE_DMA(&IveHandle,&stSrc,&stDst,&stDmaCtrl,bInstant);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("HI_MPI_IVE_DMA fail,Error(%#x)\n",s32Ret);
            (HI_VOID)HI_MPI_VI_ReleaseFrame(viChn, &stFrameInfo);
            continue;
        }

        bInstant = HI_TRUE;
        s32Ret = HI_MPI_IVE_Integ(&IveHandle, &pstOd->stSrc, &pstOd->stInteg,&pstOd->stIntegCtrl,bInstant);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("HI_MPI_IVE_Integ fail,Error(%#x)\n",s32Ret);
            (HI_VOID)HI_MPI_VI_ReleaseFrame(viChn, &stFrameInfo);
            continue;
        }

        s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);		
		while(HI_ERR_IVE_QUERY_TIMEOUT == s32Ret)
		{
			usleep(100);					
			s32Ret = HI_MPI_IVE_Query(IveHandle,&bFinish,bBlock);	
		}
        if(HI_SUCCESS != s32Ret)
        {
           SAMPLE_PRT("HI_MPI_IVE_Query fail,Error(%#x)\n",s32Ret);
           (HI_VOID)HI_MPI_VI_ReleaseFrame(viChn, &stFrameInfo);
           continue; 
        }
       
        for(j = 0; j < IVE_CHAR_CALH; j++)
		{
            pu64TopRow = (0 == j) ? (pu64VirData) : ( pu64VirData + (j * pstOd->u32H -1) * pstOd->stInteg.u16Stride[0]);
            pu64BtmRow = pu64VirData + ((j + 1) * pstOd->u32H - 1) * pstOd->stInteg.u16Stride[0];
            
			for(i = 0;i < IVE_CHAR_CALW; i++)
			{      
                u64TopLeft  = (0 == j) ? (0) :((0 == i) ? (0) : (pu64TopRow[i * pstOd->u32W-1]));
                u64TopRight = (0 == j) ? (0) : (pu64TopRow[(i + 1) * pstOd->u32W - 1]);
                u64BtmLeft  = (0 == i) ? (0) : (pu64BtmRow[i * pstOd->u32W - 1]);
                u64BtmRight = pu64BtmRow[(i + 1) * pstOd->u32W -1];

                u64BlockSum = (u64TopLeft & 0xfffffffLL) + (u64BtmRight & 0xfffffffLL)
                            - (u64BtmLeft & 0xfffffffLL) - (u64TopRight & 0xfffffffLL);

                u64BlockSq  = (u64TopLeft >> 28) + (u64BtmRight >> 28)
                            - (u64BtmLeft >> 28) - (u64TopRight >> 28);
   
               // mean
				stChar[j * IVE_CHAR_CALW + i].s32X = u64BlockSum/(pstOd->u32W*pstOd->u32H);
               // sigma=sqrt(1/(w*h)*sum((x(i,j)-mean)^2)= sqrt(sum(x(i,j)^2)/(w*h)-mean^2)
               fSqVar = u64BlockSq/(pstOd->u32W*pstOd->u32H) - stChar[j * IVE_CHAR_CALW + i].s32X * stChar[j * IVE_CHAR_CALW + i].s32X;
			   stChar[j * IVE_CHAR_CALW + i].s32Y = (HI_U32)sqrt(fSqVar);
			}
		}

        s32Ret = SAMPLE_IVE_Linear2DClassifer(&stChar[0],IVE_CHAR_NUM,
                    stIveLinerData.pstLinearPoint,stIveLinerData.s32LinearNum);            
        if(s32Ret > stIveLinerData.s32ThreshNum)
        {
            SAMPLE_PRT("\033[0;31m Occlusion detected!\033[0;39m\n");
        }
        else
        {
            SAMPLE_PRT("Enter any key to quit!\n");
        }       
       
        //Send to vo
        s32Ret = HI_MPI_VO_SendFrame(voLayer,voChn,&stFrameInfo,s32SetFrameMilliSec);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("HI_MPI_VO_SendFrame fail,Error(%#x)\n",s32Ret);
            (HI_VOID)HI_MPI_VI_ReleaseFrame(viChn, &stFrameInfo);
            continue;
        }
     
        s32Ret = HI_MPI_VI_ReleaseFrame(viChn, &stFrameInfo);
        if (HI_SUCCESS != s32Ret)
        {
           SAMPLE_PRT("HI_MPI_VI_ReleaseFrame fail,ViChn(%d),Error(%#x)\n",viChn,s32Ret);
           continue;
        }
     }
     return HI_NULL;
}


HI_VOID SAMPLE_IVE_Od(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
	HI_U16 u16Width = 1920;
	HI_U16 u16Height = 1080;
	
	SAMPLE_IVE_OD_S stOd;
	SAMPLE_IVE_VI_VO_CONFIG_S stViVoConfig; 
    pthread_t hIveThread;

    SAMPLE_COMM_IVE_CheckIveMpiInit(); 

	s_bStopSignal = HI_FALSE;	
	stViVoConfig.enPicSize = PIC_HD1080;
	stViVoConfig.enNorm =	VIDEO_ENCODING_MODE_PAL;
	stViVoConfig.enVoIntfType = VO_INTF_BT656;//hi3518ev200 only support BT656/LCD
	stViVoConfig.stViConfig.enViMode = SAMPLE_VI_MODE_BT1120_1080P;
	stViVoConfig.stViConfig.enNorm = VIDEO_ENCODING_MODE_PAL;
	stViVoConfig.stViConfig.enRotate = ROTATE_NONE; 			
	stViVoConfig.stViConfig.enViChnSet = VI_CHN_SET_NORMAL;
	stViVoConfig.stViConfig.enWDRMode = WDR_MODE_NONE; 

    s32Ret = SAMPLE_IVE_Od_Init(&stOd,u16Width,u16Height);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("SAMPLE_IVE_Detect_Init fail\n");
        goto OD_END_0;
    }

	s32Ret = SAMPLE_COMM_IVE_BT1120_720P_PreView(&stViVoConfig,HI_TRUE,HI_FALSE,HI_TRUE,HI_FALSE,HI_FALSE,1);
	if(s32Ret != HI_SUCCESS)
	{
		SAMPLE_PRT("SAMPLE_COMM_IVE_BT1120_720P_PreView fail\n");
		goto OD_END_1;
	}

    pthread_create(&hIveThread, 0, SAMPLE_IVE_OdProc, (HI_VOID*)&stOd);

    SAMPLE_VI_PAUSE();

    s_bStopSignal = HI_TRUE;    
    pthread_join(hIveThread,HI_NULL);
	SAMPLE_COMM_IVE_BT1120_720P_Stop(&stViVoConfig,HI_FALSE,HI_TRUE,HI_FALSE,HI_FALSE,HI_FALSE,1);

OD_END_1:    
    SAMPLE_IVE_Od_Uninit(&(stOd));
OD_END_0:
	SAMPLE_COMM_IVE_IveMpiExit();	
}



