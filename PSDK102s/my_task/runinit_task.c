#include "runinit_task.h"
#include "psdk_debug.h" 
#include "cocoguojia_sdcard.h"

static void PowerUpAndShowLogo(void);                   /* 系统刚上和电显示LOGO */
static void ReadEepromAndShowResultAndSetDoWhit(void);  /* 读EEPROM并显示上次设置结果并且处理设置参数 */
static void ShowSystemInit(void);                       /* OLED显示系统初始化 */


 /*say: RUN_TASK 此任务负责具体的逻辑 配合一些关键的状态标志
 控制系统的走向 在系统状态里面 才能改变系统状态标志 */


/*************************************************
Function: void  RUNINIT_TASK (void const * argument)
Description: 控制开机走向
Return: 无返回值
*************************************************/
u8  system_folder[10] = "SYSTEM";  /* 存储系统文件的文件夹名字 */
u8  system_txt[10] = "config.txt"; /* 存储版本号的文件 */
u8  sys_save_finish = 0;           /* 系统版本号存储完成标志位 */

void  RUNINIT_TASK (void const * argument)
{ 
    PowerUpAndShowLogo();                   /* 系统刚上和电显示LOGO */   
    osDelay(20); 
	   
    ReadEepromAndShowResultAndSetDoWhit();  /* 读EEPROM并显示上次设置结果并且处理设置参数 */
    ShowSystemInit();                       /* OLED显示 系统初始化 */
	
    osDelay(100);
	  camTaskCanTakeOverFlag = 1;	
    osDelay(500);
		
		while(1)
		{
	//		PSDK_LOG_DEBUG("RUNINIT_TASK running  \r\n" );	
			if(0 == readingFlag)
			{		
				 /* *******在内存卡中存储系统版本号********* */
				 write_system_version_number((const TCHAR *)system_folder,(const TCHAR *)system_txt);
				 sys_save_finish = 3;
				 osThreadTerminate(RUNINIT_TASK_Handle); /* 删除RUNINIT_TASK_Handle任务 */ 
			}
				
			osDelay(100);
		}
   
}
 



/* 系统刚上和电显示LOGO */
static void PowerUpAndShowLogo(void)
{
    BaseType_t xReturn = pdPASS;
	  uint16_t n = 0;  
    xReturn = xReturn;	

	
	  offOrOnTureStateFlag=OLEDSHOWSTATE_LOGO;			    /* 显示LOGO */                             
    osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */                                							 			 							 
	  n=10;
    while(n--)/* 蜂鸣器间歇鸣叫2s 红灯闪烁 */
		{		
			FMQ_ON;
			RGBLEDR_ON;
		  KEY_LED_ON;
					osDelay(100);
			FMQ_OFF;
			RGBLED_ALLOFF;
			KEY_LED_OFF;
			    osDelay(100);
		}

	FMQ_ON;/* 蜂鸣器长鸣半秒 表示系统上电成功 */
	osDelay(500);
	FMQ_OFF;
	osDelay(100);
	RGBLEDG_ON;/* 绿灯常亮 */
}



/* 读EEPROM并显示上次设置结果并且处理设置参数 */
static void ReadEepromAndShowResultAndSetDoWhit(void)
{
    BaseType_t xReturn = pdPASS;
    EventBits_t eventTemp = 0;
    uint8_t theLastsignalOrAutoModelFlag = 0;
    uint8_t i = 0;
    xReturn = xReturn;

	/* 读eeprom */
	 cocoguojia_readSetParameter();


   /* 进入到设置模式 page3 显示当前设置结果 */
	  offOrOnTureStateFlag = OLEDSHOWSTATE_SETMODEL4;
    osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */
   	FMQ_ON;
    osDelay(500);
    FMQ_OFF;				 
    osDelay(200);
    osDelay(1000);
    osDelay(1000);
   
    /* 按键检测 */
    eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);/* 事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件 */

    if(KEY_SHORT == (eventTemp&KEY_SHORT))/* 按键短按了 */
    {
        currentRowNum = signalOrAutoModelFlag;
        theLastsignalOrAutoModelFlag = signalOrAutoModelFlag;	
		    setModelFlag = 1;       /* 进入到设置界面 page2 */
    }
    
		if(1 == setModelFlag)       /* 如果进入到设置界面 */
		{
				/* 显示设置界面1 */
				offOrOnTureStateFlag = OLEDSHOWSTATE_SETMODEL1;
				osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);/* 任务的句柄，发送的数据 32位，覆盖当前通知	*/					 
				FMQ_ON;
				osDelay(500);
				FMQ_OFF;
				osDelay(100);
		}
	while(1 == setModelFlag)
	{
  /* 设置检测 在显示系统初始化之前按一下按钮即可 */		
        eventTemp = xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);/* 事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件 */

        if(KEY_SHORT == (eventTemp&KEY_SHORT))/* 按键短按了 */
        {
            if(0 == currentRowNum)
            {       
                currentRowNum = 2;/* signal模式 */
                signalOrAutoModelFlag = currentRowNum;
            }
            else if(2 == currentRowNum)
            {
                currentRowNum = 4;/* auto 2s */
                signalOrAutoModelFlag = currentRowNum;
            }		
            else if(4 == currentRowNum)
            {
                currentRowNum = 6;/* auto 3s */
                signalOrAutoModelFlag = currentRowNum;
            }
            else
            {
                signalOrAutoModelFlag = theLastsignalOrAutoModelFlag;/* 保存一下之前的设置 */
                currentRowNum = 0;
            }             
        }
        else if(KEY_LONG == (eventTemp&KEY_LONG))/* 按键长按了 */
        {	
            if(0 != currentRowNum)
            {

                /* 进入到设置模式 page3 */
                offOrOnTureStateFlag=OLEDSHOWSTATE_SETMODEL3;
                osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                /* 写eeprom  */
                cocoguojia_writeSetParameter();												 
                osDelay(200);
                osDelay(3000);
                setModelFlag=0;
            }
            else
            {
                /* 进入到修复模式 page1 */
                offOrOnTureStateFlag=OLEDSHOWSTATE_REPAIRMODEL1;
                osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                POWER_EN;           /* 开机@@@@@@@@@@@@@@@@@ */
                osDelay(1000);
                for(i=0;i<20;i++)
                {
                   POWER_EN;        /* 开机@@@@@@@@@@@@@@@@@ */
                   osDelay(1000);
                   if((1==camPowerFlagTable[1])&&(1==camPowerFlagTable[2])&&(1==camPowerFlagTable[3])&&(1==camPowerFlagTable[4])&&(1==camPowerFlagTable[5]))
                   {		   
                       i=200;
                   }
                }
								osDelay(3000);
								osDelay(3000);
								PHOTOALL_EN;           /* 拍照@@@@@@@@@@@@@@@@@  */
								osDelay(100);
								PHOTOALL_NO;
				        osDelay(3000);

                /* 进入到修复模式 page2 */
                offOrOnTureStateFlag=OLEDSHOWSTATE_REPAIRMODEL2;
                osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                i=0;
                while(0==i)
                {
                     eventTemp=xEventGroupWaitBits(Key_EventGroup,KEY_SHORT|KEY_LONG,pdTRUE,pdFALSE,0);//事件对象句柄，接受任务感兴趣的事件，退出时清除事件位，满足感兴趣的任意事件,不等待         
                     if(KEY_SHORT==(eventTemp&KEY_SHORT))/* 按键短按了  */
                     {
                        OK_EN;       /* KEY 按键@@@@@@@ */
							          osDelay(100);
						           	OK_NO;
                        FMQ_ON;
                        osDelay(100);
                        FMQ_OFF;
                        osDelay(100);
                     } 
                     else if(KEY_LONG==(eventTemp&KEY_LONG))/* 长按了 */
                     {
                           i=1;
                     }
                     else
                     {
                       osDelay(10);
                     }                         
                }                 
                

                /* 进入到修复模式 page3  */
                offOrOnTureStateFlag=OLEDSHOWSTATE_REPAIRMODEL3;
                osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);/* 任务的句柄，发送的数据 32位，覆盖当前通知  */
                FMQ_ON;
                osDelay(500);
                FMQ_OFF;
                osDelay(100);
                POWER_NO;     /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                osDelay(1000);
                for(i=0;i<20;i++)
                {
                   POWER_NO;  /* 关机@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
                   osDelay(1000);
                   if((0==camPowerFlagTable[1])&&(0==camPowerFlagTable[2])&&(0==camPowerFlagTable[3])&&(0==camPowerFlagTable[4])&&(0==camPowerFlagTable[5]))
                   {
                        i=200;
                   }
                }
                osDelay(2000);
								

                /* 进入到修复模式 page4 */
                offOrOnTureStateFlag=OLEDSHOWSTATE_REPAIRMODEL4;
                osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */
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


/* OLED显示系统初始化 */
static void ShowSystemInit(void)
{
	  /* 显示 系统初始化 */
	  offOrOnTureStateFlag=OLEDSHOWSTATE_INIT;
    osMessagePut(oledShowMessageQHandle,offOrOnTureStateFlag,0);/* 任务的句柄，发送的数据 32位，覆盖当前通知 */
    FMQ_ON;
    osDelay(500);
    FMQ_OFF;
    osDelay(100);    
								 
}

