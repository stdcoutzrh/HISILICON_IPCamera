#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"
#include "hi_comm_vb.h"
#include "mpi_vb.h"
#include "hi_comm_aio.h"
#include "mpi_aenc.h"


#define VALUE_BETWEEN(x,min,max) (((x)>=(min)) && ((x) <= (max)))

HI_S32 main(int argc, char *argv[])
{
    AENC_CHN AencChn;
    HI_U32 u32Size = 1024;
    AUDIO_SAVE_FILE_INFO_S stSaveFileInfo;
    HI_S32 s32Ret;
    FILE *fp = NULL;
    HI_CHAR aCurPath[258]; /*256 + '\n'+'\0'*/    
    HI_S32 s32CurrentPath = -1;

    printf("Usage: ./aenc_dump [chn] [name] [size] [path]\n");
    printf("chn: aenc chn id.\n");
	printf("name: file name    (default).\n");
    printf("size: file size(KB)(defaultly 1M).\n");
    printf("path: path for saving(current path if NULL).\n\n\n");

    if ((argc < 2)||(argc > 5))
    {
    	printf("wrong arg!!!!\n\n");
    	return -1;
    }

    AencChn = atoi(argv[1]);/* chn id*/
    if (!VALUE_BETWEEN(AencChn, 0, AENC_MAX_CHN_NUM))
    {
    	printf("chn id must be [0,32]!!!!\n\n");
    	return -1;
    }

	if (argc>=3)
    {
        if (!VALUE_BETWEEN(strlen(argv[2]), 0, 256))
        {
        	printf("path lenth must be [0,256]!!!!\n\n");
        	return -1;
        }
        memcpy(stSaveFileInfo.aFileName, argv[2], strlen(argv[2]));
        stSaveFileInfo.aFileName[strlen(argv[2])] = '\0';
    }
    else
    {
        memcpy(stSaveFileInfo.aFileName, "default", strlen("default")+1);
    }
	
    if (argc>=4)
    {
        if (!VALUE_BETWEEN(atoi(argv[3]), 1, 10*1024))
        {
        	printf("file size must be [1, 10*1024]!!!!\n\n");
        	return -1;
        }
        u32Size = atoi(argv[3]);/* frame count*/
    }
	
    if (argc>=5)
    {
        if (!VALUE_BETWEEN(strlen(argv[4]), 0, 256))
        {
        	printf("path lenth must be [0, 256]!!!!\n\n");
        	return -1;
        }
        memcpy(stSaveFileInfo.aFilePath,argv[4],strlen(argv[4]));
        stSaveFileInfo.aFilePath[strlen(argv[4])] = '\0';

        if (stSaveFileInfo.aFilePath[0] == '.' && (stSaveFileInfo.aFilePath[1] == '\0' || stSaveFileInfo.aFilePath[1] == '/'))
        {
            s32CurrentPath = 0;
        }
    }
    else
    {
        s32CurrentPath = 1;
    }

    if (s32CurrentPath != -1)
    {
        fp = popen("pwd", "r");
        fgets(aCurPath, sizeof(aCurPath), fp);
        if (!VALUE_BETWEEN(strlen(aCurPath), 0, 256))
        {
            printf("path lenth must be [0,256]!!!!\n\n");
            pclose(fp);
            return -1;
        }
        aCurPath[strlen(aCurPath) - 1] = '/'; /*replace '\n' with '/'*/

        if (s32CurrentPath == 0)
        {
            HI_U32 i = 2;
            HI_U32 u32Len = strlen(aCurPath);
            if (!VALUE_BETWEEN(strlen(aCurPath)+strlen(stSaveFileInfo.aFilePath), 0, 256))
            {
                printf("path lenth must be [0,256]!!!!\n\n");
                pclose(fp);
                return -1;
            }

            while (strlen(stSaveFileInfo.aFilePath) > 1 && stSaveFileInfo.aFilePath[i] != '\0')
            {
                
                aCurPath[u32Len+i-2] = stSaveFileInfo.aFilePath[i];

                i ++;
            }

            aCurPath[u32Len+i-2] = '\0';
        }
        snprintf(stSaveFileInfo.aFilePath, 256, "%s", aCurPath);
        
        pclose(fp);
        //memcpy(stSaveFileInfo.aFilePath,"./",strlen("./")+1);
    }
	
    stSaveFileInfo.u32FileSize = u32Size;
    stSaveFileInfo.bCfg = HI_TRUE;
	
    printf("File path:%s, file name:%s, file size:%d*1024\n\n", stSaveFileInfo.aFilePath, stSaveFileInfo.aFileName, stSaveFileInfo.u32FileSize);
    s32Ret = HI_MPI_AENC_SaveFile(AencChn,&stSaveFileInfo);
    if (s32Ret != HI_SUCCESS)
    {
    	printf("HI_MPI_AENC_SaveFile() error,ret=%x!!!!\n\n",s32Ret);
    	return -1;
    }

    return HI_SUCCESS;
}

