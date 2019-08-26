

#ifndef ___COCOGUOJIA__USART_H__
#define ___COCOGUOJIA__USART_H__

///////////////////////////////////////////////
//ͷ�ļ� ucosIII �����������ͷ�ļ�
/* FreeRTOSͷ�ļ� */
#include "myinclude.h" 

//���ڽ��ջ�������С�붨��
#define USART1RXDMAXSIZE            8
#define USART2RXDMAXSIZE            200
#define USART3RXDMAXSIZE            8
#define USART4RXDMAXSIZE            8
#define USART5RXDMAXSIZE            8


extern uint8_t  cocoguojia_usart1ReceiveTable[USART1RXDMAXSIZE];	//USART1���ڽ��ջ�����
extern uint16_t cocoguojia_usart1ReceiveWritePoint;				    //USART1���ڽ��ն���ͷָ��
extern uint16_t cocoguojia_usart1ReceiveReadPoint;				    //USART1���ڽ��ն���βָ��

extern uint8_t  cocoguojia_usart2ReceiveTable[USART2RXDMAXSIZE];	//USART1���ڽ��ջ�����
extern uint16_t cocoguojia_usart2ReceiveWritePoint;				    //USART1���ڽ��ն���ͷָ��
extern uint16_t cocoguojia_usart2ReceiveReadPoint;				    //USART1���ڽ��ն���βָ��


extern void cocoguojia_usartNConfigurationInit  (uint8_t usartNum,uint32_t baud);
extern void cocoguojia_usartNPutChar            (uint8_t uartNum,uint8_t chr);
extern void cocoguojia_usartNPutStr             (uint16_t uartNum,const uint8_t *str);
extern void cocoguojia_usartNPutTableForNum     (uint8_t uartNum,uint8_t *sp, uint16_t num);

#endif








