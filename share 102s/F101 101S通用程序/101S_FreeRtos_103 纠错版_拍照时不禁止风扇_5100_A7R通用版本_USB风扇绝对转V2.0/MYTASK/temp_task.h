#ifndef __TEMP_TASK_H__
#define __TEMP_TASK_H__
#include "myinclude.h"

extern void  TEMP_TASK(void const * argument);			
extern uint16_t  g_ds18b20Temp;                   //ds18b20 ÎÂ¶ÈÔÝ´æ
extern TaskHandle_t 	TEMP_TASK_Handle;

#endif


