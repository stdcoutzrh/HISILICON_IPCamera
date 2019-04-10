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


typedef struct hiTEST_MEMORY_S
{
	IVE_SRC_IMAGE_S stSrc1;
	IVE_SRC_IMAGE_S stSrc2;
	IVE_DST_IMAGE_S stDst;	
	IVE_DST_MEM_INFO_S  stHist;
	IVE_SUB_CTRL_S stSubCtrl;
	FILE *pFpSrc;
	FILE *pFpDst;

}TEST_MEMORY_S;

/******************************************************************************
* function : test memory uninit
******************************************************************************/
static HI_VOID SAMPLE_IVE_TestMemory_Uninit(TEST_MEMORY_S *pstTestMem)
{
    IVE_MMZ_FREE(pstTestMem->stSrc1.u32PhyAddr[0],pstTestMem->stSrc1.pu8VirAddr[0]);
	IVE_MMZ_FREE(pstTestMem->stSrc2.u32PhyAddr[0],pstTestMem->stSrc2.pu8VirAddr[0]);
	IVE_MMZ_FREE(pstTestMem->stDst.u32PhyAddr[0],pstTestMem->stDst.pu8VirAddr[0]);
    IVE_MMZ_FREE(pstTestMem->stHist.u32PhyAddr,pstTestMem->stHist.pu8VirAddr);
    
    IVE_CLOSE_FILE(pstTestMem->pFpSrc);
    IVE_CLOSE_FILE(pstTestMem->pFpDst);   
}
/******************************************************************************
* function : test memory init
******************************************************************************/
static HI_S32 SAMPLE_IVE_TestMemory_Init(TEST_MEMORY_S *pstTestMem,HI_CHAR *pchSrcFileName,
	HI_CHAR *pchDstFileName,HI_U16 u16Width,HI_U16 u16Height)
{
    HI_S32 s32Ret;
    HI_U32 u32Size;

    memset(pstTestMem,0,sizeof(TEST_MEMORY_S));

	s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTestMem->stSrc1),IVE_IMAGE_TYPE_U8C1,u16Width,u16Height);
    if(s32Ret != HI_SUCCESS)
    {
       SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
       goto TEST_MEMORY_INIT_FAIL;
    }
	s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTestMem->stSrc2),IVE_IMAGE_TYPE_U8C1,u16Width,u16Height);
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
	   goto TEST_MEMORY_INIT_FAIL;
	}
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTestMem->stDst),IVE_IMAGE_TYPE_U8C1,u16Width,u16Height);
	if(s32Ret != HI_SUCCESS)
	{
	   SAMPLE_PRT("SAMPLE_COMM_IVE_CreateImage fail\n");
	   goto TEST_MEMORY_INIT_FAIL;
	}   
	
	u32Size = IVE_HIST_NUM * sizeof(HI_U32);
	s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstTestMem->stHist),u32Size);
    if(s32Ret != HI_SUCCESS)
    {
       SAMPLE_PRT("SAMPLE_COMM_IVE_CreateMemInfo fail\n");
       goto TEST_MEMORY_INIT_FAIL;
    }

    pstTestMem->stSubCtrl.enMode = IVE_SUB_MODE_ABS;
    
    pstTestMem->pFpSrc = fopen(pchSrcFileName,"rb");
    if(HI_NULL == pstTestMem->pFpSrc)
    {
        SAMPLE_PRT("Open file %s fail\n",pchSrcFileName);
        s32Ret = HI_FAILURE;
        goto TEST_MEMORY_INIT_FAIL;
    }

    pstTestMem->pFpDst = fopen(pchDstFileName,"wb");
    if(HI_NULL == pstTestMem->pFpDst)
    {
        SAMPLE_PRT("Open file %s fail\n",pchDstFileName);
        s32Ret = HI_FAILURE;
        goto TEST_MEMORY_INIT_FAIL;
    }

TEST_MEMORY_INIT_FAIL:
    
    if(HI_SUCCESS != s32Ret)
	{
        SAMPLE_IVE_TestMemory_Uninit(pstTestMem);
	}
	return s32Ret;      
}
/******************************************************************************
* function : test memory 
******************************************************************************/
static HI_VOID SAMPLE_IVE_TestMemoryProc(TEST_MEMORY_S *pstTestMem)
{
    HI_S32 s32Ret;   
	HI_U32 *pu32Hist;
	HI_U32 i;
    IVE_HANDLE IveHandle;
	HI_BOOL bInstant = HI_FALSE;
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bFinish = HI_FALSE;
    IVE_SRC_DATA_S stSrcData;
    IVE_DST_DATA_S stDstData;
    IVE_DMA_CTRL_S stDmaCtrl;
	 
    s32Ret = SAMPLE_COMM_IVE_ReadFile(&(pstTestMem->stSrc1),pstTestMem->pFpSrc);
    if(s32Ret != HI_SUCCESS)
    {
		SAMPLE_PRT("SAMPLE_COMM_IVE_ReadFile fail\n");
		return;
    }

    stDmaCtrl.enMode = IVE_DMA_MODE_DIRECT_COPY;
    
    stSrcData.pu8VirAddr = pstTestMem->stSrc1.pu8VirAddr[0];
    stSrcData.u32PhyAddr = pstTestMem->stSrc1.u32PhyAddr[0];
    stSrcData.u16Width = pstTestMem->stSrc1.u16Width;
    stSrcData.u16Height = pstTestMem->stSrc1.u16Height;
    stSrcData.u16Stride = pstTestMem->stSrc1.u16Stride[0];

    stDstData.pu8VirAddr = pstTestMem->stSrc2.pu8VirAddr[0];
    stDstData.u32PhyAddr = pstTestMem->stSrc2.u32PhyAddr[0];
    stDstData.u16Width = pstTestMem->stSrc2.u16Width;
    stDstData.u16Height = pstTestMem->stSrc2.u16Height;
    stDstData.u16Stride = pstTestMem->stSrc2.u16Stride[0];
    s32Ret = HI_MPI_IVE_DMA(&IveHandle,&stSrcData, &stDstData, 
                &stDmaCtrl, bInstant);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_IVE_DMA fail,Error(%#x)\n",s32Ret);
        return;
    }

    s32Ret = HI_MPI_IVE_Sub(&IveHandle, &pstTestMem->stSrc1, &pstTestMem->stSrc2, &pstTestMem->stDst,&pstTestMem->stSubCtrl, bInstant);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_IVE_Sub fail,Error(%#x)\n",s32Ret);
        return;
    }

    bInstant = HI_TRUE;
    s32Ret = HI_MPI_IVE_Hist(&IveHandle, &pstTestMem->stDst, &pstTestMem->stHist, bInstant);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_IVE_Hist fail,Error(%#x)\n",s32Ret);
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
    
    s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstTestMem->stDst,pstTestMem->pFpDst);
    if(s32Ret != HI_SUCCESS)
    {
       SAMPLE_PRT("SAMPLE_COMM_IVE_WriteFile fail\n");
       return;
    }

    pu32Hist = (HI_U32 *)pstTestMem->stHist.pu8VirAddr;

    if(pu32Hist[0] != pstTestMem->stSrc1.u16Width * pstTestMem->stSrc1.u16Height)
    {
       
        SAMPLE_PRT("Test mem error,pu32Hist[0] = %d\n",pu32Hist[0]);
        for(i = 1;i < IVE_HIST_NUM;i++)
        {
            if(pu32Hist[i] != 0)
            {
                SAMPLE_PRT("Test mem error, pu32Hist[%d] = %d\n",i,pu32Hist[i]);
            }
        }
    }
   
}
/******************************************************************************
* function : Show test memory sample
******************************************************************************/
HI_VOID SAMPLE_IVE_TestMemory(HI_VOID)
{
	HI_S32 s32Ret;
	TEST_MEMORY_S stTestMem;
	HI_U16 u16Width = 720;
	HI_U16 u16Height = 576;
	HI_CHAR *pchSrcFile = "./data/input/testmem/test_mem_in.yuv";
	HI_CHAR *pchDstFile = "./data/output/testmem/test_mem_out.yuv";

    SAMPLE_COMM_IVE_CheckIveMpiInit();
    s32Ret = SAMPLE_IVE_TestMemory_Init(&stTestMem,pchSrcFile,pchDstFile,u16Width,u16Height);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_IVE_TestMemory_Init fail\n");
        goto TEST_MEMORY_FAIL;
    }

    SAMPLE_IVE_TestMemoryProc(&stTestMem);
    
    SAMPLE_IVE_TestMemory_Uninit(&stTestMem);

TEST_MEMORY_FAIL:
    SAMPLE_COMM_IVE_IveMpiExit();
}




