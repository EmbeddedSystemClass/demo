#ifndef __U2RECE_TASK_H__
#define __U2RECE_TASK_H__

#include "myinclude.h"

extern void Cam_status_update_TASK  (void const * argument);			


//------------------------------------------------
//OLED显示状态预定义
#define OLEDSHOWSTATE_RUN   6			
#define OLEDSHOWSTATE_INIT 	7		
#define OLEDSHOWSTATE_LOGO	8		

#define OLEDSHOWSTATE_SETMODEL1 9
//#define OLEDSHOWSTATE_SETMODEL2	10	
#define OLEDSHOWSTATE_SETMODEL3	11	
#define OLEDSHOWSTATE_SETMODEL4	12	

#define OLEDSHOWSTATE_REPAIRMODEL1 13
#define OLEDSHOWSTATE_REPAIRMODEL2 14
#define OLEDSHOWSTATE_REPAIRMODEL3 15
#define OLEDSHOWSTATE_REPAIRMODEL4 16

//------------------------------------------------
//系统状态预定义
//0=真实关机  1=真实开机  2=关机进行中还不确定是否真实关机 3=开机进行中还不确定是否真实开机 4=关机失败  5=开机失败
#define SYSTEMSTATE_OFFTRUE 		   0
#define SYSTEMSTATE_ONTRUE 				 1
#define SYSTEMSTATE_GOTOSHUT 			 2
#define SYSTEMSTATE_GOTOOPEN			 3
#define SYSTEMSTATE_SHUTFAIL 			 4
#define SYSTEMSTATE_OPENFAIL 			 5
#define SYSTEMSTATE_CAMLOSE 			 6


#define TEMPEREATURE_MAX 30

#define TTLEDGELONG 			         5
#define     MODELFLAG_PWM    0
#define     MODELFLAG_TTL_H  1
#define     MODELFLAG_TTL_L  2


//------------------------------------------------
//USART2 发送命令 预定于
#define USART2COM_OPEN 		     1
#define USART2COM_SHUT 		     2
#define USART2COM_PHOTO 		 3
#define USART2COM_ASK 		     4
#define USART2COM_KEY 		     5




extern uint8_t 	 camTaskCanTakeOverFlag;				//1=cam任务可以接管
extern uint8_t 	 camAllStateFlag;						    //2=相机状态未知 0=相机全体都关机 1=相机全体都开机
extern uint8_t   oledStateFlag;							    //0表示不运行
extern uint8_t   offOrOnTureStateFlag;					//0=off关机  1=on开机  2=关机进行中还不确定是否真实关机 3=开机进行中还不确定是否真实开机 状态标志
extern uint16_t  tim5CatchNum;							    //捕获并且被触发的有效总次数
extern uint16_t  photoPicNum[6];         				//5个相机分别的照片数
extern uint8_t   camHotShotTable[6];         		//5个相机的热靴有效标志
extern uint8_t   camPowerFlagTable[6];        	//5个相机的开关机标志
extern uint8_t 	 capture_FinishAndStartPhotoFlag;			//捕捉结束并且要进行拍照流程标志 1=开始拍照流程
extern uint8_t   readingFlag;								    //1=正在读中  0=正常工作状态
extern uint8_t   setModelFlag;							    //1=进入到设置模式
extern uint8_t   currentRowNum;							    //当前行 Num
extern uint8_t   signalOrAutoModelFlag;				  //2=signal模式   4=auto 2s  6=auto 3s
extern uint8_t   camLoseFlagTable[6];           //5个相机的丢失标志 即1=丢失 上电后会检查一次
extern uint16_t  g_ds18b20Temp;                   //ds18b20 温度暂存
extern uint8_t   ppsRtkHealthNum;               //ppsRtk定位质量数值


//--------------------------------
//按键1短按 长按 事件位
#define KEY_SHORT (0x00000001<<0)
#define KEY_LONG  (0x00000001<<1)

#endif

