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
    evt=osMessageGet(SendMessageQHandle,osWaitForever);
         
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
    

/*开机*/
static void comFor5CamOn(void)
{
	

    osDelay(20);
}


/*关机*/
static void comFor5CamOff(void)
{
    

    osDelay(20);
}


/*拍照*/
static void comFor5CamPhoto(void)
{
    

    osDelay(20);
}




/*查询状态*/
static void askFor5Cam(void)
{
    

    osDelay(30);
    

}



/*OK按键*/
static void comForKey(void)
{
    
    osDelay(20);
}








