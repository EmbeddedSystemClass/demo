#include "rgb_task.h"
#include "psdk_debug.h" 
 
 
 
uint16_t triggeStartDelayN=0;   //开机后 开始拍照之前的延时计数器 5s后拍照
uint8_t  g_shutPhotoFlag=0;     //自动拍照模式(2s 或者 3s 拍照模式时) 0=不关拍照标志   1=关拍照标志


//-------------------------------------------------------------------------
//任务函数
void  RGB_TASK  (void const * argument)
{
    uint16_t k=0;
    uint16_t triggeN=0;//自动拍照模式时 时间间隔定时器
	while (1) 
	{                           
        if(1==g_camTaskCanTakeOverFlag)
        {        
            if(SYSTEMSTATE_ONTRUE==g_offOrOnTureStateFlag)
            {
                KEY_LED_ON;
                k++;
                if(1==g_readingFlag)
                {
                   k=0;            
                }
                else
                {                  
                    if(50==k)
                    {
                        if(1==g_openCamStateFlag) 
                        { 
                            RGBLEDY_ON;
                        }
                        else if(3==g_openCamStateFlag) 
                        { 
                            RGBLEDY_ON;
                        } 
                        else
                        {                            
                            RGBLEDG_ON;//绿灯常亮
                        }
                    }
                    else if(100<=k)
                    {
                        if(1==g_openCamStateFlag) 
                        { 
                            LEDR_OFF;//红灯常灭
                            LEDG_OFF;//绿灯常灭
                        }
                        else if(3==g_openCamStateFlag) 
                        { 
                           LEDR_OFF;//红灯常灭
                           LEDG_OFF;//绿灯常灭
                        } 
                        else
                        {
                            LEDR_OFF;//红灯常灭                            
                            LEDG_OFF;//绿灯常灭
                        }
                        k=0;
                    }
                }                    
                
                //--------------------------------------------------------------------------------------------
                //自动模式时 定时拍照
                if(0==g_shutPhotoFlag)//自动拍照模式(2s 或者 3s 拍照模式时) 0=不关拍照标志  1=关拍照标志
                {
                    if(2==g_openCamStateFlag)
                    {
                        triggeStartDelayN++;
                        triggeN++;
                    }
                    else
                    {
                        triggeStartDelayN=0;
                        triggeN=0;
                    }    
                    if(500<triggeStartDelayN)
                    {
                        triggeStartDelayN=500;
                        if(4==g_signalOrAutoModelFlag)
                        {
                            if(200<=triggeN)
                            {
                                g_capture_FinishAndStartPhotoFlag=1;
                                triggeN=0;
                            }
                        }
                        else if (6==g_signalOrAutoModelFlag)
                        {
                            if(300<=triggeN)
                            {
                                g_capture_FinishAndStartPhotoFlag=1;
                                triggeN=0;
                            }
                        }
                        else//如果是信号触发拍照
                        {
                            triggeStartDelayN=0;
                            triggeN=0;
                        }
                    }                        
                }
                else
                {
                    triggeStartDelayN=0;    //开机后 开始拍照之前的延时计数器 5s后拍照
                    triggeN=0;              //自动拍照模式(2s 或者 3s 拍照模式时) 0=不关拍照标志   1=关拍照标志
                }    
            }
            else
            {
                g_shutPhotoFlag=0;      //自动拍照模式(2s 或者 3s 拍照模式时) 0=不关拍照标志  1=关拍照标志
                triggeStartDelayN=0;    //开机后 开始拍照之前的延时计数器 5s后拍照
                triggeN=0;              //自动拍照模式(2s 或者 3s 拍照模式时) 0=不关拍照标志   1=关拍照标志
                KEY_LED_OFF;
                k=0;
                if(1==g_readingFlag)
                {
                   ;            
                } 
                else
                {    
                    RGBLEDG_ON;//绿灯常亮  
                }                    
            } 
        } 
        else
        {
            ;
        }
        osDelay(10);
	}	
}


