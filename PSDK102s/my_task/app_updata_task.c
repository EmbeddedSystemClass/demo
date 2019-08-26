#include "app_updata_task.h"
#include "cocoguojia_sdcard.h"
#include "runinit_task.h"
#include "psdk_debug.h"
#include "Cam_status_task.h"

/*************************************************
Function: void  SJ_TASK (void const * argument)
Description: 检测GPS盘指定位置有没有 升级固件
Return: 无返回值
*************************************************/

void  SJ_TASK (void const * argument)
{
	u8 i = 0;
  while(1)
	{
//	PSDK_LOG_DEBUG("SJ_TASK running  \r\n" );	
	 if((0==readingFlag) && (sys_save_finish == 3) && (SYSTEMSTATE_ONTRUE != offOrOnTureStateFlag))     /*在不拍照和在存储卡中存储完系统版本号的时候读取有没有升级的文件*/
			{
				 i = read_bin((const TCHAR *)system_folder);  /*得在不拍照的前提下 检测有没有bin文件*/			 
				 if(i == 1)
				 {
					  SCB->AIRCR =0X05FA0000|(u32)0x04;	  
					  i = 0;
				 }
		  }

	 osDelay(3000); 
	}

} 
 
	



