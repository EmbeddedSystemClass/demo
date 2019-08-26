#include "oled_task.h"
 
 /* **************************************************
 say:
 OLED 是显示模块 只是个显示模块 是个纯粹的四肢模块 没有任何决定权
 所以写OLED任务的时候，不要有任何会影响到别人调用的程序段
 也不给延时(就是最后留个100ms的延时 必须有的)
 提供最纯粹的调用函数 最纯粹的听话函数
 关于状态标志 值牵扯OLED自己状态标志
 和系统状态标志没有一点关系 千万不要与系统桩体标志或者其他非OLED自身的标志相关联 否则会混乱的 
 *****************************************************************************************/
 
 
/*************************************************
Function: void  OLED_TASK (void const * argument)
Description: OLED屏 显示当前相机的工作状态和拍照数量
Return: 无返回值
*************************************************/
 
TaskHandle_t 	OLED_TASK_Handle = NULL;		//创建任务句柄


void  OLED_TASK (void* parameter)
{
  BaseType_t xReturn = pdPASS;
	uint32_t p_msg = 0;
	uint8_t n = 0;
	
	OLED_Clear();	
    vTaskDelay(10);	
    
	while (1) 
	{	     
     xReturn = xTaskNotifyWait(0,0XFFFFFFFF,&p_msg,0);/* 进入函数的时候不清除任务bit，推出的时候清除所有的bit，保存任务通知&p_msg，阻塞时间=不等待 */
         
		if(pdPASS == xReturn)
		{
            OLED_Clear();
			g_oledStateFlag = p_msg;
            vTaskDelay(10);	 
            n = 0;            
		}
		else
		{
				vTaskDelay(200);
		}    
		
		if(OLEDSHOWSTATE_LOGO == g_oledStateFlag)
		{
			cocoguojia_oledShowShareuavLogoAndID();
		}
		

		if( (SYSTEMSTATE_OFFTRUE == g_oledStateFlag) || (SYSTEMSTATE_ONTRUE == g_oledStateFlag) )
		{
			if((10 < n) && (20 >= n))
			{	
				if(1 == g_triggerFlag)
				{
					n = 40;
                    if(SYSTEMSTATE_ONTRUE == g_oledStateFlag)
                    {
                        cocoguojia_oledRunShowFor1(1,1);
                    }
                    else
                    {
                         cocoguojia_oledRunShowFor1(1,0);
                    }    
					vTaskDelay(200);				                    
				}
				else
				{	
					cocoguojia_oledRunShowFor2();
					n++;
				}	
			}
			else if(10 >= n)
			{
				if(1 == g_triggerFlag)
				{
					n = 1;	
					g_triggerFlag = 0;
					if(SYSTEMSTATE_ONTRUE == g_oledStateFlag)
                    {
                        cocoguojia_oledRunShowFor1(1,1);
                    }
                    else
                    {
                         cocoguojia_oledRunShowFor1(1,0);
                    }    
					vTaskDelay(200);							
				}
				else
				{	
					if(SYSTEMSTATE_ONTRUE == g_oledStateFlag)
                    {
                        cocoguojia_oledRunShowFor1(0,1);
                    }
                    else
                    {
                         cocoguojia_oledRunShowFor1(0,0);
                    }    
					n++;
				}
			}
			else
			{
				OLED_Clear();
				vTaskDelay(2);		
				n=0;
			}
			
			if(40 <= n)
			{
				OLED_Clear();
				vTaskDelay(2);		
				n=0;
			}				
		}
        
    if(OLEDSHOWSTATE_REPAIRMODEL1 == g_oledStateFlag)
		{
			cocoguojia_oledShowRepairModel(1);
		}
    if(OLEDSHOWSTATE_REPAIRMODEL2 == g_oledStateFlag)
		{
			cocoguojia_oledShowRepairModel(2);
		}
    if(OLEDSHOWSTATE_REPAIRMODEL3 == g_oledStateFlag)
		{
			cocoguojia_oledShowRepairModel(3);
		}
    if(OLEDSHOWSTATE_REPAIRMODEL4 == g_oledStateFlag)
		{
			cocoguojia_oledShowRepairModel(4);
		}
		
		

		if(OLEDSHOWSTATE_INIT == g_oledStateFlag)        //显示 系统初始化中
		{
			cocoguojia_oledShowSystemInit();
		}
		

		
		if(SYSTEMSTATE_GOTOSHUT == g_oledStateFlag)     //显示 关机中
		{
			cocoguojia_oledShowShuting();
		}
		

		
		if(SYSTEMSTATE_GOTOOPEN == g_oledStateFlag)     //显示 开机中
		{
			cocoguojia_oledShowOpening();
		}
		

		
		if(SYSTEMSTATE_SHUTFAIL == g_oledStateFlag)     //关机失败
		{
			cocoguojia_oledShowShutWrong();
		}
		
		
	
		if(SYSTEMSTATE_OPENFAIL == g_oledStateFlag)     //开机失败
		{
			cocoguojia_oledShowOpenWrong();
		}	
        
        if(SYSTEMSTATE_CAMLOSE == g_oledStateFlag)     //开机失败
		{
			cocoguojia_oledShowLoseWrong();
		}	
         

		
		if(OLEDSHOWSTATE_SETMODEL1 == g_oledStateFlag)    //进入到设置模式 page1
		{
			cocoguojia_oledShowSetModel1();
		}

		
		if(OLEDSHOWSTATE_SETMODEL3 == g_oledStateFlag)    //进入到设置结果显示界面
		{
			cocoguojia_oledShowSetModel3(0);
		}
		
		
		if(OLEDSHOWSTATE_SETMODEL4 == g_oledStateFlag)    //进入到设置结果显示界面
		{
			cocoguojia_oledShowSetModel3(1);
		}
        
        vTaskDelay(10);		
	 }	
}




