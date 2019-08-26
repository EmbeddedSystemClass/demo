/**
 ******************************************************************************
 * @file    application.c
 * @version V1.3.0
 * @date    2018/09/19
 * @brief   An example program of PSDK for demo board.
 *
 * @copyright (c) 2017-2018 DJI. All rights reserved.
 *
 * All information contained herein is, and remains, the property of DJI.
 * The intellectual and technical concepts contained herein are proprietary
 * to DJI and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of DJI.
 *
 * If you receive this source code without DJI’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify DJI of its removal. DJI reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 ******************************************************************************
 */
#include "myinclude.h"
/* Includes ------------------------------------------------------------------*/
#include <app_config.h>
#include <psdk.h>
#include "led_button_switch.h"
#include <string.h>
#include "sys.h"
#include "log_printf.h"
#include <stdio.h>
#include <psdk_upper.h>
#include "flash.h"
#include "pps.h"
#include "app_utils.h"
#include <os_util.h>
#include "application.h"
#include "app_infor.h"
#include "test_app_func.h"
#include "test_msg_push.h"
#include "test_payload_cam_emu.h"
#include "test_payload_gimbal_emu.h"
#include "test_positioning.h"
#include "test_osdk_func.h"
#include "hotshoe.h"
#include "math.h"
#include "stdlib.h"
#include "test_msg_push.h"


#if SKYPORT_FW_FIX_VER_NEED
static T_PsdkUserFixSkyport s_fixSkyport = {0};
#endif

#if PSDK_APP_ALIAS_NEED
static T_PsdkUserCustomInfo s_productAlias = {0};
#endif
static T_PsdkUpper s_psdkUpperHandle;               /*PSDK协议处理结构*/
static uint8_t s_uartRecBuf[PSDK_UART_REC_BUF_LEN] = {0}; /*用于从UART接口接收数据的缓冲区*/
static T_PsdkUserInfo s_userInfo = {0};                   /*PSDK用户信息*/


uint32_t g_can_uart_sw_settings = 0;
extern uint8_t timesigner;
extern double longitude;
extern double latitude;
extern double height;
extern uint32_t ppsNewTimeMs;

/*函数声明*/
void UTC_to_BJtime(T_PsdkPositioningGetPosReq *utc_time, int8_t	timezone);

/*************************************************
Function: static void FillInUserInfo(T_PsdkUserInfo *userInfo)；
Description: 检查用户信息字符串并将用户信息填写到结构中
Formal parameters：T_PsdkUserInfo userinfo指向要写入的用户信息结构的指针
Return: 无返回值
*************************************************/
static void FillInUserInfo(T_PsdkUserInfo *userInfo)
{
    memset(userInfo->APP_Name, 0, sizeof(userInfo->APP_Name));
    memset(userInfo->APP_Id, 0, sizeof(userInfo->APP_Id));
    memset(userInfo->APP_Key, 0, sizeof(userInfo->APP_Key));
    memset(userInfo->DeveloperAccount, 0, sizeof(userInfo->DeveloperAccount));

    if (strlen(USER_APP_NAME) <= sizeof(userInfo->APP_Name) &&
        strlen(USER_APP_ID) <= sizeof(userInfo->APP_Id) &&
        strlen(USER_APP_KEY) <= sizeof(userInfo->APP_Key) &&
        strlen(USER_DEVELOPER_ACCOUNT) <= sizeof(userInfo->DeveloperAccount)) {
        strcpy(userInfo->APP_Name, USER_APP_NAME);
        strcpy(userInfo->APP_Id, USER_APP_ID);
        strcpy(userInfo->APP_Key, USER_APP_KEY);
        strcpy(userInfo->DeveloperAccount, USER_DEVELOPER_ACCOUNT);
    } else {
        PSDK_LOG_ERROR("Length of user information string is beyond limit. Please check.\r\n");
    }

    if (!strcmp(userInfo->APP_Name, "your_app_name") ||
        !strcmp(userInfo->APP_Id, "your_app_id") ||
        !strcmp(userInfo->APP_Key, "your_app_key") ||
        !strcmp(userInfo->DeveloperAccount, "your_developer_account")) {
        PSDK_LOG_ERROR("Please fill in correct user information to app_infor.h file.\r\n");
    }
}


/**
 * @brief Low level PSDK frame send function.
 * @param pSendData Pointer to frame to be sent.
 * @param dataLen Frame length.
 * @return PSDK function process state.
 */
E_PsdkStat Psdk_Write(const uint8_t *pSendData, uint16_t dataLen)
{
    UART_Write(PSDK_UART_NUM, pSendData, dataLen);
    return PSDK_STAT_OK;
}

/**
 * @brief Low level PSDK frame read function.
 * @param pReadData Pointer to frame to be read.
 * @param dataLen Frame length.
 * @retval -1: Read error.
 *          not -1: Counts of bytes read actually.
 */
int Psdk_Read(uint8_t *pReadData, uint16_t dataLen)
{
    int res;

    res = UART_Read(PSDK_UART_NUM, pReadData, dataLen);

    return res;
}

/**
 * @brief PSDK test task function.
 * @details  将传感器MS5607的数据通过透明传输和浮动窗口样式发送到移动应用程序
 * @param parameter
 * @return None.
 */
uint8_t bukepaizhaoTable[] = {0xE4,0xB8,0x8D,0xE5,0x8F,0xAF,0xE6,0x8B,0x8D,0xE7,0x85,0xA7,0xE7,0x8A,0xB6,0xE6,0x80,0x81,0};  /*UTF8编码*/
uint8_t xinhaoTable[] = {0,0,0,0,0,0,0,0,0,0};     /*用于存储当前的工作模式*/
uint16_t g_ds18b20ToShow = 0;                      /*ds18b20 温度值 最高位位1则是负数*/
static char s_printBuffer[512] = {0};                  /*存储用于发送给屏幕窗口信息的字符串*/
void PsdkTestTask(void const *parameter)
{
	
	  uint8_t *bukepaizhaoSp = 0;
    uint8_t fuhaoTable[] = {0,0,0};
    uint16_t realSendLen = 0;
    uint16_t ds18b20ToShow = 0;

    while (1) 
    {
        if(2 == g_signalOrAutoModelFlag)         /*2=signal模式*/ 
        {
            xinhaoTable[0] = 'S';
            xinhaoTable[1] = 'I';
            xinhaoTable[2] = 'G';
            xinhaoTable[3] = 'N';
            xinhaoTable[4] = 'A';
            xinhaoTable[5] = 'L';
            xinhaoTable[6] = 0;
            xinhaoTable[7] = 0;
        }
        else if(4 == g_signalOrAutoModelFlag)    /*2s自动拍模式*/
        {
            xinhaoTable[0] = 'a';
            xinhaoTable[1] = 'u';
            xinhaoTable[2] = 't';
            xinhaoTable[3] = '0';
            xinhaoTable[4] = ' ';
            xinhaoTable[5] = '2';
            xinhaoTable[6] = 's';
            xinhaoTable[7] = 0;
        }
        else                                   /*3s自动拍模式*/
        {
            xinhaoTable[0] = 'a';
            xinhaoTable[1] = 'u';
            xinhaoTable[2] = 't';
            xinhaoTable[3] = '0';
            xinhaoTable[4] = ' ';
            xinhaoTable[5] = '3';
            xinhaoTable[6] = 's';
            xinhaoTable[7] = 0;
        }
             
			 if(2 == g_canPhotoStateFlag)                /*表示可以拍照*/  
			 {
					 bukepaizhaoSp = (bukepaizhaoTable+3);  
			 }
			 else
			 {
					 bukepaizhaoSp = bukepaizhaoTable;
			 }  
        
         
			 if(0x8000 <= g_ds18b20ToShow)
			 {
					fuhaoTable[0] = '-';
					ds18b20ToShow = g_ds18b20ToShow - 0x80000;
			 }
			 else
			 {
					fuhaoTable[0] = '+';
					ds18b20ToShow = g_ds18b20ToShow;
			 }

			sprintf(((char*)s_printBuffer),"%s %s%d℃ \r\n %s & RTK Health=%d\r\n P=%d\r\n PIC1=%d\r\n PIC2=%d\r\n PIC3=%d\r\n PIC4=%d\r\n PIC5=%d\r\n",
			&bukepaizhaoSp[0],&fuhaoTable[0],ds18b20ToShow,&xinhaoTable[0],g_ppsRtkHealthNum,
			g_tim5CatchNum,g_photoPicNum[1],g_photoPicNum[2],g_photoPicNum[3],g_photoPicNum[4],g_photoPicNum[5]);
			PsdkAppFunc_PushMsgToFloatingWindow(&s_psdkUpperHandle, s_printBuffer);         /* 将此信息推送到大疆飞行员浮动窗口*/
			PsdkAppFunc_TransferToApp(&s_psdkUpperHandle, (const uint8_t *) s_printBuffer   /*将此信息传送到MSD*/
																,(uint16_t)(strlen(s_printBuffer) + 1),&realSendLen);
			PsdkOsdkFunc_TransferToOsdk(&s_psdkUpperHandle, (const uint8_t *) s_printBuffer /*将此信息传送到OSDK*/
																	,(uint16_t)(strlen(s_printBuffer) + 1), &realSendLen);           														
			osDelay(1000);
    }
}

/**
 * @brief PSDK data receive task function.
 * @note All callback function process in this task.  所有回调函数都处理此任务。
 * @param parameter
 * @return None.
 */

void ProcessPsdkRecTask(void const *parameter)
{
	
    int res = 0;
    UNUSED(parameter);
    while (1) {
        res = Psdk_Read(s_uartRecBuf, sizeof(s_uartRecBuf));

        if (res > 0) {
            //===================== PSDK Recievie Function ==============================//
            //all callbacks process in this thread.
            PsdkUpper_ProcessReceiveData(&s_psdkUpperHandle, s_uartRecBuf, res);
        }

        osDelay(1);
    }
}

/**
 * @brief Initialize the load application.
 * @param None.
 * @return None.
 */
void application_init(void)
{
    LOG_Init();                        /*调试串口输出初始化 用的是串口1(波特率115200)*/
    UART_Init(PSDK_UART_NUM,256000);   /*大疆串口初始化 用的是串口3*/
    UART_Init(UART_NUM_2,9600);        /*RS485 用的是串口2*/
    PPS_Init();                        /*pps用于外部中端15*/
    HotShoe_Init();                    /*热靴用于外部中断1*/ 
    //===================== PSDK init ==============================//
    // 回调函数 注册在这里 在初始化里
    //base init
    FillInUserInfo(&s_userInfo);                    /*PSDK用户信息写入*/
    PsdkUpper_Init(&s_psdkUpperHandle, &s_userInfo);/*0完成加载应用信息初始化、计算PSDK数据传输密钥、PSDK协议初始化和挂载状态命令集注册*/
    PsdkUpper_RegSendFunction(&s_psdkUpperHandle, Psdk_Write);  /*SDK顶级结构体的发送回调函数为Psdk_Write*/
    PsdkAppFunc_Init(&s_psdkUpperHandle);                       /*1应用程序命令集 注册到 顶级结构体*/
    PsdkAppFunc_SetReceiveFromAppCallback(&s_psdkUpperHandle, Test_ReceiveFromMobileSDKFunc);
    PsdkAppFunc_SetWidgetList(&s_psdkUpperHandle, g_TestWidgetList, g_TestWidgetCount);
    PsdkAppFunc_RegGetWidgetValueCallback(&s_psdkUpperHandle, Test_GetWidgetValueFunc);
    PsdkAppFunc_RegSetWidgetValueCallback(&s_psdkUpperHandle, Test_SetWidgetValueFunc);

    //Msg subscribe Function init, callback functions in Application/psdk_test/test_msg_push.c
    PsdkMsgSub_Init(&s_psdkUpperHandle);                        /*2数据推送命令集 注册到 顶级结构体*/
    PsdkMsgSub_RegMsgCallbackList(&s_psdkUpperHandle, &g_PushDataMsgCallbackList);

    //Camera function Emulate Test, callback functions in Application/psdk_test/test_payload_cam_emu.c
    //If want transfer video to Mobile SDK or DJI Pilot, this function must call to init Payload camera.
    PsdkCamera_Init(&s_psdkUpperHandle, &g_testCameraHandler);  /*3相机命令集 注册到 顶级结构体*/

    //Gimbal function Emulate Test, callback functions in Application/psdk_test/test_payload_gimbal_emu.c
    PsdkGimbal_Init(&s_psdkUpperHandle, &g_testGimbalHandler);  /*4万向命令集 注册到 顶级结构体*/
    //===================== PSDK init End ============================//
    
     //OSDK function Test, callback functions and configure parameter in Application/psdk_test/test_osdk_func.c
    PsdkOsdkFunc_Init(&s_psdkUpperHandle, &g_testOsdkFuncHandler);
    //===================== PSDK init End ============================//

    //set fixed SKYPORT FW version
#if SKYPORT_FW_FIX_VER_NEED
    s_fixSkyport.verMajor  = SKYPORT_FW_FIX_VER_MAJOR;
    s_fixSkyport.verMinor  = SKYPORT_FW_FIX_VER_MINOR;
    s_fixSkyport.verModify = SKYPORT_FW_FIX_VER_MODIFY;
    s_fixSkyport.verDebug  = SKYPORT_FW_FIX_VER_DEBUG;

    PsdkUpper_SetFixVersion(&s_fixSkyport);
#endif

    //set PSDK product alias
#if PSDK_APP_ALIAS_NEED
    strcpy(s_productAlias.ProductAlias, PSDK_APP_ALIAS);
    PsdkUpper_SetProductAlias(&s_productAlias);
#endif
}

uint32_t g_tickHookNumTemp=0;

//-------------------------------------------------------------------------



uint32_t triggertime = 0;                     /*接收当前系统时间值*/
T_PsdkPositioningGetPosReq bj_timer = {0};    /*存储北京时间结构体*/
u8 g_gps003Table[200] = {0};                  /*存储上次的pos点*/
 psdk_f64_t Relative_hight = 0;
void  PPS_TASK (void const * argument)
{
	  uint8_t j = 0,i = 0,writebit = 0,exitbit = 1,flag1 = 1,flag2 = 0;
	  uint8_t *g_gps001TableTemp = NULL;       /*指向要存储的数据*/
		while (1) 
		{
				if((ppssignal == 1) &&(flag1 == 1))
			 {
			   triggertime = OS_GET_TIME_MS ;
				 Test_RequestPosition(&s_psdkUpperHandle,triggertime);	
				 if(writesdpos.posDetailInfo[1].targetPointPos.height != 0)
				 {
				  Relative_hight = writesdpos.posDetailInfo[1].targetPointPos.height;
					 flag2 = 0;
				 }
				 else
				 {
				  flag2 = 1;
				 }				 
				 osDelay(500);
			 }		
			
			  osDelay(2);
				exitbit= 1;
	    	if(HotShoe_isTriggered() == true)     /*检查REX信号是否触发一次*/
				{
					if(flag2 == 1)
					{
					   flag1 = 1;
					}
					else
					{
					   flag1 = 0;
					}					
					 triggertime = OS_GET_TIME_MS;   
					 timesigner=1;                      /*用来获取txt时间*/
					 while(exitbit)
					 {
						  osDelay(2);
							if(triggertime < ppsNewTimeMs)  /*判断两个时间*/
							{
									if((ppsNewTimeMs-triggertime)<2000)
									{
										Test_RequestPosition(&s_psdkUpperHandle,triggertime);	/*时间同步*/
										UTC_to_BJtime(&utc_time,8);               

													if(0==g_nameFlag)   /*0=还没确认过文本名字的日期   1=确认过文本名字的日期 2=创建过表头*/
													{
														for(i=0;i<20;i++)
														 {			
																g_cocoguojia_sdcard_txtname20[i]=0;
														 }		
														 sprintf((&g_cocoguojia_sdcard_txtname20[0]),"%4d.%02d.%02d",									
																										bj_timer.baseTimestamp.year,
																										bj_timer.baseTimestamp.month ,
																										bj_timer.baseTimestamp.day);
														 g_nameFlag=1;                     
													} 
										
										writebit=1;
									}
									else
									{
										 exitbit=0;
									}
							}		
							
							if(writebit==1)
							{
								 writebit=0;
								 if(2==g_gpsTableFlag)
								 {
										for(j=0;j<200;j++)
										{
												g_gps001Table[j]=0;
										}
										g_gps001TableTemp=&g_gps001Table[0];                 
								 }             
								 else 
								 {
										for(j=0;j<200;j++)
										{
												g_gps002Table[j]=0;
										}
										g_gps001TableTemp=&g_gps002Table[0];
								 }
							   if(g_ppsRtkHealthNum !=0 )
								 {
										 sprintf(((char*)g_gps001TableTemp),"%.8lf,%.8lf,%lf,%2d,%2d,%2d,%lf,%d,%4d.%02d.%02d,%2d.%02d.%02d.%d",
																												writesdpos.posDetailInfo[1].targetPointPos.longitude,
																												writesdpos.posDetailInfo[1].targetPointPos.latitude,
																												writesdpos.posDetailInfo[1].targetPointPos.height,
																												writesdpos.posDetailInfo[1].uavAtti.pitchAttiAngle,
																												writesdpos.posDetailInfo[1].uavAtti.rollAttiAngle,
																												writesdpos.posDetailInfo[1].uavAtti.yawAttiAngle,
										 												            writesdpos.posDetailInfo[1].targetPointPos.height - Relative_hight,		
																													g_ppsRtkHealthNum	,									 
																												bj_timer.baseTimestamp.year,
																												bj_timer.baseTimestamp.month ,
																												bj_timer.baseTimestamp.day,
																												bj_timer.baseTimestamp.hour,
																												bj_timer.baseTimestamp.minute,
																												bj_timer.baseTimestamp.second,   
																												bj_timer.eventDetailInfo[0].timeOffsetUs);
									   
											if(2==g_gpsTableFlag)
											{
												g_gpsTableFlag=1;
											}             
											else 
											{                 
												g_gpsTableFlag=2;
											}
											 strcpy((char *)g_gps003Table,(char *)g_gps001TableTemp);     /*记录的点*/
								 }
								 else
								 {
											strcpy((char *)g_gps001TableTemp,(char *)g_gps003Table); /*上次记录的点*/                  
											if(2==g_gpsTableFlag){
													g_gpsTableFlag=1;
											}             
											else {                 
													g_gpsTableFlag=2;
											}
								 }										 
											cocoguojia_sdWrireGpsRtk();       /*写TF卡 写TF卡信息  */
											exitbit=0;
									}
						 }
				}  
		}    
}		

 

 void vApplicationTickHook(void)
 {
	 phototime++;
 }
 void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
	PSDK_LOG_DEBUG("任务：%s 发现栈溢出\r\n", pcTaskName);
}
 
/*************************************************
Function: void UTC_to_BJtime(T_PsdkPositioningGetPosReq *utc_time, int8_t	timezone)
Description: 用于UTC时间转换成北京时间
参数1：utc_time UTC时间结构体
参数2: timezone 时间差
Return: 无返回值
*************************************************/
 void UTC_to_BJtime(T_PsdkPositioningGetPosReq *utc_time, int8_t	timezone)
{
	  int year,month,day,hour;
    int lastday = 0;					/*last day of this month*/
    int lastlastday = 0;			/*last day of last month*/

    year	 = utc_time->baseTimestamp.year;			
    month  = utc_time->baseTimestamp.month;
    day 	 = utc_time->baseTimestamp.day;
    hour 	 = utc_time->baseTimestamp.hour + timezone; 
	
    if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12)
		{
				lastday = 31;
				if(month == 3)
				{
					if((year%400 == 0)||(year%4 == 0 && year%100 != 0))				/*if this is lunar year*/
							lastlastday = 29;
					else
							lastlastday = 28;
				}
				if(month == 8)
						lastlastday = 31;
    }
    else if(month == 4 || month == 6 || month == 9 || month == 11)
		{
				lastday = 30;
				lastlastday = 31;
    }
    else
		{
				lastlastday = 31;
				if((year%400 == 0)||(year%4 == 0 && year%100 != 0))
						lastday = 29;
				else
						lastday = 28;
		}

    if(hour >= 24)                /*if >24, day+1*/
		{					   
				hour -= 24;
				day += 1; 

				if(day > lastday)    /*next month,  day-lastday of this month*/
				{ 		
						day -= lastday;
						month += 1;

						if(month > 12)
						{		/*next year , month-12*/
								month -= 12;
								year += 1;
						}
				}
    }
		
    if(hour < 0)                  /*if <0, day-1*/
		{										
				hour += 24;
				day -= 1; 
				if(day < 1)          /*month-1, day=last day of last month*/
					{					  
							day = lastlastday;
							month -= 1;
							if(month < 1)
							{ 			/* last year , month=12*/
									month = 12;
									year -= 1;
							}
					}
    }
				 
    /*transfer value to NMEA_result.local_time*/
		bj_timer.baseTimestamp.year  = year;
		bj_timer.baseTimestamp.month = month;
		bj_timer.baseTimestamp.day   = day;
		bj_timer.baseTimestamp.hour  = hour;
		bj_timer.baseTimestamp.minute = utc_time->baseTimestamp.minute;
	  bj_timer.baseTimestamp.second = 	utc_time->baseTimestamp.second ;    
		bj_timer.eventDetailInfo[0].timeOffsetUs = utc_time->eventDetailInfo[0].timeOffsetUs; 
}





