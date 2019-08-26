#ifndef __BSP_USART2_H__
#define	__BSP_USART2_H__



#include "myinclude.h"

#define PRINTF_BEFORE if(0==1)
//#define PRINTF_BEFORE 



#define PRINTF_BEFORE1 if(0==1)
//#define PRINTF_BEFORE1 



//#define PRINTF_BEFORE2 if(0==1)
#define PRINTF_BEFORE2 

/*
//------------------------------------------------
//18554016766@126.com
//shareuav122333
//-------------------------------------------------------------------------------------------------
//用户开发帐户
#define USER_APP_NAME               "SHARE-101S"
#define USER_APP_ID                 "65187"
#define USER_APP_KEY                "f37923a5b2f94477defd81783ef1005"
#define USER_DEVELOPER_ACCOUNT      "18554016766@126.com"
*/

extern int  Rs485Read(uint8_t *pReadData, uint16_t dataLen);
extern void Rs485SendStr(uint8_t *pReadData);

#endif




