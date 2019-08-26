
#include "ff.h"

#ifndef __COCOGUOJIA_SDCARD_H
#define	__COCOGUOJIA_SDCARD_H

#include "myinclude.h"


extern u8 g_registeredWorkspaceN;//总共少次 写TF卡进入到不稳定状态

extern uint8_t g_nameFlag;//0=还没确认过文本名字的日期   1=确认过文本名字的日期

extern u8 g_gpsTableFlag;
extern u8 g_gps001Table[200];
extern u8 g_gps002Table[200];

void write_system_version_number(const TCHAR *fileName,const TCHAR *txt__name);
extern int read_bin(const TCHAR *fileName);
//-------------------------------------------------------------------------
//使能这个定义 则我们可以用这个库函数的读SD卡的相关函数 否则不能
//#define I_WANT_TO_READ_SDCARD  1

#ifdef I_WANT_TO_READ_SDCARD
extern BYTE  g_cocoguojia_sdcard_myReadBuffer1024[1024] ;	//读缓冲区 如果不读 这个可以屏蔽
#endif

//----------------------------------------------------------------------------------------------
//根据实际情况来改变文件txt的名称
extern TCHAR g_cocoguojia_sdcard_txtname20[20];//TCHAR系统定义为char  另: FatFs  const TCHAR *fileName

//-------------------------------------------------------------------------
//使能这个定义 则我们可以用这个库函数的读SD卡的相关函数 否则不能
//#define I_WANT_TO_READ_SDCARD  0


extern void cocoguojia_sddelayinit(void);
extern void cocoguojia_RegisteredWorkspace		(void);
extern void cocoguojia_CancleWorkspace			(void);
extern void cocoguojia_WriteFileSystem			(const TCHAR *fileName);

extern void cocoguojia_sdWrireGpsRtk(void);


#ifdef I_WANT_TO_READ_SDCARD
extern void cocoguojia_BeforeReadClearBuffer	(BYTE *buff,unsigned short n);
extern void cocoguojia_ReadFileSystem			(const TCHAR *fileName);
#endif

#endif 
