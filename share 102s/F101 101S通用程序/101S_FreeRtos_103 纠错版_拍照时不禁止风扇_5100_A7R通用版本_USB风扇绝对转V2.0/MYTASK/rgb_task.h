 
 
 
   
	

#ifndef __RGB_TASK_H__
#define __RGB_TASK_H__

#include "myinclude.h"

extern TaskHandle_t 	RGB_TASK_Handle;

extern void RGB_TASK	(void* parameter);	



extern TaskHandle_t 	CPU_TASK_Handle 	;		//创建任务句柄

extern void CPU_TASK(void* parameter);


#endif


