

///////////////////////////////////////////////////
//自身头文件
#include "cocoguojia_usart.h"


uint8_t  cocoguojia_usart1ReceiveTable[USART1RXDMAXSIZE];	//USART1串口接收缓冲区
uint16_t cocoguojia_usart1ReceiveWritePoint=0;				//USART1串口接收队列头指针
uint16_t cocoguojia_usart1ReceiveReadPoint=0;				//USART1串口接收队列尾指针



uint8_t *usart2TPSendData=0;                                //串口2要发送数据的指针
uint16_t usart2TSendLong=0;                                 //串口2当前该发送的字节长度

uint8_t  g_usart2forSendRunFlag=0;                          //串口2总发送正在进行标志
uint8_t  cocoguojia_usart2ReceiveTable[USART2RXDMAXSIZE];	//USART2串口接收缓冲区
uint16_t cocoguojia_usart2ReceiveWritePoint=0;				//USART2串口接收队列头指针
uint16_t cocoguojia_usart2ReceiveReadPoint=0;				//USART2串口接收队列尾指针



/////////////////////////////////////////////////////////////////////////
//功能:            支持5个串口的USART初始化定义 可以自定义波特率
//形式参数:        uint8_t uartNum=串口号 u32 baud=要设定的波特率
void cocoguojia_usartNConfigurationInit(uint8_t usartNum,uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//定义USART库函数结构体  	USART_InitStructure
	USART_InitTypeDef USART_InitStructure;
	
    //定义NVIC库函数结构体  	USART_InitStructure
	NVIC_InitTypeDef NVIC_InitStructure;

	switch(usartNum)
	{
		case 1:
		//----------------------------------------------
		//RCC 
		//使能 APB1总线上的USART1时钟
  	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
        
		//----------------------------------------------
		//GPIO
        //USART1的GPIO配置
        //USART1 Tx 的GPIO配置为推挽复用模式
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
        
        //USART1 Rx 的GPIO配置为浮空复用模式
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOA,&GPIO_InitStructure);

        //------------------------------------------------------------------------------------
        //配置串口的工作参数
        USART_InitStructure.USART_BaudRate=(uint32_t)baud;							//波特率设置
        USART_InitStructure.USART_WordLength=USART_WordLength_8b;						//数据长度
        USART_InitStructure.USART_StopBits=USART_StopBits_1;							//停止位
        USART_InitStructure.USART_Parity=USART_Parity_No;								//校验方式
        USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//硬件流控制
        USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;						//工作模式 收发一起
        USART_Init(USART1,&USART_InitStructure);										//完成串口的初始化设置
        
            
        //------------------------------------------------------------------------------------
        //USART1的中断控制器配置
        NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;				//配置USART1的中断源
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;		//抢占优先级设置
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//子优先级设置
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//使能中断
        NVIC_Init(&NVIC_InitStructure);
        
        
        USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);				//使能串口接收中断
        USART_Cmd(USART1,ENABLE);									//使能串口1
		break;

		case 2:
		//----------------------------------------------
		//RCC 
		//使能 APB1总线上的USART2时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  	
		//----------------------------------------------
		//GPIO
		//USART2的Tx脚（PA.2）为推挽输出模式 翻转频率50MHz
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOA , &GPIO_InitStructure); 
    	//USART2的Rx脚（PA.3）为浮空输入脚
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    	GPIO_Init(GPIOA , &GPIO_InitStructure);
   
		//----------------------------------------------
		//USART2使能对应的NVIC中断 
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                                //配置串口中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;             			//占先式优先级设置
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 				//副优先级设置
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                 //中断使能
		NVIC_Init(&NVIC_InitStructure);                                                 //中断初始化
        
		//----------------------------------------------
		//USART2 设置参数初始化 
    	USART_InitStructure.USART_BaudRate = baud;                                      //波特率为
    	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8位数据长度
    	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //1个停止位
    	USART_InitStructure.USART_Parity = USART_Parity_No ;                            //无校验
    	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //禁用硬件流控制
    	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;                   //发送+接收使能
    	USART_Init(USART2 , &USART_InitStructure);

        //----------------------------------------------
		//USART2 中断初始化
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);                                  //先禁止USART2接收中断		
    	USART_ClearFlag(USART2, USART_FLAG_RXNE);                                        //清除中断标志   
        USART_ITConfig(USART2, USART_IT_TC, ENABLE);						                //使能发送完成中断
        USART_ClearFlag(USART2, USART_FLAG_TC);                                          //清发送完成标志
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                                   //使能USART2接收中断
        USART_Cmd(USART2, ENABLE);                                                       //使能USART2外设 所以在这里使能USART2	  
		break;
		
		case 3:
		//----------------------------------------------
		//RCC 
		//使能 APB1总线上的USART3时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

		//----------------------------------------------
		//GPIO
		//USART3的Tx脚（PB.10）为推挽输出模式 翻转频率50MHz
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOB , &GPIO_InitStructure); 
    	//USART3的Rx脚（PB.11）为浮空输入脚
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    	GPIO_Init(GPIOB , &GPIO_InitStructure);

		//----------------------------------------------
		//USART3 
		//UART3使能对应的NVIC中断 
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;                                //配置串口中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;             			//占先式优先级设置
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 				//副优先级设置
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                 //中断使能
		NVIC_Init(&NVIC_InitStructure);                                                 //中断初始化
        
		//----------------------------------------------
		//USART2 设置参数初始化 
    	USART_InitStructure.USART_BaudRate = baud;                                      //波特率为
    	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8位数据长度
    	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //1个停止位
    	USART_InitStructure.USART_Parity = USART_Parity_No ;                            //无校验
    	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //禁用硬件流控制
    	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;                   //发送+接收使能
    	USART_Init(USART3 , &USART_InitStructure);

        //----------------------------------------------
		//USART2 中断初始化
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);                                  //先禁止USART2接收中断		
    	USART_ClearFlag(USART3, USART_FLAG_RXNE);                                        //清除中断标志   
        USART_ITConfig(USART3, USART_IT_TC, ENABLE);						                //使能发送完成中断
        USART_ClearFlag(USART3, USART_FLAG_TC);                                          //清发送完成标志
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                                   //使能USART2接收中断
        USART_Cmd(USART3, ENABLE);                                                       //使能USART2外设 所以在这里使能USART2	  
		break;

		case 4:
		//----------------------------------------------
		//RCC 
		//使能 APB1总线上的USART4时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
   
		//----------------------------------------------
		//GPIO
		//USART4的Tx脚（PC.10）为推挽输出模式 翻转频率50MHz
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOC , &GPIO_InitStructure); 
    	//USART4的Rx脚（PC.11）为浮空输入脚
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    	GPIO_Init(GPIOC , &GPIO_InitStructure);

		//----------------------------------------------
		//USART4 
		//UART4使能对应的NVIC中断 
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;                                //配置串口中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;             			//占先式优先级设置
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 				//副优先级设置
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                 //中断使能
		NVIC_Init(&NVIC_InitStructure);                                                 //中断初始化
        
		//----------------------------------------------
		//USART4 设置参数初始化 
    	USART_InitStructure.USART_BaudRate = baud;                                      //波特率为
    	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8位数据长度
    	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //1个停止位
    	USART_InitStructure.USART_Parity = USART_Parity_No ;                            //无校验
    	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //禁用硬件流控制
    	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;                   //发送+接收使能
    	USART_Init(UART4 , &USART_InitStructure);

        //----------------------------------------------
		//USART4 中断初始化
		USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);                                  //先禁止USART2接收中断		
    	USART_ClearFlag(UART4, USART_FLAG_RXNE);                                        //清除中断标志   
        USART_ITConfig(UART4, USART_IT_TC, ENABLE);						                //使能发送完成中断
        USART_ClearFlag(UART4, USART_FLAG_TC);                                          //清发送完成标志
        USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);                                   //使能USART2接收中断
        USART_Cmd(UART4, ENABLE);                                                       //使能USART2外设 所以在这里使能USART2	  
		break;
        
        case 5:
        //----------------------------------------------
        //UART5
        
		//----------------------------------------------
		//RCC 
		//使能 APB1总线上的USART4时钟
  		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);                           //使能UART5 时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);      //使能UART5 管脚时钟
        
		//----------------------------------------------
		//UART5的GPIO设置
		//USART5的Tx脚（PC.12）为推挽输出模式 翻转频率50MHz
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOC , &GPIO_InitStructure); 
    	//USART5的Rx脚（PD.2）为浮空输入脚
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    	GPIO_Init(GPIOD , &GPIO_InitStructure);

		//----------------------------------------------
		//UART5使能对应的NVIC中断 
		NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;                                //配置串口中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                       //占先式优先级设置
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                              //副优先级设置
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                 //中断使能
		NVIC_Init(&NVIC_InitStructure);                                                 //中断初始化
        
		//----------------------------------------------
		//UART5 设置参数初始化 
    	USART_InitStructure.USART_BaudRate = baud;                                      //波特率为
    	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8位数据长度
    	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //1个停止位
    	USART_InitStructure.USART_Parity = USART_Parity_No ;                            //无校验
    	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //禁用硬件流控制
    	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;                   //发送+接收使能
    	USART_Init(UART5 , &USART_InitStructure);

        //----------------------------------------------
		//UART5 中断初始化
		USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);                                  //先禁止UART5接收中断		
    	USART_ClearFlag(UART5, USART_FLAG_RXNE);                                        //清除中断标志   
        USART_ITConfig(UART5, USART_IT_TC, ENABLE);						                //使能发送完成中断
        USART_ClearFlag(UART5, USART_FLAG_TC);                                          //清发送完成标志
        USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);                                   //使能UART5接收中断
        USART_Cmd(UART5, ENABLE);                                                       //使能UART5外设 没有DMA功能 所以在这里使能UART5			
		break;
	
		default:
		break;
	}	
}


//////////////////////////////////////////////////////////////////////////////
//功能:         发送1个字节
//形式参数:     uint8_t uartNum=串口号  uint8_t OneByte=要发送的字节
//备注:         1~4串口 DMA发送   5串口TXE中断发送
//补充：        在这里用了局部变量sendDataForOneByte[1]，是因为就一个字节，即使任务切换，也不会造成太大的堆栈负担
void cocoguojia_usartNPutChar(uint8_t uartNum,uint8_t chr)
{	
    uint8_t sendDataForOneByte[1];
	switch(uartNum)
	{
        case 1:
        USART_SendData(USART1, chr);
        while(RESET==USART_GetFlagStatus(USART1,USART_FLAG_TC))
        {
            ;
        }
        break;
        
        case 2:
        while(1==g_usart2forSendRunFlag)   
        {
             vTaskDelay(1);//周期模式延时一个时钟节拍 
        }              
        sendDataForOneByte[0]=chr;
              
        //给长度赋值 和 缓冲区重新定义 
        //一定要在上文两个"while"之后 
        //否则当前发送的数据将会被破坏
        usart2TSendLong=1;
        usart2TPSendData=&sendDataForOneByte[0];
        
        if(0!=usart2TSendLong)                                       //如果发送长度不为0
        {
            usart2TSendLong--;                                       //发送长度减1
            g_usart2forSendRunFlag=1;                                  //1=UART5发送资源被占用                                                                
            USART_ClearFlag(USART2,USART_FLAG_TC);                    //先清除TC标志
            USART2->DR=(*usart2TPSendData++);                         //向DR写一个数据,别忘了++
        }               
        break;

      
        default:
        break;
	}
}




#define COCO_MAXSTRTEMP2 128
uint8_t coco_tempStr2[COCO_MAXSTRTEMP2];
void cocoguojia_usartNPutStr(uint16_t uartNum,const uint8_t *str)
{   
    const uint8_t * ucBuffer=str;
    uint16_t i;
     
	switch(uartNum)
	{
        case 1:
        do
        {
            USART_SendData(USART1, *ucBuffer);
            while(RESET==USART_GetFlagStatus(USART1,USART_FLAG_TC))
            {
                ;
            }
            i++;
        }
        while(0!=(*(ucBuffer+i)));	
        break;
        
		case 2:
		while(1==g_usart2forSendRunFlag)   
		{
			vTaskDelay(1);//周期模式延时一个时钟节拍 
		}
		            
		i=0;
		while(0!=*ucBuffer)
		{
			coco_tempStr2[i]=*ucBuffer;
			i++;
			ucBuffer++;
			if(COCO_MAXSTRTEMP2==i)
			{
				break;
			}
		}
        
        if(COCO_MAXSTRTEMP2>i)
        {
            coco_tempStr2[i]=0; 
                            
            //给长度赋值 和 缓冲区重新定义 
            //一定要在上文两个"while"之后 
            //否则当前发送的数据将会被破坏
            usart2TSendLong=i;  
            usart2TPSendData=&coco_tempStr2[0];
            
            if(0!=usart2TSendLong)                                       //如果发送长度不为0
            {
                usart2TSendLong--;                                       //发送长度减1
                g_usart2forSendRunFlag=1;                                  //1=UART5发送资源被占用                                                                
                USART_ClearFlag(USART2,USART_FLAG_TC);                     //先清除TC标志
                USART2->DR=(*usart2TPSendData++);                        //向DR写一个数据,别忘了++
            }  
        }        
		break;
 
	  
		default:
		break;
	}                 
}

#define COCO_MAXDMATEMP2 128
uint8_t coco_tempTable2[COCO_MAXDMATEMP2];
void cocoguojia_usartNPutTableForNum(uint8_t uartNum,uint8_t *sp, uint16_t num)
{   
	uint8_t * ucBuffer=sp;
	uint16_t i;
		 
	switch(uartNum)
	{
        case 1:
        for(i=0;i<num;i++)
		{    
            USART_SendData(USART1, *ucBuffer);
            while(RESET==USART_GetFlagStatus(USART1,USART_FLAG_TC))
            {
                ;
            }
            ucBuffer++;
        }   
        break;
        
		case 2:
		while(1==g_usart2forSendRunFlag)//如果数据在发送中 则原地等待   
		{
			 vTaskDelay(1);//周期模式延时一个时钟节拍 
		}
			   
		//把要发的数据先存入缓冲区5里            
		for(i=0;i<num;i++)
		{
			coco_tempTable2[i]=*ucBuffer;
			ucBuffer++;
		}
			  
		//给长度赋值 和 缓冲区重新定义 
		//一定要在上文两个"while"之后 
		//否则当前发送的数据将会被破坏
		usart2TSendLong=num;
		usart2TPSendData=&coco_tempTable2[0];
		
		if(0!=usart2TSendLong)                                       //如果发送长度不为0
		{
			usart2TSendLong--;                                       //发送长度减1
			g_usart2forSendRunFlag=1;                                  //1=UART2发送资源被占用                                                                
			USART_ClearFlag(USART2,USART_FLAG_TC);                      //先清除TC标志
			USART2->DR=(*usart2TPSendData++);                         //向DR写一个数据,别忘了++
		}               
		break;

		default:
		break;
	}                 
}

//------------------------------------------------------------------------------------------------------
//重新定向c库函数printf到串口 把MDK的工程选项"Use MicroLIB"勾选上 这样高度优化使代码更少,占资源更少
int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(uint8_t)ch);
	while(RESET==USART_GetFlagStatus(USART1,USART_FLAG_TC))
	{
		;
	}
	return(ch);
}

//------------------------------------------------------------------------------------------------------
//重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

//串口1的中断函数
void USART1_IRQHandler(void)
{
	uint8_t dat;
	uint16_t rearTemp=0;
    uint32_t ulReturn;
    
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
    if(RESET!=USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		dat=USART_ReceiveData(USART1);
		
		cocoguojia_usart1ReceiveTable[cocoguojia_usart1ReceiveWritePoint]=dat;         	//接收到的数据写入尾指针     
        rearTemp=((cocoguojia_usart1ReceiveWritePoint)+1)%USART1RXDMAXSIZE ;  			//尾指针加一后 取余运算 先存到暂存寄存器里
        if(cocoguojia_usart1ReceiveReadPoint!=rearTemp)                   				//如果队不满 头指针不等于尾指针
		{          
            cocoguojia_usart1ReceiveWritePoint=rearTemp;                      			//赋予尾指针取余后的值
		}
        else
        {
            ;                                               							//队列满则不处理了
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);                                 //清除中断标志        
	}
	   
    //这一步 为了在退出前 清零中断标志 中断内处理时候 可能有延时 导致前面说的那种情况
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {         
          USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除中断标志     
	}
 
	
    //防止有接收溢出中断的Bug
    if(SET==USART_GetFlagStatus(USART1,USART_FLAG_ORE))
    {
        USART_ClearFlag(USART1,USART_FLAG_ORE);
        USART_ReceiveData(USART1);
    }
	
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}


//////////////////////////////////////////////////////////////////////
//串口2中断处理函数
//注意在STM中串口2 
void USART2_IRQHandler(void)
{
	uint8_t RxData;
    uint16_t rearTemp;
    uint32_t ulReturn;
 
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
    
    //处理接收到的数据 
    //这里采用先入队再尾指针++的方式
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{    	                
        RxData = USART_ReceiveData(USART2);
		RxData=RxData;
        
        cocoguojia_usart2ReceiveTable[cocoguojia_usart2ReceiveWritePoint]=RxData;        //接收到的数据写入尾指针     
        rearTemp=((cocoguojia_usart2ReceiveWritePoint)+1)%USART2RXDMAXSIZE ;  			//尾指针加一后 取余运算 先存到暂存寄存器里
        if(cocoguojia_usart2ReceiveReadPoint!=rearTemp)                   				//如果队不满 头指针不等于尾指针
		{          
            cocoguojia_usart2ReceiveWritePoint=rearTemp;                      			//赋予尾指针取余后的值
		}
        else
        {
            ;                                               							//队列满则不处理了
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);//清除中断标志         
	}
    
    //这一步 为了在退出前 清零中断标志 中断内处理时候 可能有延时 导致前面说的那种情况
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {         
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);//清除中断标志     
	}
    
    //防止有接收溢出中断的Bug
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
    {
        USART_ClearFlag(USART2,USART_FLAG_ORE);
        USART_ReceiveData(USART2);
    }

	//处理发送的数据		
    if(USART_GetITStatus(USART2, USART_IT_TC)==SET)          //如果发送完成标志置位
	{	
        if(0==usart2TSendLong)                            //如果发送长度为0了
        {
            g_usart2forSendRunFlag=0;                       //发送进行中标志清零
            USART_ClearITPendingBit(USART2, USART_IT_TC);   //清零TC中断标志
        }
        else                                                //如果发送长度不为0
        {             
            USART2->DR=*(usart2TPSendData++);              //发送指针赋值DR后 自加一
            usart2TSendLong--;                            //发送长度 自减一
        }
	}
    
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}











