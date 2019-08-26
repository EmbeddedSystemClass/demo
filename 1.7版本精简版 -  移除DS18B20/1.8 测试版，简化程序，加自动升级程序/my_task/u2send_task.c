#include "u2send_task.h"
#include "psdk_debug.h"   

static void comFor5CamOn(void);
static void comFor5CamOff(void);
static void comFor5CamPhoto(void);
static void askFor5Cam(void);
static void comForKey(void);



void  U2SEND_TASK (void const * argument)
{
 
	  osEvent evt;
    uint32_t n=0;
	while (1) 
	{	            
    evt=osMessageGet(usart2SendMessageQHandle,osWaitForever);
         
		if(osEventMessage==evt.status) /*收到消息里*/
		{
			n=evt.value.v;
            switch(n)
            {
                case USART2COM_OPEN:
                comFor5CamOn();
                break;  

                case USART2COM_SHUT:
                comFor5CamOff();
                break;

                case USART2COM_PHOTO:
                comFor5CamPhoto();
                break;

                case USART2COM_ASK:
                askFor5Cam();
                break;

                case USART2COM_KEY:
                comForKey();
                break;

                default:
                break;
            }                
		}
  }
}
    

/*开机 自身23ms+8延时*/
static void comFor5CamOn(void)
{
	
    RS485_TXD_EN;
    osDelay(3);
    cocoguojia_usartSendStr(2,"@@@");  /*前导@*/
    cocoguojia_usartSendStr(2,"@X1#"); /*发送关机命令*/
    osDelay(20);
}


/*关机 自身23ms+8延时*/
static void comFor5CamOff(void)
{
    
    RS485_TXD_EN;
    osDelay(3);
    cocoguojia_usartSendStr(2,"@@@");  /*前导@*/
    cocoguojia_usartSendStr(2,"@X2#"); /*发送关机命令*/
    osDelay(20);
}


/*拍照 自身23ms+8延时*/
static void comFor5CamPhoto(void)
{
    
    RS485_TXD_EN;
    cocoguojia_usartSendStr(2,"@@@");  /*前导@*/
    cocoguojia_usartSendStr(2,"@X3#"); /*发送关机命令*/
    
}

extern uint8_t  g_usart2forSendRunFlag;    /*串口2总发送正在进行标志*/


/*查询状态 自身180ms+20ms延时*/
static void askFor5Cam(void)
{
    
    RS485_TXD_EN;
    osDelay(3);
    cocoguojia_usartSendStr(2,"@14#");/*发送查询命令*/    
    osDelay(7);
    RS485_RXD_EN;
    osDelay(30);
    
    RS485_TXD_EN;
    osDelay(3);
    cocoguojia_usartSendStr(2,"@24#");/*发送查询命令*/   
    osDelay(7);
    RS485_RXD_EN;
    osDelay(30);
    
    RS485_TXD_EN;
    osDelay(3);
    cocoguojia_usartSendStr(2,"@34#");/*发送查询命令 */
    osDelay(7);
    RS485_RXD_EN;
    osDelay(30);
   
    RS485_TXD_EN;
    osDelay(3);
    cocoguojia_usartSendStr(2,"@44#");/*发送查询命令*/
    osDelay(7);
    RS485_RXD_EN;
    osDelay(30);
    
    RS485_TXD_EN;
    osDelay(3);
    cocoguojia_usartSendStr(2,"@54#");/*发送查询命令*/
    osDelay(7);
    RS485_RXD_EN;
    osDelay(30);
    
    RS485_TXD_EN;   
}



/*OK按键 自身23ms+5延时*/
static void comForKey(void)
{
    
    RS485_TXD_EN;
    osDelay(3);
    cocoguojia_usartSendStr(2,"@@@"); /*前导@*/
    cocoguojia_usartSendStr(2,"@X5#");/*发送关机命令*/
    osDelay(20);
}








