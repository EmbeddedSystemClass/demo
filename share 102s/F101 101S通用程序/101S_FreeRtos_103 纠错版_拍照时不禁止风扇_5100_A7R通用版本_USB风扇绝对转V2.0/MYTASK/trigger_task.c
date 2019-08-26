#include "trigger_task.h" 
 
TaskHandle_t 	TRIGGER_TASK_Handle 	= NULL;		//创建任务句柄



//-------------------------------------------------------------------------
//任务函数
void  TRIGGER_TASK ( void * p_arg )
{
		EventBits_t eventTemp=0;
		uint8_t openWrongNum=0;//连续开机失败计数 连续大于等于3则开机失败
		uint8_t i=0;
	  uint8_t newT=0;//为0的时候 代表
	  uint8_t ttlStartFlag=0;//0=刚开始要个起始 1=正检测有效阶段 2=等待结束要个结尾

 
    vTaskDelay(1000);
    //#################################
    taskENTER_CRITICAL();           								    //进入临界区
    g_ds18b20Temp= cocoguojia_ds18b20GetTemp();
    taskEXIT_CRITICAL();            								    //退出临界区
    //################################## 
    
    while (0==g_camTaskCanTakeOverFlag) 
    {
        vTaskDelay(10);
    }
    vTaskDelay(100);
    
	while (1) 
	{  	
		if(0==g_pwmOrTtlModelFlag)			//PWM模式 
		{
			;
		}
		else if(1==g_pwmOrTtlModelFlag)	//TTL-H模式
		{
			if(0==ttlStartFlag)           //0=刚开始要个起始 
			{
					if(READTTL_IS0)             //电平为0          
					{
						if(1==newT)               //之前曾经为高
						{
							g_ttlCurrentNum=0;      //重新基数
							newT=0;                 //之前为0的标志也清零
						}		
						g_ttlCurrentNum++;        //当前触发计数
					}
					else                        //电平为1 
					{
							newT=1;
							g_ttlCurrentNum++;
							if(9999<=g_ttlCurrentNum)
							{
								g_ttlCurrentNum=9999;
							}
					}	
					if(10==g_ttlCurrentNum)     //加到10 表示低电平有10ms 标志低电平阶段得到认可 所以
					{
						ttlStartFlag=1;         //1=正检测有效阶段
					}
			}
			else if(1==ttlStartFlag)      //1=正检测有效阶段
			{	
					g_ttlCurrentNum++;
					if(READTTL_IS1)           //电平为1 
					{
						 if(g_ttlCurrentNum>=TTLEDGELONG)
						 {
								 ttlStartFlag=2;
						 }  
					}
					else
					{
						g_ttlCurrentNum=0;
					}	
			}
			else                          //2=等待结束要个结尾
			{
					g_ttlCurrentNum++;
					if(9999<=g_ttlCurrentNum)
					{
						g_ttlCurrentNum=9999;
					}
									
					if(READTTL_IS0)    //电平为0
					{           
						if(g_ttlCurrentNum<=(2020))  //在边沿模式下 只要边沿小于2s 都算是有效边沿
						{
								g_ttlTriggerNum=g_ttlCurrentNum;                                       
								g_triggerFlag=1;							
								g_capture_FinishAndStartPhotoFlag=1;//触发
						}	
						else
						{
								;//重新来
						}                  
						g_ttlCurrentNum=0;
						ttlStartFlag=0;						
					}
					else
					{
						;
					}	
			}	
		}
		else 								//TTL-L模式
		{
			if(0==ttlStartFlag)
			{
				if(READTTL_IS1)
				{
					if(1==newT)
					{
						g_ttlCurrentNum=0;
						newT=0;
					}		
					g_ttlCurrentNum++;
				}
				else
				{
					newT=1;
					g_ttlCurrentNum++;
					if(9999<=g_ttlCurrentNum)
					{
						g_ttlCurrentNum=9999;
					}
				}	
				if(10==g_ttlCurrentNum)
				{
					ttlStartFlag=1;
				}
			}
			else if(1==ttlStartFlag)
			{	
				g_ttlCurrentNum++;
				if(READTTL_IS0)
				{
                   if(g_ttlCurrentNum>=TTLEDGELONG)
                   {
                      ttlStartFlag=2;
                       	
                   }    
				}
				else
				{
					g_ttlCurrentNum=0;
				}	
			}
			else
			{
				g_ttlCurrentNum++;
				if(9999<=g_ttlCurrentNum)
				{
					g_ttlCurrentNum=9999;
				}
				if(READTTL_IS1)
				{                 
                    if(g_ttlCurrentNum<=2020)
                    {
                        g_ttlTriggerNum=g_ttlCurrentNum;    
                        g_triggerFlag=1;
                        g_capture_FinishAndStartPhotoFlag=1;//触发
                    }	
                    else
                    {
                        ;//重新来
                    }    
					g_ttlCurrentNum=0;
					ttlStartFlag=0;						
				}
				else
				{
					;
				}	
			}	
		}
    
       if(SYSTEMSTATE_ONTRUE==g_offOrOnTureStateFlag)		//处于真是开机状态 在这里再一次限定一下(中断里也有限定)
       {
            if(1==g_capture_FinishAndStartPhotoFlag)        //拍照标志置位
            {					
                cocoguojia_tim5CatchDisable();				//进行拍照流程 捕捉中断暂时禁止了  以免打扰实时性
                g_tim5CatchNum++;
                                                                 
                FMQ_ON; 
                g_triggerFlag=1;
                LEDPOINT_ON;
                                       
				PHOTOALL_EN;              //拍照@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@              
                vTaskDelay(100);
                vTaskDelay(120);
                vTaskDelay(50);
                        
                
                LEDPOINT_OFF;
                FMQ_OFF;  					
                           
                
                g_capture_FinishAndStartPhotoFlag=0;		//拍照流程结束 将其清零
                cocoguojia_tim5CatchEnable();				//流程结束后 使能捕捉中断	 
                
                for(i=1;i<=5;i++)
                {
                    if(1!=g_camPowerFlagTable[i])
                    {
                        i=210;
                    }
                }
                //i=50;//调试///////////////////////////
                if(100<i)
                {
                    openWrongNum++;
                    if(3<=openWrongNum)
                    {
                        openWrongNum=0;
                        g_offOrOnTureStateFlag=SYSTEMSTATE_OPENFAIL;
                        xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);            //任务的句柄，发送的数据 32位，覆盖当前通知
                        FMQ_ON;
                        vTaskDelay(500);
                        FMQ_OFF;
                        vTaskDelay(100);     
                    }
                }
                else
                {
                    openWrongNum=0;
                }              
            }
            else    
            {

            }
            
             eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待          
             if(KEY_SHORT==(eventTemp&KEY_SHORT))//按键短按了
             {
                    g_offOrOnTureStateFlag=SYSTEMSTATE_GOTOSHUT;
                    xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);            //任务的句柄，发送的数据 32位，覆盖当前通知
                    FMQ_ON;
                    vTaskDelay(500);
                    FMQ_OFF;
                    vTaskDelay(100);
             }        
        }
        else
        {           
            if(1==g_capture_FinishAndStartPhotoFlag)
            {
                g_capture_FinishAndStartPhotoFlag=0;			//关机状态 将其清零
            }
        }	
        vTaskDelay(1); 
    } 		
}

