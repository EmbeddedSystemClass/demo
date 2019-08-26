#include "trigger_task.h" 
#include "psdk_debug.h" 

/*************************************************
Function: void TRIGGER_TASK(void const * argument)
Description: 根据拍照标志位，控制相机拍照
Return: 无返回值
*************************************************/
uint8_t    openCamStateFlag = 0;     /*0=没开机 1=待开 2=开机 3=待关  openCamStateFlag*/
uint8_t    canPhotoStateFlag = 0;    /*2=可以拍照*/
uint32_t   phototime = 0;

void  TRIGGER_TASK(void const * argument)
{
    EventBits_t eventTemp = 0;
	  uint8_t openWrongNum = 0;  /*连续开机失败计数 连续大于等于3则开机失败*/
    uint8_t i = 0;
    uint8_t daikaiflag = 0;    /*1=待开检测可以跳过了*/
    uint8_t stopAtoFlag = 0;   /*0=还没关拍照  1=已经关拍照了*/
    
    
	  osDelay(2000);
    taskENTER_CRITICAL();           								    /*进入临界区*/
    g_ds18b20Temp= cocoguojia_ds18b20GetTemp();
    taskEXIT_CRITICAL();            								    /*退出临界区*/
    osDelay(1000);
    
		
    while (0 == camTaskCanTakeOverFlag)                 /*等待相机接管*/
    {
        osDelay(10);
    }
    osDelay(100);
    daikaiflag = 0;
	while (1) 
	{  
			PSDK_LOG_DEBUG("TRIGGER_TASK running  \r\n" );			
      /*处于真是开机状态 在这里再一次限定一下(中断里也有限定)*/		
       if(SYSTEMSTATE_ONTRUE == offOrOnTureStateFlag)	 
       {      
           if(0 == daikaiflag) 
           {               
               if((4 == signalOrAutoModelFlag) || (6 == signalOrAutoModelFlag))  /*如果是自动拍照模式*/
               {
                 openCamStateFlag = 1;
                 eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,osWaitForever);/*事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待*/          
                 if(KEY_SHORT == (eventTemp&KEY_SHORT))  /*按键短按了*/
                 {                                 
                        FMQ_ON;
                        osDelay(500);
                        FMQ_OFF;
                        osDelay(100);
                        capture_FinishAndStartPhotoFlag = 0;   /*清除大疆协议拍照标志*/
                        openCamStateFlag = 2;
                        canPhotoStateFlag = 2;                     
                 }        
               }
               else
               {
                  openCamStateFlag = 2;
                  canPhotoStateFlag = 2;                   
               }
           }     
           daikaiflag = 1;
            /* 拍照标志置位 (读取usb时候也不拍照)   读大疆拍照协议标志 */ 
            if((1 == capture_FinishAndStartPhotoFlag) && (0 == readingFlag))  
            {		
                tim5CatchNum++;          /* 捕获并且被触发的有效总次数 */                       
                                  
                 /* 这里在限定一下 确保读数据的时候不拍照 */
                if(0 == readingFlag)
                {                                                       
										PHOTOALL_EN;           /* 拍照@@@@@@@@@@@@@@@@@  */
										osDelay(100);
										PHOTOALL_NO;                         
                    osDelay(200);
                    capture_FinishAndStartPhotoFlag = 0;		/* 拍照流程结束 将其清零 */                                                 
                }                                        
               osDelay(60); 
								
								
							/* 检测相机热靴是否到了 判断相机是否拍照 假如没拍照则补拍一次（热靴信号一定要准）*/
               for(i = 0; i <= 4; i++)
               {							
                    if(1 != camHotShotTable[i])
                    {
											i = 210;            /* 无热靴 ,无拍照 */
                    }
               } 								
							 if(100 < i)                /* 相机无热靴 不拍照才会进入 */
							 {
										PHOTOALL_EN;          /* 补拍@@@@@@@@@@@@@@@@@  */
										osDelay(100);
										PHOTOALL_NO;                         
                    osDelay(200);
               }
               for(i = 0; i <= 4; i++)    /* 热靴标志位清零  */
               {							
                     camHotShotTable[i] = 0;
               } 									
								
							 
							/* #########检测相机的开关机状态############ */
               for(i = 1; i <= 5; i++)
               {
                    if(1 != camPowerFlagTable[i])
                    {
											i = 210;            /* 开关机标志出问题才会进入 */
                    }
               } 
							 
							 if(100 < i)                /* 相机关机问题才会进入 */
							 {
									openWrongNum++;
									if(3 <= openWrongNum)
									{
											openWrongNum = 0;
											offOrOnTureStateFlag = SYSTEMSTATE_OPENFAIL;
											osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);
											FMQ_ON;
											osDelay(500);
											FMQ_OFF;
											osDelay(100);     
									}
                }
                else
                {
                    openWrongNum = 0;
                }
                capture_FinishAndStartPhotoFlag = 0;		/* 拍照流程结束 将其清零 */                
							
            }
            else    
            {
								;
            }
                     
             eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);/* 事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待 */         
             if(KEY_SHORT == (eventTemp&KEY_SHORT))   /* 按键短按了 */
             {
                 if((4 == signalOrAutoModelFlag) || (6 == signalOrAutoModelFlag))
                 {               
                     if(0 == stopAtoFlag)
                     {
                        canPhotoStateFlag = 0;
                        openCamStateFlag = 3;
                        stopAtoFlag = 1;
                        shutPhotoFlag = 1;
                        FMQ_ON;
                        osDelay(500);
                        FMQ_OFF;
                        osDelay(100);
                     }
                     else
                     {
                        canPhotoStateFlag = 0;
                        openCamStateFlag = 0;
                        offOrOnTureStateFlag = SYSTEMSTATE_GOTOSHUT;
                        osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);  /*任务的句柄，发送的数据 32位，覆盖当前通知*/
                        FMQ_ON;
                        osDelay(500);
                        FMQ_OFF;
                        osDelay(100);
                        stopAtoFlag = 0;
                        shutPhotoFlag = 0;                         
                     }    
                 }
                 else
                 {
                    canPhotoStateFlag = 0;
                    openCamStateFlag = 0;
                    offOrOnTureStateFlag = SYSTEMSTATE_GOTOSHUT;
                    osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);  /*任务的句柄，发送的数据 32位，覆盖当前通知*/
                    FMQ_ON;
                    osDelay(500);
                    FMQ_OFF;
                    osDelay(100);
                 }
             }        
        }
        else
        {
            daikaiflag = 0;
            canPhotoStateFlag = 0;
            openCamStateFlag = 0;
            
            if(1 == capture_FinishAndStartPhotoFlag)
            {
                capture_FinishAndStartPhotoFlag = 0;			/*拍照状态 将其清零*/
            }
        }	
        osDelay(2); 
    } 		
}

