#ifndef __FAN_TASK_H__
#define __FAN_TASK_H__
#include "myinclude.h"

#define FANSTATE_EN                 1
#define FANSTATE_NO                 2
#define FANSTATE_ABSOLUTELYNO       3
#define FANSTATE_RETURNLASTLAST     4

extern uint8_t g_fanDs18b20Flag;//DS18B20逻辑的 0=不转 1=转

extern uint8_t g_fanTurePhotoEnFlag;//1=拍照启动了


extern void  FAN_TASK(void const * argument);			

#endif

