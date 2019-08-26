#include "app_updata_task.h"
#include "cocoguojia_sdcard.h"
#include "runinit_task.h"
#include "psdk_debug.h"
#include "u2rece_task.h"


void  SJ_TASK (void const * argument)
{
	u8 i = 0;
  while(1)
	{
			PSDK_LOG_DEBUG("SJ_TASK running  \r\n" );	
	 if((0==g_readingFlag) && (sys_save_finish == 3) && (SYSTEMSTATE_ONTRUE != g_offOrOnTureStateFlag))     /*在不拍照和在存储卡中存储完系统版本号的时候读取有没有升级的文件*/
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
 
	



