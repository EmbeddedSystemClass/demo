#include "rgb_task.h"
#include "psdk_debug.h" 
 
 
uint16_t triggeStartDelayN=0;   /* 开机后 开始拍照之前的延时计数器 5s后拍照 */
uint8_t  shutPhotoFlag=0;       /* 自动拍照模式(2s 或者 3s 拍照模式时) 0=不关拍照标志   1=关拍照标志 */

/*************************************************
Function: void  RGB_TASK  (void const * argument)
Description: 不同工作模式下灯的状态变化，
Return: 无返回值
*************************************************/
void  RGB_TASK  (void const * argument)
{
    uint16_t k=0;
    uint16_t triggeN=0;//自动拍照模式时 时间间隔定时器
	while (1) 
	{  
//	PSDK_LOG_DEBUG("RGB_TASK running  \r\n" );				
        if(1==camTaskCanTakeOverFlag)
        {        
            if(SYSTEMSTATE_ONTRUE==offOrOnTureStateFlag)
            {
                KEY_LED_ON;
                k++;
                if(1==readingFlag)
                {
                   k=0;            
                }
                else
                {                  
                    if(50==k)
                    {
                        if(1==openCamStateFlag) 
                        { 
                            RGBLEDY_ON;
                        }
                        else if(3==openCamStateFlag) 
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
                        if(1==openCamStateFlag) 
                        { 
                            LEDR_OFF;//红灯常灭
                            LEDG_OFF;//绿灯常灭
                        }
                        else if(3==openCamStateFlag) 
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
                if(0==shutPhotoFlag)//自动拍照模式(2s 或者 3s 拍照模式时) 0=不关拍照标志  1=关拍照标志
                {
                    if(2==openCamStateFlag)
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
                        if(4==signalOrAutoModelFlag)
                        {
                            if(200<=triggeN)
                            {
                                capture_FinishAndStartPhotoFlag=1;
                                triggeN=0;
                            }
                        }
                        else if (6==signalOrAutoModelFlag)
                        {
                            if(300<=triggeN)
                            {
                                capture_FinishAndStartPhotoFlag=1;
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
                shutPhotoFlag=0;      //自动拍照模式(2s 或者 3s 拍照模式时) 0=不关拍照标志  1=关拍照标志
                triggeStartDelayN=0;    //开机后 开始拍照之前的延时计数器 5s后拍照
                triggeN=0;              //自动拍照模式(2s 或者 3s 拍照模式时) 0=不关拍照标志   1=关拍照标志
                KEY_LED_OFF;
                k=0;
                if(1==readingFlag)
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


