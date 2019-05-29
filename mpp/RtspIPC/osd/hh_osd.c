/**************************************************************************
 * 	FileName:		osd.c
 *	Description:	OSD Process(×ÖÄ»µþ¼Ó)
 *	Copyright(C):	2006-2008 HHDigital Inc.
 *	Version:		V 1.0
 *	Author:			ChenYG
 *	Created:		2006-08-06
 *	Updated:
 *
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

//#include "global_config.h"
#include "hh_osd.h"
#include "hh_osd_api.h"
//#include "toolbox.h"
//#include "syslog.h"



HH_VIDEO_RESOLUTION ipc_conf;

int UART_TEMP_FLAG;

HH_OSD_LOGO		*pGOsdLog[HH_MAX_OSD_NUM];	//={0}
VIDEO_OSD_STATUS *pvideo_osd_status;

#if 0
static char *S_WEEK[7]   = {"ÐÇÆÚÈÕ", "ÐÇÆÚÒ»", "ÐÇÆÚ¶þ", "ÐÇÆÚÈý",
							"ÐÇÆÚËÄ", "ÐÇÆÚÎå", "ÐÇÆÚÁù"};
#else
static char *S_WEEK[7]   = {"SUN", "MON", "TUE", "WED",
							"THU", "FRI", "SAT"};
#endif
pthread_mutex_t gOsdMutex   = PTHREAD_MUTEX_INITIALIZER;
unsigned int           nOsdRebuild = 0;

HH_OSD_LOGO* HH_OSD_GetLogoHandle(int nCh, int bMin, int nOsdType)
{
	int nIndex      = 0;
	int nEncMaxType = 3;

	if (nCh < 0 ||  bMin < 0 || bMin >= nEncMaxType
		|| nOsdType < 0 || nOsdType >= HH_MAX_OSD_PER_GROUP)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_GetLogoHandle param err\n");
		return NULL;
	}

	nIndex = nCh * nEncMaxType *  HH_MAX_OSD_PER_GROUP +
			 bMin * HH_MAX_OSD_PER_GROUP + nOsdType;

	if (nIndex >= HH_MAX_OSD_NUM)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_GetLogoHandle index err %d\n", nIndex);
		return NULL;
	}
	//COMM_SYSLOG(LOG_ERR,"\nHH_OSD_GetLogoHandle nCh=%d,bMin=%d,nOsdType=%d,nIndex=%d,pGOsdLog[%d]=%d\n",nCh,bMin,nOsdType,nIndex,nIndex,pGOsdLog[nIndex]);
	return 	pGOsdLog[nIndex];
}

float HH_OSD_GetOsdRateX(int nCh, int nMinEnc)
{
	int	nEncWidth  = 0;
	int	nEncHeight = 0;
	int nEncType   = 0;

	//HH_VENC_GetVEncSize(nCh, nMinEnc, &nEncWidth, &nEncHeight, &nEncType);

	if(nMinEnc == 0)
	{
		nEncWidth = 1280 ;

	}
	else if(nMinEnc == 1)
	{
		//nEncWidth = 640 ;
		nEncWidth = 960 ;

	}
	//COMM_SYSLOG(LOG_ERR,"nEncWidth = %d" , nEncWidth);
    return (float)nEncWidth / 640;
}

float HH_OSD_GetOsdRateY(int nCh, int nMinEnc)
{
	int	nEncWidth  = 0;
	int	nEncHeight = 0;
	int nEncType   = 0;

	if(nMinEnc == 0)
	{
		 return (float)3;

	}
	else if(nMinEnc == 1)
	{
		return (float)1;

	}
}

int HH_OSD_GetOrg(int nCh, int nEnc, int osdType, HI_OSD_ORG  *pOsdOrg)
{
	int	nEncWidth  = 0;
	int	nEncHeight = 0;

	if (pOsdOrg == NULL || nCh < 0  || osdType < 0 ||
		osdType >= HH_MAX_OSD_PER_GROUP)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_GetOrg Param Err\n");
		return -1;
	}
	if(nEnc == 0)
	{
		switch(osdType)
		{
			case HH_OSD_TYPE_TIME:
				pOsdOrg->nOrgX = 0	;
				pOsdOrg->nOrgY = 0 ;
				break;

			case HH_OSD_TYPE_TITLE:
				pOsdOrg->nOrgX = 0;
				pOsdOrg->nOrgY	= ipc_conf.height - 48;
				break;

		}

	}else if(nEnc == 1)
	{
		switch(osdType)
		{
			case HH_OSD_TYPE_TIME:
				pOsdOrg->nOrgX = 0	;
				pOsdOrg->nOrgY = 0 ;
				break;

			case HH_OSD_TYPE_TITLE:
				pOsdOrg->nOrgX = 0;
				pOsdOrg->nOrgY	= ipc_conf.height - 32;
				break;
		}
	}
	COMM_SYSLOG(LOG_DEBUG,">>>>>pOsdOrg->nOrgX = %d pOsdOrg->nOrgY = %d \n ",pOsdOrg->nOrgX ,pOsdOrg->nOrgX);
	return 0;
}

int HH_OSD_GetColor(int nCh, HI_OSD_COLOR *pOsdColor)
{
	if (nCh < 0 || pOsdColor == NULL)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_GetColor Param Err\n");
		return -1;
	}

	pOsdColor->bgAlpha = 0;					//±³¾°ÉèÎªÍ¸Ã÷
	pOsdColor->fgAlpha = 0x7F;               //Ç°¾°²»Í¸Ã÷
	 //ºÚÉ«
	if (pvideo_osd_status->nOsdColor == 1)
	{
		pOsdColor->fgColor  = HH_OSD_COLOR_BLACK;
		pOsdColor->edgeColor= HH_OSD_COLOR_WHITE;
		pOsdColor->selColor = HH_OSD_COLOR_WHITE;
	}//»ÆÉ«
	else if (pvideo_osd_status->nOsdColor == 2)
	{
		pOsdColor->fgColor  = HH_OSD_COLOR_YELLOW;
		pOsdColor->edgeColor= HH_OSD_COLOR_BLACK;
		pOsdColor->selColor = HH_OSD_COLOR_WHITE;
	}//ºìÉ«
	else if (pvideo_osd_status->nOsdColor == 3)
	{
		pOsdColor->fgColor  = HH_OSD_COLOR_RED;
		pOsdColor->edgeColor= HH_OSD_COLOR_BLACK;
		pOsdColor->selColor = HH_OSD_COLOR_WHITE;
	}//À¶É«
	else if (pvideo_osd_status->nOsdColor == 4)
	{
		pOsdColor->fgColor  = HH_OSD_COLOR_BLUE;
		pOsdColor->edgeColor= HH_OSD_COLOR_BLACK;
		pOsdColor->selColor = HH_OSD_COLOR_WHITE;
	}
	else
	{
		pOsdColor->fgColor  = HH_OSD_COLOR_WHITE;
		pOsdColor->edgeColor= HH_OSD_COLOR_BLACK;
		pOsdColor->selColor = HH_OSD_COLOR_BLACK;
		pOsdColor->bgColor = 64;
	}
	pOsdColor->bgColor = pOsdColor->fgColor; //±³¾°ÉèÎªÍ¸Ã÷ ÑÕÉ«Ëæ±ãÐ´
	return 0;
}

int HH_OSD_GetMenuColor(int nCh, HI_OSD_COLOR *pOsdColor)
{
	if (nCh != 0 || pOsdColor == NULL)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_GetMenuColor Param Err\n");
		return -1;
	}

	pOsdColor->bgAlpha = 0x7F;//±³¾°ÉèÎªÍ¸Ã÷
	pOsdColor->fgAlpha = 0x7F;//Ç°¾°²»Í¸Ã÷

	pOsdColor->fgColor  = HH_OSD_COLOR_WHITE;
	pOsdColor->edgeColor= HH_OSD_COLOR_BLACK;
	pOsdColor->selColor = HH_OSD_COLOR_YELLOW;

	//pOsdColor->bgColor = pOsdColor->fgColor; //±³¾°ÉèÎªÍ¸Ã÷ ÑÕÉ«Ëæ±ãÐ´
	pOsdColor->bgColor = HH_OSD_COLOR_BLACK;
	return 0;
}


int HH_OSD_GetTimeTitle(int nCh, int bMin, char *pTitle)
{

	time_t timep;
	struct tm *pLocalTime;
	time(&timep);
	pLocalTime = localtime(&timep);

	if(pvideo_osd_status->stDateOSD.Show ||
	   pvideo_osd_status->stTimeOSD.Show ||
	   pvideo_osd_status->stWeekOSD.Show )
	{
		if (pvideo_osd_status->stDateOSD.Show )
		{
			if (pvideo_osd_status->nOsdPrefer == 0){
                		sprintf(pTitle,"%04d-%02d-%02d",1900+pLocalTime->tm_year,1+pLocalTime->tm_mon, pLocalTime->tm_mday) ;
			}else if (pvideo_osd_status->nOsdPrefer == 1){
				sprintf(pTitle,"%02d-%02d-%04d",1+pLocalTime->tm_mon,pLocalTime->tm_mday, 1900+pLocalTime->tm_year) ;
			}else{
				sprintf(pTitle,"%02d-%02d-%04d",pLocalTime->tm_mday,1+pLocalTime->tm_mon, 1900+pLocalTime->tm_year) ;
			}

		}

		if(pvideo_osd_status->stTimeOSD.Show )
		{
			sprintf(pTitle,"%s %02d:%02d:%02d %s",pTitle, pLocalTime->tm_hour, pLocalTime->tm_min, pLocalTime->tm_sec, S_WEEK[pLocalTime->tm_wday % 7] ) ;
		}

		//if(pvideo_osd_status->stWeekOSD.Show )
		//{
		//	sprintf(pTitle,"%s ",pTitle, S_WEEK[curTime.week % 7]);
		//}
	}
	return 0;
}

int HH_OSD_GetFrameTitle(int nCh, int bMin, char *pTitle)
{
    //extern VideoEnc_Info _VideoEncInfo[MAX_VIDEO_NUM];
	bMin = bMin % 2;

	if(pvideo_osd_status->stBitrateOSD.Show)
	{
	//
	}
	return 0;

}


int HH_OSD_GetPTZTitle(int nCh, int bMin, char *pTitle)
{

	return 0;
}
int HH_OSD_GetChNameTitle(int nCh, int bMin,char *pTitle)
{
    if(pvideo_osd_status->stTitleOSD.Show)
    {
		sprintf(pTitle,"%s",pvideo_osd_status->Title);
		//printf("$$$$$$$$$######pTitle[%s]\n",pTitle);

    }
	return 0;
}

int HH_OSD_GetTitle(int nCh, int bMin, int nOsdType, char *pTitle)
{
	if (nOsdType == HH_OSD_TYPE_TIME)
	{
		HH_OSD_GetTimeTitle(nCh, bMin, pTitle);// time
	}
	else if (nOsdType == HH_OSD_TYPE_TITLE)
	{
		HH_OSD_GetChNameTitle(nCh, bMin, pTitle);//×Ö·û
	}

	return 0;
}

int HH_OSD_GetFontInt(int nCh, int bMin, int nOsdType)
{
	if (nOsdType == HH_OSD_TYPE_TITLE)
	{
		return 1;
	}
	return 0;
}

int HH_OSD_GetShow(int nCh, int bMin, int nOsdType)
{
	if (nOsdType == HH_OSD_TYPE_TIME)
	{
		return (pvideo_osd_status->stTimeOSD.Show ||
				pvideo_osd_status->stDateOSD.Show ||
				pvideo_osd_status->stWeekOSD.Show ||
				pvideo_osd_status->stBitrateOSD.Show);
	}
	else if (nOsdType == HH_OSD_TYPE_TITLE)
	{
		return pvideo_osd_status->stTitleOSD.Show;
	}

	return 0;
}

int HH_OSD_Show_Refresh(int nCh, int nMinEnc, int osdType)
{
	HH_OSD_LOGO *pOsdLogo    = NULL;
	int          showsStatus = 0;
	if (nCh < 0  ||  nMinEnc < 0 || nMinEnc > 2
		|| osdType < 0 || osdType >= HH_MAX_OSD_PER_GROUP)
	{
		COMM_SYSLOG(LOG_ERR,"HH_Check_Show_Refresh param err\n");
		return -1;
	}

	if ((pOsdLogo = HH_OSD_GetLogoHandle(nCh, nMinEnc, osdType)) == NULL)
	{
		COMM_SYSLOG(LOG_ERR,"HH_Check_Show_Refresh Get Handle Err\n");
		return -2;
	}

	showsStatus = HH_OSD_GetShow(nCh, nMinEnc, osdType);
	if (showsStatus != pOsdLogo->nStatus)
	{
		HI_OSD_Set_Show(pOsdLogo, showsStatus);
		pOsdLogo->nStatus = showsStatus; //2010-07-12 ÐÞ¸Ä²»ÄÜÒþ²ØµÄÎÊÌâ
	}
	return 0;
}

int HH_OSD_ChName_Refresh(int nCh, int nMinEnc, int osdType)
{
	HH_OSD_LOGO *pOsdLogo                 = NULL;
	char   szTitle[HH_MAX_TITLE_NUM]= {0};
	int          nNeedReCreate            = 0;
	int          ret                      = 0;

	if (nCh < 0  ||  nMinEnc < 0 || nMinEnc > 2)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_ChName_Refresh param err\n");
		return -1;
	}

	if ((pOsdLogo = HH_OSD_GetLogoHandle(nCh, nMinEnc, osdType)) == NULL)
	{
		//COMM_SYSLOG(HH_LOG_ERR,"HH_OSD_ChName_Refresh Get Handle Err nCh=%d,osdType=%d\n", nCh, osdType);
		return -2;
	}

	memset(szTitle, 0, sizeof(szTitle));
	HH_OSD_GetTitle(nCh, nMinEnc, osdType, szTitle);

	if (strncmp(szTitle, pOsdLogo->szOsdTitle, HH_MAX_TITLE_NUM) != 0 ||
		HI_OSD_Check_ImaSizeChange(pOsdLogo))
	{
		memcpy(pOsdLogo->szOsdTitle, szTitle, HH_MAX_TITLE_NUM - 1);
		nNeedReCreate |= HI_OSD_Parse_OsdTitle(pOsdLogo);
		nNeedReCreate |= HI_OSD_Get_BmpSize(pOsdLogo);

		if (nNeedReCreate)
		{
			HI_OSD_Destroy(pOsdLogo);
			//COMM_SYSLOG(LOG_ERR,"111111111111111111111111111111111111111111111111\n");
			if ((ret = HI_Create_Osd_Reg(pOsdLogo)) != 0)
			{
				COMM_SYSLOG(LOG_ERR,"HH_ChName_Refresh Create Reg Err %x\n", ret);
				return ret;
			}
			HI_OSD_Refresh_Org(pOsdLogo);
			//HI_OSD_Refresh_Color(pOsdLogo);
		}

		if ((ret = HI_OSD_Create_Bitmap(pOsdLogo)) != 0)
		{
			COMM_SYSLOG(LOG_ERR,"HH_ChName_Refresh Create Bitmap Err %x\n", ret);
			return ret;
		}
		HH_OSD_Show_Refresh(nCh, nMinEnc, osdType);
	}


	return 0;
}

int HH_OSD_ChnAllName_Refresh(int nCh)
{
	int nEncType = 0;

	if (nCh < 0 )
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_ChnAllName_Refresh param err\n");
		return -1;
	}

	for (nEncType = 0; nEncType < VIDEO_NUM; ++nEncType)
	{
		HH_OSD_ChName_Refresh(nCh, nEncType, HH_OSD_TYPE_TITLE);
	}
	return 0;
}

int HH_OSD_AllName_Refresh()
{
	int nCh      = 0;

	for (nCh = 0; nCh < VIDEO_NUM; ++nCh)
	{
		HH_OSD_ChnAllName_Refresh(nCh);
		//COMM_SYSLOG(LOG_ERR,"111111\n");
	}
	return 0;
}

int HH_OSD_ChnTime_Refresh(int nCh)
{
	int nEncType = 0;

	for (nEncType = 0; nEncType < VIDEO_NUM; ++nEncType)
	{
		HH_OSD_ChName_Refresh(nCh, nEncType, HH_OSD_TYPE_TIME);
	}
	return 0;
}

int HH_OSD_AllTime_Refresh()
{
	int nCh      = 0;

	for (nCh = 0; nCh < VIDEO_NUM; ++nCh)
	{
		HH_OSD_ChnTime_Refresh(nCh);
	}
	return 0;
}

int HH_OSD_Org_Refresh(int nCh, int nEnc, int osdType)
{
	HH_OSD_LOGO *pOsdLogo = NULL;

	if (nCh < 0 ||  nEnc < 0 || nEnc > 2
		|| osdType < 0 || osdType >= HH_MAX_OSD_PER_GROUP)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_Org_Refresh param err\n");
		return -1;
	}

	if ((pOsdLogo = HH_OSD_GetLogoHandle(nCh, nEnc, osdType)) == NULL)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_Org_Refresh Get Handle Err\n");
		return -2;
	}

	HH_OSD_GetOrg(nCh, nEnc, osdType, &pOsdLogo->osdPos);
	return HI_OSD_Refresh_Org(pOsdLogo);
}

int HH_OSD_Color_Refresh(int nCh, int nEnc, int osdType)
{
	HH_OSD_LOGO *pOsdLogo                 = NULL;

	if (nCh < 0   ||  nEnc < 0 || nEnc > 2
		|| osdType < 0 || osdType >= HH_MAX_OSD_PER_GROUP)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_Color_Refresh param err\n");
		return -1;
	}

	if ((pOsdLogo = HH_OSD_GetLogoHandle(nCh, nEnc, osdType)) == NULL)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_Org_Refresh Get Handle Err\n");
		return -2;
	}

	HH_OSD_GetColor(nCh, &pOsdLogo->osdColor);

	return HI_OSD_Refresh_Color(pOsdLogo);
}

int HH_OSD_ChnColor_Refresh(int nCh)
{
	int nEncType = 0;

	if (nCh < 0 )
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_ChnColor_Refresh param err\n");
		return -1;
	}

	for (nEncType = 0; nEncType < 3; ++nEncType)
	{
		HH_OSD_Color_Refresh(nCh, nEncType, HH_OSD_TYPE_TIME);
		HH_OSD_Color_Refresh(nCh, nEncType, HH_OSD_TYPE_TITLE);
	}
	return 0;
}

int HH_OSD_ChnOrg_Refresh(int nCh)
{
	int nEncType = 0;

	if (nCh < 0)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_ChnAllName_Refresh param err\n");
		return -1;
	}

	for (nEncType = 0; nEncType < 3; ++nEncType)
	{
		HH_OSD_Org_Refresh(nCh, nEncType, HH_OSD_TYPE_TIME);
		HH_OSD_Org_Refresh(nCh, nEncType, HH_OSD_TYPE_TITLE);
	}
	return 0;
}

int HH_OSD_ChnShow_Refresh(int nCh)
{
	int nEncType = 0;

	if (nCh < 0 )
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_ChnAllName_Refresh param err\n");
		return -1;
	}

	for (nEncType = 0; nEncType < 2; ++nEncType)
	{
		HH_OSD_Show_Refresh(nCh, nEncType, HH_OSD_TYPE_TIME);
		HH_OSD_Show_Refresh(nCh, nEncType, HH_OSD_TYPE_TITLE);
	}
	return 0;
}
int HH_OSD_Destory(int nCh, int nEnc, int osdType)
{
	HH_OSD_LOGO *pOsdLogo = NULL;

	if (nCh < 0  ||  nEnc < 0 || nEnc > 2
		|| osdType < 0 || osdType >= HH_MAX_OSD_PER_GROUP)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_Org_Refresh param err\n");
		return -1;
	}

	if ((pOsdLogo = HH_OSD_GetLogoHandle(nCh, nEnc, osdType)) == NULL)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_Org_Refresh Get Handle Err\n");
		return -2;
	}

	memset(pOsdLogo->szOsdTitle, 0, HH_MAX_TITLE_NUM);
	return HI_OSD_Destroy(pOsdLogo);
}

int HH_OSD_Rebuild(int nCh, int nEnc, int osdType)
{
	HH_OSD_LOGO *pOsdLogo = NULL;

	if (nCh < 0  ||  nEnc < 0 || nEnc > 2
		|| osdType < 0 || osdType >= HH_MAX_OSD_PER_GROUP)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_Org_Refresh param err\n");
		return -1;
	}

	if ((pOsdLogo = HH_OSD_GetLogoHandle(nCh, nEnc, osdType)) == NULL)
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_Org_Refresh Get Handle Err\n");
		return -2;
	}

	HH_OSD_GetOrg(nCh, nEnc, osdType, &pOsdLogo->osdPos);
	HH_OSD_GetColor(nCh, &pOsdLogo->osdColor);
	HH_OSD_GetTitle(nCh, nEnc, osdType, pOsdLogo->szOsdTitle);
	pOsdLogo->nStatus = HH_OSD_GetShow(nCh, nEnc, osdType);

	return HI_OSD_Build(pOsdLogo);
}

int HH_OSD_ChnReBuild(int nCh)
{
	int nEncType = 0;

	if (nCh < 0 )
	{
		COMM_SYSLOG(LOG_ERR,"HH_OSD_ChnAllName_Refresh param err\n");
		return -1;
	}
	nOsdRebuild = 1;
	for (nEncType = 0; nEncType < 3; ++nEncType)
	{
		HH_OSD_Destory(nCh, nEncType, HH_OSD_TYPE_TIME);
		HH_OSD_Destory(nCh, nEncType, HH_OSD_TYPE_TITLE);
	}
	for (nEncType = 0; nEncType < 3; ++nEncType)
	{
		HH_OSD_Rebuild(nCh, nEncType, HH_OSD_TYPE_TIME);
		HH_OSD_Rebuild(nCh, nEncType, HH_OSD_TYPE_TITLE);
	}
	nOsdRebuild = 0;

	return 0;
}

int HH_OSD_All_Refresh()
{
	if (nOsdRebuild == 0)
	{
		HH_OSD_AllTime_Refresh();
		HH_OSD_AllName_Refresh();
	}
	return 0;
}


/**************************************************************
* º¯ÊýÃû£º	 HH_OSD_SetOsdPosDefault
* º¯Êý½éÉÜ:   ÉèÖÃOSDÄ¬ÈÏ×ø±ê£¬µ±OSDÖØµþÊ±µ÷ÓÃ
* ÊäÈë²ÎÊý:
* Êä³ö²ÎÊý:
* ·µ»ØÖµ:
* ×÷Õß:	 by yj 2012-09-17
**************************************************************/

int HH_OSD_SetOsdPosDefault()
{
	int nCh      = 0;
	pvideo_osd_status->stDateOSD.X			= 0;
	pvideo_osd_status->stDateOSD.Y			= 32;
	pvideo_osd_status->stTitleOSD.X			= 0;
	pvideo_osd_status->stTitleOSD.Y			= 32;
	pvideo_osd_status->nOsdColor = 5;
	pvideo_osd_status->stDateOSD.Show = 1;
	pvideo_osd_status->stTimeOSD.Show = 1;
	pvideo_osd_status->stWeekOSD.Show = 1 ;
	pvideo_osd_status->nOsdPrefer = 0 ;
	pvideo_osd_status->stBitrateOSD.Show = 0;
	pvideo_osd_status->stTitleOSD.Show  = 1 ;
	if(strlen(pvideo_osd_status->Title) == 0)
	{
		sprintf(pvideo_osd_status->Title,"%s",	 "ZIOTLAB");
	}

	ipc_conf.width = 1280;
	ipc_conf.height = 720;

	return 0;
}


int HH_OSD_SetOsdParams(int Osd_type,int Osd_Show)
{
	pvideo_osd_status->stDateOSD.Show = Osd_Show;
	pvideo_osd_status->stTimeOSD.Show = Osd_Show;
	pvideo_osd_status->stWeekOSD.Show = Osd_Show;
	pvideo_osd_status->nOsdPrefer = Osd_type;
	return 0 ;

}

int HH_OSD_unInit()
{
	free(pvideo_osd_status);
	return 0 ;
}

int HH_OSD_Init()
{
	int 		 nCh      = 0;
	int 		 nEncType = 0;
	int 		 osdType  = 0;
	int 		 index    = 0;
	HI_OSD_ORG	 osdOrg;
	HI_OSD_COLOR osdColor;
	char         szTitle[HH_MAX_TITLE_NUM] = {0};
	COMM_SYSLOG(LOG_DEBUG,"HH_OSD_Init Start...\n");
	//Êµ¼ÊÏîÄ¿ÖÐÊ¹ÓÃµÄ½á¹¹Ìå£¬ÓÃÀ´¿ØÖÆOSDµÄ¿ª¹Øµ
	pvideo_osd_status = (VIDEO_OSD_STATUS *)malloc(sizeof(VIDEO_OSD_STATUS));
	//VIDEO_NUM  vencµÄchn£¬Ä¿Ç°Ö»ÓÐ1Â·ÊÓÆµ£¬ËùÒÔÖ±½ÓÐ´1.
	//HH_MAX_OSD_PER_GROUP  OSD ÇøÓò¸öÊý
	for (nEncType = 0; nEncType < VIDEO_NUM; nEncType++) //  chanel 0
	{
		for (osdType = 0; osdType < HH_MAX_OSD_PER_GROUP; osdType++)  // 2 ÇøÓò
		{

			memset(&osdOrg,   0, sizeof(HI_OSD_ORG));
			memset(&osdColor, 0, sizeof(HI_OSD_COLOR));
			memset(szTitle,   0, sizeof(szTitle));

			HH_OSD_SetOsdPosDefault();

			HH_OSD_GetOrg(nCh, nEncType, osdType, &osdOrg);//»ñÈ¡ÏÔÊ¾µÄ×ø±ê

			HH_OSD_GetColor(nCh, &osdColor);
			HH_OSD_GetTitle(nCh, nEncType, osdType, szTitle);
			
			//index  OSD×ª»»³É¾ä±ú£¬×ªÎªÎª 0 ºÍ 1£¬Êµ¼ÊÊ¹ÓÃÖ±½Ó µÈÓÚ osdType¼´¿É
			index = nEncType * HH_MAX_OSD_PER_GROUP + osdType;//osdmenu
			//printf("index=========%d",index);
			//printf("szTitle=========%s",szTitle);
			if (NULL == HI_Create_Osd(index,&pGOsdLog[index], (VENC_GRP)nEncType, osdOrg, osdColor, szTitle, HH_OSD_GetFontInt(nCh, nEncType, osdType), HH_OSD_GetShow(nCh, nEncType, osdType)))
				COMM_SYSLOG(LOG_ERR,"HI_Create_Osd %d %d %d failed !\n", nCh,nEncType, osdType);
		}
	}

	COMM_SYSLOG(LOG_DEBUG,"HH_OSD_Init End...\n");
	return 0;
}


