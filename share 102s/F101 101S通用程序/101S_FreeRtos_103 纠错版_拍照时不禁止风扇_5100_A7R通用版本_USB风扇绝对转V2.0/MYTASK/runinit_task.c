#include "runinit_task.h"

static void PowerUpAndShowLogo(void);                   /* 系统刚上和电显示LOGO */
static void ReadEepromAndShowResultAndSetDoWhit(void);  /* 读EEPROM并显示上次设置结果并且处理设置参数 */
static void ShowSystemInit(void);                       /* LED显示系统初始化 */


/***********************************
say:
RUN_TASK 此任务负责具体的逻辑 配合一些关键的状态标志 控制系统的走向
在系统状态里面 才能改变系统状态标志 
***********************************/

 
/*************************************************
Function: void  RUNINIT_TASK (void const * argument)
Description: 控制开机走向
Return: 无返回值
*************************************************/
TaskHandle_t 	RUNINIT_TASK_Handle = NULL;		//创建任务句柄


void  RUNINIT_TASK (void* parameter)
{ 


    PowerUpAndShowLogo();                   /* 系统刚上和电显示LOGO */ 
       
    ReadEepromAndShowResultAndSetDoWhit();  /* 读EEPROM并显示上次设置结果并且处理设置参数 */
    ShowSystemInit();                       /* OLED显示 系统初始化 */
	
    vTaskDelay(100);
	  g_camTaskCanTakeOverFlag=1;	


    vTaskDelay(500);
    vTaskDelete(RUNINIT_TASK_Handle); 			/* 删除RUNINIT_TASK_Handle任务 */ 
    vTaskDelay(10);

}
 

/* 系统刚上和电显示LOGO */
static void PowerUpAndShowLogo(void)
{
    BaseType_t xReturn = pdPASS;

	  uint16_t n = 0;
    
    xReturn = xReturn;	

	  g_offOrOnTureStateFlag = OLEDSHOWSTATE_LOGO;			    //显示LOGO                             
    xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);//任务的句柄，发送的数据 32位，覆盖当前通知                                 							 			 							 
	  n=10;
    while(n--)//蜂鸣器间歇鸣叫2s 红灯闪烁
		{		
			FMQ_ON;
			RGBLEDR_ON;
			LEDPOINT_ON;
					KEY_LED_ON;
					vTaskDelay(100);
			FMQ_OFF;
			RGBLED_ALLOFF;
			LEDPOINT_OFF;
					KEY_LED_OFF;
			vTaskDelay(100);
		}

	FMQ_ON;/* 蜂鸣器长鸣半秒 表示系统上电成功 */
	vTaskDelay(500);
	FMQ_OFF;
	vTaskDelay(100);
	RGBLEDG_ON;/* 绿灯常亮 */
}



uint8_t currentPageNum = 1;//当前的配置页 1=pwm ttl选择  2=ttl 边沿选择  3=最终结果显示


/* 读EEPROM并显示上次设置结果并且处理设置参数 */
static void ReadEepromAndShowResultAndSetDoWhit(void)
{
    BaseType_t xReturn = pdPASS;
    EventBits_t eventTemp = 0;
    uint8_t theLastpwmOrTtlModelFlag = 0;
    uint8_t i = 0;
    xReturn = xReturn;

 	/* 读eeprom */ 
	cocoguojia_readSetParameter();

   /* 进入到设置模式 page3 显示当前设置结果 */
	  g_offOrOnTureStateFlag = OLEDSHOWSTATE_SETMODEL4;
    xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);//任务的句柄，发送的数据 32位，覆盖当前通知
   	FMQ_ON;
    vTaskDelay(500);
    FMQ_OFF;				 
    vTaskDelay(200);
    vTaskDelay(1000);
    vTaskDelay(1000);
    
     /* 按键检测 */
    eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件  
    if(KEY_SHORT == (eventTemp&KEY_SHORT))/* 按键短按了 */
    {
		if(0 == g_pwmOrTtlModelFlag)
		{        
			g_currentRowNum = 2;  /* 上次设置为 PWM */
		}
		else if(1 == g_pwmOrTtlModelFlag)
		{
			g_currentRowNum = 4;  /* 上次设置为 TTL-H */
		}
		else
		{
			g_currentRowNum = 6;  /* 上次设置为 TTL-L */
		}	
        theLastpwmOrTtlModelFlag = g_pwmOrTtlModelFlag;		
		    g_setModelFlag = 1; /* 进入到设置界面 page1 */
    }
   
    
	if(1 == g_setModelFlag)   /* 如果进入到设置界面 */
	{

		/* 显示设置界面1 */
		g_offOrOnTureStateFlag = OLEDSHOWSTATE_SETMODEL1;
		xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */						 
		FMQ_ON;
        vTaskDelay(500);
        FMQ_OFF;
        vTaskDelay(100);
	}
	while(1 == g_setModelFlag)
	{

	    /* 设置检测 在显示系统初始化之前按一下按钮即可 */		
        eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);/* 事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件 */

        if(KEY_SHORT == (eventTemp&KEY_SHORT))/* 按键短按了 */
        {
            if(0 == g_currentRowNum)
            {
                g_pwmOrTtlModelFlag = 0;
                g_currentRowNum = 2;
            }
            else if(2 == g_currentRowNum)
            {
                g_pwmOrTtlModelFlag = 1;
                g_currentRowNum = 4;
            }		
            else if(4 == g_currentRowNum)
            {
                g_pwmOrTtlModelFlag = 2;
                g_currentRowNum = 6;
            }
            else
            {
                g_pwmOrTtlModelFlag = theLastpwmOrTtlModelFlag;
                g_currentRowNum = 0;
            }             
        }
        else if(KEY_LONG == (eventTemp&KEY_LONG))/* 按键长按了 */
        {	
            if(0 != g_currentRowNum)
            {

                /* 进入到设置模式 page3 */
                g_offOrOnTureStateFlag = OLEDSHOWSTATE_SETMODEL3;
                xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);

                /* 写eeprom */
                cocoguojia_writeSetParameter();												 
                vTaskDelay(200);
                vTaskDelay(3000);
                g_setModelFlag = 0;
            }
            else
            {

                /* 进入到修复模式 page1 */
                g_offOrOnTureStateFlag=OLEDSHOWSTATE_REPAIRMODEL1;
                xReturn=xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);//任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);               
                POWER_EN;                     /* 开机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                vTaskDelay(100);
                for(i = 0; i < 20; i++)
                {
                                             
                   POWER_EN;                  /* 开机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                   vTaskDelay(500);
                   if((1==g_camPowerFlagTable[1])&&(1==g_camPowerFlagTable[2])&&(1==g_camPowerFlagTable[3])&&(1==g_camPowerFlagTable[4])&&(1==g_camPowerFlagTable[5]))
                   {
                        i = 200;
                   }
                }
                
                /* 进入到修复模式 page2 */
                g_offOrOnTureStateFlag = OLEDSHOWSTATE_REPAIRMODEL2;
                xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                i=0;
                while(0 == i)
                {
                     eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待         
                     if(KEY_SHORT == (eventTemp&KEY_SHORT))/* 按键短按了 */
                     {
                        OK_EN;       /* KEY 按键@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
							          vTaskDelay(100);
						           	OK_NO;
											 
											 
                        FMQ_ON;
                        vTaskDelay(100);
                        FMQ_OFF;
                        vTaskDelay(100);
                     } 
                     else if(KEY_LONG == (eventTemp&KEY_LONG))/* 长按了 */
                     {
                           i = 1;
                     }
                     else
                     {
                       vTaskDelay(10);
                     }                         
                }                 
                

                /* 进入到修复模式 page3 */
                g_offOrOnTureStateFlag = OLEDSHOWSTATE_REPAIRMODEL3;
                xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);//任务的句柄，发送的数据 32位，覆盖当前通知
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
								POWER_NO;                     /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
                vTaskDelay(1000);
                for(i = 0; i < 20; i++)
                {
																							
								   POWER_NO;                  /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                   vTaskDelay(500);
                   if((0==g_camPowerFlagTable[1])&&(0==g_camPowerFlagTable[2])&&(0==g_camPowerFlagTable[3])&&(0==g_camPowerFlagTable[4])&&(0==g_camPowerFlagTable[5]))
                   {
                        i = 200;
                   }
                }
                vTaskDelay(2000);
    
                /* 进入到修复模式 page4 */
                g_offOrOnTureStateFlag = OLEDSHOWSTATE_REPAIRMODEL4;
                xReturn = xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                vTaskDelay(500);
                FMQ_OFF;
                vTaskDelay(100);
                while(1)
                {
                    vTaskDelay(100);
                }               
            }  
        }
        else 
        {
            ;
        }    
		vTaskDelay(10);
	}
	
	if(0==g_pwmOrTtlModelFlag)
	{
		cocoguojia_tim5CatchInit();
	}
	else
	{
		cocoguojia_ttlTestInit();
	}	
}



/* OLED显示系统初始化 */
static void ShowSystemInit(void)
{

	/* 显示 系统初始化 */
	g_offOrOnTureStateFlag = OLEDSHOWSTATE_INIT;
    xTaskNotify(OLED_TASK_Handle,g_offOrOnTureStateFlag,eSetValueWithOverwrite);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */
    FMQ_ON;
    vTaskDelay(500);
    FMQ_OFF;
    vTaskDelay(100);    
								 
}

