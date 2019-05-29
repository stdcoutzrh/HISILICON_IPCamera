#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "mpi_vb.h"
#include "mpi_sys.h"
#include "hi_comm_isp.h"
#include "mpi_isp.h"
#include "hi_sns_ctrl.h"
#include "hi_ae_comm.h"
#include "hi_awb_comm.h"
#include "mpi_ae.h"
#include "mpi_awb.h"
#include "isp_dehaze.h"


HI_U32  au32Size[4]     = {0};
HI_U32  au32PhyAddr[4]  = {0};
HI_VOID* apVitAddr[4]   = {HI_NULL};    /* virt addr malloc memory */
HI_VOID* apVirtAddr[4]  = {HI_NULL};    /* virt addr mmap */
FILE*    afp[4]         = {HI_NULL};

extern HI_S32 HI_MPI_AE_Ctrl(ALG_LIB_S* pstAeLib, HI_U32 u32Cmd, HI_VOID* pValue);
extern HI_S32 HI_MPI_AWB_Ctrl(ALG_LIB_S* pstAwbLib, HI_U32 u32Cmd, HI_VOID* pValue);

extern HI_S32 ISP_DehazeCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue);

static inline HI_VOID Useage(HI_VOID)
{
    printf("\nNOTICE: This tool only can be used for DEBUG_ING !!!\n");
    printf("usage: ./Isp debug [ispdepth] [aedepth] [awbdepth][defogdepth]. sample: ./isp_debug 10 10 10 10\n");
    printf("[ispdepth]----------the frame number of isp\n");
    printf("[aedepth] ----------the frame number of ae\n");
    printf("[awbdepth]----------the frame number of awb\n\n");
    printf("[defogdepth]----------the frame number of defog\n\n");

    return;
}

HI_S32 ISP_DEBUG_Exit(HI_VOID)
{
    HI_S32 i, s32Ret = 0;
    ISP_DEBUG_INFO_S  stDebug;
    ALG_LIB_S stLib;

    // stop isp debug
    stDebug.bDebugEn = 0;
    stDebug.u32Depth = 0;
    stDebug.u32PhyAddr = 0;
    s32Ret = HI_MPI_ISP_SetDebug(0, &stDebug);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_ISP_SetDebug failed 0x%x! Line: %d\n", s32Ret, __LINE__);
        return HI_FAILURE;
    }

    // stop AE debug
    stDebug.bDebugEn = 0;
    stDebug.u32Depth = 0;
    stDebug.u32PhyAddr = 0;
    stLib.s32Id = 0;
    strcpy(stLib.acLibName, HI_AE_LIB_NAME);
    s32Ret = HI_MPI_AE_Ctrl(&stLib, AE_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_AE_Ctrl failed 0x%x!\n", s32Ret);
        return HI_FAILURE;
    }

    // stop AWB debug
    stDebug.bDebugEn = 0;
    stDebug.u32Depth = 0;
    stDebug.u32PhyAddr = 0;
    stLib.s32Id = 0;
    strcpy(stLib.acLibName, HI_AWB_LIB_NAME);
    s32Ret = HI_MPI_AWB_Ctrl(&stLib, AWB_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_ISP_AwbCtrl failed 0x%x!\n", s32Ret);
        return HI_FAILURE;
    }

    // stop DEHAZE debug
        stDebug.bDebugEn = 0;
        stDebug.u32Depth = 0;
        stDebug.u32PhyAddr = 0;         

        s32Ret = ISP_DehazeCtrl(0, DEHAZE_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
        if (HI_SUCCESS != s32Ret)
        {
            printf("ISP_DehazeCtrl failed 0x%x!\n", s32Ret);
            return HI_FAILURE;
        }

    sleep(1);

    for (i = 0; i < 4; i++)
    {
        if (HI_NULL != afp[i])
        {
            fclose(afp[i]);
            afp[i] = HI_NULL;
        }

        if ((HI_NULL != apVirtAddr[i]) && (0 != au32Size[i]))
        {
            HI_MPI_SYS_Munmap(apVirtAddr[i], au32Size[i]);
            apVirtAddr[i] = HI_NULL;
            au32Size[i] = 0;
        }

        if ((0 != au32PhyAddr[i]) && (HI_NULL != apVitAddr[i]))
        {
            HI_MPI_SYS_MmzFree(au32PhyAddr[i], apVitAddr[i]);
            au32PhyAddr[i] = 0;
            apVitAddr[i] = HI_NULL;
        }
    }

    return HI_SUCCESS;
}

HI_VOID ISP_DEBUG_HandleSig(HI_S32 s32SigNo)
{
    if (SIGINT == s32SigNo || SIGTERM == s32SigNo)
    {
        ISP_DEBUG_Exit();
    }

    exit(0);
}

HI_S32 main(int argc, char* argv[])
{
    HI_S32 i, s32Ret = 0;

    HI_U32  au32Depth[4]    = {0};
    HI_CHAR* acFilename[4]  = {HI_NULL};
    ISP_DEBUG_INFO_S  stDebug;
    ALG_LIB_S stLib;

    signal(SIGINT, ISP_DEBUG_HandleSig);
    signal(SIGTERM, ISP_DEBUG_HandleSig);

    Useage();

    if ((argc <= 1) || (argc > 5))
    {
        Useage();
        return HI_FAILURE;
    }

    if (argc > 1)
    {
        au32Depth[0] = atoi(argv[1]);/* the frame depth */
        if (0 == au32Depth[0])
        {
            Useage();
            return HI_FAILURE;
        }
    }

    if (argc > 2)
    {
        au32Depth[1] = atoi(argv[2]);/* the frame depth */
        if (0 == au32Depth[1])
        {
            Useage();
            return HI_FAILURE;
        }
    }

    if (argc > 3)
    {
        au32Depth[2] = atoi(argv[3]);/* the frame depth */
        if (0 == au32Depth[2])
        {
            Useage();
            return HI_FAILURE;
        }
    }

    if (argc > 4)
    {
        au32Depth[3] = atoi(argv[4]);/* the frame depth */
        if (0 == au32Depth[3])
        {
            Useage();
            return HI_FAILURE;
        }
    }
 

    if (argc > 1)
    {
        au32Size[0] = sizeof(ISP_DBG_ATTR_S) + sizeof(ISP_DBG_STATUS_S) * au32Depth[0];
        s32Ret = HI_MPI_SYS_MmzAlloc_Cached(&au32PhyAddr[0], &apVitAddr[0], HI_NULL, HI_NULL, au32Size[0]);
		
        if (HI_SUCCESS != s32Ret)
        {
           stDebug.bDebugEn = HI_FALSE;
		   
             printf("Buf not enough!\n");
            return HI_FAILURE;
        }

        stDebug.bDebugEn = HI_TRUE;
        stDebug.u32Depth = au32Depth[0];
        stDebug.u32PhyAddr = au32PhyAddr[0];
        s32Ret = HI_MPI_ISP_SetDebug(0, &stDebug);
        if (HI_SUCCESS != s32Ret)
        {
            stDebug.bDebugEn = HI_FALSE;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			HI_MPI_ISP_SetDebug(0, &stDebug);
            printf("HI_MPI_ISP_SetDebug failed 0x%x!\n", s32Ret);
            return HI_FAILURE;
        }
    }

    if (argc > 2)
    {
        au32Size[1] = sizeof(AE_DBG_ATTR_S) + sizeof(AE_DBG_STATUS_S) * au32Depth[1];
        s32Ret = HI_MPI_SYS_MmzAlloc_Cached(&au32PhyAddr[1], &apVitAddr[1], HI_NULL, HI_NULL, au32Size[1]);
        if (HI_SUCCESS != s32Ret)
        {   
            stDebug.bDebugEn = HI_FALSE;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			HI_MPI_ISP_SetDebug(0, &stDebug);
            printf("Buf not enough!\n");
            return HI_FAILURE;
        }

        stDebug.bDebugEn = HI_TRUE;
        stDebug.u32Depth = au32Depth[1];
        stDebug.u32PhyAddr = au32PhyAddr[1];
        stLib.s32Id = 0;
        strcpy(stLib.acLibName, HI_AE_LIB_NAME);
        s32Ret = HI_MPI_AE_Ctrl(&stLib, AE_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
        if (HI_SUCCESS != s32Ret)
        {
			stDebug.bDebugEn = HI_FALSE;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			HI_MPI_ISP_SetDebug(0, &stDebug);
			stDebug.bDebugEn = 0;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			stLib.s32Id = 0;
			strcpy(stLib.acLibName, HI_AE_LIB_NAME);
			HI_MPI_AE_Ctrl(&stLib, AE_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
            printf("HI_MPI_AE_Ctrl failed 0x%x!\n", s32Ret);
            return HI_FAILURE;
        }
    }

    if (argc > 3)
    {
        au32Size[2] = sizeof(AWB_DBG_ATTR_S) + sizeof(AWB_DBG_STATUS_S) * au32Depth[2];
        s32Ret = HI_MPI_SYS_MmzAlloc_Cached(&au32PhyAddr[2], &apVitAddr[2], HI_NULL, HI_NULL, au32Size[2]);
        if (HI_SUCCESS != s32Ret)
        {
        	stDebug.bDebugEn = HI_FALSE;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			HI_MPI_ISP_SetDebug(0, &stDebug);
			stDebug.bDebugEn = 0;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			stLib.s32Id = 0;
			strcpy(stLib.acLibName, HI_AE_LIB_NAME);
			HI_MPI_AE_Ctrl(&stLib, AE_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
            printf("Buf not enough!\n");
            return HI_FAILURE;
        }

        stDebug.bDebugEn = HI_TRUE;
        stDebug.u32Depth = au32Depth[2];
        stDebug.u32PhyAddr = au32PhyAddr[2];
        stLib.s32Id = 0;
        strcpy(stLib.acLibName, HI_AWB_LIB_NAME);
        s32Ret = HI_MPI_AWB_Ctrl(&stLib, AWB_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
        if (HI_SUCCESS != s32Ret)
        {
        	stDebug.bDebugEn = HI_FALSE;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			HI_MPI_ISP_SetDebug(0, &stDebug);
			stDebug.bDebugEn = 0;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			stLib.s32Id = 0;
			strcpy(stLib.acLibName, HI_AE_LIB_NAME);
			HI_MPI_AE_Ctrl(&stLib, AE_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
			stDebug.bDebugEn = 0;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			stLib.s32Id = 0;
			strcpy(stLib.acLibName, HI_AWB_LIB_NAME);
			HI_MPI_AWB_Ctrl(&stLib, AWB_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
            printf("HI_MPI_ISP_AwbCtrl failed 0x%x!\n", s32Ret);
            return HI_FAILURE;
        }
    }

     
    if (argc > 4)
       {
           au32Size[3] = sizeof(DEHAZE_DBG_ATTR_S) + sizeof(DEHAZE_DBG_STATUS_S) * au32Depth[3];
           s32Ret = HI_MPI_SYS_MmzAlloc_Cached(&au32PhyAddr[3], &apVitAddr[3], HI_NULL, HI_NULL, au32Size[3]);
         
           if (HI_SUCCESS != s32Ret)
           {
			stDebug.bDebugEn = HI_FALSE;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			HI_MPI_ISP_SetDebug(0, &stDebug);
			stDebug.bDebugEn = 0;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			stLib.s32Id = 0;
			strcpy(stLib.acLibName, HI_AE_LIB_NAME);
			HI_MPI_AE_Ctrl(&stLib, AE_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
			stDebug.bDebugEn = 0;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			stLib.s32Id = 0;
			strcpy(stLib.acLibName, HI_AWB_LIB_NAME);
			HI_MPI_AWB_Ctrl(&stLib, AWB_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
             printf("Buf not enough!\n");
               return HI_FAILURE;
           }
    
           stDebug.bDebugEn = HI_TRUE;
           stDebug.u32Depth = au32Depth[3];
           stDebug.u32PhyAddr = au32PhyAddr[3];
           stLib.s32Id = 0;
       
           s32Ret = ISP_DehazeCtrl(0, DEHAZE_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
           if (HI_SUCCESS != s32Ret)
           {
           	stDebug.bDebugEn = HI_FALSE;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			HI_MPI_ISP_SetDebug(0, &stDebug);
			stDebug.bDebugEn = 0;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			stLib.s32Id = 0;
			strcpy(stLib.acLibName, HI_AE_LIB_NAME);
			HI_MPI_AE_Ctrl(&stLib, AE_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
			stDebug.bDebugEn = 0;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			stLib.s32Id = 0;
			strcpy(stLib.acLibName, HI_AWB_LIB_NAME);
			HI_MPI_AWB_Ctrl(&stLib, AWB_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
			stDebug.bDebugEn = HI_FALSE;
			stDebug.u32Depth = 0;
			stDebug.u32PhyAddr = 0;
			stLib.s32Id = 0;
			ISP_DehazeCtrl(0, DEHAZE_DEBUG_ATTR_SET, (HI_VOID*)&stDebug);
			printf("HI_MPI_ISP_DehazeCtrl failed 0x%x!\n", s32Ret);
			return HI_FAILURE;
           }
       }

    printf("Waiting, press any key to stop write debug info!\n");
    getchar();

    for (i = 0; i < argc - 1; i++)
    {
        apVirtAddr[i] = HI_MPI_SYS_Mmap(au32PhyAddr[i], au32Size[i]);
        if (HI_NULL == apVirtAddr[i])
        {
           stDebug.bDebugEn = HI_FALSE;
            printf("HI_MPI_SYS_Mmap failed!\n");
            return HI_FAILURE;
        }

        if (0 == i)
        {
            acFilename[i] = "isp_debug_sys.dat";
        }
        else if (1 == i)
        {
            acFilename[i] = "isp_debug_ae.dat";
        }
        else if (2 == i)
        {
            acFilename[i] = "isp_debug_awb.dat";
        }
        else if(3 == i)
        {
            acFilename[i] = "isp_debug_defog.dat";

        }
        else
        {
        }

        afp[i] = fopen(acFilename[i], "wb");
        if (afp[i] == HI_NULL)
        {
            stDebug.bDebugEn = HI_FALSE;
            printf("open file %s error \n", acFilename[i]);
            return HI_FAILURE;
        }
        else
        {
            if (3 == i)
            {
               s32Ret = ISP_DehazeCtrl(0, DEHAZE_DEBUG_WAIT_END, HI_NULL);
               if (HI_SUCCESS != s32Ret)
               {   
                   stDebug.bDebugEn = HI_FALSE;
                   printf("DEHAZE_DEBUG_WAIT_END failed 0x%x!\n", s32Ret);
                   goto EXIT;
               }
            }
            
            fwrite(apVirtAddr[i], 1, au32Size[i], afp[i]);
        }
    }

EXIT:
    ISP_DEBUG_Exit();

    return HI_SUCCESS;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


