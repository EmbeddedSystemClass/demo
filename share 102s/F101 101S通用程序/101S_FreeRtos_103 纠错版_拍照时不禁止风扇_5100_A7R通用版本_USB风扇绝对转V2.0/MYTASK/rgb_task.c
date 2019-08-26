#include "rgb_task.h"
 
TaskHandle_t 	RGB_TASK_Handle 	= NULL;		//创建任务句柄


//-------------------------------------------------------------------------
//任务函数
void  RGB_TASK ( void * p_arg )
{
    uint8_t ledrflag=0;//1=刚刚是在报错
    uint16_t k=0;
    uint8_t i=0;
    uint8_t num=0;
	while (1) 
	{ 

               
		if(SYSTEMSTATE_OFFTRUE==g_offOrOnTureStateFlag)
		{
			KEY_LED_OFF;
		}
		else if(SYSTEMSTATE_ONTRUE==g_offOrOnTureStateFlag)
		{
			KEY_LED_ON;
		}
		else
		{
			;
		}	
      
        if(1==g_camTaskCanTakeOverFlag)
        {        
            if(SYSTEMSTATE_ONTRUE==g_offOrOnTureStateFlag)  //真实开机
            {
                KEY_LED_ON;
                if(READ_ENING)
                {
                    k=0;
                    if((0==g_readingFlag)||(1==ledrflag))
                    {
                        RGBLED_ALLOFF;//灭
                    }
                    RGBLEDB_ON;       //蓝色灯常亮
                    FAN_ON;           //USB插入 风扇绝对转
                    g_readingFlag=1;
                    ledrflag=0;
                }
                else
                { 
                    num=0;
                    for(i=1;i<=5;i++)
                    {
                        num+=g_theTurelostNumTable[i];   //统计丢片数量
                    }
                    if((1==g_readingFlag)||(1==ledrflag))
                    {              
                        RGBLED_ALLOFF;//灭            
                    }
                    g_readingFlag=0;
                    ledrflag=0;
                    k++;
                    //----------------------------------------------------------------------------
                    //不真实丢片的时候 1s闪烁一次 每个相机最少都丢9张的时候 那是 闪烁一次100ms
                    if((50-num)==k)
                    {
                        RGBLEDG_ON;//绿灯闪烁
                    }
                    else  if(100-num-num<=k)
                    {
                        RGBLED_ALLOFF;//灭
                        k=0;
                    }                            
                }  
            }
            else
            {
                KEY_LED_OFF;                
                if(READ_ENING)
                {
                    if((0==g_readingFlag)||(1==ledrflag))
                    {
                        RGBLED_ALLOFF;//灭
                    }
                    k=0;
                    RGBLEDB_ON;     //蓝色灯常亮
                    FAN_ON;         //USB插入 风扇绝对转
                    g_readingFlag=1;
                    ledrflag=0;
                }
                else
                { 
                    if(1==g_readingFlag)
                    {
                        RGBLED_ALLOFF;//灭                                        
                    }  
                    
                    if((SYSTEMSTATE_CAMLOSE==g_offOrOnTureStateFlag)||(SYSTEMSTATE_SHUTFAIL==g_offOrOnTureStateFlag)||(SYSTEMSTATE_OPENFAIL==g_offOrOnTureStateFlag))
                    {
                        ledrflag=1;
                        k++;
                        //----------------------------------------------------------------------------
                        //不真实丢片的时候 1s闪烁一次 每个相机最少都丢9张的时候 那是 闪烁一次100ms
                        if(50==k)
                        {
                            RGBLEDR_ON;//红灯闪烁
                        }
                        else if(100<=k)
                        {
                            RGBLED_ALLOFF;//灭
                            k=0;
                        }         
                    }
                    else
                    {
                        if(1==ledrflag)
                        {
                            RGBLED_ALLOFF;//灭
                        }
                        ledrflag=0; 
                        k=0;
                        RGBLEDG_ON;//绿灯常亮
                    }                 
                    g_readingFlag=0;                                       
                }
            } 
        } 
        else
        {
            ;
        }
        
		vTaskDelay(10);
	}	
}


