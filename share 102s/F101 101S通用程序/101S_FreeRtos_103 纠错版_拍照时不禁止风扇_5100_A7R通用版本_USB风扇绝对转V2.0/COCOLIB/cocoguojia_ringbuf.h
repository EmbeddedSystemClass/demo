
#ifndef __RINGBUF_COCOGUOJIA_H
#define __BINGBUF_COCOGUOJIA_H

#include "myinclude.h"

//-----------------------------------------------------------
//����2 ���ν���ԭʼ������
#define USART2_RECRAW_RINGBUGLENMAX 512
//����2 ���ν��շ��� ������
#define  USART2_RECPARSE_RINGBUGLENMAX 64


typedef struct Usart2RingBufDoStruct_T{
    uint8_t  rawBuf[USART2_RECRAW_RINGBUGLENMAX];       //����2 ���ν��ջ�����
    uint16_t rawReadIndex;
    uint16_t rawWriteIndex;
    
   uint8_t  parseBuf[USART2_RECPARSE_RINGBUGLENMAX];   //����2 ���ν��շ���������
   uint16_t parseIndex;
} T_Usart2RingBufDoStruct;



extern  T_Usart2RingBufDoStruct    U2RecBufStruct;


/*
//-----------------------------------------------------------
//����3 ���ν���ԭʼ������
#define USART3_RECRAW_RINGBUGLENMAX 1024
//����3 ���ν��շ��� ������
#define  USART3_RECPARSE_RINGBUGLENMAX 512
typedef struct Usart3RingBufDoStruct_T{
    uint8_t  rawBuf[USART3_RECRAW_RINGBUGLENMAX];       //����2 ���ν��ջ�����
    uint16_t rawReadIndex;
    uint16_t rawWriteIndex;
    
   uint8_t  parseBuf[USART3_RECPARSE_RINGBUGLENMAX];   //����2 ���ν��շ���������
   uint16_t parseIndex;
} T_Usart3RingBufDoStruct;
//extern  T_Usart3RingBufDoStruct    U3RecBufStruct;
*/



extern void cocoguojia_ringBufInit(void); 


#endif




