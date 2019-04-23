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


#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "hi_comm_ive.h"

#include "mpi_vb.h"
#include "mpi_sys.h"
#include "mpi_ive.h"

#include "sample_comm_ive.h"


typedef struct hiSAMPLE_IVE_CANNY_INFO_S
{
	IVE_SRC_IMAGE_S stSrc;
	IVE_DST_IMAGE_S stEdge;
	IVE_DST_IMAGE_S stMag;
	IVE_MEM_INFO_S  stStack;
	IVE_CANNY_HYS_EDGE_CTRL_S  stCannyHysEdgeCtrl;
	IVE_MAG_AND_ANG_CTRL_S stMagAndAngCtrl;
	IVE_THRESH_U16_CTRL_S stThrU16Ctrl;
	FILE *pFpSrc;
	FILE *pFpDst;
}SAMPLE_IVE_CANNY_INFO_S;

/******************************************************************************
* function : Canny uninit
******************************************************************************/
static HI_VOID SAMPLE_IVE_Canny_Uninit(SAMPLE_IVE_CANNY_INFO_S *pstCannyInfo)
{
    IVE_MMZ_FREE(pstCannyInfo->stSrc.u32PhyAddr[0],pstCannyInfo->stSrc.pu8VirAddr[0]);
	IVE_MMZ_FREE(pstCannyInfo->stEdge.u32PhyAddr[0],pstCannyInfo->stEdge.pu8VirAddr[0]);
    IVE_MMZ_FREE(pstCannyInfo->stMag.u32PhyAddr[0],pstCannyInfo->stMag.pu8VirAddr[0]);
	IVE_MMZ_FREE(pstCannyInfo->stStack.u32PhyAddr,pstCannyInfo->stStack.pu8VirAddr);
    IVE_MMZ_FREE(pstCannyInfo->stCannyHysEdgeCtrl.stMem.u32PhyAddr,\
		pstCannyInfo->stCannyHysEdgeCtrl.stMem.pu8VirAddr);
    
    IVE_CLOSE_FILE(pstCannyInfo->pFpSrc);
    IVE_CLOSE_FILE(pstCannyInfo->pFpDst);  
}
/******************************************************************************
* function : Canny init
******************************************************************************/
static HI_S32 SAMPLE_IVE_Canny_Init(SAMPLE_IVE_CANNY_INFO_S *pstCannyInfo,
	HI_CHAR *pchSrcFileName,HI_CHAR *pchDstFileName,HI_U16 u16Width,HI_U16 u16Height)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Size = 0;
    HI_S8 as8Mask[25] = {0,0,0,0,0,
                         0,-1,0,1,0,
                         0,-2,0,2,0,
                         0,-1,0,1,0,
                         0,0,0,0,0};

	memset(pstCannyInfo,0,sizeof(SAMPLE_IVE_CANNY_INFO_S));
    memcpy(pstCannyInfo->stCannyHysEdgeCtrl.as8Mask,as8Mask,25);
    memcpy(pstCannyInfo->stMagAndAngCtrl.as8Mask,as8Mask,25);    
    pstCannyInfo->stCannyHysEdgeCtrl.u16HighThr = 150;
    pstCannyInfo->stCannyHysEdgeCtrl.u16LowThr = 50;
    pstCannyInfo->stMagAndAngCtrl.enOutCtrl = IVE_MAG_AND_ANG_OUT_CTRL_MAG;
    pstCannyInfo->stMagAndAngCtrl.u16Thr = 0;
    pstCannyInfo->stThrU16Ctrl.enMode = IVE_THRESH_U16_MODE_U16_TO_U8_MIN_MID_MAX;
    pstCannyInfo->stThrU16Ctrl.u16HighThr = 100;
    pstCannyInfo->stThrU16Ctrl.u16LowThr = 100;
    pstCannyInfo->stThrU16Ctrl.u8MaxVal = 255;
    pstCannyInfo->stThrU16Ctrl.u8MidVal = 0;
    pstCannyInfo->stThrU16Ctrl.u8MinVal = 0;

	s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstCannyInfo->stSrc,IVE_IMAGE_TYPE_U8C1,u16Width,u16Height);
    if(s32Ret != HI_SUCCESS)
    {
       SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
       goto CANNY_INIT_FAIL;
    }
	s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstCannyInfo->stEdge,IVE_IMAGE_TYPE_U8C1,u16Width,u16Height);
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
	   goto CANNY_INIT_FAIL;
	}    
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstCannyInfo->stMag,IVE_IMAGE_TYPE_U16C1,u16Width,u16Height);
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
	   goto CANNY_INIT_FAIL;
	} 
    u32Size = pstCannyInfo->stSrc.u16Stride [0] * pstCannyInfo->stSrc.u16Height * 4 + sizeof(IVE_CANNY_STACK_SIZE_S);
	s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&pstCannyInfo->stStack,u32Size);
    if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateMemInfo fail\n");
	   goto CANNY_INIT_FAIL;
	}
	u32Size = pstCannyInfo->stSrc.u16Stride [0] * pstCannyInfo->stSrc.u16Height * 3;
	s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&pstCannyInfo->stCannyHysEdgeCtrl.stMem,u32Size);
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateMemInfo fail\n");
	   goto CANNY_INIT_FAIL;
	}   
    
    pstCannyInfo->pFpSrc = fopen(pchSrcFileName,"rb");
    if(HI_NULL == pstCannyInfo->pFpSrc)
    {
        SAMPLE_PRT("Open file %s fail\n",pchSrcFileName);
        s32Ret = HI_FAILURE;
        goto CANNY_INIT_FAIL;
    }

    pstCannyInfo->pFpDst = fopen(pchDstFileName,"wb");
    if(HI_NULL == pstCannyInfo->pFpDst)
    {
        SAMPLE_PRT("Open file %s fail\n",pchDstFileName);
        s32Ret = HI_FAILURE;
        goto CANNY_INIT_FAIL;
    }

CANNY_INIT_FAIL:
    
    if(HI_SUCCESS != s32Ret)
	{
        SAMPLE_IVE_Canny_Uninit(pstCannyInfo);
	}
	return s32Ret;      
}
/******************************************************************************
* function : show complate canny sample
******************************************************************************/
static HI_VOID SAMPLE_IVE_Complate_Canny(SAMPLE_IVE_CANNY_INFO_S *pstCannyInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bInstant = HI_TRUE;  
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bFinish = HI_FALSE;
	IVE_HANDLE IveHandle;
    
    s32Ret = SAMPLE_COMM_IVE_ReadFile(&(pstCannyInfo->stSrc),pstCannyInfo->pFpSrc);
    if(s32Ret != HI_SUCCESS)
    {
       SAMPLE_PRT("SAMPLE_COMM_IVE_ReadFile fail\n");
       return;
    }

    s32Ret = HI_MPI_IVE_CannyHysEdge(&IveHandle, &pstCannyInfo->stSrc, \
		&pstCannyInfo->stEdge, &pstCannyInfo->stStack, \
		&pstCannyInfo->stCannyHysEdgeCtrl, bInstant);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_IVE_CannyHysEdge fail,Error(%#x)\n",s32Ret);
        return;
    }

    s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);	
	while(HI_ERR_IVE_QUERY_TIMEOUT == s32Ret)
	{
		usleep(100);					
		s32Ret = HI_MPI_IVE_Query(IveHandle,&bFinish,bBlock);	
	}
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_IVE_Query fail,Error(%#x)\n",s32Ret);
        return;
    }

    s32Ret = HI_MPI_IVE_CannyEdge(&pstCannyInfo->stEdge, &pstCannyInfo->stStack);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_IVE_CannyEdge fail,Error(%#x)\n",s32Ret);
        return;
    }

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstCannyInfo->stEdge,pstCannyInfo->pFpDst);
    if(s32Ret != HI_SUCCESS)
    {
       SAMPLE_PRT("SAMPLE_COMM_IVE_WriteFile fail\n");
       return;
    }
}
/******************************************************************************
* function : show part canny sample
******************************************************************************/
static HI_VOID SAMPLE_IVE_Part_Canny(SAMPLE_IVE_CANNY_INFO_S *pstCannyInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bInstant = HI_TRUE; 
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bFinish = HI_FALSE;
	IVE_HANDLE IveHandle;
    
    s32Ret = SAMPLE_COMM_IVE_ReadFile(&pstCannyInfo->stSrc,pstCannyInfo->pFpSrc);
    if(s32Ret != HI_SUCCESS)
    {
		SAMPLE_PRT("SAMPLE_COMM_IVE_ReadFile fail\n");
		return;
    }
	bInstant = HI_FALSE;
    s32Ret = HI_MPI_IVE_MagAndAng(&IveHandle, &pstCannyInfo->stSrc, &pstCannyInfo->stMag,\
		HI_NULL, &pstCannyInfo->stMagAndAngCtrl, bInstant);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_IVE_MagAndAng fail,Error(%#x)\n",s32Ret);
        return;
    }
	bInstant = HI_TRUE;
    s32Ret = HI_MPI_IVE_Thresh_U16(&IveHandle,&pstCannyInfo->stMag,&pstCannyInfo->stEdge,\
		&pstCannyInfo->stThrU16Ctrl, bInstant);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_IVE_Thresh_U16 fail,Error(%#x)\n",s32Ret);
        return;
    }
    
    s32Ret = HI_MPI_IVE_Query(IveHandle, &bFinish, bBlock);	
	while(HI_ERR_IVE_QUERY_TIMEOUT == s32Ret)
	{
		usleep(100);					
		s32Ret = HI_MPI_IVE_Query(IveHandle,&bFinish,bBlock);	
	}
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_IVE_Query fail,Error(%#x)\n",s32Ret);
        return;
    }

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstCannyInfo->stEdge,pstCannyInfo->pFpDst);
    if(s32Ret != HI_SUCCESS)
    {
       SAMPLE_PRT("SAMPLE_COMM_IVE_WriteFile fail\n");
       return;
    }
}
/******************************************************************************
* function : show canny sample
******************************************************************************/
HI_VOID SAMPLE_IVE_Canny(HI_CHAR chComplete)
{
	SAMPLE_IVE_CANNY_INFO_S stCannyInfo;
	HI_U16 u16Width = 720;
	HI_U16 u16Height = 576;
	HI_CHAR *pchSrcFileName = "./data/input/canny/canny.yuv";
	HI_CHAR achDstFileName[IVE_FILE_NAME_LEN];
	HI_S32 s32Ret;
	
    SAMPLE_COMM_IVE_CheckIveMpiInit();

	sprintf(achDstFileName,"./data/output/canny/cannyout_complete_%c.yuv",chComplete);
    s32Ret = SAMPLE_IVE_Canny_Init(&stCannyInfo,pchSrcFileName,achDstFileName,u16Width,u16Height);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_IVE_Canny_Init fail\n");
        goto CANNY_FAIL;
    }

    if('0' == chComplete)
    {
        SAMPLE_IVE_Part_Canny(&stCannyInfo);
    }
    else
    {
        SAMPLE_IVE_Complate_Canny(&stCannyInfo);
    }
   
    SAMPLE_IVE_Canny_Uninit(&stCannyInfo);

CANNY_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
}



