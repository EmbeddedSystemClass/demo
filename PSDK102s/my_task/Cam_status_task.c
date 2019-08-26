#include "Cam_status_task.h"
#include "psdk_debug.h" 
/* say:
Cam_status_task 此任务负责具体的逻辑 配合一些关键的状态标志 控制系统的走向
在系统状态里面 才能改变系统状态标志*/

uint8_t 	   camAllStateFlag = 0;						        /* 2=相机有开的有关的 0=相机全体都关机 1=相机全体都开机 */
uint8_t 	   oledStateFlag = 0;							        /* 0表示不运行 */
uint16_t  	 tim5CatchNum = 0;							        /* 捕获并且被触发的有效总次数 */
uint8_t 	   capture_FinishAndStartPhotoFlag = 0;		/* 捕捉结束并且要进行拍照流程标志 1=开始拍照流程 */
uint8_t  	   camPowerFlagTable[6] = {0,0,0,0,0,0};  /* 5个相机的开关机标志 1表示开机 0表示关机*/
uint16_t  	 photoPicNum[6] = {0,0,0,0,0,0};        /* 5个相机分别的照片数 */
uint8_t  	   camHotShotTable[6] = {0,0,0,0,0,0};    /* 5个相机的热靴有效标志 */
uint8_t  	   readingFlag = 0;							          /* 1=正在读中  0=正常工作状态 */
uint8_t      setModelFlag = 0;							        /* 1=进入到设置模式 */
uint8_t      signalOrAutoModelFlag = 2;						  /* 2=signal模式   4=auto 2s  6=auto 3s */
uint8_t      currentRowNum = 2;						          /* 当前行 Num 当前行 OLED的当前行 范围=2 4 6  2=signal模式   4=auto 2s  6=auto 3s */
uint8_t 	   camTaskCanTakeOverFlag = 0;					  /* cam任务可以接管标志 1=cam任务可以接管 */
uint8_t 	   offOrOnTureStateFlag = SYSTEMSTATE_GOTOSHUT;
uint8_t  	   camLoseFlagTable[6] = {0,0,0,0,0,0};        /* 5个相机的存在标志 即不丢失 上电后会检查一次 */
uint8_t      theTurelostNumTable[6] = {0,0,0,0,0,0};     /* 实际丢片数 */
uint8_t      ppsRtkHealthNum = 0;                        /* ppsRtk定位质量数值 */

/*************************************************
Function: void  Cam_status_update_TASK (void const * argument)
Description: 检测相机开关机状态
Return: 无返回值
*************************************************/
void  Cam_status_update_TASK (void const * argument)
{ 
	while (1) 
	{
			//PSDK_LOG_DEBUG("Cam_status_update_TASK running  \r\n" );	
			osDelay(100);
     /* 判断相机的开关机状态 */
      if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			 camPowerFlagTable[1] = 1;
			}
			else
			{
			 camPowerFlagTable[1] = 0;
			}
			
		  if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			 camPowerFlagTable[2] = 1;
			}
			else
			{
			 camPowerFlagTable[2] = 0;
			}
			
			if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			 camPowerFlagTable[3] = 1;
			}
			else
			{
			 camPowerFlagTable[3] = 0;
			}
			
			if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			 camPowerFlagTable[4] = 1;
			}
			else
			{
			 camPowerFlagTable[4] = 0;
			}
			
			if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			 camPowerFlagTable[5] = 1;
			}
			else
			{
			 camPowerFlagTable[5] = 0;
			}		 
  }
}
