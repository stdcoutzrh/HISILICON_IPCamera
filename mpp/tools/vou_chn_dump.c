#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"
#include "hi_comm_vb.h"
#include "mpi_vb.h"
#include "hi_comm_vo.h"
#include "mpi_vo.h"
#include "mpi_vgs.h"

typedef struct hiDUMP_MEMBUF_S
{
    VB_BLK  hBlock;
    VB_POOL hPool;
    HI_U32  u32PoolId;

    HI_U32  u32PhyAddr;
    HI_U8*   pVirAddr;
    HI_S32  s32Mdev;
} DUMP_MEMBUF_S;

static VIDEO_FRAME_INFO_S g_stFrame;
static char* g_pVBufVirt_Y = NULL;
static char* g_pVBufVirt_C = NULL;
static HI_U32 g_Ysize, g_Csize;
static FILE* g_pfd = NULL;
static VB_POOL g_hPool  = VB_INVALID_POOLID;
static DUMP_MEMBUF_S g_stMem;
static VO_CHN g_VoChn = 0;


/******************************************************************************
* function : to process abnormal case
******************************************************************************/
void VOU_Chn_HandleSig(HI_S32 signo)
{
    if (SIGINT == signo || SIGTERM == signo)
    {
        if (0 != g_stFrame.stVFrame.u32PhyAddr[0])
        {
            HI_MPI_VO_ReleaseChnFrame(0, g_VoChn, &g_stFrame);
            memset(&g_stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
        }

        if (NULL != g_pVBufVirt_Y)
        {
            HI_MPI_SYS_Munmap(g_pVBufVirt_Y, g_Ysize);
            g_pVBufVirt_Y = NULL;
        }

        if (NULL != g_pVBufVirt_C)
        {
            HI_MPI_SYS_Munmap(g_pVBufVirt_C, g_Csize);
            g_pVBufVirt_C = NULL;
        }

        if (NULL != g_pfd)
        {
            fclose(g_pfd);
            g_pfd = NULL;
        }

        if (VB_INVALID_HANDLE != g_stMem.hBlock)
        {
            HI_MPI_VB_ReleaseBlock(g_stMem.hBlock);
            g_stMem.hBlock = VB_INVALID_HANDLE;
        }

        if (VB_INVALID_POOLID != g_hPool)
        {
            HI_MPI_VB_DestroyPool( g_hPool );
        }

        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }
    exit(-1);
}



/* sp420 to p420 ; sp422 to p422  */
void sample_yuv_dump(VIDEO_FRAME_S* pVBuf)
{
    unsigned int w, h;
    char* pMemContent;
    unsigned char TmpBuff[4096];
    HI_U32 phy_addr;
    PIXEL_FORMAT_E  enPixelFormat = pVBuf->enPixelFormat;
    HI_U32 u32UvHeight;

    g_Ysize = (pVBuf->u32Stride[0]) * (pVBuf->u32Height);
    if (PIXEL_FORMAT_YUV_SEMIPLANAR_420 == enPixelFormat)
    {
        g_Csize = (pVBuf->u32Stride[1]) * (pVBuf->u32Height) / 2;
        u32UvHeight = pVBuf->u32Height / 2;
    }
    else if (PIXEL_FORMAT_YUV_SEMIPLANAR_422 == enPixelFormat)
    {
        g_Csize = (pVBuf->u32Stride[1]) * (pVBuf->u32Height);
        u32UvHeight = pVBuf->u32Height;
    }
    else
    {
        g_Csize = 0;
        u32UvHeight = 0;
    }

    phy_addr = pVBuf->u32PhyAddr[0];

    g_pVBufVirt_Y = (HI_CHAR*) HI_MPI_SYS_Mmap(phy_addr, g_Ysize);
    if (NULL == g_pVBufVirt_Y)
    {
        return;
    }

    /* save Y ----------------------------------------------------------------*/
    fprintf(stderr, "saving......Y......");
    fflush(stderr);
    for (h = 0; h < pVBuf->u32Height; h++)
    {
        pMemContent = g_pVBufVirt_Y + h * pVBuf->u32Stride[0];
        fwrite(pMemContent, pVBuf->u32Width, 1, g_pfd);
    }
    fflush(g_pfd);

    if (PIXEL_FORMAT_YUV_400 != enPixelFormat)
    {
        g_pVBufVirt_C = (HI_CHAR*) HI_MPI_SYS_Mmap(pVBuf->u32PhyAddr[1], g_Csize);
        if (NULL == g_pVBufVirt_C)
        {
            HI_MPI_SYS_Munmap(g_pVBufVirt_Y, g_Ysize);
            g_pVBufVirt_Y = NULL;
            return;
        }

        /* save U ----------------------------------------------------------------*/
        fprintf(stderr, "U......");
        fflush(stderr);
        for (h = 0; h < u32UvHeight; h++)
        {
            pMemContent = g_pVBufVirt_C + h * pVBuf->u32Stride[1];

            pMemContent += 1;

            for (w = 0; w < pVBuf->u32Width / 2; w++)
            {
                TmpBuff[w] = *pMemContent;
                pMemContent += 2;
            }
            fwrite(TmpBuff, pVBuf->u32Width / 2, 1, g_pfd);
        }
        fflush(g_pfd);

        /* save V ----------------------------------------------------------------*/
        fprintf(stderr, "V......");
        fflush(stderr);
        for (h = 0; h < u32UvHeight; h++)
        {
            pMemContent = g_pVBufVirt_C + h * pVBuf->u32Stride[1];

            for (w = 0; w < pVBuf->u32Width / 2; w++)
            {
                TmpBuff[w] = *pMemContent;
                pMemContent += 2;
            }
            fwrite(TmpBuff, pVBuf->u32Width / 2, 1, g_pfd);
        }
        fflush(g_pfd);
        HI_MPI_SYS_Munmap(g_pVBufVirt_C, g_Csize);
        g_pVBufVirt_C = NULL;
    }
    fprintf(stderr, "done %d!\n", pVBuf->u32TimeRef);
    fflush(stderr);

    HI_MPI_SYS_Munmap(g_pVBufVirt_Y, g_Ysize);
    g_pVBufVirt_Y = NULL;

}

HI_S32 SAMPLE_MISC_VoDump(VO_DEV VoDev, HI_S32 s32Cnt)
{
    HI_S32 i, s32Ret;
    HI_CHAR szYuvName[128];
    HI_CHAR szPixFrm[10];
    HI_U32  u32BlkSize = 0;

    VIDEO_FRAME_INFO_S stFrmInfo;
    VGS_HANDLE hHandle;
    VGS_TASK_ATTR_S stTask;
    HI_U32 u32LumaSize              = 0;
    HI_U32 u32ChrmSize              = 0;
    HI_U32 u32PicLStride            = 0;
    HI_U32 u32PicCStride            = 0;
    HI_U32 u32Width                 = 0;
    HI_U32 u32Height                = 0;
    HI_BOOL bSendToVgs              = HI_FALSE;

    /* Get Frame to make file name*/
    s32Ret = HI_MPI_VO_GetChnFrame(VoDev, g_VoChn, &g_stFrame, 0);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VO(%d)_GetChnFrame errno %#x\n", VoDev, s32Ret);
        return -1;
    }

    /* make file name */
    if (PIXEL_FORMAT_YUV_SEMIPLANAR_420 == g_stFrame.stVFrame.enPixelFormat)
    {
        strcpy(szPixFrm, "p420");
    }
    else if (PIXEL_FORMAT_YUV_SEMIPLANAR_422 == g_stFrame.stVFrame.enPixelFormat)
    {
        strcpy(szPixFrm, "p422");
    }
    else if (PIXEL_FORMAT_YUV_400 == g_stFrame.stVFrame.enPixelFormat)
    {
        strcpy(szPixFrm, "single");
    }
    else
    {
        return -1;
    }

    sprintf(szYuvName, "./vo(%d,%d)_%d_%d_%s_%d.yuv", VoDev, g_VoChn,
            g_stFrame.stVFrame.u32Width, g_stFrame.stVFrame.u32Height, szPixFrm, s32Cnt);
    printf("Dump YUV frame of vo(%d,%d) to file: \"%s\"\n", VoDev, g_VoChn, szYuvName);

    bSendToVgs = (g_stFrame.stVFrame.enCompressMode > 0) || (g_stFrame.stVFrame.enVideoFormat > 0);

    HI_MPI_VO_ReleaseChnFrame(VoDev, g_VoChn, &g_stFrame);

    /* open file */
    g_pfd = fopen(szYuvName, "wb");

    if (NULL == g_pfd)
    {
        return -1;
    }

    u32PicLStride = g_stFrame.stVFrame.u32Stride[0];
    u32PicCStride = g_stFrame.stVFrame.u32Stride[0];
    u32LumaSize = g_stFrame.stVFrame.u32Stride[0] * g_stFrame.stVFrame.u32Height;
    u32Width    = g_stFrame.stVFrame.u32Width;
    u32Height   = g_stFrame.stVFrame.u32Height;
    if (PIXEL_FORMAT_YUV_400 == g_stFrame.stVFrame.enPixelFormat)
    {
        u32ChrmSize = 0;
    }
    else
    {
        u32ChrmSize = (g_stFrame.stVFrame.u32Stride[0] * g_stFrame.stVFrame.u32Height) >> (1 + (PIXEL_FORMAT_YUV_SEMIPLANAR_420 == g_stFrame.stVFrame.enPixelFormat));
    }

    if (bSendToVgs)
    {
        if (PIXEL_FORMAT_YUV_400 == g_stFrame.stVFrame.enPixelFormat)
        {
            u32BlkSize = g_stFrame.stVFrame.u32Stride[0] * g_stFrame.stVFrame.u32Height;
        }
        else
        {
            u32BlkSize = (PIXEL_FORMAT_YUV_SEMIPLANAR_420 == g_stFrame.stVFrame.enPixelFormat)
                         ? (g_stFrame.stVFrame.u32Stride[0] * g_stFrame.stVFrame.u32Height * 3 >> 1) : (g_stFrame.stVFrame.u32Stride[0] * g_stFrame.stVFrame.u32Height * 2);
        }

        /*create comm vb pool*/
        g_hPool   = HI_MPI_VB_CreatePool( u32BlkSize, 1, NULL);
        if (g_hPool == VB_INVALID_POOLID)
        {
            printf("HI_MPI_VB_CreatePool failed! \n");
            goto END1;
        }

        g_stMem.hPool = g_hPool;
    }

    /* get VO frame  */
    for (i = 0; i < s32Cnt; i++)
    {
        s32Ret = HI_MPI_VO_GetChnFrame(VoDev, g_VoChn, &g_stFrame, 20);
        if (HI_SUCCESS != s32Ret)
        {
            printf("get vo(%d,%d) frame err\n", VoDev, g_VoChn);
            printf("only get %d frame\n", i);
            break;
        }

        if (bSendToVgs)
        {
            while ((g_stMem.hBlock = HI_MPI_VB_GetBlock(g_stMem.hPool, u32BlkSize, NULL)) == VB_INVALID_HANDLE)
            {
                ;
            }

            g_stMem.u32PhyAddr = HI_MPI_VB_Handle2PhysAddr(g_stMem.hBlock);


            g_stMem.pVirAddr = (HI_U8*) HI_MPI_SYS_Mmap( g_stMem.u32PhyAddr, u32BlkSize );
            if (g_stMem.pVirAddr == NULL)
            {
                printf("Mem dev may not open\n");
                HI_MPI_VB_ReleaseBlock(g_stMem.hBlock);
                goto END2;
            }

            memset(&stFrmInfo.stVFrame, 0, sizeof(VIDEO_FRAME_S));
            stFrmInfo.stVFrame.u32PhyAddr[0] = g_stMem.u32PhyAddr;
            stFrmInfo.stVFrame.u32PhyAddr[1] = stFrmInfo.stVFrame.u32PhyAddr[0] + u32LumaSize;
            stFrmInfo.stVFrame.u32PhyAddr[2] = stFrmInfo.stVFrame.u32PhyAddr[1] + u32ChrmSize;

            stFrmInfo.stVFrame.pVirAddr[0] = g_stMem.pVirAddr;
            stFrmInfo.stVFrame.pVirAddr[1] = (HI_U8*) stFrmInfo.stVFrame.pVirAddr[0] + u32LumaSize;
            stFrmInfo.stVFrame.pVirAddr[2] = (HI_U8*) stFrmInfo.stVFrame.pVirAddr[1] + u32ChrmSize;

            stFrmInfo.stVFrame.u32Width     = u32Width;
            stFrmInfo.stVFrame.u32Height    = u32Height;
            stFrmInfo.stVFrame.u32Stride[0] = u32PicLStride;
            stFrmInfo.stVFrame.u32Stride[1] = u32PicCStride;
            stFrmInfo.stVFrame.u32Stride[2] = u32PicCStride;

            stFrmInfo.stVFrame.enCompressMode = COMPRESS_MODE_NONE;
            stFrmInfo.stVFrame.enPixelFormat  = g_stFrame.stVFrame.enPixelFormat;
            stFrmInfo.stVFrame.enVideoFormat  = VIDEO_FORMAT_LINEAR;

            stFrmInfo.stVFrame.u64pts     = (i * 40);
            stFrmInfo.stVFrame.u32TimeRef = (i * 2);

            stFrmInfo.u32PoolId = g_hPool;

            s32Ret = HI_MPI_VGS_BeginJob(&hHandle);
            if (s32Ret != HI_SUCCESS)
            {
                printf("HI_MPI_VGS_BeginJob failed\n");
                HI_MPI_VB_ReleaseBlock(g_stMem.hBlock);
                g_stMem.hBlock = VB_INVALID_HANDLE;
                HI_MPI_VO_ReleaseChnFrame(VoDev, g_VoChn, &g_stFrame);
                goto END2;
            }

            memcpy(&stTask.stImgIn, &g_stFrame, sizeof(VIDEO_FRAME_INFO_S));
            memcpy(&stTask.stImgOut , &stFrmInfo, sizeof(VIDEO_FRAME_INFO_S));
            s32Ret = HI_MPI_VGS_AddScaleTask(hHandle, &stTask);
            if (s32Ret != HI_SUCCESS)
            {
                printf("HI_MPI_VGS_AddScaleTask failed\n");
                HI_MPI_VGS_CancelJob(hHandle);
                HI_MPI_VB_ReleaseBlock(g_stMem.hBlock);
                g_stMem.hBlock = VB_INVALID_HANDLE;
                HI_MPI_VO_ReleaseChnFrame(VoDev, g_VoChn, &g_stFrame);
                goto END2;
            }

            s32Ret = HI_MPI_VGS_EndJob(hHandle);
            if (s32Ret != HI_SUCCESS)
            {
                printf("HI_MPI_VGS_EndJob failed\n");
                HI_MPI_VGS_CancelJob(hHandle);
                HI_MPI_VB_ReleaseBlock(g_stMem.hBlock);
                g_stMem.hBlock = VB_INVALID_HANDLE;
                HI_MPI_VO_ReleaseChnFrame(VoDev, g_VoChn, &g_stFrame);
                goto END2;
            }

            /* save VO frame to file */
            sample_yuv_dump(&stFrmInfo.stVFrame);

            HI_MPI_VB_ReleaseBlock(g_stMem.hBlock);
            g_stMem.hBlock = VB_INVALID_HANDLE;
        }
        else
        {
            /* save VO frame to file */
            sample_yuv_dump(&g_stFrame.stVFrame);
        }

        /* release frame after using */
        s32Ret = HI_MPI_VO_ReleaseChnFrame(VoDev, g_VoChn, &g_stFrame);
        memset(&g_stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
        if (HI_SUCCESS != s32Ret)
        {
            printf("Release vo(%d,%d) frame err\n", VoDev, g_VoChn);
            printf("only get %d frame\n", i);
            break;
        }
    }


END2:
    if (VB_INVALID_POOLID != g_hPool)
    {
        HI_MPI_VB_DestroyPool(g_hPool);
        g_hPool = VB_INVALID_POOLID;
    }
    memset(&g_stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));

END1:

    fclose(g_pfd);

    return 0;
}

HI_S32 main(int argc, char* argv[])
{
    VO_DEV VoDev = 0;
    HI_S32 s32FrmCnt = 1;

    printf("\nNOTICE: This tool only can be used for TESTING !!!\n");
    printf("usage: ./vou_chn_dump [vodev] [vochn] [frmcnt]. sample: ./vou_chn_dump 0 0 5\n\n");

    if (argc > 4)
    {
        printf("Too many parameters!\n");
        return HI_FAILURE;
    }

    if (argc > 1)
    {
        VoDev = atoi(argv[1]);
    }

    if (argc > 2)
    {
        g_VoChn = atoi(argv[2]);
    }

    if (argc > 3)
    {
        s32FrmCnt = atoi(argv[3]);
        if (s32FrmCnt <= 0)
        {
            printf("The frmcnt(%d) is wrong!\n", s32FrmCnt);
            return HI_FAILURE;
        }
    }

    signal(SIGINT, VOU_Chn_HandleSig);
    signal(SIGTERM, VOU_Chn_HandleSig);

    SAMPLE_MISC_VoDump(VoDev, s32FrmCnt);

    return HI_SUCCESS;
}


