#include "Cam_status_task.h"
 
 //cocoguojia say:
 //RUN_TASK 此任务负责具体的逻辑 配合一些关键的状态标志 控制系统的走向
 //在系统状态里面 才能改变系统状态标志

 //---------------------------------------------------------------------------
 //任务级全局变量
TaskHandle_t 	Cam_status_update_TASK_Handle	= NULL;		//创建任务句柄
 


uint8_t 	  g_triggerFlag=0;							  //1=PWM触发发生了 为1时要显示第一页(当在第2页的时候要切换成第一页 当在第一页的时候 给第一页显示重新算时间)  由OLED任务清零
uint32_t 	  g_oledStateFlag=0;							  //0表示不运行
uint16_t    g_tim5PlauseUs=0;							  //检测到的PWM脉宽宽度
uint16_t    g_tim5TriggerPlauseUs=0;				      //触发时 检测到的PWM脉宽宽度
uint16_t    g_tim5CatchNum=0;							  //捕获并且被触发的有效总次数
uint8_t 	  g_capture_FinishAndStartPhotoFlag=0;		  //捕捉结束并且要进行拍照流程标志 1=开始拍照流程
uint8_t  	  g_camPowerFlagTable[6]={0,0,0,0,0,0};         //5个相机的开关机标志
uint16_t    g_photoPicNum[6]={0,0,0,0,0,0};         	  //5个相机分别的照片数
uint8_t    	g_readingFlag=0;							  //1=正在读中  0=正常工作状态
uint8_t  	  g_lastReadingFlag=0;						  //上次状态 1=正在读中  0=正常工作状态
uint8_t     g_setModelFlag=0;							  //1=进入到设置模式
uint8_t     g_pwmOrTtlModelFlag=0;				          //0=pwm  1=ttl-H   2=ttl-L
uint16_t    g_ttlCurrentNum=0;							  //当前触发计数
uint16_t    g_ttlTriggerNum=0;							  //触发是的触发计数
uint8_t     g_currentRowNum=2;						      //当前行 Num 当前行 OLED的当前行 范围=2 4 6
uint8_t 	  g_camTaskCanTakeOverFlag=0;			          //1=cam任务可以接管
uint32_t 	  g_offOrOnTureStateFlag=SYSTEMSTATE_GOTOSHUT;
uint8_t  	  g_camLoseFlagTable[6]={0,0,0,0,0,0};          //5个相机的存在标志 即不丢失 上电后会检查一次
uint8_t     g_theTurelostNumTable[6]={0,0,0,0,0,0};       //实际丢片数
uint8_t     g_camHotShotTable[6]={0,0,0,0,0,0}; 

//-------------------------------------------------------------------------
//任务函数
void  Cam_status_update_TASK (void* parameter)
{ 

	while (1) 
	{ 
		vTaskDelay(100);
		 /* 判断相机的开关机状态 */
		  if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
		   {
			   g_camPowerFlagTable[1] = 1;
			}
			else
			{
			   g_camPowerFlagTable[1] = 0;
			}
			
		    if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			   g_camPowerFlagTable[2] = 1;
			}
			else
			{
			  g_camPowerFlagTable[2] = 0;
			}
			
			if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			  g_camPowerFlagTable[3] = 1;
			}
			else
			{
			  g_camPowerFlagTable[3] = 0;
			}
			
			if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			  g_camPowerFlagTable[4] = 1;
			}
			else
			{
			  g_camPowerFlagTable[4] = 0;
			}
			
			if( GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1))
			{
			  g_camPowerFlagTable[5] = 1;
			}
			else
			{
			  g_camPowerFlagTable[5] = 0;
			}
	}
}







