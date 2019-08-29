 
#include "trigger_task.h" 
#include "psdk_debug.h" 
uint16_t g_ds18b20Temp=0;     //ds18b20 温度暂存

uint8_t g_openCamStateFlag=0;//0=没开机 1=待开 2=开机 3=待关

uint8_t g_canPhotoStateFlag=0;//2=可以拍照
uint32_t phototime=0,phototimeadd=0;
unsigned portBASE_TYPE uxHighWaterMarktrigger; 

//-------------------------------------------------------------------------
//任务函数
void  TRIGGER_TASK(void const * argument)
{
    EventBits_t eventTemp=0;
    uint8_t openWrongNum=0;//连续开机失败计数 连续大于等于3则开机失败
    uint8_t n=0;
    uint8_t i=0;
    uint8_t daikaiflag=0;//1=待开检测可以跳过了
    uint16_t k=20000;

    uint32_t sendCom=0;
    
    uint8_t stopAtoFlag=0;//0=还没关拍照     1=已经关拍照了
    
	  uxHighWaterMarktrigger=uxTaskGetStackHighWaterMark( TRIGGER_TASK_Handle );    //查询大小  
	  PSDK_LOG_DEBUG("trigger_TASK  %d\r\n", uxHighWaterMarktrigger );
    
	  osDelay(2000);
    //#################################
    taskENTER_CRITICAL();           								    //进入临界区
    cocoguojia_ds18b20GetTemp();
    cocoguojia_ds18b20GetTemp();
    cocoguojia_ds18b20GetTemp();
    taskEXIT_CRITICAL();            								    //退出临界区
    //################################## 
    
    osDelay(1000);
    //#################################
    taskENTER_CRITICAL();           								    //进入临界区
    g_ds18b20Temp= cocoguojia_ds18b20GetTemp();
    taskEXIT_CRITICAL();            								    //退出临界区
    //################################## 
    
    while (0==g_camTaskCanTakeOverFlag) 
    {
        osDelay(10);
    }
    osDelay(100);
    daikaiflag=0;
	while (1) 
	{             
       if(SYSTEMSTATE_ONTRUE==g_offOrOnTureStateFlag)		                //处于真是开机状态 在这里再一次限定一下(中断里也有限定)
       {      
           if(0==daikaiflag) 
           {               
               if((4==g_signalOrAutoModelFlag)||(6==g_signalOrAutoModelFlag))  //如果是自动拍照模式
               {
                 g_openCamStateFlag=1;
                 eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,osWaitForever);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待          
                 if(KEY_SHORT==(eventTemp&KEY_SHORT))//按键短按了
                 {                                 
                        FMQ_ON;
                        osDelay(500);
                        FMQ_OFF;
                        osDelay(100);
                        g_capture_FinishAndStartPhotoFlag=0;   //清除大疆协议拍照标志
                        g_openCamStateFlag=2;
                        g_canPhotoStateFlag=2;                     
                 }        
               }
               else
               {
                  g_openCamStateFlag=2;
                  g_canPhotoStateFlag=2;                   
               }
           }
            
           daikaiflag=1;
             
//          if((1==g_capture_FinishAndStartPhotoFlag)&&(0==g_readingFlag))  //拍照标志置位 (读取usb时候也不拍照)   读大疆拍照协议标志
            if((1==g_capture_FinishAndStartPhotoFlag)&&(0==g_readingFlag))  //拍照标志置位 (读取usb时候也不拍照)   读大疆拍照协议标志
            {		
								phototimeadd = phototime;
								phototime=0;
                g_tim5CatchNum++;
                                
                for(n=1;n<=5;n++)							                //所有热靴检测有效标志全部清零
                {
                    g_camRtkFlagTable[n]=0;
                }
                                  
                //这里在限定一下 确保读数据的时候不拍照
                if(0==g_readingFlag)
                {
                    FMQ_ON; 
                    g_fanTurePhotoEnFlag=1;//风扇要绝对停止了 拍照启动了                                                          
                    sendCom=USART2COM_PHOTO;
                    osMessagePut(usart2SendMessageQHandle,sendCom,0);//拍照                              
                    osDelay(100);
                    osDelay(120);
                    osDelay(50);
                    g_capture_FinishAndStartPhotoFlag=0;		//拍照流程结束 将其清零                   
                    g_fanTurePhotoEnFlag=0;//风扇要恢复了 拍照停止了
                    FMQ_OFF;                                 
                }                    
         
               k=0;              
               //osDelay(60);
               //LEDPOINT_OFF;
               //#################################
               taskENTER_CRITICAL();           								    //进入临界区
               g_ds18b20Temp=cocoguojia_ds18b20GetTemp();
               taskEXIT_CRITICAL();            								    //退出临界区
               //##################################                 
               sendCom=USART2COM_ASK;
               osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态 
//               osDelay(180);
//               osDelay(20);                
//               osDelay(50);                                 
                for(i=1;i<=5;i++)
                {
                    if(1!=g_camPowerFlagTable[i])
                    {
											i=210;            //开关机标志出问题才会进入
                    }
                }
                
                
                #ifdef DEBUG_TIAOSHI
                i=50;//调试 时候使能///////////////////////////
                #endif
                
                if(100<i)   //相机关机问题才会进入
                {
                    openWrongNum++;
                    if(3<=openWrongNum)
                    {
                        openWrongNum=0;
                        g_offOrOnTureStateFlag=SYSTEMSTATE_OPENFAIL;
                        osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);
                        FMQ_ON;
                        osDelay(500);
                        FMQ_OFF;
                        osDelay(100);     
                    }
                }
                else
                {
                    openWrongNum=0;
                }
                g_capture_FinishAndStartPhotoFlag=0;		//拍照流程结束 将其清零                
            }
            else    //必须有这个 30秒刷新一次  如果不刷新 一直开机但不拍照的时候 就不能温度检测 温度就会一直在一个数值上
            {
                k++;
                if(15000<=k)
                {
                    //#################################
                    taskENTER_CRITICAL();           								    //进入临界区
                    g_ds18b20Temp=cocoguojia_ds18b20GetTemp();
                     taskEXIT_CRITICAL();            								    //退出临界区
                    //################################## 
                    k=0;
                }
            }
                     
             eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待          
             if(KEY_SHORT==(eventTemp&KEY_SHORT))//按键短按了
             {
                 if((4==g_signalOrAutoModelFlag)||(6==g_signalOrAutoModelFlag))
                 {               
                     if(0==stopAtoFlag)
                     {
                        g_canPhotoStateFlag=0;
                        g_openCamStateFlag=3;
                        stopAtoFlag=1;
                        g_shutPhotoFlag=1;
                        FMQ_ON;
                        osDelay(500);
                        FMQ_OFF;
                        osDelay(100);
                     }
                     else
                     {
                         g_canPhotoStateFlag=0;
                        g_openCamStateFlag=0;
                        g_offOrOnTureStateFlag=SYSTEMSTATE_GOTOSHUT;
                        osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                        FMQ_ON;
                        osDelay(500);
                        FMQ_OFF;
                        osDelay(100);
                        stopAtoFlag=0;
                        g_shutPhotoFlag=0;                         
                     }    
                 }
                 else
                 {
                    g_canPhotoStateFlag=0;
                    g_openCamStateFlag=0;
                    g_offOrOnTureStateFlag=SYSTEMSTATE_GOTOSHUT;
                    osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                    FMQ_ON;
                    osDelay(500);
                    FMQ_OFF;
                    osDelay(100);
                 }
             }        
        }
        else
        {
            daikaiflag=0;
            g_canPhotoStateFlag=0;
            g_openCamStateFlag=0;
            k++;
            if(15000<=k)
            {
                //#################################
                taskENTER_CRITICAL();           								    //进入临界区
                g_ds18b20Temp=cocoguojia_ds18b20GetTemp();
                taskEXIT_CRITICAL();            								    //退出临界区
                //################################## 
                k=0;
            }
            
            if(1==g_capture_FinishAndStartPhotoFlag)
            {
                g_capture_FinishAndStartPhotoFlag=0;			//拍照状态 将其清零
            }
        }	
        osDelay(2); 
    } 		
}

