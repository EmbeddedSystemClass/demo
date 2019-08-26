#include "cam_task.h"
#include "psdk_debug.h"

/*************************************************
Function: void  CAM_TASK (void const * argument)
Description: 判断相机处于什么样的状态，从而进行界面的切换和开关机动作
Return: 无返回值
*************************************************/
void  CAM_TASK (void const * argument)
{
		uint8_t n = 0,i = 0,j = 0;
	  EventBits_t eventTemp = 0;
	
    /* 等待标志camTaskCanTakeOverFlag置位 cam任务可以接管标志置位 */
    while (0==camTaskCanTakeOverFlag)   /* 在run_task.c里面 置1 */
    {
        osDelay(10);
    }
		
    osDelay(100);
    POWER_NO;                           /* 关机指令@@@@@@@@@@@@@@@@@@@ */
    osDelay(100);                             

    /* 检测是否真正关机 */
    for(i = 1; i <= 5; i++)
    {
        if(0 != camPowerFlagTable[i])
        {
            i = 210;
        }
    }
    if(200 <= i)
    {
        offOrOnTureStateFlag = SYSTEMSTATE_SHUTFAIL;    /* 关机失败 */    	
    }
    else
    {
        offOrOnTureStateFlag = SYSTEMSTATE_OFFTRUE;     /* 关机成功 */ 
    }
    osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);           /* 任务的句柄，发送的数据 32位，覆盖当前通知 */  
    FMQ_ON;
    osDelay(500);
    FMQ_OFF;
    osDelay(100);
    n = 0;    
   
	while (1) 
	{ 
//PSDK_LOG_DEBUG("CAM_TASK running  \r\n" );		
     /* 真实关机了 */
    if(SYSTEMSTATE_OFFTRUE == offOrOnTureStateFlag)        
		{                               
				osDelay(300);            
				for(i = 1; i <= 5; i++)
				{
						if(0 != camPowerFlagTable[i])
						{
								i = 210;
						}
				}
				
				/* 如果有相机没关机或者突然开机 */
				if(200 <= i)
				{
						POWER_NO;       /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
						osDelay(300);  
						offOrOnTureStateFlag = SYSTEMSTATE_SHUTFAIL;
						osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);       /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
						FMQ_ON;
						osDelay(500);
						FMQ_OFF;
						osDelay(100);
						n = 0;                
				}
				
			 
				/* 检测是否有按键被按下 按下则进入 开机中 */             
				eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);/* 事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待 */         
				if(KEY_SHORT == (eventTemp&KEY_SHORT)) /* 按键短按了 */
				{
						POWER_EN;         /* 开机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
						osDelay(300);
						offOrOnTureStateFlag = SYSTEMSTATE_GOTOOPEN;
						osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);            /* 任务的句柄，发送的数据 32位，覆盖当前通知 */ 
						FMQ_ON;
						osDelay(500);
						FMQ_OFF;
						osDelay(100);
						n = 0;
				}        
    }
		
     /* 关机失败 */ 
    if(SYSTEMSTATE_SHUTFAIL == offOrOnTureStateFlag)									         
		{	
            n++;
            if((1 == n) || (0 == (n % 3)))
            {
                FMQ_ON;
            }
            else
            {
                FMQ_OFF;
            }    
            if(8 <= n)
            {
                POWER_NO;       /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                osDelay(100);                
                n = 0;
            }                                
            osDelay(100);       
            for(i = 1; i <=5 ; i++)
            {
                if(0 != camPowerFlagTable[i])
                {
                    i = 210;
                }
            }
            if(100 > i)
            {
                offOrOnTureStateFlag = SYSTEMSTATE_OFFTRUE;
                osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);      /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n = 0;   	
            }
            else  /* 开机中 */
            {                
                eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);/* 事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待 */        
                if(KEY_SHORT == (eventTemp&KEY_SHORT))/* 按键短按了 */
                {
										POWER_EN;      /* 开机@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
										osDelay(100);
                    offOrOnTureStateFlag = SYSTEMSTATE_GOTOOPEN;
                    osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0); /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                    FMQ_ON;
                    osDelay(500);
                    FMQ_OFF;
                    osDelay(100);
                    n=0;
                } 
            }                
    } 
		
		
    /* 开机失败 */  
    if(SYSTEMSTATE_OPENFAIL == offOrOnTureStateFlag)	        
		{	
            n++;
            if((1 == n)||(0 == (n%3)))
            {
                FMQ_ON;
            }
            else
            {
                FMQ_OFF;
            }    
            if(8 <= n)
            {
                POWER_EN;       /* 开机 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                osDelay(100);
                n = 0;
            } 
						
            osDelay(300);         
            for(i = 1; i <= 5; i++)
            {
                if(1 != camPowerFlagTable[i])
                {
                    i = 210;
                }
            }
            if(100 > i)
            {
								osDelay(3000);
								osDelay(3000);
								osDelay(3000);
                PHOTOALL_EN;               /* 拍照@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */  
							  osDelay(100);
							  PHOTOALL_NO;							
								osDelay(3000);		
                offOrOnTureStateFlag = SYSTEMSTATE_ONTRUE;
                osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);   /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n=0;   	
            }
            else
            {                
                eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待          
                if(KEY_SHORT == (eventTemp&KEY_SHORT))//按键短按了
                {
									  POWER_NO;
                    offOrOnTureStateFlag = SYSTEMSTATE_GOTOSHUT;
                    osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                    FMQ_ON;
                    osDelay(500);
                    FMQ_OFF;
                    osDelay(100);
                } 
            }                
    } 
		
		
     /* 关机中 */  
    if(SYSTEMSTATE_GOTOSHUT == offOrOnTureStateFlag)									       
		{	
            POWER_NO;    /*关机 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
            osDelay(100);
            
            for(j = 0; j < 50; j++)   //5s
            {                                 
                osDelay(100);                
                for(i = 1; i <= 5; i++)
                {
                    if(1 == camPowerFlagTable[i])
                    {
                        i = 210;
                    }
                }
                if(100 > i)
                {
                    j = 220;
                }   
            }
            if(200 <= j)
            {
                offOrOnTureStateFlag = SYSTEMSTATE_OFFTRUE;
                osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n = 0;   	
            }
            else
            {
                POWER_NO;            /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                osDelay(100);              
                offOrOnTureStateFlag = SYSTEMSTATE_SHUTFAIL;     	
                osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);             //任务的句柄，发送的数据 32位，覆盖当前通知    
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n=0;    
            } 
    }
		
		
    /* 开机中 */
    if(SYSTEMSTATE_GOTOOPEN == offOrOnTureStateFlag)									         
		{	  
            POWER_EN;           /* 开机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
            osDelay(100);      
            for(j = 0; j < 50; j++)//15s
            {                       
                osDelay(300);      
                for(i = 1; i <= 5; i++)
                {
                    if(0 == camPowerFlagTable[i])
                    {
                        i = 210;
                    }
                }
                if(100 > i)
                {
                    j = 220;
                }   
            }
            #ifdef DEBUG_TIAOSHI
            j = 230;//调试///////调试//调试///调试/////调试////////调试/////
            #endif          
            if(200 <= j)
            { 
				  osDelay(3000);
			    osDelay(3000);
			    osDelay(3000);
          PHOTOALL_EN;            /* 拍照 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  */
					osDelay(100);		
					PHOTOALL_NO;							
			    osDelay(3000);				
                offOrOnTureStateFlag = SYSTEMSTATE_ONTRUE;                
                osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);    /* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n = 0; 
                                
            }
            else
            {
                osDelay(300);
                offOrOnTureStateFlag = SYSTEMSTATE_OPENFAIL;     	
                osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);    /* 任务的句柄，发送的数据 32位，覆盖当前通知 */   
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n = 0;    
            } 
        }                               
        osDelay(1);
    }
}




