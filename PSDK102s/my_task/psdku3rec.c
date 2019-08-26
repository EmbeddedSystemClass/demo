 
#include "psdku3rec.h"

////////////////////////////////////////////////////////////////////////////////////////
//全局变量
SemaphoreHandle_t uart3Mutex_Handle=NULL;           //定义串口2的接收数据缓冲区的 互斥信号量
xQueueHandle gpsQueue_Handle=NULL;                  //定义传递GPS信息的队列句柄

static uint16_t PsdkProtoUpper_Key(uint8_t *pKey);

//----------------------------------------------------
//PsdkRec任务实现
void PSDKU3REC_TASK (void const * argument)
{
   uint8_t res = 0;
   PsdkProtoUpper_Key(&g_keyTable[0]);
    
    while (1) 
	{	
        //-----------------------------------------------------------------------------------    
        //xReturn = xSemaphoreTake(uart3Mutex_Handle, 0);                           //获取互斥量 在操作缓冲区时 线获取互斥量uart2Mutex_Handle 如果没获到则一直等待
        res=Bsp_usart3ReadChar(&Usart3RingRecBufDoStruct,&PsdkProtFrameAllStruct);  //串口3接收数据并处理并正确回复
        //xReturn = xSemaphoreGive( uart3Mutex_Handle );                            //释放互斥量
        //-----------------------------------------------------------------------------------
        res=res;
  
        //延时1个tick(1);
        osDelay(1);         
    }
}


//-----------------------------------------------------------------------------------------------------
//密钥原来还要初始化计算一下
static uint16_t PsdkProtoUpper_Key(uint8_t *pKey)
{
    MD5_CTX md5Ctx;
    uint16_t md5InputLen = (uint16_t) (strlen(USER_APP_NAME) + strlen(USER_APP_ID) +
                                       strlen(USER_APP_KEY) + strlen(USER_DEVELOPER_ACCOUNT));
    
    uint8_t *md5InputData = pvPortMalloc(md5InputLen);
    
    uint8_t *pCal = md5InputData;
    
    //calc key
    memcpy(pCal, USER_APP_NAME,strlen(USER_APP_NAME));
    pCal += strlen(USER_APP_NAME);

    memcpy(pCal, USER_APP_ID, strlen(USER_APP_ID));
    pCal += strlen(USER_APP_ID);

    memcpy(pCal, USER_APP_KEY, strlen(USER_APP_KEY));
    pCal += strlen(USER_APP_KEY);

    memcpy(pCal,USER_DEVELOPER_ACCOUNT, strlen(USER_DEVELOPER_ACCOUNT));
    pCal += strlen(USER_DEVELOPER_ACCOUNT);

    md5_init(&md5Ctx);
    md5_update(&md5Ctx, md5InputData, pCal - md5InputData);
    md5_final(&md5Ctx, pKey);
    vPortFree(md5InputData);

    return 0;
}






