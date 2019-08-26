#ifndef __TRIGGER_TASK_H__
#define __TRIGGER_TASK_H__

#include "myinclude.h"

extern void TRIGGER_TASK (void const * argument);

extern uint8_t canPhotoStateFlag;//2=可以拍照
extern uint32_t  phototime;
extern uint8_t openCamStateFlag;//0=没开机 1=待开 2=开机 3=待关
#endif


