#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "loadbmp.h"

OSD_COMP_INFO s_OSDCompInfo[OSD_COLOR_FMT_BUTT] = {{0, 4, 4, 4},   /*RGB444*/
                                                    {4, 4, 4, 4},   /*ARGB4444*/
                                                    {0, 5, 5, 5},   /*RGB555*/
                                                    {0, 5, 6, 5},   /*RGB565*/
                                                    {1, 5, 5, 5},   /*ARGB1555*/
                                                    {0, 0, 0, 0},   /*RESERVED*/
                                                    {0, 8, 8, 8},   /*RGB888*/
                                                    {8, 8, 8, 8}    /*ARGB8888*/
                                                    };
inline HI_U16 OSD_MAKECOLOR_U16(HI_U8 r, HI_U8 g, HI_U8 b, OSD_COMP_INFO compinfo)
{
    HI_U8 r1, g1, b1;
    HI_U16 pixel = 0;
    HI_U32 tmp = 15;

    r1 = g1 = b1 = 0;
    r1 = r >> (8 - compinfo.rlen);
    g1 = g >> (8 - compinfo.glen);
    b1 = b >> (8 - compinfo.blen);
    while(compinfo.alen)
    {
        pixel |= (1 << tmp);
        tmp --;
        compinfo.alen--;
    }
    
    pixel |= (r1 | (g1 << compinfo.blen) | (b1 << (compinfo.blen + compinfo.glen)));
    return pixel;
}

HI_S32 GetBmpInfo(const char *filename,     OSD_BITMAPFILEHEADER  *pBmpFileHeader
,OSD_BITMAPINFO *pBmpInfo)
{
    FILE *pFile;

    HI_U16    bfType;

    if(NULL == filename)
    {
        printf("OSD_LoadBMP: filename=NULL\n");
        return -1;
    }

    if( (pFile = fopen((char *)filename, "rb")) == NULL)
    {
        printf("Open file faild:%s!\n", filename);
        return -1;
    }

    (void)fread(&bfType, 1, sizeof(bfType), pFile);
    if(bfType != 0x4d42)
    {
        printf("not bitmap file\n");
        fclose(pFile);
        return -1;
    }
    
    (void)fread(pBmpFileHeader, 1, sizeof(OSD_BITMAPFILEHEADER), pFile);
    (void)fread(pBmpInfo, 1, sizeof(OSD_BITMAPINFO), pFile);
    fclose(pFile);
    
    return 0;  
}


