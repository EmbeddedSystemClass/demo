
#include "cam_task.h"
#include "psdk_debug.h"

unsigned portBASE_TYPE uxHighWaterMarkcam; 

//-------------------------------------------------------------------------
//任务函数
void  CAM_TASK (void const * argument)
{
    uint32_t sendCom=0;   
    EventBits_t eventTemp=0;
		uint8_t n=0;
    uint8_t i=0;
    uint8_t j=0;

    //------------------------------------------------------------
    //等待标志g_camTaskCanTakeOverFlag置位 cam任务可以接管标志置位
    while (0==g_camTaskCanTakeOverFlag)   //在run_task.c里面 置1
    {
        osDelay(10);
    }
    osDelay(100);
    
    sendCom=USART2COM_SHUT;
    osMessagePut(usart2SendMessageQHandle,sendCom,100);//关机
    osDelay(100);
    
    //------------------------------------------------------------
    //问3此状态 主要是确定5个相机是否有缺失的相机 
    sendCom=USART2COM_ASK;
    osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态 
    osDelay(300); 
    sendCom=USART2COM_ASK;
    osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态 
    osDelay(300); 
    //------------------------------------------------------------
    //检测确实标志 确定相机是否缺失 
    if((0==g_camLoseFlagTable[1])||(0==g_camLoseFlagTable[2])||(0==g_camLoseFlagTable[3])||(0==g_camLoseFlagTable[4])||(0==g_camLoseFlagTable[5]))
    {
        g_offOrOnTureStateFlag=SYSTEMSTATE_CAMLOSE;									              //显示相机丢失界面
        osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);                            //任务的句柄，发送的数据 32位，覆盖当前通知
        FMQ_ON;
        osDelay(500);
        FMQ_OFF;
        osDelay(100);
        
        i=1;
        while(1==i)
        {
            //等待按键被按一下后 可以退出相机缺失报警
            eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);  //事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待
            if((eventTemp&(KEY_SHORT))==KEY_SHORT)
            {
                FMQ_OFF;
                i=0;
            }
            else
            {
                FMQ_ON;
                sendCom=USART2COM_ASK;
                osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态 
                osDelay(300);
                FMQ_OFF;
                osDelay(300);
                if((1==g_camLoseFlagTable[1])&&(1==g_camLoseFlagTable[2])&&(1==g_camLoseFlagTable[3])&&(1==g_camLoseFlagTable[4])&&(1==g_camLoseFlagTable[5]))
                {
                    i=0;
                }
            }
        }                 
    }
    
    //----------------------------------------------------------------------------------------
    //检测是否真正关机
    for(i=1;i<=5;i++)
    {
        if(0!=g_camPowerFlagTable[i])
        {
            i=210;
        }
    }
    if(200<=i)
    {
        g_offOrOnTureStateFlag=SYSTEMSTATE_SHUTFAIL;    //关机失败    	
    }
    else
    {
        g_offOrOnTureStateFlag=SYSTEMSTATE_OFFTRUE;     //关机成功
    }
    osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);                        //任务的句柄，发送的数据 32位，覆盖当前通知    
    FMQ_ON;
    osDelay(500);
    FMQ_OFF;
    osDelay(100);
    n=0;    
   
	while (1) 
	{  
        //-----------------------------------------------------------------------------------------------------------
        //真实关机了
        if(SYSTEMSTATE_OFFTRUE==g_offOrOnTureStateFlag)        
		{             
            sendCom=USART2COM_ASK;
            osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态
            osDelay(300);            
            for(i=1;i<=5;i++)
            {
                if(0!=g_camPowerFlagTable[i])
                {
                    i=210;
                }
            }
            //--------------------------------------------
            //如果有相机没关机或者突然开机
            if(200<=i)
            {
                sendCom=USART2COM_SHUT;
                osMessagePut(usart2SendMessageQHandle,sendCom,100);//关机
                osDelay(300);  
                g_offOrOnTureStateFlag=SYSTEMSTATE_SHUTFAIL;
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n=0;                
            }
            
            //--------------------------------------------
            //检测是否有按键被按下 按下则进入 开机中             
            eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待          
            if(KEY_SHORT==(eventTemp&KEY_SHORT))//按键短按了
            {
                g_offOrOnTureStateFlag=SYSTEMSTATE_GOTOOPEN;
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n=0;
            }        
        }

        if(SYSTEMSTATE_SHUTFAIL==g_offOrOnTureStateFlag)									//关机失败          
		{	
            n++;
            if((1==n)||(0==(n%3)))
            {
                FMQ_ON;
            }
            else
            {
                FMQ_OFF;
            }    
            if(8<=n)
            {
                sendCom=USART2COM_SHUT;
                osMessagePut(usart2SendMessageQHandle,sendCom,100);//关机
                osDelay(100);                
                n=0;
            }
           
            sendCom=USART2COM_ASK;
            osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态
            osDelay(300);       
            for(i=1;i<=5;i++)
            {
                if(0!=g_camPowerFlagTable[i])
                {
                    i=210;
                }
            }
            if(100>i)
            {
                g_offOrOnTureStateFlag=SYSTEMSTATE_OFFTRUE;
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n=0;   	
            }
            else
            {                
                eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待          
                if(KEY_SHORT==(eventTemp&KEY_SHORT))//按键短按了
                {
                    g_offOrOnTureStateFlag=SYSTEMSTATE_GOTOOPEN;
                    osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                    FMQ_ON;
                    osDelay(500);
                    FMQ_OFF;
                    osDelay(100);
                    n=0;
                } 
            }                
        } 

        if(SYSTEMSTATE_OPENFAIL==g_offOrOnTureStateFlag)									//开机失败          
		{	
            n++;
            if((1==n)||(0==(n%3)))
            {
                FMQ_ON;
            }
            else
            {
                FMQ_OFF;
            }    
            if(8<=n)
            {
                sendCom=USART2COM_OPEN;
                osMessagePut(usart2SendMessageQHandle,sendCom,100);//开机 
                osDelay(100);
                n=0;
            }          
            sendCom=USART2COM_ASK;
            osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态
            osDelay(300);         
            for(i=1;i<=5;i++)
            {
                if(1!=g_camPowerFlagTable[i])
                {
                    i=210;
                }
            }
            if(100>i)
            {
								osDelay(3000);
								osDelay(3000);
								osDelay(3000);
								sendCom=USART2COM_PHOTO;
								osMessagePut(usart2SendMessageQHandle,sendCom,0);//拍照   
								osDelay(3000);		
                g_offOrOnTureStateFlag=SYSTEMSTATE_ONTRUE;
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n=0;   	
            }
            else
            {                
                eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待          
                if(KEY_SHORT==(eventTemp&KEY_SHORT))//按键短按了
                {
                    g_offOrOnTureStateFlag=SYSTEMSTATE_GOTOSHUT;
                    osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                    FMQ_ON;
                    osDelay(500);
                    FMQ_OFF;
                    osDelay(100);
                } 
            }                
        } 

        if(SYSTEMSTATE_GOTOSHUT==g_offOrOnTureStateFlag)									//关机中          
		{	
            sendCom=USART2COM_SHUT;
            osDelay(200);
            osMessagePut(usart2SendMessageQHandle,sendCom,100);//关机 
            osDelay(100);
            
            for(j=0;j<50;j++)   //15s
            {              
                sendCom=USART2COM_ASK;
                osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态
                osDelay(300);
                
                for(i=1;i<=5;i++)
                {
                    if(1==g_camPowerFlagTable[i])
                    {
                        i=210;
                    }
                }
                if(100>i)
                {
                    j=220;
                }   
            }
            if(200<=j)
            {
                g_offOrOnTureStateFlag=SYSTEMSTATE_OFFTRUE;
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n=0;   	
            }
            else
            {
                sendCom=USART2COM_SHUT;
                osMessagePut(usart2SendMessageQHandle,sendCom,100);//关机
                osDelay(100); 
                
                g_offOrOnTureStateFlag=SYSTEMSTATE_SHUTFAIL;     	
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);             //任务的句柄，发送的数据 32位，覆盖当前通知    
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n=0;    
            } 
        }

        if(SYSTEMSTATE_GOTOOPEN==g_offOrOnTureStateFlag)									//开机中          
		{	  
            sendCom=USART2COM_OPEN;
            osMessagePut(usart2SendMessageQHandle,sendCom,100);//开机
            osDelay(100);      
            for(j=0;j<50;j++)//15s
            {
                sendCom=USART2COM_ASK;
                osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态
                osDelay(300);      
                for(i=1;i<=5;i++)
                {
                    if(0==g_camPowerFlagTable[i])
                    {
                        i=210;
                    }
                }
                if(100>i)
                {
                    j=220;
                }   
            }
            #ifdef DEBUG_TIAOSHI
            j=230;//调试///////调试//调试///调试/////调试////////调试/////
            #endif          
            if(200<=j)
            { 
				osDelay(3000);
			    osDelay(3000);
			    osDelay(3000);
			    sendCom=USART2COM_PHOTO;
                osMessagePut(usart2SendMessageQHandle,sendCom,0);//拍照   
			    osDelay(3000);				
                g_offOrOnTureStateFlag=SYSTEMSTATE_ONTRUE;                
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);            //任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n=0; 
                                
            }
            else
            {
                sendCom=USART2COM_ASK;
                osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态
                osDelay(300);
                g_offOrOnTureStateFlag=SYSTEMSTATE_OPENFAIL;     	
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);             //任务的句柄，发送的数据 32位，覆盖当前通知    
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                n=0;    
            } 
        }                               
        osDelay(1);
    }
}




