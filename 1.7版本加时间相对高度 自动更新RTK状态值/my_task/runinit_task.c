 
#include "runinit_task.h"
#include "psdk_debug.h" 

static void PowerUpAndShowLogo(void);                   //系统刚上和电显示LOGO
static void ReadEepromAndShowResultAndSetDoWhit(void);  //读EEPROM并显示上次设置结果并且处理设置参数
static void ShowSystemInit(void);                       //OLED显示系统初始化


 //cocoguojia say:
 //RUN_TASK 此任务负责具体的逻辑 配合一些关键的状态标志 控制系统的走向
 //在系统状态里面 才能改变系统状态标志

unsigned portBASE_TYPE uxHighWaterMarkrun;  


//-------------------------------------------------------------------------
//任务函数
void  RUNINIT_TASK (void const * argument)
{ 
    uint32_t sendCom=0;

		uxHighWaterMarkrun=uxTaskGetStackHighWaterMark( CAM_TASK_Handle );    //查询大小  
	  PSDK_LOG_DEBUG("runinit_TASK  %d\r\n", uxHighWaterMarkrun );
    PowerUpAndShowLogo();                   //系统刚上和电显示LOGO
    
    osDelay(20); 
    sendCom=USART2COM_ASK;
    osMessagePut(usart2SendMessageQHandle,sendCom,0); 
    osDelay(300);
    sendCom=USART2COM_ASK;
    osMessagePut(usart2SendMessageQHandle,sendCom,0);  
    osDelay(300);
    sendCom=USART2COM_ASK;
    osMessagePut(usart2SendMessageQHandle,sendCom,0); 
    osDelay(300);
    
    ReadEepromAndShowResultAndSetDoWhit();  //读EEPROM并显示上次设置结果并且处理设置参数
    ShowSystemInit();                       //OLED显示系统初始化
	
    osDelay(100);
	g_camTaskCanTakeOverFlag=1;	

    osDelay(500);
    osThreadTerminate(RUNINIT_TASK_Handle); //删除RUNINIT_TASK_Handle任务 
}
 


//------------------------------------------
//系统刚上和电显示LOGO
static void PowerUpAndShowLogo(void)
{
    BaseType_t xReturn=pdPASS;

	uint16_t n;
    
    xReturn=xReturn;	

	g_offOrOnTureStateFlag=OLEDSHOWSTATE_LOGO;			    //显示LOGO                             
    osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);//任务的句柄，发送的数据 32位，覆盖当前通知                                 							 			 							 
	n=10;
    while(n--)//蜂鸣器间歇鸣叫2s 红灯闪烁
	{		
		FMQ_ON;
		RGBLEDR_ON;
		//LEDPOINT_ON;
        KEY_LED_ON;
        osDelay(100);
		FMQ_OFF;
		RGBLED_ALLOFF;
		//LEDPOINT_OFF;
        KEY_LED_OFF;
		osDelay(100);
	}

	FMQ_ON;//蜂鸣器长鸣半秒 表示系统上电成功
	osDelay(500);
	FMQ_OFF;
	osDelay(100);
	RGBLEDG_ON;//绿灯常亮 
}



uint8_t currentPageNum=1;//当前的配置页 1=pwm ttl选择  2=ttl 边沿选择  3=最终结果显示

//--------------------------------------------
//读EEPROM并显示上次设置结果并且处理设置参数
static void ReadEepromAndShowResultAndSetDoWhit(void)
{
    BaseType_t xReturn=pdPASS;
    EventBits_t eventTemp=0;
    uint8_t theLastsignalOrAutoModelFlag=0;
    uint8_t i=0;
    uint32_t sendCom=0;
    xReturn=xReturn;

    //---------------------------------------------------------------------------------
	//读eeprom 
	cocoguojia_readSetParameter();

	//---------------------------------------------------------------------------------------------------
	//进入到设置模式 page3 显示当前设置结果
	g_offOrOnTureStateFlag=OLEDSHOWSTATE_SETMODEL4;
    osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);//任务的句柄，发送的数据 32位，覆盖当前通知
   	FMQ_ON;
    osDelay(500);
    FMQ_OFF;				 
    osDelay(200);
    osDelay(1000);
    osDelay(1000);
    
    sendCom=USART2COM_ASK;
    xReturn=osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态   
    osDelay(1000);

    //按键检测
    eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件 

    if(KEY_SHORT==(eventTemp&KEY_SHORT))//按键短按了
    {
        g_currentRowNum=g_signalOrAutoModelFlag;
        theLastsignalOrAutoModelFlag=g_signalOrAutoModelFlag;	
		g_setModelFlag=1;       //进入到设置界面 page2
    }
    
	if(1==g_setModelFlag)       //如果进入到设置界面
	{
		//---------------------------------------------------------------------------------------------------
		//显示设置界面1
		g_offOrOnTureStateFlag=OLEDSHOWSTATE_SETMODEL1;
		osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);//任务的句柄，发送的数据 32位，覆盖当前通知						 
		FMQ_ON;
        osDelay(500);
        FMQ_OFF;
        osDelay(100);
	}
	while(1==g_setModelFlag)
	{
        //--------------------------------------------------------------------
	    //设置检测 在显示系统初始化之前按一下按钮即可		
        eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件

        if(KEY_SHORT==(eventTemp&KEY_SHORT))//按键短按了
        {
            if(0==g_currentRowNum)
            {       
                g_currentRowNum=2;//signal模式
                g_signalOrAutoModelFlag=g_currentRowNum;
            }
            else if(2==g_currentRowNum)
            {
                g_currentRowNum=4;//auto 2s
                g_signalOrAutoModelFlag=g_currentRowNum;
            }		
            else if(4==g_currentRowNum)
            {
                g_currentRowNum=6;//auto 3s
                g_signalOrAutoModelFlag=g_currentRowNum;
            }
            else
            {
                g_signalOrAutoModelFlag=theLastsignalOrAutoModelFlag;//保存一下之前的设置
                g_currentRowNum=0;
            }             
        }
        else if(KEY_LONG==(eventTemp&KEY_LONG))//按键长按了
        {	
            if(0!=g_currentRowNum)
            {
                //---------------------------------------------------------------------------------------------------
                //进入到设置模式 page3
                g_offOrOnTureStateFlag=OLEDSHOWSTATE_SETMODEL3;
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);//任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                //---------------------------------------------------------------------------------
                //写eeprom 
                cocoguojia_writeSetParameter();												 
                osDelay(200);
                osDelay(3000);
                g_setModelFlag=0;
            }
            else
            {
                //---------------------------------------------------------------------------------------------------
                //进入到修复模式 page1
                g_offOrOnTureStateFlag=OLEDSHOWSTATE_REPAIRMODEL1;
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);//任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                sendCom=USART2COM_OPEN;
                osMessagePut(usart2SendMessageQHandle,sendCom,100);//开机
                osDelay(1000);
                for(i=0;i<20;i++)
                {
                   sendCom=USART2COM_ASK;
                   osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态
                   osDelay(500);
                   sendCom=USART2COM_OPEN;//开机
                   osMessagePut(usart2SendMessageQHandle,sendCom,100);
                   osDelay(500);
                   if((1==g_camPowerFlagTable[1])&&(1==g_camPowerFlagTable[2])&&(1==g_camPowerFlagTable[3])&&(1==g_camPowerFlagTable[4])&&(1==g_camPowerFlagTable[5]))
                   {		   
                       i=200;
                   }
                }
				osDelay(3000);
				osDelay(3000);
				osDelay(3000);
				sendCom=USART2COM_PHOTO;
                osMessagePut(usart2SendMessageQHandle,sendCom,0);//拍照   
				osDelay(3000);
                
                //---------------------------------------------------------------------------------------------------
                //进入到修复模式 page2
                g_offOrOnTureStateFlag=OLEDSHOWSTATE_REPAIRMODEL2;
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);//任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                i=0;
                while(0==i)
                {
                     eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待         
                     if(KEY_SHORT==(eventTemp&KEY_SHORT))//按键短按了
                     {
                        sendCom=USART2COM_KEY;
                        osMessagePut(usart2SendMessageQHandle,sendCom,0);//KEY 按键
                        FMQ_ON;
                        osDelay(100);
                        FMQ_OFF;
                        osDelay(100);
                     } 
                     else if(KEY_LONG==(eventTemp&KEY_LONG))//长按了
                     {
                           i=1;
                     }
                     else
                     {
                       osDelay(10);
                     }                         
                }                 
                
                //---------------------------------------------------------------------------------------------------
                //进入到修复模式 page3
                g_offOrOnTureStateFlag=OLEDSHOWSTATE_REPAIRMODEL3;
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);//任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                sendCom=USART2COM_SHUT;
                osMessagePut(usart2SendMessageQHandle,sendCom,100);//关机
                osDelay(1000);
                for(i=0;i<20;i++)
                {
                   sendCom=USART2COM_ASK;
                   osMessagePut(usart2SendMessageQHandle,sendCom,0);//分别问5个相机的状态
                   osDelay(500);
                   sendCom=USART2COM_SHUT;
                   osMessagePut(usart2SendMessageQHandle,sendCom,100);//关机
                   osDelay(500);
                   if((0==g_camPowerFlagTable[1])&&(0==g_camPowerFlagTable[2])&&(0==g_camPowerFlagTable[3])&&(0==g_camPowerFlagTable[4])&&(0==g_camPowerFlagTable[5]))
                   {
                        i=200;
                   }
                }
                osDelay(2000);
                //---------------------------------------------------------------------------------------------------
                //进入到修复模式 page4
                g_offOrOnTureStateFlag=OLEDSHOWSTATE_REPAIRMODEL4;
                osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);//任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                while(1)
                {
                    osDelay(100);
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


//--------------------------------------------
//OLED显示系统初始化
static void ShowSystemInit(void)
{
	//---------------------------------------------------------------------------------------------------
	//显示 系统初始化
	g_offOrOnTureStateFlag=OLEDSHOWSTATE_INIT;
    osMessagePut(oledShowMessageQHandle,g_offOrOnTureStateFlag,0);//任务的句柄，发送的数据 32位，覆盖当前通知
    FMQ_ON;
    osDelay(500);
    FMQ_OFF;
    osDelay(100);    
								 
}

