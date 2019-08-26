

///////////////////////////////////////////////////
//����ͷ�ļ�
#include "cocoguojia_usart.h"


uint8_t  cocoguojia_usart1ReceiveTable[USART1RXDMAXSIZE];	//USART1���ڽ��ջ�����
uint16_t cocoguojia_usart1ReceiveWritePoint=0;				//USART1���ڽ��ն���ͷָ��
uint16_t cocoguojia_usart1ReceiveReadPoint=0;				//USART1���ڽ��ն���βָ��



uint8_t *usart2TPSendData=0;                                //����2Ҫ�������ݵ�ָ��
uint16_t usart2TSendLong=0;                                 //����2��ǰ�÷��͵��ֽڳ���

uint8_t  g_usart2forSendRunFlag=0;                          //����2�ܷ������ڽ��б�־
uint8_t  cocoguojia_usart2ReceiveTable[USART2RXDMAXSIZE];	//USART2���ڽ��ջ�����
uint16_t cocoguojia_usart2ReceiveWritePoint=0;				//USART2���ڽ��ն���ͷָ��
uint16_t cocoguojia_usart2ReceiveReadPoint=0;				//USART2���ڽ��ն���βָ��



/////////////////////////////////////////////////////////////////////////
//����:            ֧��5�����ڵ�USART��ʼ������ �����Զ��岨����
//��ʽ����:        uint8_t uartNum=���ں� u32 baud=Ҫ�趨�Ĳ�����
void cocoguojia_usartNConfigurationInit(uint8_t usartNum,uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//����USART�⺯���ṹ��  	USART_InitStructure
	USART_InitTypeDef USART_InitStructure;
	
    //����NVIC�⺯���ṹ��  	USART_InitStructure
	NVIC_InitTypeDef NVIC_InitStructure;

	switch(usartNum)
	{
		case 1:
		//----------------------------------------------
		//RCC 
		//ʹ�� APB1�����ϵ�USART1ʱ��
  	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
        
		//----------------------------------------------
		//GPIO
        //USART1��GPIO����
        //USART1 Tx ��GPIO����Ϊ���츴��ģʽ
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
        
        //USART1 Rx ��GPIO����Ϊ���ո���ģʽ
        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOA,&GPIO_InitStructure);

        //------------------------------------------------------------------------------------
        //���ô��ڵĹ�������
        USART_InitStructure.USART_BaudRate=(uint32_t)baud;							//����������
        USART_InitStructure.USART_WordLength=USART_WordLength_8b;						//���ݳ���
        USART_InitStructure.USART_StopBits=USART_StopBits_1;							//ֹͣλ
        USART_InitStructure.USART_Parity=USART_Parity_No;								//У�鷽ʽ
        USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//Ӳ��������
        USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;						//����ģʽ �շ�һ��
        USART_Init(USART1,&USART_InitStructure);										//��ɴ��ڵĳ�ʼ������
        
            
        //------------------------------------------------------------------------------------
        //USART1���жϿ���������
        NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;				//����USART1���ж�Դ
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;		//��ռ���ȼ�����
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//�����ȼ�����
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//ʹ���ж�
        NVIC_Init(&NVIC_InitStructure);
        
        
        USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);				//ʹ�ܴ��ڽ����ж�
        USART_Cmd(USART1,ENABLE);									//ʹ�ܴ���1
		break;

		case 2:
		//----------------------------------------------
		//RCC 
		//ʹ�� APB1�����ϵ�USART2ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  	
		//----------------------------------------------
		//GPIO
		//USART2��Tx�ţ�PA.2��Ϊ�������ģʽ ��תƵ��50MHz
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOA , &GPIO_InitStructure); 
    	//USART2��Rx�ţ�PA.3��Ϊ���������
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    	GPIO_Init(GPIOA , &GPIO_InitStructure);
   
		//----------------------------------------------
		//USART2ʹ�ܶ�Ӧ��NVIC�ж� 
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                                //���ô����ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;             			//ռ��ʽ���ȼ�����
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 				//�����ȼ�����
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                 //�ж�ʹ��
		NVIC_Init(&NVIC_InitStructure);                                                 //�жϳ�ʼ��
        
		//----------------------------------------------
		//USART2 ���ò�����ʼ�� 
    	USART_InitStructure.USART_BaudRate = baud;                                      //������Ϊ
    	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8λ���ݳ���
    	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //1��ֹͣλ
    	USART_InitStructure.USART_Parity = USART_Parity_No ;                            //��У��
    	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //����Ӳ��������
    	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;                   //����+����ʹ��
    	USART_Init(USART2 , &USART_InitStructure);

        //----------------------------------------------
		//USART2 �жϳ�ʼ��
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);                                  //�Ƚ�ֹUSART2�����ж�		
    	USART_ClearFlag(USART2, USART_FLAG_RXNE);                                        //����жϱ�־   
        USART_ITConfig(USART2, USART_IT_TC, ENABLE);						                //ʹ�ܷ�������ж�
        USART_ClearFlag(USART2, USART_FLAG_TC);                                          //�巢����ɱ�־
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                                   //ʹ��USART2�����ж�
        USART_Cmd(USART2, ENABLE);                                                       //ʹ��USART2���� ����������ʹ��USART2	  
		break;
		
		case 3:
		//----------------------------------------------
		//RCC 
		//ʹ�� APB1�����ϵ�USART3ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

		//----------------------------------------------
		//GPIO
		//USART3��Tx�ţ�PB.10��Ϊ�������ģʽ ��תƵ��50MHz
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOB , &GPIO_InitStructure); 
    	//USART3��Rx�ţ�PB.11��Ϊ���������
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    	GPIO_Init(GPIOB , &GPIO_InitStructure);

		//----------------------------------------------
		//USART3 
		//UART3ʹ�ܶ�Ӧ��NVIC�ж� 
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;                                //���ô����ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;             			//ռ��ʽ���ȼ�����
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 				//�����ȼ�����
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                 //�ж�ʹ��
		NVIC_Init(&NVIC_InitStructure);                                                 //�жϳ�ʼ��
        
		//----------------------------------------------
		//USART2 ���ò�����ʼ�� 
    	USART_InitStructure.USART_BaudRate = baud;                                      //������Ϊ
    	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8λ���ݳ���
    	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //1��ֹͣλ
    	USART_InitStructure.USART_Parity = USART_Parity_No ;                            //��У��
    	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //����Ӳ��������
    	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;                   //����+����ʹ��
    	USART_Init(USART3 , &USART_InitStructure);

        //----------------------------------------------
		//USART2 �жϳ�ʼ��
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);                                  //�Ƚ�ֹUSART2�����ж�		
    	USART_ClearFlag(USART3, USART_FLAG_RXNE);                                        //����жϱ�־   
        USART_ITConfig(USART3, USART_IT_TC, ENABLE);						                //ʹ�ܷ�������ж�
        USART_ClearFlag(USART3, USART_FLAG_TC);                                          //�巢����ɱ�־
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);                                   //ʹ��USART2�����ж�
        USART_Cmd(USART3, ENABLE);                                                       //ʹ��USART2���� ����������ʹ��USART2	  
		break;

		case 4:
		//----------------------------------------------
		//RCC 
		//ʹ�� APB1�����ϵ�USART4ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
   
		//----------------------------------------------
		//GPIO
		//USART4��Tx�ţ�PC.10��Ϊ�������ģʽ ��תƵ��50MHz
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOC , &GPIO_InitStructure); 
    	//USART4��Rx�ţ�PC.11��Ϊ���������
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    	GPIO_Init(GPIOC , &GPIO_InitStructure);

		//----------------------------------------------
		//USART4 
		//UART4ʹ�ܶ�Ӧ��NVIC�ж� 
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;                                //���ô����ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;             			//ռ��ʽ���ȼ�����
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 				//�����ȼ�����
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                 //�ж�ʹ��
		NVIC_Init(&NVIC_InitStructure);                                                 //�жϳ�ʼ��
        
		//----------------------------------------------
		//USART4 ���ò�����ʼ�� 
    	USART_InitStructure.USART_BaudRate = baud;                                      //������Ϊ
    	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8λ���ݳ���
    	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //1��ֹͣλ
    	USART_InitStructure.USART_Parity = USART_Parity_No ;                            //��У��
    	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //����Ӳ��������
    	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;                   //����+����ʹ��
    	USART_Init(UART4 , &USART_InitStructure);

        //----------------------------------------------
		//USART4 �жϳ�ʼ��
		USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);                                  //�Ƚ�ֹUSART2�����ж�		
    	USART_ClearFlag(UART4, USART_FLAG_RXNE);                                        //����жϱ�־   
        USART_ITConfig(UART4, USART_IT_TC, ENABLE);						                //ʹ�ܷ�������ж�
        USART_ClearFlag(UART4, USART_FLAG_TC);                                          //�巢����ɱ�־
        USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);                                   //ʹ��USART2�����ж�
        USART_Cmd(UART4, ENABLE);                                                       //ʹ��USART2���� ����������ʹ��USART2	  
		break;
        
        case 5:
        //----------------------------------------------
        //UART5
        
		//----------------------------------------------
		//RCC 
		//ʹ�� APB1�����ϵ�USART4ʱ��
  		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);                           //ʹ��UART5 ʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);      //ʹ��UART5 �ܽ�ʱ��
        
		//----------------------------------------------
		//UART5��GPIO����
		//USART5��Tx�ţ�PC.12��Ϊ�������ģʽ ��תƵ��50MHz
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOC , &GPIO_InitStructure); 
    	//USART5��Rx�ţ�PD.2��Ϊ���������
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    	GPIO_Init(GPIOD , &GPIO_InitStructure);

		//----------------------------------------------
		//UART5ʹ�ܶ�Ӧ��NVIC�ж� 
		NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;                                //���ô����ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                       //ռ��ʽ���ȼ�����
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                              //�����ȼ�����
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                 //�ж�ʹ��
		NVIC_Init(&NVIC_InitStructure);                                                 //�жϳ�ʼ��
        
		//----------------------------------------------
		//UART5 ���ò�����ʼ�� 
    	USART_InitStructure.USART_BaudRate = baud;                                      //������Ϊ
    	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //8λ���ݳ���
    	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //1��ֹͣλ
    	USART_InitStructure.USART_Parity = USART_Parity_No ;                            //��У��
    	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //����Ӳ��������
    	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;                   //����+����ʹ��
    	USART_Init(UART5 , &USART_InitStructure);

        //----------------------------------------------
		//UART5 �жϳ�ʼ��
		USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);                                  //�Ƚ�ֹUART5�����ж�		
    	USART_ClearFlag(UART5, USART_FLAG_RXNE);                                        //����жϱ�־   
        USART_ITConfig(UART5, USART_IT_TC, ENABLE);						                //ʹ�ܷ�������ж�
        USART_ClearFlag(UART5, USART_FLAG_TC);                                          //�巢����ɱ�־
        USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);                                   //ʹ��UART5�����ж�
        USART_Cmd(UART5, ENABLE);                                                       //ʹ��UART5���� û��DMA���� ����������ʹ��UART5			
		break;
	
		default:
		break;
	}	
}


//////////////////////////////////////////////////////////////////////////////
//����:         ����1���ֽ�
//��ʽ����:     uint8_t uartNum=���ں�  uint8_t OneByte=Ҫ���͵��ֽ�
//��ע:         1~4���� DMA����   5����TXE�жϷ���
//���䣺        ���������˾ֲ�����sendDataForOneByte[1]������Ϊ��һ���ֽڣ���ʹ�����л���Ҳ�������̫��Ķ�ջ����
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
             vTaskDelay(1);//����ģʽ��ʱһ��ʱ�ӽ��� 
        }              
        sendDataForOneByte[0]=chr;
              
        //�����ȸ�ֵ �� ���������¶��� 
        //һ��Ҫ����������"while"֮�� 
        //����ǰ���͵����ݽ��ᱻ�ƻ�
        usart2TSendLong=1;
        usart2TPSendData=&sendDataForOneByte[0];
        
        if(0!=usart2TSendLong)                                       //������ͳ��Ȳ�Ϊ0
        {
            usart2TSendLong--;                                       //���ͳ��ȼ�1
            g_usart2forSendRunFlag=1;                                  //1=UART5������Դ��ռ��                                                                
            USART_ClearFlag(USART2,USART_FLAG_TC);                    //�����TC��־
            USART2->DR=(*usart2TPSendData++);                         //��DRдһ������,������++
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
			vTaskDelay(1);//����ģʽ��ʱһ��ʱ�ӽ��� 
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
                            
            //�����ȸ�ֵ �� ���������¶��� 
            //һ��Ҫ����������"while"֮�� 
            //����ǰ���͵����ݽ��ᱻ�ƻ�
            usart2TSendLong=i;  
            usart2TPSendData=&coco_tempStr2[0];
            
            if(0!=usart2TSendLong)                                       //������ͳ��Ȳ�Ϊ0
            {
                usart2TSendLong--;                                       //���ͳ��ȼ�1
                g_usart2forSendRunFlag=1;                                  //1=UART5������Դ��ռ��                                                                
                USART_ClearFlag(USART2,USART_FLAG_TC);                     //�����TC��־
                USART2->DR=(*usart2TPSendData++);                        //��DRдһ������,������++
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
		while(1==g_usart2forSendRunFlag)//��������ڷ����� ��ԭ�صȴ�   
		{
			 vTaskDelay(1);//����ģʽ��ʱһ��ʱ�ӽ��� 
		}
			   
		//��Ҫ���������ȴ��뻺����5��            
		for(i=0;i<num;i++)
		{
			coco_tempTable2[i]=*ucBuffer;
			ucBuffer++;
		}
			  
		//�����ȸ�ֵ �� ���������¶��� 
		//һ��Ҫ����������"while"֮�� 
		//����ǰ���͵����ݽ��ᱻ�ƻ�
		usart2TSendLong=num;
		usart2TPSendData=&coco_tempTable2[0];
		
		if(0!=usart2TSendLong)                                       //������ͳ��Ȳ�Ϊ0
		{
			usart2TSendLong--;                                       //���ͳ��ȼ�1
			g_usart2forSendRunFlag=1;                                  //1=UART2������Դ��ռ��                                                                
			USART_ClearFlag(USART2,USART_FLAG_TC);                      //�����TC��־
			USART2->DR=(*usart2TPSendData++);                         //��DRдһ������,������++
		}               
		break;

		default:
		break;
	}                 
}

//------------------------------------------------------------------------------------------------------
//���¶���c�⺯��printf������ ��MDK�Ĺ���ѡ��"Use MicroLIB"��ѡ�� �����߶��Ż�ʹ�������,ռ��Դ����
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
//�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

//����1���жϺ���
void USART1_IRQHandler(void)
{
	uint8_t dat;
	uint16_t rearTemp=0;
    uint32_t ulReturn;
    
    //�����ٽ�Σ��ٽ�ο���Ƕ��
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
    if(RESET!=USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		dat=USART_ReceiveData(USART1);
		
		cocoguojia_usart1ReceiveTable[cocoguojia_usart1ReceiveWritePoint]=dat;         	//���յ�������д��βָ��     
        rearTemp=((cocoguojia_usart1ReceiveWritePoint)+1)%USART1RXDMAXSIZE ;  			//βָ���һ�� ȡ������ �ȴ浽�ݴ�Ĵ�����
        if(cocoguojia_usart1ReceiveReadPoint!=rearTemp)                   				//����Ӳ��� ͷָ�벻����βָ��
		{          
            cocoguojia_usart1ReceiveWritePoint=rearTemp;                      			//����βָ��ȡ����ֵ
		}
        else
        {
            ;                                               							//�������򲻴�����
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);                                 //����жϱ�־        
	}
	   
    //��һ�� Ϊ�����˳�ǰ �����жϱ�־ �ж��ڴ���ʱ�� ��������ʱ ����ǰ��˵���������
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {         
          USART_ClearITPendingBit(USART1, USART_IT_RXNE);//����жϱ�־     
	}
 
	
    //��ֹ�н�������жϵ�Bug
    if(SET==USART_GetFlagStatus(USART1,USART_FLAG_ORE))
    {
        USART_ClearFlag(USART1,USART_FLAG_ORE);
        USART_ReceiveData(USART1);
    }
	
    // �˳��ٽ�� 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}


//////////////////////////////////////////////////////////////////////
//����2�жϴ�����
//ע����STM�д���2 
void USART2_IRQHandler(void)
{
	uint8_t RxData;
    uint16_t rearTemp;
    uint32_t ulReturn;
 
    //�����ٽ�Σ��ٽ�ο���Ƕ��
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
    
    //������յ������� 
    //��������������βָ��++�ķ�ʽ
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{    	                
        RxData = USART_ReceiveData(USART2);
		RxData=RxData;
        
        cocoguojia_usart2ReceiveTable[cocoguojia_usart2ReceiveWritePoint]=RxData;        //���յ�������д��βָ��     
        rearTemp=((cocoguojia_usart2ReceiveWritePoint)+1)%USART2RXDMAXSIZE ;  			//βָ���һ�� ȡ������ �ȴ浽�ݴ�Ĵ�����
        if(cocoguojia_usart2ReceiveReadPoint!=rearTemp)                   				//����Ӳ��� ͷָ�벻����βָ��
		{          
            cocoguojia_usart2ReceiveWritePoint=rearTemp;                      			//����βָ��ȡ����ֵ
		}
        else
        {
            ;                                               							//�������򲻴�����
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);//����жϱ�־         
	}
    
    //��һ�� Ϊ�����˳�ǰ �����жϱ�־ �ж��ڴ���ʱ�� ��������ʱ ����ǰ��˵���������
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {         
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);//����жϱ�־     
	}
    
    //��ֹ�н�������жϵ�Bug
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
    {
        USART_ClearFlag(USART2,USART_FLAG_ORE);
        USART_ReceiveData(USART2);
    }

	//�����͵�����		
    if(USART_GetITStatus(USART2, USART_IT_TC)==SET)          //���������ɱ�־��λ
	{	
        if(0==usart2TSendLong)                            //������ͳ���Ϊ0��
        {
            g_usart2forSendRunFlag=0;                       //���ͽ����б�־����
            USART_ClearITPendingBit(USART2, USART_IT_TC);   //����TC�жϱ�־
        }
        else                                                //������ͳ��Ȳ�Ϊ0
        {             
            USART2->DR=*(usart2TPSendData++);              //����ָ�븳ֵDR�� �Լ�һ
            usart2TSendLong--;                            //���ͳ��� �Լ�һ
        }
	}
    
    // �˳��ٽ�� 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}











