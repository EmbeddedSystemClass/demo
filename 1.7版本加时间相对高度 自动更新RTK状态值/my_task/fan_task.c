 
 

#include "fan_task.h"
#include "psdk_debug.h"  

uint8_t g_fanDs18b20Flag=0;         //DS18B20逻辑的  0=不转 1=转
uint8_t g_fanTurePhotoEnFlag=0;     //拍照逻辑的 0=风扇要恢复了 拍照停止了 1=风扇要绝对停止了 拍照启动了  
 
unsigned portBASE_TYPE uxHighWaterMarkfan;  
 
//-------------------------------------------------------------------------
//任务函数
void  FAN_TASK (void const * argument)
{
    uint32_t n=0;
    osDelay(10);	
    uint8_t fanDelayFlag=0;//0=不够3分钟 1=够了3分钟 连续开机后要3分钟才会检测风扇会不会转 但注意usb插入绝对转 拍照瞬间0.1s绝对不转 (程序逻辑已经处理 有usb插入时候不会拍照)
		uxHighWaterMarkfan=uxTaskGetStackHighWaterMark( FAN_TASK_Handle );    //查询大小  
	  PSDK_LOG_DEBUG("fan_TASK  %d\r\n", uxHighWaterMarkfan );	
	while (1) 
	{
        
       
     osDelay(1);          
    }		
}


