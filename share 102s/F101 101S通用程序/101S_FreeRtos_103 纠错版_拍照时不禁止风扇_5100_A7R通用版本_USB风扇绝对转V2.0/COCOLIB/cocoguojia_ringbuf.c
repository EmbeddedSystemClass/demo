

#include "cocoguojia_ringbuf.h"

T_Usart2RingBufDoStruct    U2RecBufStruct;
//T_Usart3RingBufDoStruct    U3RecBufStruct;


void cocoguojia_ringBufInit(void) 
{
    //------------------------------------------------------------------------------------------
    //����2���ν�������ʼ��
    U2RecBufStruct.rawReadIndex=0;
    U2RecBufStruct.rawWriteIndex=0; 
    U2RecBufStruct.parseIndex=0; 
    memset((U2RecBufStruct.rawBuf),   0, sizeof((U2RecBufStruct.rawBuf)));        //��շ������մ�������
    memset((U2RecBufStruct.parseBuf), 0, sizeof(U2RecBufStruct.parseBuf));        //��մ��ڽ��մ�������
    
    /*
    //------------------------------------------------------------------------------------------
    //����3���ν�������ʼ��
    U3RecBufStruct.rawReadIndex=0;
    U3RecBufStruct.rawWriteIndex=0; 
    U3RecBufStruct.parseIndex=0; 
    memset((U3RecBufStruct.rawBuf),   0, sizeof((U3RecBufStruct.rawBuf)));        //��շ������մ�������
    memset((U3RecBufStruct.parseBuf), 0, sizeof(U3RecBufStruct.parseBuf));        //��մ��ڽ��մ�������
    */
}	




