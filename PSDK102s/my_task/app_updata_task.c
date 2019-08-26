#include "app_updata_task.h"
#include "cocoguojia_sdcard.h"
#include "runinit_task.h"
#include "psdk_debug.h"
#include "Cam_status_task.h"

/*************************************************
Function: void  SJ_TASK (void const * argument)
Description: ���GPS��ָ��λ����û�� �����̼�
Return: �޷���ֵ
*************************************************/

void  SJ_TASK (void const * argument)
{
	u8 i = 0;
  while(1)
	{
//	PSDK_LOG_DEBUG("SJ_TASK running  \r\n" );	
	 if((0==readingFlag) && (sys_save_finish == 3) && (SYSTEMSTATE_ONTRUE != offOrOnTureStateFlag))     /*�ڲ����պ��ڴ洢���д洢��ϵͳ�汾�ŵ�ʱ���ȡ��û���������ļ�*/
			{
				 i = read_bin((const TCHAR *)system_folder);  /*���ڲ����յ�ǰ���� �����û��bin�ļ�*/			 
				 if(i == 1)
				 {
					  SCB->AIRCR =0X05FA0000|(u32)0x04;	  
					  i = 0;
				 }
		  }

	 osDelay(3000); 
	}

} 
 
	



