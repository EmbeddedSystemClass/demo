

#ifndef ___COCOGUOJIA__USART_H__
#define ___COCOGUOJIA__USART_H__

///////////////////////////////////////////////
/* FreeRTOS头文件 */
#include "myinclude.h" 

//#define DEBUG_TIAOSHI 	1

#define  E_UartNum uint8_t

#define    UART_NUM_1   1
#define    UART_NUM_2   2
#define    UART_NUM_3   3

#define USING_UART_PORT_1
#define USING_UART_PORT_2
#define USING_UART_PORT_3

#define UART_ERROR  -1

/*
typedef enum {
    UART_NUM_1  = 1,
    UART_NUM_2  = 2,
    UART_NUM_3  = 3,
} E_UartNum;
*/

//-----------------------------
//coocguojia 加
#ifdef DEBUG_TIAOSHI
#define UART_Printf_EN if(1==1)
#else
#define UART_Printf_EN if(0==1)
#endif

//--------------------------------------------------
//调试输出缓存
#define LOG_STR_LEN_MAX             255


#define UART_Printf_cocoguojia UART_Printf_EN UART_Printf
#define LOG(fmt, ...)       UART_Printf("[%s]" fmt "\r\n",__FUNCTION__,##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) UART_Printf("[Debug][%s]" fmt "\r\n", __FUNCTION__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  UART_Printf("[INFO][%s]" fmt "\r\n", __FUNCTION__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  UART_Printf("[WARN][%s]" fmt "\r\n", __FUNCTION__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) UART_Printf("[ERROR][%s]" fmt "\r\n", __FUNCTION__, ##__VA_ARGS__)

extern void LOG_Init(void);
extern void UART_Init(E_UartNum uartNum, uint32_t baudRate);
extern int UART_Read(E_UartNum uartNum, uint8_t *buf, uint16_t readSize);
extern int UART_Write(E_UartNum uartNum, const uint8_t *buf, uint16_t writeSize);
extern void cocoguojia_usartSendChar(E_UartNum uartNum, uint8_t dat);
extern void cocoguojia_usartSendStr(E_UartNum uartNum, uint8_t *pReadData);
extern int cocoguojia_usartReadDataForlen(E_UartNum uartNum, uint8_t *pReadData, uint16_t len);

extern void UART_Printf(const char *fmt, ...);//调试输出函数

#endif










