 
 
 
#include "oled_task.h"
#include "psdk_debug.h"
 
 //cocoguojia say:
 //OLED 是显示模块 只是个显示模块 是个纯粹的四肢模块 没有任何决定权
 //所以写OLED任务的时候，不要有任何会影响到别人调用的程序段
 //也不给延时(就是最后留个100ms的延时 必须有的)
 //提供最纯粹的调用函数 最纯粹的听话函数
 //关于状态标志 值牵扯OLED自己状态标志
 //和系统状态标志没有一点关系 千万不要与系统桩体标志或者其他非OLED自身的标志相关联 否则会混乱的
unsigned portBASE_TYPE uxHighWaterMark;  
 
//-------------------------------------------------------------------------
//任务函数
void  OLED_TASK (void const * argument)
{
	uint8_t n=0;
	osEvent evt;
	OLED_Clear();	
    osDelay(10);	
  uxHighWaterMark=uxTaskGetStackHighWaterMark( OLED_TASK_Handle );    //查询大小  
	PSDK_LOG_DEBUG("OLED_TASK  %d\r\n",uxHighWaterMark);
	while (1) 
	{	     
        evt=osMessageGet(oledShowMessageQHandle,0); 
   
	




		
		if(SYSTEMSTATE_SHUTFAIL==g_oledStateFlag)     //关机失败
		{
			cocoguojia_oledShowShutWrong();
		}
		
		
	
		if(SYSTEMSTATE_OPENFAIL==g_oledStateFlag)     //开机失败
		{
			cocoguojia_oledShowOpenWrong();
		}	
        
        if(SYSTEMSTATE_CAMLOSE==g_oledStateFlag)     //开机失败
		{
			cocoguojia_oledShowLoseWrong();
		}	
         

		
		if(OLEDSHOWSTATE_SETMODEL1==g_oledStateFlag)    //进入到设置模式 page1
		{
			cocoguojia_oledShowSetModel1();
		}
        

		
		if(OLEDSHOWSTATE_SETMODEL3==g_oledStateFlag)    //进入到设置结果显示界面
		{
			cocoguojia_oledShowSetModel2(0);
		}
		
		
		if(OLEDSHOWSTATE_SETMODEL4==g_oledStateFlag)    //进入到设置结果显示界面
		{
			cocoguojia_oledShowSetModel2(1);
		}
        
        osDelay(10);		
	 }	
}




