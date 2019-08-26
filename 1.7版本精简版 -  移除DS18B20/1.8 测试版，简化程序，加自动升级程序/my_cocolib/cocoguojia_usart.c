///////////////////////////////////////////////////
//自身头文件
#include "cocoguojia_usart.h"

///////////////////////////////////////////////////
//自身用二级头文件
#include "cocoguojia_usart_ringbuffer.h"

//------------------------------------
//调试输出初始化过的变量标志
static uint8_t isLogInit = 0;


//------------------------------------------------
//USART 接收发送缓存大小定义
#define UART_READ_BUF_SIZE      512
#define UART_WRITE_BUF_SIZE     512

//----------------------------------------------------------------
//USART优先级定义 抢占优先级0到15 子优先级固定为0--为了配合FreeRTOS
//一般FreeRTOS configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY设置为5 就是0~4不受管
//我们抢占优先级设置为3 所以USART的中断不归FreeRTOS管理
#define UART1_IRQ_PRIO_PRE      3
#define UART1_IRQ_PRIO_SUB      0

#define UART2_IRQ_PRIO_PRE      3
#define UART2_IRQ_PRIO_SUB      0

#define UART3_IRQ_PRIO_PRE      3
#define UART3_IRQ_PRIO_SUB      0

//------------------------------------------------
//USART管脚定义
//U1
#define UART1_GPIO_TX       GPIO_Pin_9
#define UART1_GPIO_RX       GPIO_Pin_10
#define UART1_GPIO          GPIOA
#define UART1_GPIO_RCC      RCC_APB2Periph_GPIOA
#define RCC_APBPeriph_UART1 RCC_APB2Periph_USART1
//U2
#define UART2_GPIO_TX       GPIO_Pin_2
#define UART2_GPIO_RX       GPIO_Pin_3
#define UART2_GPIO          GPIOA
#define UART2_GPIO_RCC      RCC_APB2Periph_GPIOA
#define RCC_APBPeriph_UART2 RCC_APB1Periph_USART2
//U3
#define UART3_GPIO_TX       GPIO_Pin_10
#define UART3_GPIO_RX       GPIO_Pin_11
#define UART3_GPIO          GPIOB
#define UART3_GPIO_RCC      RCC_APB2Periph_GPIOB
#define RCC_APBPeriph_UART3 RCC_APB1Periph_USART3

//---------------------------------------------
//USART 环形缓存初始化
#define UART_RINGBUFFER_INIT(n)                                                             \
{                                                                                           \
    RingBuf_Init(&s_uart##n##ReadRingBuffer, s_uart##n##ReadBuf, UART_READ_BUF_SIZE);       \
    RingBuf_Init(&s_uart##n##WriteRingBuffer, s_uart##n##WriteBuf, UART_WRITE_BUF_SIZE);    \
}



//---------------------------------------------
//USART IO口初始化
#define GPIO_CONFIG_UART(n)                                                         \
{                                                                                   \
    GPIO_InitTypeDef GPIO_InitStructure;                                            \
                                                                                    \
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                                \
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                               \
    GPIO_InitStructure.GPIO_Pin =  UART##n##_GPIO_TX;                               \
    GPIO_Init(UART##n##_GPIO, &GPIO_InitStructure);                                 \
                                                                                    \
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;                          \
    GPIO_InitStructure.GPIO_Pin = UART##n##_GPIO_RX;                                \
    GPIO_Init(UART##n##_GPIO, &GPIO_InitStructure);                                 \
}

//---------------------------------------------
//USART 模式初始化
#define UART_CONFIG(n, bd)                                                      \
{                                                                               \
    USART_InitTypeDef USART_InitStructure;                                          \
                                                                                    \
    USART_InitStructure.USART_BaudRate = bd;                                        \
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     \
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          \
    USART_InitStructure.USART_Parity = USART_Parity_No;                             \
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; \
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 \
                                                                                    \
    USART_Init(USART##n, &USART_InitStructure);                                     \
                                                                                    \
    USART_Cmd(USART##n, ENABLE);                                                    \
                                                                                    \
    USART_ITConfig(USART##n, USART_IT_RXNE, ENABLE);                                \
}

//---------------------------------------------------------------------
//UART嵌套矢量中断控制器（NVIC）配置
#define UART_NVIC_CONFIG(n, prePrio, subPrio)                                   \
{                                                                               \
    NVIC_InitTypeDef NVIC_InitStructure;                                            \
                                                                                    \
    NVIC_InitStructure.NVIC_IRQChannel = USART##n##_IRQn;                           \
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prePrio;                 \
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPrio;                        \
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                 \
    NVIC_Init(&NVIC_InitStructure);                                                 \
}

//    if(USART_GetITStatus(USART##n, USART_IT_RXNE) != RESET)         \
//		 {                                                              \
//        USART_ClearITPendingBit(USART##n, USART_IT_RXNE);           \
//	   } 
//---------------------------------------------------------------------
//UART中断请求处理程序功能
#define UART_IRQ_HANDLER(n)                                         \
void USART##n##_IRQHandler(void)                                    \
{                                                                   \
    uint8_t data;                                                   \
                                                                    \
    if (USART_GetITStatus(USART##n, USART_IT_RXNE) != RESET)        \
    {                                                               \
        data = (uint8_t)(USART_ReceiveData(USART##n) & 0x00FF);     \
        RingBuf_Put(&s_uart##n##ReadRingBuffer, &data, 1);          \
    }                                                               \
    if(USART_GetITStatus(USART##n, USART_IT_TXE) != RESET)          \
    {                                                               \
        if(RingBuf_Get(&s_uart##n##WriteRingBuffer, &data, 1))      \
        {                                                           \
            USART_SendData(USART##n, data);                         \
        }                                                           \
        else                                                        \
        {                                                           \
            USART_ITConfig(USART##n, USART_IT_TXE, DISABLE);        \
        }                                                           \
    }                                                               \
                                                             \
                                                                    \
    if(USART_GetFlagStatus(USART##n,USART_FLAG_ORE)==SET)           \
    {                                                               \
        USART_ClearFlag(USART##n, USART_FLAG_ORE);                  \
        USART_ReceiveData(USART##n);                                \
    }                                                               \
}

//---------------------------------------------------------
//私有变量
#ifdef USING_UART_PORT_1
//UART1 接收缓存
static T_RingBuffer s_uart1ReadRingBuffer = {0};
//UART1 发送缓存
static T_RingBuffer s_uart1WriteRingBuffer = {0};
//UART1 读缓存
static uint8_t s_uart1ReadBuf[UART_READ_BUF_SIZE] = {0};
//UART1 写缓存
static uint8_t s_uart1WriteBuf[UART_WRITE_BUF_SIZE] = {0};
//UART1 mutex
static osMutexId s_uart1Mutex;
#endif

#ifdef USING_UART_PORT_2
static T_RingBuffer s_uart2ReadRingBuffer = {0};
static T_RingBuffer s_uart2WriteRingBuffer = {0};
static uint8_t s_uart2ReadBuf[UART_READ_BUF_SIZE] = {0};
static uint8_t s_uart2WriteBuf[UART_WRITE_BUF_SIZE] = {0};
static osMutexId s_uart2Mutex;
#endif


#ifdef USING_UART_PORT_3
static T_RingBuffer s_uart3ReadRingBuffer  = {0};
static T_RingBuffer s_uart3WriteRingBuffer = {0};
static uint8_t s_uart3ReadBuf[UART_READ_BUF_SIZE] = {0};
static uint8_t s_uart3WriteBuf[UART_WRITE_BUF_SIZE] = {0};
static osMutexId s_uart3Mutex;
#endif

//----------------------------------------------------------------
//串口初始化
void UART_Init(E_UartNum uartNum, uint32_t baudRate)
{    
    switch (uartNum) 
   {
#ifdef USING_UART_PORT_1
        case UART_NUM_1:
        {
            UART_RINGBUFFER_INIT(1);
            RCC_APB2PeriphClockCmd(UART1_GPIO_RCC, ENABLE);
            RCC_APB2PeriphClockCmd(RCC_APBPeriph_UART1, ENABLE);
            GPIO_CONFIG_UART(1);
            UART_NVIC_CONFIG(1, UART1_IRQ_PRIO_PRE, UART1_IRQ_PRIO_SUB);
            UART_CONFIG(1, baudRate);
            osMutexDef(uart1Mutex);
            s_uart1Mutex = osMutexCreate(osMutex(uart1Mutex));
        }
        break;
#endif

#ifdef USING_UART_PORT_2
        case UART_NUM_2:
        {
            UART_RINGBUFFER_INIT(2);
            RCC_APB2PeriphClockCmd(UART2_GPIO_RCC, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APBPeriph_UART2, ENABLE);
            GPIO_CONFIG_UART(2);
            UART_NVIC_CONFIG(2, UART2_IRQ_PRIO_PRE, UART2_IRQ_PRIO_SUB);
            UART_CONFIG(2, baudRate);
            osMutexDef(uart2Mutex);
            s_uart2Mutex = osMutexCreate(osMutex(uart2Mutex));
        }
        break;
#endif

#ifdef USING_UART_PORT_3
        case UART_NUM_3: 
        {
            UART_RINGBUFFER_INIT(3);
            RCC_APB2PeriphClockCmd(UART3_GPIO_RCC, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APBPeriph_UART3, ENABLE);
            GPIO_CONFIG_UART(3);
            UART_NVIC_CONFIG(3, UART3_IRQ_PRIO_PRE, UART3_IRQ_PRIO_SUB);
            UART_CONFIG(3, baudRate);
            osMutexDef(uart3Mutex);
            s_uart3Mutex = osMutexCreate(osMutex(uart3Mutex));
        }
        break;
#endif

        default:
        break;
    }
}

//----------------------------------------------------------------
//串口 读取缓存区数据
//返回 读取缓存区数据的长度
int UART_Read(E_UartNum uartNum, uint8_t *buf, uint16_t readSize)
{
    uint16_t readRealSize;
    switch (uartNum)
    {
        #ifdef USING_UART_PORT_1
        case UART_NUM_1: 
        {
            osMutexWait(s_uart1Mutex, osWaitForever);
            readRealSize = RingBuf_Get(&s_uart1ReadRingBuffer, buf, readSize);
            osMutexRelease(s_uart1Mutex);
        }
        break;
        #endif

        #ifdef USING_UART_PORT_2
        case UART_NUM_2: 
        {
            osMutexWait(s_uart2Mutex, osWaitForever);
            readRealSize = RingBuf_Get(&s_uart2ReadRingBuffer, buf, readSize);
            osMutexRelease(s_uart2Mutex);
        }
        break;
        #endif

        #ifdef USING_UART_PORT_3
        case UART_NUM_3: 
        {
            osMutexWait(s_uart3Mutex, osWaitForever);
            readRealSize = RingBuf_Get(&s_uart3ReadRingBuffer, buf, readSize);
            osMutexRelease(s_uart3Mutex);

        }
        break;
        #endif
        
        default:
        return UART_ERROR;
    }
    
    return readRealSize;
}

//----------------------------------------------------------------
//串口 写缓存区数据
//返回 写缓存区数据的长度
int UART_Write(E_UartNum uartNum, const uint8_t *buf, uint16_t writeSize)
{
    int writeRealLen;
    switch (uartNum) 
    {
#ifdef USING_UART_PORT_1
        case UART_NUM_1: 
        {
            osMutexWait(s_uart1Mutex, osWaitForever);
            writeRealLen = RingBuf_Put(&s_uart1WriteRingBuffer, buf, writeSize);
            USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
            osMutexRelease(s_uart1Mutex);

        }
        break;
#endif

#ifdef USING_UART_PORT_2
        case UART_NUM_2: 
        {
            osMutexWait(s_uart2Mutex, osWaitForever);
            writeRealLen = RingBuf_Put(&s_uart2WriteRingBuffer, buf, writeSize);
            USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
            osMutexRelease(s_uart2Mutex);

        }
        break;
#endif

#ifdef USING_UART_PORT_3
        case UART_NUM_3: 
        {
            osMutexWait(s_uart3Mutex, osWaitForever);
            writeRealLen = RingBuf_Put(&s_uart3WriteRingBuffer, buf, writeSize);
            USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
            osMutexRelease(s_uart3Mutex);
        }
        break;
#endif

        default:
        return UART_ERROR;
    }
    return writeRealLen;
}

//----------------------------------------------------------------
//串口 发送一个字符
void cocoguojia_usartSendChar(E_UartNum uartNum, uint8_t dat)
{  
    uint8_t *datTemp=&dat;  
    UART_Write(uartNum,datTemp,1);
}

//----------------------------------------------------------------
//串口 写缓存区数据
void cocoguojia_usartSendStr(E_UartNum uartNum, uint8_t *pReadData)
{  
    uint16_t len=strlen((const char*)&pReadData[0]);
    UART_Write(uartNum,pReadData,(len+1));
}

//----------------------------------------------------------------
//读串口缓存区数据
//返回读到的数据长度
int cocoguojia_usartReadDataForlen(E_UartNum uartNum, uint8_t *pReadData, uint16_t len)
{
    int res=0;
    res = UART_Read(uartNum, pReadData, len);
    return res;
}


//------------------------------------
//调试输出初始化
void LOG_Init(void)
{
    UART_Init(UART_NUM_1, 460800);
    isLogInit = 1;
}

//------------------------------------
//调试输出函数
void UART_Printf(const char *fmt, ...)
{
    if (isLogInit) 
    {
        char buffer[LOG_STR_LEN_MAX];//调试输出缓存  LOG_STR_LEN_MAX=255
        va_list args;
        va_start (args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        UART_Write(UART_NUM_1, (uint8_t *)buffer, (uint16_t)(strlen(buffer) + 1));
        va_end(args);
    }
}


//---------------------------------------------------------------------
//UART中断函数
#ifdef USING_UART_PORT_1
    UART_IRQ_HANDLER(1)
#endif


#ifdef USING_UART_PORT_2
    UART_IRQ_HANDLER(2)
#endif

#ifdef USING_UART_PORT_3
    UART_IRQ_HANDLER(3)
#endif
