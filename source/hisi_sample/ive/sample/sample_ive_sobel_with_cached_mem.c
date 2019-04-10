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


typedef struct hiSAMPLE_IVE_SOBEL_S
{
	IVE_SRC_IMAGE_S stSrc1;
	IVE_SRC_IMAGE_S stSrc2;
	IVE_DST_IMAGE_S stDstH1;
	IVE_DST_IMAGE_S stDstH2;
	IVE_DST_IMAGE_S stDstV1;
	IVE_DST_IMAGE_S stDstV2;
	IVE_SOBEL_CTRL_S stSobelCtrl;
	FILE *pFpSrc;
	FILE *pFpDstH1;
	FILE *pFpDstH2;
	FILE *pFpDstV1;
	FILE *pFpDstV2;
}SAMPLE_IVE_SOBEL_S;

static HI_BOOL s_bFlushCached = HI_TRUE;
/******************************************************************************
* function : show Sobel uninit
******************************************************************************/
static HI_VOID SAMPLE_IVE_Sobel_Uninit(SAMPLE_IVE_SOBEL_S *pstSobel)
{
    IVE_MMZ_FREE(pstSobel->stSrc1.u32PhyAddr[0],pstSobel->stSrc1.pu8VirAddr[0]);
	IVE_MMZ_FREE(pstSobel->stSrc2.u32PhyAddr[0],pstSobel->stSrc2.pu8VirAddr[0]);
	IVE_MMZ_FREE(pstSobel->stDstH1.u32PhyAddr[0],pstSobel->stDstH1.pu8VirAddr[0]);
    IVE_MMZ_FREE(pstSobel->stDstH2.u32PhyAddr[0],pstSobel->stDstH2.pu8VirAddr[0]);
    IVE_MMZ_FREE(pstSobel->stDstV1.u32PhyAddr[0],pstSobel->stDstV1.pu8VirAddr[0]);
    IVE_MMZ_FREE(pstSobel->stDstV2.u32PhyAddr[0],pstSobel->stDstV2.pu8VirAddr[0]);
    
    IVE_CLOSE_FILE(pstSobel->pFpSrc);
    IVE_CLOSE_FILE(pstSobel->pFpDstH1);
	IVE_CLOSE_FILE(pstSobel->pFpDstH2);
    IVE_CLOSE_FILE(pstSobel->pFpDstV1);
	IVE_CLOSE_FILE(pstSobel->pFpDstV2);   
}
/******************************************************************************
* function : show Sobel init
******************************************************************************/
static HI_S32 SAMPLE_IVE_Sobel_Init(SAMPLE_IVE_SOBEL_S *pstSobel,
	HI_CHAR *pchSrcFileName,HI_U16 u16Width,HI_U16 u16Height)
{
    HI_S32 s32Ret = HI_SUCCESS;
	HI_CHAR achFileName[IVE_FILE_NAME_LEN];
    HI_S8 as8Mask[25] = {0,0,0,0,0,
                         0,-1,0,1,0,
                         0,-2,0,2,0,
                         0,-1,0,1,0,
                         0,0,0,0,0};
	
   	memset(pstSobel,0,sizeof(SAMPLE_IVE_SOBEL_S));

	s32Ret = SAMPLE_COMM_IVE_CreateImageByCached(&(pstSobel->stSrc1),IVE_IMAGE_TYPE_U8C1,u16Width,u16Height);
    if(s32Ret != HI_SUCCESS)
    {
       SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImageByCached fail\n");
       goto SOBEL_INIT_FAIL;
    }
	s32Ret = SAMPLE_COMM_IVE_CreateImageByCached(&(pstSobel->stSrc2),IVE_IMAGE_TYPE_U8C1,u16Width,u16Height);
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImageByCached fail\n");
	   goto SOBEL_INIT_FAIL;
	}
	s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstSobel->stDstH1),IVE_IMAGE_TYPE_S16C1,u16Width,u16Height);
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
	   goto SOBEL_INIT_FAIL;
	}
	s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstSobel->stDstH2),IVE_IMAGE_TYPE_S16C1,u16Width,u16Height);
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
	   goto SOBEL_INIT_FAIL;
	}
	s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstSobel->stDstV1),IVE_IMAGE_TYPE_S16C1,u16Width,u16Height);
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
	   goto SOBEL_INIT_FAIL;
	}
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstSobel->stDstV2),IVE_IMAGE_TYPE_S16C1,u16Width,u16Height);
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
	   goto SOBEL_INIT_FAIL;
	} 
    
    pstSobel->stSobelCtrl.enOutCtrl = IVE_SOBEL_OUT_CTRL_BOTH;
    memcpy(pstSobel->stSobelCtrl.as8Mask,as8Mask,25);
   
    pstSobel->pFpSrc = fopen(pchSrcFileName,"rb");
    if(HI_NULL == pstSobel->pFpSrc)
    {
        SAMPLE_PRT("Open file %s fail\n",pchSrcFileName);
        s32Ret = HI_FAILURE;
        goto SOBEL_INIT_FAIL;
    }
	
	sprintf(achFileName,"./data/output/sobel/sobelh1.dat");
    pstSobel->pFpDstH1 = fopen(achFileName,"wb");
    if(HI_NULL == pstSobel->pFpDstH1)
    {
        SAMPLE_PRT("Open file %s fail\n",achFileName);
        s32Ret = HI_FAILURE;
        goto SOBEL_INIT_FAIL;
    }
	sprintf(achFileName,"./data/output/sobel/sobelh2.dat");
	pstSobel->pFpDstH2 = fopen(achFileName,"wb");
	if(HI_NULL == pstSobel->pFpDstH2)
	{
		SAMPLE_PRT("Open file %s fail\n",achFileName);
		s32Ret = HI_FAILURE;
		goto SOBEL_INIT_FAIL;
	}
	sprintf(achFileName,"./data/output/sobel/sobelv1.dat");
	pstSobel->pFpDstV1 = fopen(achFileName,"wb");
	if(HI_NULL == pstSobel->pFpDstV1)
	{
		SAMPLE_PRT("Open file %s fail\n",achFileName);
		s32Ret = HI_FAILURE;
		goto SOBEL_INIT_FAIL;
	}
	sprintf(achFileName,"./data/output/sobel/sobelv2.dat");
	pstSobel->pFpDstV2 = fopen(achFileName,"wb");
	if(HI_NULL == pstSobel->pFpDstV2)
	{
		SAMPLE_PRT("Open file %s fail\n",achFileName);
		s32Ret = HI_FAILURE;
		goto SOBEL_INIT_FAIL;
	}

SOBEL_INIT_FAIL:
    
    if(HI_SUCCESS != s32Ret)
	{
        SAMPLE_IVE_Sobel_Uninit(pstSobel);
	}
	return s32Ret;      
}
/******************************************************************************
* function : show Sobel proc
******************************************************************************/
static HI_VOID SAMPLE_IVE_SobelProc(SAMPLE_IVE_SOBEL_S *pstSobel)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bInstant = HI_TRUE;
    IVE_HANDLE IveHandle;
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bFinish = HI_FALSE;

	s32Ret = SAMPLE_COMM_IVE_ReadFile(&(pstSobel->stSrc1),pstSobel->pFpSrc);
	if(s32Ret != HI_SUCCESS)
    {
		SAMPLE_PRT("SAMPLE_COMM_IVE_ReadFile fail\n");
		return;
    }

	memcpy(pstSobel->stSrc2.pu8VirAddr[0],pstSobel->stSrc1.pu8VirAddr[0],pstSobel->stSrc2.u16Stride[0] * pstSobel->stSrc2.u16Height);
    s32Ret = HI_MPI_SYS_MmzFlushCache(pstSobel->stSrc1.u32PhyAddr[0], pstSobel->stSrc1.pu8VirAddr[0],
		pstSobel->stSrc1.u16Stride[0]* pstSobel->stSrc1.u16Height);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_SYS_MmzFlushCache fail,Error(%#x)\n",s32Ret);
        return;
    }
    s32Ret = HI_MPI_IVE_Sobel(&IveHandle, &pstSobel->stSrc1, &pstSobel->stDstH1, &pstSobel->stDstV1, &pstSobel->stSobelCtrl,bInstant);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_IVE_Sobel fail,Error(%#x)\n",s32Ret);
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

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstSobel->stDstH1, pstSobel->pFpDstH1);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("SAMPLE_COMM_IVE_WriteFile fail\n");
        return;
    }

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstSobel->stDstV1, pstSobel->pFpDstV1);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("SAMPLE_COMM_IVE_WriteFile fail\n");
        return;
    }
 
    //Second sobel
	// The result of sobel my be error,if you do not call HI_MPI_SYS_MmzFlushCache to flush cache
    if(s_bFlushCached == HI_TRUE)
    {
        s32Ret = HI_MPI_SYS_MmzFlushCache(pstSobel->stSrc2.u32PhyAddr[0], pstSobel->stSrc2.pu8VirAddr[0], 
                            pstSobel->stSrc2.u16Stride[0]* pstSobel->stSrc2.u16Height);
        if (s32Ret != HI_SUCCESS)
    	{
	        SAMPLE_PRT("HI_MPI_SYS_MmzFlushCache fail,Error(%#x)\n",s32Ret);
	        return;
    	}
    }    
    
    s32Ret = HI_MPI_IVE_Sobel(&IveHandle, &pstSobel->stSrc2, &pstSobel->stDstH2, &pstSobel->stDstV2, &pstSobel->stSobelCtrl,bInstant);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_IVE_Sobel fail,Error(%#x)\n",s32Ret);
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

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstSobel->stDstH2, pstSobel->pFpDstH2);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("SAMPLE_COMM_IVE_WriteFile fail\n");
        return;
    }

    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstSobel->stDstV2, pstSobel->pFpDstV2);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("SAMPLE_COMM_IVE_WriteFile fail\n");
        return;
    }
    return;
}
/******************************************************************************
* function : show Sobel sample
******************************************************************************/
HI_VOID SAMPLE_IVE_Sobel(HI_VOID)
{
	HI_S32 s32Ret;
	SAMPLE_IVE_SOBEL_S stSobel;
	HI_U16 u16Width = 720;
	HI_U16 u16Height = 576;
	HI_CHAR *pchSrcFileName = "./data/input/sobel/sobel.yuv";
	
    SAMPLE_COMM_IVE_CheckIveMpiInit();
	
    s32Ret = SAMPLE_IVE_Sobel_Init(&stSobel,pchSrcFileName,u16Width,u16Height);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_IVE_Sobel_Init fail\n");
        goto SOBEL_FAIL;
    }

    SAMPLE_IVE_SobelProc(&stSobel);
    
    SAMPLE_IVE_Sobel_Uninit(&stSobel);

SOBEL_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
}



