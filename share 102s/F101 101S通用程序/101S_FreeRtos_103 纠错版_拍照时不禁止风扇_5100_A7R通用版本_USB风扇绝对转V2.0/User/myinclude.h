#ifndef __MY_PSDK_INCLUDE__
#define __MY_PSDK_INCLUDE__

/* FreeRTOSͷ�ļ� */

#include "stm32f10x.h"

#include <stdio.h>


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include <string.h>
#include "main.h"
#include "bsp.h"
#include "hotshoe.h"

#include "key_task.h"
#include "cam_task.h"
#include "oled_task.h"
#include "rgb_task.h"
#include "runinit_task.h"
#include "Cam_status_task.h"
#include "trigger_task.h" 
#include "temp_task.h"
#include "cocoguojia_init.h"





//���ʹ��CPU ͳ������ ��Ҫ���� CPU_TASK_EN
//#define CPU_TASK_EN  1


#endif






