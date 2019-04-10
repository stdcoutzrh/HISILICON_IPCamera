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
#include "ivs_md.h"

#include "sample_comm_ive.h"

#define SAMPLE_IVE_MD_IMAGE_NUM 2

typedef struct hiSAMPLE_IVE_MD_S
{
	IVE_SRC_IMAGE_S astImg[SAMPLE_IVE_MD_IMAGE_NUM];	
	IVE_DST_MEM_INFO_S stBlob;
	MD_ATTR_S stMdAttr;		
	SAMPLE_RECT_ARRAY_S stRegion;
	FILE *pFp;
	VB_POOL hVbPool;
	HI_U16 u16BaseWitdh;
	HI_U16 u16BaseHeight;
}SAMPLE_IVE_MD_S;

static HI_BOOL s_bStopSignal = HI_FALSE;

static HI_VOID SAMPLE_IVE_Md_Uninit(SAMPLE_IVE_MD_S *pstMd)
{
	HI_S32 i;
	HI_S32 s32Ret = HI_SUCCESS;
	
	for (i = 0; i < SAMPLE_IVE_MD_IMAGE_NUM; i++)
	{	
    	IVE_MMZ_FREE(pstMd->astImg[i].u32PhyAddr[0],pstMd->astImg[i].pu8VirAddr[0]);
	}
	
    IVE_MMZ_FREE(pstMd->stBlob.u32PhyAddr,pstMd->stBlob.pu8VirAddr);
	if (NULL != pstMd->pFp)
	{				
	    s32Ret = HI_MPI_VB_DestroyPool(pstMd->hVbPool);
	    if (HI_SUCCESS != s32Ret)
	    {
	        SAMPLE_PRT("HI_MPI_VB_DestroyPool fail,Error(%#x)\n",s32Ret);
	    }	
		
		IVE_CLOSE_FILE(pstMd->pFp);
	}

	
	s32Ret = HI_IVS_MD_Exit();
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("HI_IVS_MD_Exit fail,Error(%#x)\n",s32Ret);
	   return ;
	}
	
}

static HI_S32 SAMPLE_IVE_Md_Init(SAMPLE_IVE_MD_S *pstMd,HI_U16 u16ExtWidth,HI_U16 u16ExtHeight,
			HI_U16 u16BaseWidth,HI_U16 u16BaseHeight,HI_CHAR *pchFileName)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 i ;
	HI_U32 u32Size;
	HI_U8 u8WndSz;	
	
   	memset(pstMd,0,sizeof(SAMPLE_IVE_MD_S));
	for (i = 0;i < SAMPLE_IVE_MD_IMAGE_NUM;i++)
	{
		s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstMd->astImg[i],IVE_IMAGE_TYPE_U8C1,u16ExtWidth,u16ExtHeight);
		if(s32Ret != HI_SUCCESS)
		{
		   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
		   goto MD_INIT_FAIL;
		}
	}
	u32Size = sizeof(IVE_CCBLOB_S);
	s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&pstMd->stBlob,u32Size);
    if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateMemInfo fail\n");
	   goto MD_INIT_FAIL;
	}
	if (NULL != pchFileName)
	{
		pstMd->pFp = fopen(pchFileName,"rb");
		if (NULL == pstMd->pFp)
		{
		 	SAMPLE_PRT("Open file %s fail\n",pchFileName);
			goto MD_INIT_FAIL;
		}

        u32Size = SAMPLE_COMM_IVE_CalcStride(u16BaseWidth,IVE_ALIGN) * u16BaseHeight *2;
    
    	pstMd->hVbPool = HI_MPI_VB_CreatePool( u32Size, 4,NULL );
		if (VB_INVALID_POOLID == pstMd->hVbPool)
		{
			SAMPLE_PRT("HI_MPI_VB_CreatePool fail\n");
			goto MD_INIT_FAIL;
		}
	}
	pstMd->u16BaseWitdh = u16BaseWidth;
	pstMd->u16BaseHeight = u16BaseHeight;
	//Set attr info
	pstMd->stMdAttr.enAlgMode = MD_ALG_MODE_BG;
	pstMd->stMdAttr.enSadMode = IVE_SAD_MODE_MB_4X4;
	pstMd->stMdAttr.enSadOutCtrl = IVE_SAD_OUT_CTRL_THRESH;
	pstMd->stMdAttr.u16SadThr = 100 * (1 << 1);//100 * (1 << 2);
	pstMd->stMdAttr.u16Width = u16ExtWidth;
	pstMd->stMdAttr.u16Height = u16ExtHeight;
	pstMd->stMdAttr.stAddCtrl.u0q16X = 32768;
	pstMd->stMdAttr.stAddCtrl.u0q16Y = 32768;
	u8WndSz = ( 1 << (2 + pstMd->stMdAttr.enSadMode));
	pstMd->stMdAttr.stCclCtrl.u16InitAreaThr = u8WndSz * u8WndSz;
	pstMd->stMdAttr.stCclCtrl.u16Step = u8WndSz;

	s32Ret = HI_IVS_MD_Init();
	if(s32Ret != HI_SUCCESS)
	{
		SAMPLE_PRT("HI_IVS_MD_Init fail,Error(%#x)\n",s32Ret);
		goto MD_INIT_FAIL;
	}

MD_INIT_FAIL:
    
    if(HI_SUCCESS != s32Ret)
	{
        SAMPLE_IVE_Md_Uninit(pstMd);
	}
	return s32Ret;    
    
}
static HI_VOID * SAMPLE_IVE_MdProc(HI_VOID * pArgs)
{
    HI_S32 s32Ret;
	SAMPLE_IVE_MD_S *pstMd;
	MD_ATTR_S *pstMdAttr;
	VIDEO_FRAME_INFO_S stBaseFrmInfo;
	VIDEO_FRAME_INFO_S stExtFrmInfo;
	VI_CHN viBaseChn = 0;
	VI_CHN viExtChn = 1;
	HI_S32 s32GetFrameMilliSec = 2000;
    HI_S32 s32SetFrameMilliSec = 2000;	
  	MD_CHN MdChn = 0;
  
    HI_BOOL bInstant = HI_TRUE;
    VO_LAYER voLayer = 0;
	VO_CHN voChn = 0;
 	HI_S32 s32CurIdx = 0;
	HI_BOOL bFirstFrm = HI_TRUE;
	
	pstMd = (SAMPLE_IVE_MD_S *)(pArgs);   
	pstMdAttr = &(pstMd->stMdAttr);
	//Create chn
	s32Ret = HI_IVS_MD_CreateChn(MdChn,&(pstMd->stMdAttr));	
	if (HI_SUCCESS != s32Ret)
	{
        SAMPLE_PRT("HI_IVS_MD_CreateChn fail,Error(%#x)\n",s32Ret);
        return NULL;
    }
		
    while (HI_FALSE == s_bStopSignal)
    {	
    	
		s32Ret = HI_MPI_VI_GetFrame(viExtChn, &stExtFrmInfo, s32GetFrameMilliSec);			
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("HI_MPI_VI_GetFrame ext chn(%d) fail,Error(%#x)\n",viExtChn,s32Ret);
			continue;
		}
		s32Ret = HI_MPI_VI_GetFrame(viBaseChn, &stBaseFrmInfo, s32GetFrameMilliSec);
		SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, EXT_RELEASE,
			"HI_MPI_VI_GetFrame base chn(%d) fail,Error(%#x)\n",viBaseChn,s32Ret);			
		if (HI_TRUE != bFirstFrm)
		{		
			s32Ret = SAMPLE_COMM_DmaImage(&stExtFrmInfo,&pstMd->astImg[s32CurIdx],bInstant);
			SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BASE_RELEASE,
							"SAMPLE_COMM_DmaImage fail,Error(%#x)\n",s32Ret);			
		}
		else
		{		
			s32Ret = SAMPLE_COMM_DmaImage(&stExtFrmInfo,&pstMd->astImg[1 - s32CurIdx],bInstant);			
			SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BASE_RELEASE,
							"SAMPLE_COMM_DmaImage fail,Error(%#x)\n",s32Ret);	
			
			bFirstFrm = HI_FALSE;			
			goto CHANGE_IDX;//first frame just init reference frame

		}
		
		s32Ret = HI_IVS_MD_Process(MdChn,&pstMd->astImg[s32CurIdx],&pstMd->astImg[1 - s32CurIdx],NULL,&pstMd->stBlob);		
        SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BASE_RELEASE,
							"HI_IVS_MD_Process fail,Error(%#x)\n",s32Ret);		   
		 
		SAMPLE_COMM_IVE_BlobToRect((IVE_CCBLOB_S *)pstMd->stBlob.pu8VirAddr,&(pstMd->stRegion),50,8,
			pstMdAttr->u16Width,pstMdAttr->u16Height,(HI_U16)stBaseFrmInfo.stVFrame.u32Width,(HI_U16)stBaseFrmInfo.stVFrame.u32Height);
		
	    //Draw rect
		s32Ret = SAMPLE_COMM_VGS_FillRect(&stBaseFrmInfo, &pstMd->stRegion, 0x0000FF00);
		 SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BASE_RELEASE,
							"SAMPLE_COMM_VGS_FillRect fail,Error(%#x)\n",s32Ret);	
		 
		 s32Ret = HI_MPI_VO_SendFrame(voLayer,voChn,&stBaseFrmInfo,s32SetFrameMilliSec);
		 SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BASE_RELEASE,
									 "HI_MPI_VO_SendFrame fail,Error(%#x)\n",s32Ret);	 
		CHANGE_IDX:
		//Change reference and current frame index
		s32CurIdx =	1 - s32CurIdx;

		BASE_RELEASE:
			s32Ret = HI_MPI_VI_ReleaseFrame(viBaseChn, &stBaseFrmInfo);
			if (HI_SUCCESS != s32Ret)
			{
				SAMPLE_PRT("HI_MPI_VI_ReleaseFrame fail,base chn(%d),Error(%#x)\n",viBaseChn,s32Ret);	
			}

		EXT_RELEASE:
			s32Ret = HI_MPI_VI_ReleaseFrame(viExtChn, &stExtFrmInfo);
			if (HI_SUCCESS != s32Ret)
			{
	            SAMPLE_PRT("HI_MPI_VI_ReleaseFrame fail,ext chn(%d),Error(%#x)\n",viExtChn,s32Ret);	
			}			
        
     }

	 //destroy 
	 s32Ret = HI_IVS_MD_DestroyChn(MdChn);	 
	 if (HI_SUCCESS != s32Ret)
	 {
		 SAMPLE_PRT("HI_IVS_MD_DestroyChn fail,Error(%#x)\n",s32Ret);  
	 }

     return HI_NULL;
}

static HI_VOID * SAMPLE_IVE_MdProc_File(HI_VOID * pArgs)
{
    HI_S32 s32Ret;
	SAMPLE_IVE_MD_S *pstMd;
	MD_ATTR_S *pstMdAttr;	
	VIDEO_FRAME_INFO_S stUserFrmInfo;
	VIDEO_FRAME_INFO_S stBaseFrmInfo;
	VIDEO_FRAME_INFO_S stExtFrmInfo;
	IVE_IMAGE_S stImg;
	HI_S32 s32GetFrameMilliSec = 2000;
    HI_S32 s32SetFrameMilliSec = 2000;	
  	MD_CHN MdChn = 0;
  
    HI_BOOL bInstant = HI_TRUE;
    VO_LAYER voLayer = 0;
	VO_CHN voChn = 0;
 	HI_S32 s32CurIdx = 0;
	HI_BOOL bFirstFrm = HI_TRUE;
	VPSS_GRP VpssGrp = 0;
	VPSS_CHN aVpssChn[2] = {VPSS_CHN0, VPSS_CHN3};
	VB_BLK     hBlkHdl;
	HI_U32 u32Size,u32LumaSize,u32ChrmSize; 
	
	pstMd = (SAMPLE_IVE_MD_S *)(pArgs);   
	pstMdAttr = &(pstMd->stMdAttr);
	//Create chn
	s32Ret = HI_IVS_MD_CreateChn(MdChn,&(pstMd->stMdAttr));	
	if (HI_SUCCESS != s32Ret)
	{
        SAMPLE_PRT("HI_IVS_MD_CreateChn fail,Error(%#x)\n",s32Ret);
        return NULL;
    }

	stUserFrmInfo.stVFrame.u32Field = VIDEO_FIELD_FRAME;
    stUserFrmInfo.stVFrame.enPixelFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stUserFrmInfo.stVFrame.u32Height = pstMd->u16BaseHeight;
    stUserFrmInfo.stVFrame.u32Width =  pstMd->u16BaseWitdh;
    stUserFrmInfo.stVFrame.u32Stride[0] = SAMPLE_COMM_IVE_CalcStride(pstMd->u16BaseWitdh,IVE_ALIGN);
    stUserFrmInfo.stVFrame.u32Stride[1] = SAMPLE_COMM_IVE_CalcStride(pstMd->u16BaseWitdh,IVE_ALIGN);
    stUserFrmInfo.stVFrame.u32Stride[2] = SAMPLE_COMM_IVE_CalcStride(pstMd->u16BaseWitdh,IVE_ALIGN);
    stUserFrmInfo.stVFrame.u32TimeRef = 0;
    stUserFrmInfo.stVFrame.u64pts = 0;
    stUserFrmInfo.stVFrame.enVideoFormat = VIDEO_FORMAT_LINEAR;

	u32Size = stUserFrmInfo.stVFrame.u32Stride[0] * stUserFrmInfo.stVFrame.u32Height *2;	 
	u32LumaSize = (stUserFrmInfo.stVFrame.u32Stride[0] * stUserFrmInfo.stVFrame.u32Height);
	u32ChrmSize = u32LumaSize >> 2;

	 stImg.enType = IVE_IMAGE_TYPE_YUV420SP;
	 stImg.u16Width = (HI_U16)stUserFrmInfo.stVFrame.u32Width;
	 stImg.u16Height = (HI_U16)stUserFrmInfo.stVFrame.u32Height;
	 stImg.u16Stride[0] = stUserFrmInfo.stVFrame.u32Stride[0];
	 stImg.u16Stride[1] = stUserFrmInfo.stVFrame.u32Stride[1];
	 stImg.u16Stride[2] = stUserFrmInfo.stVFrame.u32Stride[2];
	 
    while (HI_FALSE == s_bStopSignal)
    {	
    	//Read file,than send to vpss process
    	hBlkHdl = HI_MPI_VB_GetBlock( pstMd->hVbPool, u32Size, NULL);
    	 if(VB_INVALID_HANDLE == hBlkHdl)
        {
             SAMPLE_PRT("HI_MPI_VB_GetBlock fail,hVbPool(%d),Error(%#x)\n",pstMd->hVbPool,s32Ret);	
			 sleep(10);
             continue;
        }

		stUserFrmInfo.u32PoolId = HI_MPI_VB_Handle2PoolId(hBlkHdl);
        stUserFrmInfo.stVFrame.u32PhyAddr[0] = HI_MPI_VB_Handle2PhysAddr( hBlkHdl );
        stUserFrmInfo.stVFrame.pVirAddr[0] =  HI_MPI_SYS_Mmap(stUserFrmInfo.stVFrame.u32PhyAddr[0], u32Size);        
        stUserFrmInfo.stVFrame.u32PhyAddr[1] = stUserFrmInfo.stVFrame.u32PhyAddr[0] + u32LumaSize;
        stUserFrmInfo.stVFrame.pVirAddr[1] = (HI_U8 *)(stUserFrmInfo.stVFrame.pVirAddr[0]) + u32LumaSize;        
        stUserFrmInfo.stVFrame.u32PhyAddr[2] = stUserFrmInfo.stVFrame.u32PhyAddr[1] + u32ChrmSize;
        stUserFrmInfo.stVFrame.pVirAddr[2] =  (HI_U8 *)(stUserFrmInfo.stVFrame.pVirAddr[1]) + u32ChrmSize;        
        stUserFrmInfo.stVFrame.enCompressMode = COMPRESS_MODE_NONE;
        stUserFrmInfo.stVFrame.u32HeaderPhyAddr[0] = 0;
        stUserFrmInfo.stVFrame.u32HeaderPhyAddr[1] = 0;
	    stUserFrmInfo.stVFrame.u64pts += 40000;
        stUserFrmInfo.stVFrame.u32TimeRef += 40000;

		stImg.u32PhyAddr[0] = stUserFrmInfo.stVFrame.u32PhyAddr[0];
		stImg.pu8VirAddr[0] = stUserFrmInfo.stVFrame.pVirAddr[0];		
		stImg.u32PhyAddr[1] = stUserFrmInfo.stVFrame.u32PhyAddr[1];
		stImg.pu8VirAddr[1] = stUserFrmInfo.stVFrame.pVirAddr[1];
		stImg.u32PhyAddr[2] = stUserFrmInfo.stVFrame.u32PhyAddr[2];
		stImg.pu8VirAddr[2] = stUserFrmInfo.stVFrame.pVirAddr[2];

		s32Ret = SAMPLE_COMM_IVE_ReadFile(&stImg,pstMd->pFp);		
		SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BLOCK_RELEASE,
									"SAMPLE_COMM_IVE_ReadFile fail,Error(%#x)\n",s32Ret);	
		//Send data to vpss		
		s32Ret = HI_MPI_VPSS_SendFrame(VpssGrp, &stUserFrmInfo, s32SetFrameMilliSec);
		SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BLOCK_RELEASE,
										"HI_MPI_VPSS_SendFrame fail,Grp(%d),Error(%#x)\n",VpssGrp,s32Ret);	
		
		//Get data from vpss		
		s32Ret = HI_MPI_VPSS_GetChnFrame(VpssGrp, aVpssChn[1], &stExtFrmInfo, s32GetFrameMilliSec);
		SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BLOCK_RELEASE,
									"HI_MPI_VPSS_GetChnFrame fail,Vpss Grp(%d),Chn(%d),Error(%#x)\n",VpssGrp,aVpssChn[1],s32Ret);	
		
		s32Ret = HI_MPI_VPSS_GetChnFrame(VpssGrp, aVpssChn[0], &stBaseFrmInfo, s32GetFrameMilliSec);
		SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, EXT_RELEASE,
									"HI_MPI_VPSS_GetChnFrame fail,Vpss Grp(%d),Chn(%d),Error(%#x)\n",VpssGrp,aVpssChn[0],s32Ret);	
			
		if (HI_TRUE != bFirstFrm)
		{		
			s32Ret = SAMPLE_COMM_DmaImage(&stExtFrmInfo,&pstMd->astImg[s32CurIdx],bInstant);
			SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BASE_RELEASE,
							"SAMPLE_COMM_DmaImage fail,Error(%#x)\n",s32Ret);			
		}
		else
		{		
			s32Ret = SAMPLE_COMM_DmaImage(&stExtFrmInfo,&pstMd->astImg[1 - s32CurIdx],bInstant);			
			SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BASE_RELEASE,
							"SAMPLE_COMM_DmaImage fail,Error(%#x)\n",s32Ret);	
			
			bFirstFrm = HI_FALSE;			
			goto CHANGE_IDX;//first frame just init reference frame

		}
		
		s32Ret = HI_IVS_MD_Process(MdChn,&pstMd->astImg[s32CurIdx],&pstMd->astImg[1 - s32CurIdx],NULL,&pstMd->stBlob);		
        SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BASE_RELEASE,
							"HI_IVS_MD_Process fail,Error(%#x)\n",s32Ret);		   
		 
		SAMPLE_COMM_IVE_BlobToRect((IVE_CCBLOB_S *)pstMd->stBlob.pu8VirAddr,&(pstMd->stRegion),50,8,
			pstMdAttr->u16Width,pstMdAttr->u16Height,(HI_U16)stBaseFrmInfo.stVFrame.u32Width,(HI_U16)stBaseFrmInfo.stVFrame.u32Height);
		
	    //Draw rect
		s32Ret = SAMPLE_COMM_VGS_FillRect(&stBaseFrmInfo, &pstMd->stRegion, 0x0000FF00);
		 SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BASE_RELEASE,
							"SAMPLE_COMM_VGS_FillRect fail,Error(%#x)\n",s32Ret);	
		 
		 s32Ret = HI_MPI_VO_SendFrame(voLayer,voChn,&stBaseFrmInfo,s32SetFrameMilliSec);
		 SAMPLE_CHECK_EXPR_GOTO(HI_SUCCESS != s32Ret, BASE_RELEASE,
									 "HI_MPI_VO_SendFrame fail,Error(%#x)\n",s32Ret);	 
		
		CHANGE_IDX:
			//Change reference and current frame index
			s32CurIdx =	1 - s32CurIdx;

		BASE_RELEASE:
			s32Ret = HI_MPI_VPSS_ReleaseChnFrame(VpssGrp,aVpssChn[0], &stBaseFrmInfo);
			if (HI_SUCCESS != s32Ret)
			{
				SAMPLE_PRT("HI_MPI_VPSS_ReleaseChnFrame fail,Grp(%d) chn(%d),Error(%#x)\n",VpssGrp,aVpssChn[0],s32Ret);	
			}

		EXT_RELEASE:
			s32Ret = HI_MPI_VPSS_ReleaseChnFrame(VpssGrp,aVpssChn[1], &stExtFrmInfo);
			if (HI_SUCCESS != s32Ret)
			{
				SAMPLE_PRT("HI_MPI_VPSS_ReleaseChnFrame fail,Grp(%d) chn(%d),Error(%#x)\n",VpssGrp,aVpssChn[1],s32Ret);	
			}	
		BLOCK_RELEASE:
			s32Ret = HI_MPI_VB_ReleaseBlock(hBlkHdl);
			if (HI_SUCCESS != s32Ret)
			{
				SAMPLE_PRT("HI_MPI_VB_ReleaseBlock fail,blkHdl(%d),Error(%#x)\n",hBlkHdl,s32Ret);	
			}
			
        	(HI_VOID)HI_MPI_SYS_Munmap(stUserFrmInfo.stVFrame.pVirAddr[0], u32Size);
     }

	 //destroy 
	 s32Ret = HI_IVS_MD_DestroyChn(MdChn);	 
	 if (HI_SUCCESS != s32Ret)
	 {
		 SAMPLE_PRT("HI_IVS_MD_DestroyChn fail,Error(%#x)\n",s32Ret);  
	 }

     return HI_NULL;
}

HI_VOID SAMPLE_IVE_Md(HI_CHAR chUseFile)
{
    HI_S32 s32Ret = HI_SUCCESS;
	
	HI_U16 u16ExtWidth = 352;
	HI_U16 u16ExtHeight = 288;
	HI_U16 u16BaseWidth = 1920;
	HI_U16 u16BaseHeight = 1080;
	HI_BOOL bOpenVi = HI_FALSE;
	HI_BOOL bOpenViExt = HI_FALSE;
	HI_BOOL bOpenVo = HI_TRUE;
	HI_BOOL bOpenVenc = HI_FALSE;
	HI_BOOL bOpenVpss = HI_FALSE;
	SAMPLE_IVE_MD_S stMd;
	SAMPLE_IVE_VI_VO_CONFIG_S stViVoConfig; 
    pthread_t hIveThread;
	typedef void * (*fun_callback)(void *);
	fun_callback fun;
	HI_U32 u32VpssChnNum = 2;
	HI_CHAR *pchFileName = NULL;
    //SAMPLE_COMM_IVE_CheckIveMpiInit(); 

	s_bStopSignal = HI_FALSE;	
	stViVoConfig.enPicSize = PIC_HD1080;
	stViVoConfig.enNorm =	VIDEO_ENCODING_MODE_PAL;
	stViVoConfig.enVoIntfType = VO_INTF_BT656;//hi3518ev200 only support BT656/LCD
	stViVoConfig.stViConfig.enViMode = SAMPLE_VI_MODE_BT1120_1080P;
	stViVoConfig.stViConfig.enNorm = VIDEO_ENCODING_MODE_PAL;
	stViVoConfig.stViConfig.enRotate = ROTATE_NONE; 			
	stViVoConfig.stViConfig.enViChnSet = VI_CHN_SET_NORMAL;
	stViVoConfig.stViConfig.enWDRMode = WDR_MODE_NONE; 

	if ('0' == chUseFile)
	{
		bOpenVi = HI_TRUE;
		bOpenViExt = HI_TRUE;
		fun =  SAMPLE_IVE_MdProc;
	}
	else
	{
		bOpenVpss = HI_TRUE;
		pchFileName  = "./data/input/md/md_sample_sp420_1080p.yuv";
		fun =  SAMPLE_IVE_MdProc_File;
		
	}
    

	s32Ret = SAMPLE_COMM_IVE_BT1120_720P_PreView(&stViVoConfig,bOpenVi,bOpenViExt,
		bOpenVo,bOpenVenc,bOpenVpss,u32VpssChnNum);
	if(s32Ret != HI_SUCCESS)
	{
		SAMPLE_PRT("SAMPLE_COMM_IVE_BT1120_720P_PreView fail\n");
		return;
	}
	s32Ret = SAMPLE_IVE_Md_Init(&stMd,u16ExtWidth,u16ExtHeight,u16BaseWidth,u16BaseHeight,pchFileName);
	if(s32Ret != HI_SUCCESS)
	{
		SAMPLE_PRT("SAMPLE_IVE_Md_Init fail\n");
		goto MD_END;
	}

    pthread_create(&hIveThread, 0, fun, (HI_VOID*)&stMd);

    SAMPLE_VI_PAUSE();

    s_bStopSignal = HI_TRUE;    
    pthread_join(hIveThread,HI_NULL);
	
MD_END:    
    SAMPLE_IVE_Md_Uninit(&(stMd));	
	SAMPLE_COMM_IVE_BT1120_720P_Stop(&stViVoConfig,bOpenVi,bOpenViExt,bOpenVo,bOpenVenc,
		bOpenVpss,u32VpssChnNum);

}


