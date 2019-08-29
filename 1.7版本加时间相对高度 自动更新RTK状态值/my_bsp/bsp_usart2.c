
#include "myinclude.h"



int Rs485Read(uint8_t *pReadData, uint16_t dataLen)
{
    int res=0;

    res = UART_Read(UART_NUM_2, pReadData, dataLen);

    return res;
}


  

void Rs485SendStr(uint8_t *pReadData)
{
    uint16_t dataLen=strlen((const char*)&pReadData[0]);

    UART_Write(UART_NUM_2,pReadData,(dataLen+ 1));
}
