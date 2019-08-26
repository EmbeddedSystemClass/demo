
/**
 1、ring_buf_p->in 、ring_buf_p->out均小于size;
只有开始使用循环队列的阶段才会是这种情况，先分析size = MIN(size, ring_buf_p->size - ring_buf_p->in + ring_buf_p->out);
这句代码；代码补全为size = MIN(size, ring_buf_p->size - （ring_buf_p->in - ring_buf_p->out）); 由于ring_buf_p-
>in为入指针，ring_buf_p->out为出指针，则（ring_buf_p->in - ring_buf_p->out）即为循环缓存区已经被使用的大小，而 ring_buf_p->size - （ring_buf_p->in - ring_buf_p->out）即为循环缓存区剩余未使用的大小，与即将要写入的数据大小取二者中较小的，保证填入的数据不会出现越界或覆盖原有的数据。我们在看看len = MIN(size, ring_buf_p->size - (ring_buf_p->in & (ring_buf_p->size - 1)));这条语句，有的人可能不太理解 (ring_buf_p->in & (ring_buf_p->size - 1)))是什么意思，其实就是和ring_buf_p->in % ring_buf_p->size 的作用是一样的，就是取余；但是 (ring_buf_p->in & (ring_buf_p->size - 1)))的代码执行效率要比ring_buf_p->in % ring_buf_p->size高很多，在一下对实时性要求很高的使用场景下，代码的执行效率是要求很苛刻的；这是又要分两种情况讨论，第一种size小于等于ring_buf_p->size - (ring_buf_p->in & (ring_buf_p->size - 1))；这说明循环缓存区的后半部分的未使用大小足够放下要写入的数据大小，数据只要一次就能完全写完进循环缓存区；第二种size大于ring_buf_p->size - (ring_buf_p->in & (ring_buf_p->size - 1))；这说明循环缓存区的后半部分的未使用大小无法放下要写入的数据大小，数据只要分两次才能写入循环缓存区；第一次写入将后半部分剩余的缓存区大小使用完，第二次写入将剩余的未写入的数据大小从循环缓存区的首地址开始写入（这也就是循环缓冲区的作用，使用较小的实际物理内存实现了线性缓存）；
**/

///////////////////////////////////////////////////////////////////////////////////
//自身头文件 此头文件为cocoguojia_usart的二级头文件 指给cocoguojia_usart引用
#include "cocoguojia_usart_ringbuffer.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define RINGBUF_MIN(a, b) (((a)<(b))?(a):(b))

/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief Cut buffer size to power of 2, in order to increase the convenience of get and put operating of buffer.
 * @param bufSize Original buffer size.
 * @return Buffer size after handling.
 */
//把bufSize调整成在bufSize长度范围内的 最大的2的n次方的长度
static uint16_t RingBuf_CutBufSizeToPowOfTwo(uint16_t bufSize)
{
    uint16_t i = 0;

    while (bufSize >> (i++) != 1);

    return (uint16_t) (1 << (i - 1));
}


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Ring buffer initialization.
 * @param pthis Pointer to ring buffer structure.指向环形缓冲结构的指针
 * @param pBuf Pointer to data buffer.数据缓冲指针
 * @param bufSize Size of data buffer.
 * @return None.
 //我们用这个函数的时候 直接缓冲区大小要是2的n次方倍即可
*/

//初始化
void RingBuf_Init(T_RingBuffer *pthis, uint8_t *pBuf, uint16_t bufSize)
{
    pthis->readIndex = 0;
    pthis->writeIndex = 0;
    pthis->bufferPtr = pBuf;//数据缓冲区
	
    if (bufSize & (bufSize - 1)) 
	{
        //buffer size is not pow of two , cut buf 缓冲区不是2的n次方 分割一下此缓存区
        pthis->bufferSize = RingBuf_CutBufSizeToPowOfTwo(bufSize);//把bufSize调整成在bufSize长度范围内的 最大的2的n次方的长度
    } 
	else 
	{
        pthis->bufferSize = bufSize;
    }
}

/*
*pthis 环形缓冲区结构体
*pData 指向要写的数据的指针
dataLen 想要写的数据长度
 */
uint16_t RingBuf_Put(T_RingBuffer *pthis, const uint8_t *pData, uint16_t dataLen)
{
    uint16_t writeUpLen;	
	
	//环形缓冲区总长度-已经写的长度+已经读的长度=剩下的长度 和 要写的数据的长度取 最小 (不是想写多长就多长的 总得有限制 这个限制就是实际剩下的长度)
    dataLen = RINGBUF_MIN(dataLen, (uint16_t) (pthis->bufferSize - pthis->writeIndex + pthis->readIndex));

    //fill up data
	//环形缓冲区总长度-已经写的长度=从写到的地方为开始到末端的长度  
	//pthis->writeIndex & (pthis->bufferSize - 1) 这样处理时因为 缓冲区的长度我们是取2的n次方 那么再缓冲区的长度减1的数据一定是小于缓冲区的长度的 
	//并且除了最高为其它位都是1的 做&运算不会影响pthis->writeIndex的 目的就是限制一下不让pthis->writeIndex超过缓冲区的长度
	//和 pthis->writeIndex %(pthis->bufferSize)的效果是一样的
	//和 dataLen比较取最小
    writeUpLen = RINGBUF_MIN(dataLen, (uint16_t) (pthis->bufferSize - (pthis->writeIndex & (pthis->bufferSize - 1))));
	
	//拷贝 先从写到的地方为开始的地方 写数据注意pData是指针 最多就写到末端 当然如果数据短的话是写不到末端的 如果要写的数据长那么最多也就写道末端 
    memcpy(pthis->bufferPtr + (pthis->writeIndex & (pthis->bufferSize - 1)), pData, writeUpLen);

    //fill begin data
	//拷贝 先从缓冲区开始的地方 写数据注意pData是指针 已经写了writeUpLen所以pData + writeUpLen  
	//要写的长度为dataLen-writeUpLen 如果真还有没有写完的数据 那么剩下的就是dataLen - writeUpLen 如果dataLen - writeUpLen=0  memcpy函数是不操作的
    memcpy(pthis->bufferPtr, pData + writeUpLen, dataLen - writeUpLen);

	pthis->writeIndex += dataLen;//长度别忘了加上 不用担心超总长度 因为有类似 (pthis->writeIndex & (pthis->bufferSize - 1))的操作

    return dataLen;
}

/*
简单地从环形缓冲区获取数据块。
指针指向环形缓冲区结构。
指针指向要读取的数据。
要读取的数据的PARAM-DATALEN长度。
要读取的数据的返回长度。
typedef struct _ringBuffer {
    uint8_t *bufferPtr;
    uint16_t bufferSize;

    uint16_t readIndex;
    uint16_t writeIndex;
} T_RingBuffer;

dataLen 一定要是2的n次方 才能用此方法
*/
uint16_t RingBuf_Get(T_RingBuffer *pthis, uint8_t *pData, uint16_t dataLen)
{
    uint16_t readUpLen;

	//#define RINGBUF_MIN(a, b) (((a)<(b))?(a):(b))
	//还没有读的长度
    dataLen = RINGBUF_MIN(dataLen, (uint16_t) (pthis->writeIndex - pthis->readIndex));

    //get up data
	//到末端方向 还没有读的长度
    readUpLen = RINGBUF_MIN(dataLen, (uint16_t) (pthis->bufferSize - (pthis->readIndex & (pthis->bufferSize - 1))));
	//到末端方向先读
    memcpy(pData, pthis->bufferPtr + (pthis->readIndex & (pthis->bufferSize - 1)), readUpLen);

    //get begin data
	//从开始到还没读的点的处理
    memcpy(pData + readUpLen, pthis->bufferPtr, dataLen - readUpLen);

    pthis->readIndex += dataLen;

    return dataLen;
}

/**
 * @brief Get unused size of ring buffer.
 * @param pthis Pointer to ring buffer structure.
 * @return Unused size of ring buffer.
 */
uint16_t RingBuf_GetUnusedSize(T_RingBuffer *pthis)
{
    return (uint16_t) (pthis->bufferSize - pthis->writeIndex + pthis->readIndex);
}
