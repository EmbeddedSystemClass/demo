
#ifndef __TIM5CATCH_COCOGUOJIA_H__
#define __TIM5CATCH_COCOGUOJIA_H__

#include "myinclude.h"

//W我们默认 3999+1  4ms一个周期
#define CATCH_PERIOD  3999

extern void cocoguojia_tim5CatchInit(void);
extern void cocoguojia_tim5CatchEnable(void);
extern void cocoguojia_tim5CatchDisable(void);

extern void cocoguojia_ttlTestInit(void);


#endif


