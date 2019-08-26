#include "bsp_usart3.h"


T_Usart3RingBufDoStruct    Usart3RingRecBufDoStruct;
T_PsdkProtFrameAllStruct   PsdkProtFrameAllStruct;


uint8_t g_keyTable[]={0x97,0xE4,0x61,0x87,0x3F,0xF2,0xF2,0x45,0x07,0xFE,0xFC,0xCC,0x1D,0xBE,0xEF,0x19};


uint8_t photoModelSetTempTable[4]={0,0,0,0};

//--------------------------------------------------------
//乒乓缓存 用来存储gps rtk的信息
uint8_t g_GpsRtkProtectFlag=0;//1=数组1处于保护 2=数组2处于保护 0=平时
uint8_t g_gpsRtkPointFlag=1;//1=当前最新的是数组1  2=当前最新的是数组2  为了数据的透明性
uint8_t g_gpsRtkTempTable1[200]={0,0,0,0};
uint8_t g_gpsRtkTempTable2[200]={0,0,0,0};
uint8_t g_gpsRtkTable1Len=0;
uint8_t g_gpsRtkTable2Len=0;

//---------------------------------------------------------------------------------------------------------
//功能:接收到完整的数据后 开始具体解析并处理数据,包括做出正确的回复
//形式参数:PSDK 数据格式结构体指针
static uint8_t Bsp_Usart3ParseGoDoWhit(T_PsdkProtFrameAllStruct *FrameAllStruct);
//-----功能:根据接到的16字节随机数据和用户信息USER_APP_KEY 进行MD5算法得出秘钥写进指针uint8_t *pKey里
//-----形式参数: uint8_t *pKey=存放MD5计算结果秘钥的指针 结果16字节长度 
//-----          uint8_t *randomData=随机数 16字节


static void PsdkPayloadStateCmd_IdentityVerify(uint8_t *outdatapr,uint8_t *inProtData);

//-----功能:按照格式要求发送数据给 大疆环形接口板
//-----形式参数:PSDK 数据格式结构体指针
       static uint8_t Bsp_Usart3SendOfPsdkFrame(T_PsdkProtFrameAllStruct *FrameAllStruct);

   
    

uint8_t psdkProtFrameHeaderTable    [PSDK_PROTHEADEBEFORERLEN];
uint8_t psdkProtFrameCrc16Table     [PSDK_PROTHEADECRC16RLEN];
uint8_t psdkProtFrameDataTable      [PSDK_PROTDATAMAXLEN];
uint8_t psdkProtFrameCrc32Table     [PSDK_PROTHEADECRC32RLEN];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//外部函数
//-------------------------------------------------------------------------------------------------
//USART3初始化
void Bsp_usart3Init(void)    
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//打开串口3的GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//打开串口3的外设时钟
   
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

	//------------------------------------------------------------------------------------
	//配置串口的工作参数
	USART_InitStructure.USART_BaudRate=(uint32_t)115200;							//波特率设置
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;						//数据长度
	USART_InitStructure.USART_StopBits=USART_StopBits_1;							//停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;								//校验方式
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//硬件流控制
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;						//工作模式 收发一起
	USART_Init(USART3,&USART_InitStructure);										//完成串口的初始化设置
	
	//------------------------------------------------------------------------------------
	//USART3的中断控制器配置
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;				//配置USART3的中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;		//抢占优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//子优先级设置
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);				//使能串口接收中断
    //USART_ITConfig(USART3, USART_IT_TXE, ENABLE);   		    //使能发送完成中断
	USART_Cmd(USART3,ENABLE);									//使能串口3

    //------------------------------------------------------------------------------------------
    //环形接收区初始化
    Usart3RingRecBufDoStruct.rawReadIndex=0;
    Usart3RingRecBufDoStruct.rawWriteIndex=0;
    
    
    Usart3RingRecBufDoStruct.parseIndex=0; 
    memset((Usart3RingRecBufDoStruct.rawBuf), 0, sizeof((Usart3RingRecBufDoStruct.rawBuf)));        //清空分析接收处理缓冲区
    memset((Usart3RingRecBufDoStruct.parseBuf), 0, sizeof(Usart3RingRecBufDoStruct.parseBuf));      //清空串口接收处理缓冲区
}	

//-------------------------------------------------------------------------------------------------
//串口2接收数据并处理并正确回复
uint8_t Bsp_usart3ReadChar(T_Usart3RingBufDoStruct *ringBufStruct,T_PsdkProtFrameAllStruct *FrameAllStruct)
{

    uint8_t endFlag=0;
    uint8_t dat;
    uint16_t i;
    uint16_t bspCrc16TempforPrintf;
    uint32_t bspCrc32TempforPrintf;

    
   
   while((ringBufStruct->rawReadIndex!=ringBufStruct->rawWriteIndex)&&(0==endFlag))
   {
        while((ringBufStruct->rawReadIndex!=ringBufStruct->rawWriteIndex)&&(0==endFlag))
        {
            dat=ringBufStruct->rawBuf[ringBufStruct->rawReadIndex];
            ringBufStruct->rawReadIndex++;
            if(ringBufStruct->rawReadIndex>=USART3_RECRAW_RINGBUGLENMAX)
            {
               ringBufStruct->rawReadIndex=0;
            }
            
            //先检测字头 一直检测 直到有字头
            if (0==ringBufStruct->parseIndex)
            {
                if (0xaa==dat)//0XAA 是字头
                {  
                    PRINTF_BEFORE1 printf("$$SOF 出现\r\n");
                    ringBufStruct->parseBuf[ringBufStruct->parseIndex]=dat;
                    ringBufStruct->parseIndex++;
                    FrameAllStruct->sof=dat;
                    return 5;
                }
                else
                {
                    ;
                }      
            }
            else
            {
                 ringBufStruct->parseBuf[ringBufStruct->parseIndex]=dat;
                 ringBufStruct->parseIndex++;
             
                 if(3==ringBufStruct->parseIndex)
                 {                     
                    FrameAllStruct->lenVer_len  =  ringBufStruct->parseBuf[ringBufStruct->parseIndex-1];           
                    FrameAllStruct->lenVer_Ver  =  FrameAllStruct->lenVer_len>>1;
                    FrameAllStruct->lenVer_len &=  0x0001;
                    FrameAllStruct->lenVer_len <<= 8;
                    FrameAllStruct->lenVer_len += ringBufStruct->parseBuf[ringBufStruct->parseIndex-2]; 

                    if(PSDK_PROTFRAMEMINLEN== FrameAllStruct->lenVer_len)
                    {
                       PRINTF_BEFORE1 printf("$$len=%d 没有帧数据\r\n", FrameAllStruct->lenVer_len);
                    }
                    else if((PSDK_PROTFRAMEMINLEN> FrameAllStruct->lenVer_len)||(PSDK_PROTFRAMEMAXLEN< FrameAllStruct->lenVer_len)) 
                    {
                       ringBufStruct->parseIndex=0;  
                       PRINTF_BEFORE1 printf("$$Wrong! len=%d 不识别\r\n",FrameAllStruct->lenVer_len); 
                       return 1;
                    }
                    else 
                    {
                        PRINTF_BEFORE1 printf("$$len=%d 有帧数据=%d\r\n",FrameAllStruct->lenVer_len,FrameAllStruct->lenVer_len-PSDK_PROTFRAMEMINLEN);                
                    }


                    if( FrameAllStruct->lenVer_Ver != 0)
                    {
                        ringBufStruct->parseIndex=0; 
                        PRINTF_BEFORE1 printf("$$ver is wrong=%d 版本不识别\r\n",  FrameAllStruct->lenVer_Ver );                        
                        return 6;
                    }
                    else
                    {
                        PRINTF_BEFORE1 printf("$$ver=0 版本号为0\r\n");    
                    }
                 }

                 if(4==ringBufStruct->parseIndex)//ACK这里有bug 改完了 主要问题是ackType_Type是第5位 即高4位的第2位 
                 {                     
                    FrameAllStruct->ackType_Ack  =  ringBufStruct->parseBuf[ringBufStruct->parseIndex-1]&0x1f; 

                    FrameAllStruct->ackType_Type  = (ringBufStruct->parseBuf[ringBufStruct->parseIndex-1]&0x10)>>5;

                    if(0==FrameAllStruct->ackType_Ack)
                    {
                        PRINTF_BEFORE1 printf("$$ackType_Ack=0  不需要ACK\r\n");
                    }
                    else if(1==FrameAllStruct->ackType_Ack)
                    {
                        PRINTF_BEFORE1 printf("$$ackType_Ack=1  需要ACK\r\n");
                    }
                    else
                    {
                        PRINTF_BEFORE1 printf("$$Wrong! ackType_Ack=%d 不识别\r\n",FrameAllStruct->ackType_Type);
                        return 2;
                    }    


                    if(0==FrameAllStruct->ackType_Type)
                    {
                        PRINTF_BEFORE1 printf("$$ackType_Type=0  为命令包\r\n");
                    }
                    else if(1==FrameAllStruct->ackType_Type)
                    {
                        PRINTF_BEFORE1 printf("$$ackType_Type=1  为ACK包\r\n");
                    }
                    else
                    {
                        PRINTF_BEFORE1 printf("$$Wrong! ackType_Type=%d 不识别\r\n",FrameAllStruct->ackType_Type);
                        return 3;
                    }  
                   }

                 if(5==ringBufStruct->parseIndex)
                 {                       
                    FrameAllStruct->paddingEnc_padding  =  (ringBufStruct->parseBuf[ringBufStruct->parseIndex-1])&0x1f; 

                    FrameAllStruct->paddingEnc_Enc  = (ringBufStruct->parseBuf[ringBufStruct->parseIndex-1]&0x20)>>5;

                    PRINTF_BEFORE1 printf("$$paddingEnc_padding=%d 填充数据长度\r\n",FrameAllStruct->paddingEnc_padding);  
    
                    if(0==FrameAllStruct->paddingEnc_Enc)
                    {
                        PRINTF_BEFORE1 printf("$$paddingEnc_Enc=0 不加密\r\n"); 
                    }
                    else if(1==FrameAllStruct->paddingEnc_Enc)
                    {
                         PRINTF_BEFORE1 printf("$$paddingEnc_Enc=1 加密\r\n"); 
                    }
                    else
                    {
                        PRINTF_BEFORE1 printf("$$Wrong! paddingEnc_Enc =%d 不识别\r\n",FrameAllStruct->paddingEnc_Enc);
                        return 4;
                    }    
                   }  

                 if(6==ringBufStruct->parseIndex)
                 {                     
                    FrameAllStruct->reserve  =  ringBufStruct->parseBuf[ringBufStruct->parseIndex-1]; 

                 
                    if(0==FrameAllStruct->reserve )
                    {
                        PRINTF_BEFORE1 printf("$$reserve=0 保留固定为0\r\n"); 
                    }
                 
                    else
                    {
                        PRINTF_BEFORE1 printf("$$Wrong! reserve =%d 不识别\r\n",FrameAllStruct->reserve );
                        return 4;
                    }    
                 }   

                 if(8==ringBufStruct->parseIndex)
                 {                     
                    FrameAllStruct->cmdSet  =  ringBufStruct->parseBuf[ringBufStruct->parseIndex-2]; 
                    FrameAllStruct->cmdID  =  ringBufStruct->parseBuf[ringBufStruct->parseIndex-1]; 
                 
                    if(0x01==FrameAllStruct->cmdSet)
                    {
                        switch(FrameAllStruct->cmdID )
                        {
                            case 1:
                            PRINTF_BEFORE printf("$+$cmdSet=1 cmdId=1       ID验证\r\n");                        
                            break;
                            
                            case 2:
                            PRINTF_BEFORE printf("$-$cmdSet=1 cmdId=2       PSDK版本获取\r\n");
                            break;
                            
                            case 3:
                            PRINTF_BEFORE printf("$-$cmdSet=1 cmdId=3       通信链路握手\r\n");
                            break;
                             
                            case 4:
                            PRINTF_BEFORE printf("$+$cmdSet=1 cmdId=4       挂载产品信息获取\r\n");
                            break;
                            
                            case 5:
                            PRINTF_BEFORE printf("$-$cmdSet=1 cmdId=5       升级SkyPoT版本获取\r\n");
                            break;
                              
                            case 6:
                            PRINTF_BEFORE printf("$+$cmdSet=1 cmdId=6       挂载产品别名获取\r\n");
                            break;
                               
                            default:
                            PRINTF_BEFORE printf("$+$cmdSet=1 cmdId=%X \r\n",FrameAllStruct->cmdID );
                            break;
                        }
                    }
                    else  if(0x02==FrameAllStruct->cmdSet )
                    {
                         switch(FrameAllStruct->cmdID )
                        {
                            case 1:
                            PRINTF_BEFORE printf("$-$cmdSet=2 cmdId=1       透明数据传输 APP->挂载\r\n");                        
                            break;
                            
                            case 2:
                            PRINTF_BEFORE printf("$-$cmdSet=2 cmdId=2       透明数据传输 挂载->APP\r\n");
                            break;
                            
                            case 3:
                            PRINTF_BEFORE printf("$-$cmdSet=2 cmdId=3       大疆导频浮动窗口消息推送\r\n");
                            break;
                             
                            case 4:
                            PRINTF_BEFORE printf("$+$cmdSet=2 cmdId=4       获取控件列表长度\r\n");
                            break;
                              
                            case 5: 
                            PRINTF_BEFORE printf("$+$cmdSet=2 cmdId=5       获取控件列表数据\r\n");
                            break;
                            
                            case 6:
                            PRINTF_BEFORE printf("$+$cmdSet=2 cmdId=6       获取控件值组编号\r\n");
                            break;
                              
                            case 7:
                            PRINTF_BEFORE printf("$+$cmdSet=2 cmdId=7       获取控件集团数据\r\n");
                            break;
                            
                            case 8:
                            PRINTF_BEFORE printf("$-$cmdSet=2 cmdId=8       设置控件值\r\n");
                            break;
                               
                            default:
                            PRINTF_BEFORE printf("$+$cmdSet=2 cmdId=%X \r\n",FrameAllStruct->cmdID );
                            break;
                        }
                    }
                    else  if(0x03==FrameAllStruct->cmdSet )
                    {
                        switch(FrameAllStruct->cmdID )
                        {
                            case 1:
                            PRINTF_BEFORE printf("$-$cmdSet=3 cmdId=1       图像传输带宽推送\r\n");                        
                            break;
                            
                            case 2:
                            //PRINTF_BEFORE printf("$-$cmdSet=3 cmdId=2       无人机姿态推送\r\n");
                            break;
                            
                            case 3:
                            PRINTF_BEFORE printf("$+$cmdSet=3 cmdId=3       电池信息推送\r\n");
                            break;
                             
                            case 4:
                            PRINTF_BEFORE printf("$-$cmdSet=3 cmdId=4       GPS数据推送\r\n");
                            break;
                              
                            case 5:
                            PRINTF_BEFORE printf("$-$cmdSet=3 cmdId=5       无人机状态推送\r\n");
                            break;
                            
                            case 6:
                            PRINTF_BEFORE printf("$-$cmdSet=3 cmdId=6       应用程序时间和日期推送\r\n");
                            break;
                              
                            case 7: 
                            PRINTF_BEFORE printf("$-$cmdSet=3 cmdId=7       气压高度推\r\n");
                            break;
                            
                            case 8:
                            PRINTF_BEFORE printf("$-$cmdSet=3 cmdId=8       GPS原始数据推送\r\n");
                            break;
                            
                            case 9:
                            PRINTF_BEFORE printf("$+$cmdSet=3 cmdId=9       RTK原始数据推送\r\n");
                            break;
                               
                            default:
                            PRINTF_BEFORE printf("$+$cmdSet=3 cmdId=%X \r\n",FrameAllStruct->cmdID );
                            break;
                        }
                    }
                    else  if(0x04==FrameAllStruct->cmdSet)
                    {
                        switch(FrameAllStruct->cmdID)
                        {
                            case 1:
                            PRINTF_BEFORE2 printf("$+$cmdSet=4 cmdId=1       设置相机工作模式\r\n");
                            break;
                            
                            case 2:
                            PRINTF_BEFORE2 printf("$+$cmdSet=4 cmdId=2       获取相机工作模式\r\n");
                            break;
                            
                            case 3:
                            PRINTF_BEFORE2 printf("$+$cmdSet=4 cmdId=3       拍摄照片\r\n");
                            break;
                             
                            case 4:
                            PRINTF_BEFORE2 printf("$+$cmdSet=4 cmdId=4       获取拍摄照片状态\r\n");
                            break;
                              
                            case 5:
                            PRINTF_BEFORE2 printf("$+$cmdSet=4 cmdId=5       设置拍摄照片模式\r\n");
                            break;
                            
                            case 6:
                            PRINTF_BEFORE2 printf("$+$cmdSet=4 cmdId=6       获取拍摄照片模式\r\n");
                            break;
                              
                            case 7:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=7       录制视频\r\n");
                            break;
                            
                            case 8:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=8       获取录像状态\r\n");
                            break;
                            
                            case 9:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=9       获取SD卡参数\r\n");
                            break;
                            
                            case 10:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=10      格式SD卡\r\n");
                            break;
                            
                            case 0x20:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x20    设置计量模式\r\n");
                            break;
                            
                            case 0x21:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=0x21    获取计量模式式\r\n");
                            break;
                            
                            case 0x22:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x22    设置定点计量区\r\n");
                            break;
                            
                            case 0x23:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=0x23    获取点计量区\r\n");
                            break;
                             
                            case 0x30:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x30    设置焦点模式\r\n");
                            break;
                              
                            case 0x31:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=0x31    获取焦点模式\r\n");
                            break;
                            
                            case 0x32:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x32    设置聚焦区\r\n");
                            break;
                              
                            case 0x33:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x33    获取聚焦区\r\n");
                            break;
                            
                            case 0x34:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x34    设置数字缩放因子\r\n");
                            break;
                            
                            case 0x35:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=0x35    获取数字缩放因子\r\n");
                            break;
                            
                            case 0x36:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=0x36    获取光学变焦规范\r\n");
                            break;
                            
                            case 0x37:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x37    设置光学变焦焦距\r\n");
                            break;
                              
                            case 0x38:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=0x38    获得光学变焦焦距\r\n");
                            break;
                            
                            case 0x39:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=0x39    获取光学缩放因子\r\n");
                            break;
                            
                            case 0x40:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x40    启动连续光学变焦\r\n");
                            break;
                            
                            case 0x41:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x41    停止连续光学变焦\r\n");
                            break;
                            
                            case 0x42:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x42    设置焦点辅助设置\r\n");
                            break;
                            
                            case 0x43:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=0x43    获得焦点辅助设置\r\n");
                            break;
                            
                            case 0x44:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=0x44    获取焦点环值上界\r\n");
                            break;
                            
                            case 0x45:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x45    设置焦点环值\r\n");
                            break;
                            
                            case 0x46:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=0x46    获取焦点环值\r\n");
                            break;
                            
                            case 0x47:
                            PRINTF_BEFORE printf("$-$cmdSet=4 cmdId=0x47    获取数字缩放规范\r\n");
                            break;
                            
                            case 0xff:
                            PRINTF_BEFORE printf("$+$cmdSet=4 cmdId=0xff    获取相机命令集支持状态\r\n");
                            break;

  
                            default:
                            PRINTF_BEFORE2 printf("$+$cmdSet=4 cmdId=%X \r\n",FrameAllStruct->cmdID );
                            break;
                        }
                    }
                    else  if(0x05==FrameAllStruct->cmdSet)
                    {
                         switch(FrameAllStruct->cmdID)
                        {
                            case 1:
                            PRINTF_BEFORE printf("$+$cmdSet=5 cmdId=1       获得万向节状态\r\n");
                            break;
                            
                            case 2:
                            PRINTF_BEFORE printf("$-$cmdSet=5 cmdId=2       设置万向节工作模式\r\n");
                            break;
                            
                            case 3:
                            PRINTF_BEFORE printf("$-$cmdSet=5 cmdId=3       向节节返回头\r\n");
                            break;
                             
                            case 4:
                            PRINTF_BEFORE printf("$-$cmdSet=5 cmdId=4       控制万向节单个速度\r\n");
                            break;
                              
                            case 5:
                            PRINTF_BEFORE printf("$-$cmdSet=5 cmdId=5       万向架校准\r\n");
                            break;
                            
                            case 0xff:
                            PRINTF_BEFORE printf("$+$cmdSet=5 cmdId=0xff    获取万向节命令集支持状态\r\n");
                            break;

                            default:
                            PRINTF_BEFORE printf("$+$cmdSet=5 cmdId=%X \r\n",FrameAllStruct->cmdID );
                            break;
                        }
                    }
                    else  
                    {
                        PRINTF_BEFORE printf("$+$Wrong!cmdSet=%d            不识别\r\n",FrameAllStruct->cmdSet);        
                        return 5;
                    }
                 }       

                if(PSDK_PROTHEADEBEFORERLEN==ringBufStruct->parseIndex)
                 {                     
                    FrameAllStruct->seqNum  =  ringBufStruct->parseBuf[ringBufStruct->parseIndex-1]; 
                    FrameAllStruct->seqNum  <<= 8;
                    FrameAllStruct->seqNum  += ringBufStruct->parseBuf[ringBufStruct->parseIndex-2]; 
                         
                    PRINTF_BEFORE1 printf("$$seqNum =%d 命令序列排序号\r\n",FrameAllStruct->seqNum );                    
                 }                 
           
                 if(PSDK_PROTHEADERLEN==ringBufStruct->parseIndex)
                 {              
                     FrameAllStruct->crc16  = PsdkCrc_Crc16(ringBufStruct->parseBuf, PSDK_PROTHEADEBEFORERLEN);
     
                     bspCrc16TempforPrintf=ringBufStruct->parseBuf[11]; 
                     bspCrc16TempforPrintf<<=8; 
                     bspCrc16TempforPrintf+=ringBufStruct->parseBuf[10];
                    if (bspCrc16TempforPrintf != FrameAllStruct->crc16) 
                    {                     
                        ringBufStruct->parseIndex = 0;
                        PRINTF_BEFORE1 printf("$$crc16=%04X rec_wrong=%04X\r\n", FrameAllStruct->crc16 ,bspCrc16TempforPrintf);
                        return 2;
                    }
                    else
                    {
                        PRINTF_BEFORE1 printf("$$crc16=%04X is Correct\r\n", FrameAllStruct->crc16);
                    }    
                 }
                 
                 //检测一下	 protParse->parseIndex 是否和  frameLen的长度 一致 可以做CRC32了
                if ( FrameAllStruct->lenVer_len ==ringBufStruct->parseIndex) 
                {  
           
                    FrameAllStruct->crc32  = PsdkCrc_Crc32(ringBufStruct->parseBuf,  (uint16_t) ( FrameAllStruct->lenVer_len - 4)); 

                    
                    bspCrc32TempforPrintf =ringBufStruct->parseBuf[( FrameAllStruct->lenVer_len - 1)];

                    bspCrc32TempforPrintf<<=8;
                    bspCrc32TempforPrintf+=ringBufStruct->parseBuf[( FrameAllStruct->lenVer_len - 2)];
                    
                    bspCrc32TempforPrintf<<=8;
                    bspCrc32TempforPrintf+=ringBufStruct->parseBuf[( FrameAllStruct->lenVer_len - 3)];
                   
                    bspCrc32TempforPrintf<<=8;
                    bspCrc32TempforPrintf+=ringBufStruct->parseBuf[( FrameAllStruct->lenVer_len - 4)]; 
                    
                    if (bspCrc32TempforPrintf !=  FrameAllStruct->crc32 )
                    {
                  
                        ringBufStruct->parseIndex = 0;
                        PRINTF_BEFORE1 printf("$$crc32=%08X rec_wrong=%08X\r\n", FrameAllStruct->crc32 ,bspCrc32TempforPrintf);
                        return 3;
                    } 
                    else 
                    {  
                        ringBufStruct->parseIndex = 0;
                        PRINTF_BEFORE1 printf("$$crc32=%08X is Correct\r\n", FrameAllStruct->crc32);
                   
                         FrameAllStruct->sendDataLen=(FrameAllStruct->lenVer_len-PSDK_PROTFRAMEMINLEN); 
                        
                       
                        if(0==FrameAllStruct->paddingEnc_Enc)//没加密
                        {
                             PRINTF_BEFORE1 printf("$$数据总长度为=%d\r\n",FrameAllStruct->lenVer_len);
                            if(0==FrameAllStruct->sendDataLen)
                            {
                                PRINTF_BEFORE1 printf("$$数据帧没有加密 没有数据帧\r\n");                      
                            }
                            else
                            {
                                PRINTF_BEFORE1 printf("$$数据帧没有加密 长度=%d\r\n",FrameAllStruct->sendDataLen);
                            }    
                            for(i=0;i<FrameAllStruct->sendDataLen;i++)
                            {
                                FrameAllStruct->sendData[i]=ringBufStruct->parseBuf[PSDK_PROTHEADERLEN+i];
                            }
                     
                        }
                        else//加密 
                        {
                                
                             if(FrameAllStruct->lenVer_len>PSDK_PROTHEADERLEN+PSDK_PROTHEADECRC32RLEN)
                             {
                                 PRINTF_BEFORE1 printf("$$加密且有数据帧\r\n");
                                 if(0==FrameAllStruct->paddingEnc_padding)
                                 {
                                    PRINTF_BEFORE1 printf("$$数据总长度为=%d\r\n",FrameAllStruct->lenVer_len); 
                                    PRINTF_BEFORE1 printf("$$加密但数据帧没有填补\r\n");
                                 }
                                 else
                                 {
                                    PRINTF_BEFORE1 printf("$$解密前数据总长度为=%d\r\n",FrameAllStruct->lenVer_len);                                     
                                    PRINTF_BEFORE1 printf("$$加密且数据帧有填补 长度为=%d\r\n",FrameAllStruct->paddingEnc_padding);                                    
                                 }
                                
                                 PRINTF_BEFORE1 printf("$$开始解密\r\n");
                                 PsdkAes_Decrypt(&(ringBufStruct->parseBuf[PSDK_PROTHEADERLEN]),&(FrameAllStruct->sendData[0]), (FrameAllStruct->lenVer_len-PSDK_PROTFRAMEMINLEN),(uint8_t const*)g_keyTable,128);
                                 FrameAllStruct->sendDataLen-=FrameAllStruct->paddingEnc_padding;
                                 FrameAllStruct->lenVer_len-=FrameAllStruct->paddingEnc_padding;
                                 PRINTF_BEFORE1 printf("$$解密后的数据总长度为=%d\r\n",FrameAllStruct->lenVer_len);
                                 FrameAllStruct->paddingEnc_padding=0;
                             }
                             else
                             {
                                 PRINTF_BEFORE1 printf("$$加密但没有数据帧\r\n");
                                 for(i=0;i<FrameAllStruct->sendDataLen;i++)
                                {
                                    FrameAllStruct->sendData[i]=ringBufStruct->parseBuf[PSDK_PROTHEADERLEN+i];
                                }
                             }    

                        }  
                        
                        
                        //PRINTF_BEFORE printf("$$接收数据帧如下:\r\n");
                        //PRINTF_BEFORE printf("$$##################cccccccccccccc\r\n");
                        //PRINTF_BEFORE cocoguojia_usartNPutTableForNum(1,&(FrameAllStruct->sendData[0]),FrameAllStruct->sendDataLen);
                        //PRINTF_BEFORE printf("\r\n##################cccccccccccc\r\n");
                                        
                        //PRINTF_BEFORE printf("$$原始数据接收完成\r\n-----------\r\n");
                        
                                                
                        Bsp_Usart3ParseGoDoWhit(FrameAllStruct);//接收到完整的数据后 开始具体解析并处理数据,包括做出正确的回复
                        return 0;
                    }
                }
            }    
        }
        osDelay(1);
   }
   return 0;
}

//------------------------------------------------------------------
//发送一个字节
void Bsp_Usart3PutChar(uint8_t Dat)
{
	USART_ClearFlag(USART3,USART_FLAG_TC);
	USART_SendData(USART3, Dat);
	while(RESET==USART_GetFlagStatus(USART3,USART_FLAG_TC))
	{
		;
	}
}

//------------------------------------------------------------------
//发送8位的数组
void Bsp_Usart3SendArray(uint8_t *array, uint16_t num)
{
    uint8_t i;
	
    for(i=0; i<num; i++)
    {
        Bsp_Usart3PutChar(array[i]);//发送一个字节数据到USART	
    }

	//等待发送完成
    while(SET!=USART_GetFlagStatus(USART3,USART_FLAG_TC))
	{
		;
	}
}

//------------------------------------------------------------------
//发送字符串
void Bsp_Usart3PutStr(uint8_t *str)
{
	uint8_t *sp=str;
	uint16_t k=0;
	do
	{
		Bsp_Usart3PutChar(*(sp+k));
		k++;
	}
	while('\0'!=(*(sp+k)));

    while(SET!=USART_GetFlagStatus(USART3,USART_FLAG_TC))
	{
		;
	}
}

////////////////////////////////////////////////////////////
//中断函数
void USART3_IRQHandler()
{
    uint8_t dat;
    uint32_t ulReturn;
    
    //进入临界段，临界段可以嵌套
    ulReturn = taskENTER_CRITICAL_FROM_ISR();
    
  
    
    if(RESET!=USART_GetITStatus(USART3, USART_IT_RXNE))
    {
        dat = (uint8_t)(USART_ReceiveData(USART3) & 0x00FF);
        
        Usart3RingRecBufDoStruct.rawBuf[Usart3RingRecBufDoStruct.rawWriteIndex]=dat;
        Usart3RingRecBufDoStruct.rawWriteIndex++;
        if(USART3_RECRAW_RINGBUGLENMAX<=Usart3RingRecBufDoStruct.rawWriteIndex)
        {
            Usart3RingRecBufDoStruct.rawWriteIndex=0;
        } 
    }
    
     
	//这一步 为了在退出前 清零中断标志 中断内处理时候 可能有延时 导致前面说的那种情况
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {         
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);//清除中断标志     
	}
 
	
    //防止有接收溢出中断的Bug
    if(SET==USART_GetFlagStatus(USART3,USART_FLAG_ORE))
    {
        USART_ClearFlag(USART3,USART_FLAG_ORE);
        USART_ReceiveData(USART3);
    }
   
    // 退出临界段 
    taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

uint8_t gpsPtr[255];   

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//一级内部函数
//-----------------------------------------------------------------------------------------------------------------------------
//功能:接收到完整的数据后 开始具体解析并处理数据,包括做出正确的回复
//形式参数:PSDK 数据格式结构体指针
static uint8_t Bsp_Usart3ParseGoDoWhit(T_PsdkProtFrameAllStruct *FrameAllStruct)    
{
     //portBASE_TYPE xStatus;
     uint8_t temp[16];
     uint8_t i=0;
     memset(&temp,0,16);
     if((0x01==FrameAllStruct->cmdSet)&&(0x01==FrameAllStruct->cmdID))              //ID验证
     {
          PRINTF_BEFORE1 printf("$$挂载ID验证命令\r\n");
          if(16==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
              PRINTF_BEFORE1 printf("$$挂载ID验证命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
              return 1;
          }      
        
          PRINTF_BEFORE1 printf("$$挂载ID验证命令 16个随机数为:\r\n");
          PRINTF_BEFORE1 printf("$$#########################aaaaaa\r\n");
          PRINTF_BEFORE1 cocoguojia_usartNPutTableForNum(1,FrameAllStruct->sendData,16);
          PRINTF_BEFORE1 printf("\r\n#########################aaaaaa\r\n");
          
          PsdkPayloadStateCmd_IdentityVerify(&temp[0],FrameAllStruct->sendData);    //产生的16字节秘钥
         
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0
               
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          memcpy(&(FrameAllStruct->sendData[1]),&temp[0],16);                       //将秘钥填之
          FrameAllStruct->sendDataLen=17;                                           //数据长度为17
          
          PRINTF_BEFORE1 printf("$$生产16字节密钥为:\r\n");
          PRINTF_BEFORE1 printf("$$#########################\r\n");
          PRINTF_BEFORE1 cocoguojia_usartNPutTableForNum(1,&temp[0],16);
          PRINTF_BEFORE1 printf("\r\n#########################\r\n");
         
         
          PRINTF_BEFORE1 printf("$$产生的16字节秘钥=%s\r\n",(FrameAllStruct->sendData)+1);
          PRINTF_BEFORE1 printf("$$数据帧长度=17\r\n");
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x01;                                              //命令大类
          FrameAllStruct->cmdID=0x01;                                               //命令细分
         
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
     if((0x01==FrameAllStruct->cmdSet)&&(0x02==FrameAllStruct->cmdID))        {PRINTF_BEFORE1 printf("$$PSDK版本获得命令,目前程序没处理--###########--\r\n");return 1;}
     
     if((0x01==FrameAllStruct->cmdSet)&&(0x03==FrameAllStruct->cmdID))        {PRINTF_BEFORE1 printf("$$通讯链路握手命令,目前程序没处理--###########--\r\n");return 1;}
     
     if((0x01==FrameAllStruct->cmdSet)&&(0x04==FrameAllStruct->cmdID))              //挂载产品信息获取
     {  
          PRINTF_BEFORE1 printf("$$挂载产品信息获取命令\r\n");
          if(0==FrameAllStruct->sendDataLen)  
          {
                ;
          }  
          else
          {
              PRINTF_BEFORE1 printf("$$挂载产品信息获取 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
              return 1;
          }      
        
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0 
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          
         
          memcpy((FrameAllStruct->sendData)+1,                                              USER_APP_NAME,  strlen(USER_APP_NAME));
          memcpy((FrameAllStruct->sendData)+33,                                             USER_APP_ID,    strlen(USER_APP_ID));
          memcpy((FrameAllStruct->sendData)+49,                                             USER_DEVELOPER_ACCOUNT,    strlen(USER_DEVELOPER_ACCOUNT));
         
          FrameAllStruct->sendDataLen=113;         //数据帧长度
         
          PRINTF_BEFORE1 printf("$$挂载产品信息=%s",(FrameAllStruct->sendData)+1);
          PRINTF_BEFORE1 printf("%s",(FrameAllStruct->sendData)+33);
          PRINTF_BEFORE1 printf("%s",(FrameAllStruct->sendData)+49);
          PRINTF_BEFORE1 printf("$$数据帧长度=%d\r\n",FrameAllStruct->sendDataLen); 
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
		    PRINTF_BEFORE1 printf("$$数据帧要加密\r\n"); 
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
                               
          FrameAllStruct->cmdSet=0x01;                                              //命令大类
          FrameAllStruct->cmdID=0x04;                                               //命令细分
          
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
     if((0x01==FrameAllStruct->cmdSet)&&(0x05==FrameAllStruct->cmdID)) {PRINTF_BEFORE1 printf("$$升级SkyPoT版本获取命令,目前程序没处理--###########--\r\n");}
     
            
     if((0x01==FrameAllStruct->cmdSet)&&(0x06==FrameAllStruct->cmdID))              //升级SkyPoT版本获取
     {
         PRINTF_BEFORE1 printf("$$挂载产品别名获取\r\n");
         if(0==FrameAllStruct->sendDataLen)  
         {
            ;
         }  
         else
         {
             PRINTF_BEFORE1 printf("$$挂载产品别名获取命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
             return 1;
         }                
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          
        
          memcpy((FrameAllStruct->sendData+1),USER_OTHER_NAME,strlen(USER_OTHER_NAME));
          FrameAllStruct->sendDataLen=33;                                            //数据长度为33
          
          PRINTF_BEFORE1 printf("$$数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x01;                                              //命令大类
          FrameAllStruct->cmdID=0x06;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     }
     
  /*   
       if((0x02==FrameAllStruct->cmdSet)&&(0x04==FrameAllStruct->cmdID))              //获取控件值组编号
     {
         PRINTF_BEFORE1 printf("$$获取控件列表长度\r\n");
         if(0==FrameAllStruct->sendDataLen)  
         {
            ;
         }  
         else
         {
             PRINTF_BEFORE1 printf("$$获取控件列表长度 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
             return 1;
         }                
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
         FrameAllStruct->sendData[1]=0x0C;                                            //返回码 执行成功 ACK
          
          FrameAllStruct->sendDataLen=2;                                          
          
          PRINTF_BEFORE1 printf("$$数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x02;                                              //命令大类
          FrameAllStruct->cmdID=0x04;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     }
     
     
      if((0x02==FrameAllStruct->cmdSet)&&(0x05==FrameAllStruct->cmdID))              //获取控件列表数据
     {
         PRINTF_BEFORE1 printf("$$获取控件列表数据\r\n");
         if(1==FrameAllStruct->sendDataLen)  
         {
            ;
         }  
         else
         {
             PRINTF_BEFORE1 printf("$$获取控件列表数据 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
             return 1;
         }                
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0 
         
          uint8_t table25[]={0x00,0x02,0x00,0x52,0x65,0x64,0x20,0x4C,0x45,0x44,     \
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     \
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     \
                             0x00,0x00,0x00,0x00,0x00}; 
          
          memcpy(FrameAllStruct->sendData,table25,35);  
          FrameAllStruct->sendDataLen=35;                                          
          
          PRINTF_BEFORE1 printf("$$数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x02;                                              //命令大类
          FrameAllStruct->cmdID=0x05;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     }
     
     
      if((0x02==FrameAllStruct->cmdSet)&&(0x06==FrameAllStruct->cmdID))              //获取控件值组编号
     {
         PRINTF_BEFORE1 printf("$$获取控件值组编号\r\n");
         if(0==FrameAllStruct->sendDataLen)  
         {
            ;
         }  
         else
         {
             PRINTF_BEFORE1 printf("$$获取控件值组编号 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
             return 1;
         }                
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
         FrameAllStruct->sendData[1]=1;                                            //返回码 执行成功 ACK
          
          FrameAllStruct->sendDataLen=2;                                          
          
          PRINTF_BEFORE1 printf("$$数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x02;                                              //命令大类
          FrameAllStruct->cmdID=0x06;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     }
     
       if((0x02==FrameAllStruct->cmdSet)&&(0x07==FrameAllStruct->cmdID))              //获取控件值组编号
     {
         PRINTF_BEFORE1 printf("$$获取控件组值数据 \r\n");
         if(1==FrameAllStruct->sendDataLen)  
         {
            ;
         }  
         else
         {
             PRINTF_BEFORE1 printf("$$获取控件组值数据  数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
             return 1;
         }                
            uint8_t table27[80]={0x00,0x0C,0x02,0x00,0x00,0x00,0x00,0x00,0x02,0x01,   \
                               0x00,0x00,0x00,0x00,0x01,0x02,0x00,0x00,0x00,0x00,   \
                               0x03,0x03,0x32,0x00,0x00,0x00,0x05,0x04,0xC8,0x00,   \
                               0x00,0x00,0x04,0x05,0x01,0x00,0x00,0x00,0x01,0x06,   \
                               0x00,0x00,0x00,0x00,0x01,0x07,0x00,0x00,0x00,0x00,   \
                               0x04,0x08,0x09,0x00,0x00,0x00,0x04,0x09,0x00,0x00,   \
                               0x00,0x00,0x04,0x0A,0x00,0x00,0x00,0x00,0x05,0x0B,   \
                               0xD0,0x07,0x00,0x00};
            
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
          memcpy(FrameAllStruct->sendData,table27,74);
        
          FrameAllStruct->sendDataLen=74;                                          
          
          PRINTF_BEFORE1 printf("$$数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x02;                                              //命令大类
          FrameAllStruct->cmdID=0x07;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     }
     

    
    if((0x03==FrameAllStruct->cmdSet)&&(0x01==FrameAllStruct->cmdID)){  PRINTF_BEFORE1 printf("$$图像传输宽带推送命令,目前程序没处理      --###########--\r\n");return 1;}
    
    if((0x03==FrameAllStruct->cmdSet)&&(0x02==FrameAllStruct->cmdID)){  PRINTF_BEFORE1 printf("$$无人机姿态推送命令,目前程序没处理        --###########--\r\n");return 1;}
    
    if((0x03==FrameAllStruct->cmdSet)&&(0x03==FrameAllStruct->cmdID))
    {  
          PRINTF_BEFORE1 printf("$$电池信息推送命令\r\n");

          if(2==FrameAllStruct->sendDataLen)  
          {
              PRINTF_BEFORE1 printf("$$电池功率百分比%d\r\n",FrameAllStruct->sendData[0]);
             if(0==FrameAllStruct->sendData[1])
             {
                 PRINTF_BEFORE1 printf("$$电池正常\r\n");
             }
             else  if(1==FrameAllStruct->sendData[0])
             {
                 PRINTF_BEFORE1 printf("$$断电很快\r\n");
             }
             else
             {
                PRINTF_BEFORE1 printf("$$电池信息推送命令 模式命令出错=%d\r\n",FrameAllStruct->sendData[0]);
                return 1;
             }           
          }
          else
          {
              PRINTF_BEFORE1 printf("$$电池信息推送命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
              return 1;
          }
    
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=1;                                            //电源关闭准备完成
          FrameAllStruct->sendDataLen=2;                                            //数据长度为2
          
          PRINTF_BEFORE1 printf("$$数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x03;                                              //命令大类
          FrameAllStruct->cmdID=0x03;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
        return 1;
    } 
    */
    

    /*
    if((0x03==FrameAllStruct->cmdSet)&&(0x04==FrameAllStruct->cmdID)){  PRINTF_BEFORE1 printf("$$GPS数据推送命令,目前程序没处理           --###########--\r\n");return 1;} 
    
    if((0x03==FrameAllStruct->cmdSet)&&(0x05==FrameAllStruct->cmdID)){  PRINTF_BEFORE1 printf("$$无人机状态推送命令,目前程序没处理         --###########--\r\n");return 1;}  
    if((0x03==FrameAllStruct->cmdSet)&&(0x06==FrameAllStruct->cmdID)){  PRINTF_BEFORE1 printf("$$应用程序时间和日期推送命令,目前程序没处理  --###########--\r\n");return 1;}  
    if((0x03==FrameAllStruct->cmdSet)&&(0x07==FrameAllStruct->cmdID)){  PRINTF_BEFORE1 printf("$$气压高度推命令,目前程序没处理             --###########--\r\n");return 1;}  
    */
    
    
     
     /*
     if((0x03==FrameAllStruct->cmdSet)&&(0x06==FrameAllStruct->cmdID))           
     {     
     
         PRINTF_BEFORE printf("$$时间数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);

         PRINTF_BEFORE printf("$$##################6666666\r\n");
         PRINTF_BEFORE cocoguojia_usartNPutTableForNum(1,FrameAllStruct->sendData,FrameAllStruct->sendDataLen);                           
         PRINTF_BEFORE printf("\r\n##################666666\r\n"); 
                    
         if(254>=FrameAllStruct->sendDataLen)
         {
            FrameAllStruct->sendData[FrameAllStruct->sendDataLen]=0;//给结尾加一个 '\0'
         }
    
     }  
    */     
 
    if((0x03==FrameAllStruct->cmdSet)&&(0x08==FrameAllStruct->cmdID))
    {
        PRINTF_BEFORE printf("$$GPS原始数据推送命令\r\n");
        PRINTF_BEFORE printf("$$数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);
        PRINTF_BEFORE printf("$$##################999999999999\r\n");
        PRINTF_BEFORE cocoguojia_usartNPutTableForNum(1,FrameAllStruct->sendData,FrameAllStruct->sendDataLen);                           
        PRINTF_BEFORE printf("\r\n##################999999999999\r\n"); 
        //return 1;
        
         //----------------------------------------------------------------------------------------
         //coocguojia 添加
         //----------------------------------------------------------------------------------------
         //在这里借用一下GPS原始数据推送命令 发送一下 DJI Pilot浮动窗口消息推送 cocoguojia  发送实时GPS定位信息 
         //u16 press=0;
         //u16 temp=0;
         //u8 i;
         //press++;
         //temp++;    
         PRINTF_BEFORE printf("$$DJI Pilot浮动窗口消息推送--借用GPS原始数据推送命令\r\n");
                       
         //memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          //sprintf((char *)FrameAllStruct->sendData,"Demo Camer Info:\r\n\r\PHOTO = %d mbar\r\n\r\1 cam = %d \r\n", press, temp); 
          //uint8_t hanzi[]={0xE5,0xA1,0x9E,0xE5,0xB0,0x94,0xE6,0x97,0xA0,0xE4,0xBA,0xBA,0xE6,0x9C,0xBA,0xE5,0xB9,0xBF,0xE8,0x90,0xA5,0xE7,0xBA,0xA2,0xE9,0xB9,0x8F};
          //memcpy(FrameAllStruct->sendData,"1234567890cocoguojia\r\n",20);  
          //for(i=0;i<27;i++)
          //{
          //      FrameAllStruct->sendData[20+i]=hanzi[i];
          //}
     
          /*
          FrameAllStruct->sendData[0]='P';
          FrameAllStruct->sendData[1]='=';
          FrameAllStruct->sendData[2]=g_tim5CatchNum/1000+'0';
          FrameAllStruct->sendData[3]=g_tim5CatchNum%1000/100+'0';
          FrameAllStruct->sendData[4]=g_tim5CatchNum%1000%100/10+'0';
          FrameAllStruct->sendData[5]=g_tim5CatchNum%1000%100%10+'0';
          FrameAllStruct->sendData[6]='\r';
          FrameAllStruct->sendData[7]='\n';
          */
          //
          sprintf((char *)(FrameAllStruct->sendData),"\r\n P=%d\r\n PIC1=%d\r\n PIC2=%d\r\n PIC3=%d\r\n PIC4=%d\r\n PIC5=%d\r\n",g_tim5CatchNum,g_photoPicNum[1],g_photoPicNum[2],g_photoPicNum[3],g_photoPicNum[4],g_photoPicNum[5]);
         
          FrameAllStruct->sendDataLen=strlen((const char*)(FrameAllStruct->sendData));                                            //数据长度为33
          
          PRINTF_BEFORE printf("$$数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);                               
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=0;                                           //命令包     
          //FrameAllStruct->paddingEnc_padding=0;                                   //填充包为0       
          FrameAllStruct->paddingEnc_Enc=1;                                         //加密                       
          FrameAllStruct->cmdSet=0x02;                                              //命令大类
          FrameAllStruct->cmdID=0x03;                                               //命令细分       
          FrameAllStruct->seqNum++;         
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //发送回复            
          return 1;          
    } 
    
    
     uint8_t *sp001;
     if((0x03==FrameAllStruct->cmdSet)&&(0x09==FrameAllStruct->cmdID))             //GPS RTK 数据
     {     
         //memset(FrameAllStruct->sendData,0,255);                                 //这里得清零 因为如果Enc有加密需求 那么填补自动就是0 
         
         PRINTF_BEFORE printf("$$RTK数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);
         PRINTF_BEFORE printf("$$##################6666666\r\n");
         PRINTF_BEFORE cocoguojia_usartNPutTableForNum(1,FrameAllStruct->sendData,FrameAllStruct->sendDataLen);                           
         PRINTF_BEFORE printf("\r\n##################666666\r\n"); 
         
          /*
          //实验
          FrameAllStruct->sendDataLen=10;
          FrameAllStruct->sendData[0]='@';
          FrameAllStruct->sendData[1]='1';
          FrameAllStruct->sendData[2]='2';
          FrameAllStruct->sendData[3]='3';
          FrameAllStruct->sendData[4]='4';
          FrameAllStruct->sendData[5]='5';
          FrameAllStruct->sendData[6]='A';
          FrameAllStruct->sendData[7]='B';
          FrameAllStruct->sendData[8]='C';
          FrameAllStruct->sendData[9]='X';    
          */         
         
         if(254>=FrameAllStruct->sendDataLen)
         {
            FrameAllStruct->sendData[FrameAllStruct->sendDataLen]=0;//给结尾加一个 '\0'
         }
         else
         {
            FrameAllStruct->sendData[254]=0;
         }
         
         if((180>=FrameAllStruct->sendDataLen)&&(0!=FrameAllStruct->sendDataLen))
         {
             sp001=FrameAllStruct->sendData;
             if(1==g_GpsRtkProtectFlag)//1处于保护 所以写2
             {
                g_gpsRtkTable2Len=FrameAllStruct->sendDataLen; 
                for(i=0;i<g_gpsRtkTable2Len;i++)
                {
                   g_gpsRtkTempTable2[i]=*sp001;
                   sp001++;
                }
                g_gpsRtkPointFlag=2;
             }
             else if(2==g_GpsRtkProtectFlag)//2处于保护 所以写1
             {
                 g_gpsRtkTable1Len=FrameAllStruct->sendDataLen;
                 for(i=0;i<g_gpsRtkTable1Len;i++)
                 {
                    g_gpsRtkTempTable1[i]=*sp001;
                    sp001++;
                 }             
                 g_gpsRtkPointFlag=1;
             }
             else//没有保护 但也可能中间出现保护 所以根据g_gpsRtkPointFlag来判断写谁
             {
                if(1==g_gpsRtkPointFlag)//写2
                {
                    g_gpsRtkTable2Len=FrameAllStruct->sendDataLen; 
                    for(i=0;i<g_gpsRtkTable2Len;i++)
                    {
                       g_gpsRtkTempTable2[i]=*sp001;
                       sp001++;
                    }
                    g_gpsRtkPointFlag=2;
                }
                else//写1
                {
                    g_gpsRtkTable1Len=FrameAllStruct->sendDataLen; 
                    for(i=0;i<g_gpsRtkTable1Len;i++)
                    {
                        g_gpsRtkTempTable1[i]=*sp001;
                        sp001++;
                    }     
                   g_gpsRtkPointFlag=1;
                } 
             }
         }       
     }   
   
    /*
    if((0x03==FrameAllStruct->cmdSet)&&(0x04==FrameAllStruct->cmdID))            //GPS 数据推送
     {     
        //memset(FrameAllStruct->sendData,0,255);                                 //这里得清零 因为如果Enc有加密需求 那么填补自动就是0 
         
         PRINTF_BEFORE printf("$$GPS 数据推送数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);

         PRINTF_BEFORE printf("$$##################6666666\r\n");
         PRINTF_BEFORE cocoguojia_usartNPutTableForNum(1,FrameAllStruct->sendData,FrameAllStruct->sendDataLen);                           
         PRINTF_BEFORE printf("\r\n##################666666\r\n");     
     } 
     */
     
     if((0x04==FrameAllStruct->cmdSet)&&(0x01==FrameAllStruct->cmdID))              //设置相机工作模式
     {
          PRINTF_BEFORE1 printf("$$设置相机工作模式命令\r\n");
          if(1==FrameAllStruct->sendDataLen)  
          {
             if(0==FrameAllStruct->sendData[0])
             {
                 PRINTF_BEFORE1 printf("$$拍摄照片模式\r\n");
             }
             else  if(1==FrameAllStruct->sendData[0])
             {
                 PRINTF_BEFORE1 printf("$$录像模式\r\n");
             }
             else
             {
                PRINTF_BEFORE1 printf("$$设置相机工作模式命令 模式命令出错=%d\r\n",FrameAllStruct->sendData[0]);
                return 1;
             }           
          } 
          else
          {
             PRINTF_BEFORE1 printf("$$设置相机工作模式命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
             return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendDataLen=1;                                            //数据长度为1
          
          PRINTF_BEFORE1 printf("$$数据帧长度=1\r\n");                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x01;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     }    
       
     if((0x04==FrameAllStruct->cmdSet)&&(0x02==FrameAllStruct->cmdID))              //获取相机工作模式
     {
          PRINTF_BEFORE1 printf("$$获取相机工作模式命令\r\n");
          if(0==FrameAllStruct->sendDataLen)  
          {
               ;
          }
          else
          {
              PRINTF_BEFORE1 printf("$$获取相机工作模式命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
              return 1;
          }
    
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=0;                                            //固定为拍摄照片模式
          FrameAllStruct->sendDataLen=2;                                            //数据长度为2
          
          PRINTF_BEFORE1 printf("$$数据帧长度=%d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x02;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
     if((0x04==FrameAllStruct->cmdSet)&&(0x03==FrameAllStruct->cmdID))              //拍照
     {
          PRINTF_BEFORE2 printf("$$拍照命令\r\n");
      
          if(1==FrameAllStruct->sendDataLen)  
          {
             if(0==FrameAllStruct->sendData[0])
             {
                 PRINTF_BEFORE2 printf("$$间隔拍摄停止\r\n");
              
             }
             else  if(1==FrameAllStruct->sendData[0])
             {
                 PRINTF_BEFORE2 printf("$$拍摄照片\r\n");
                 g_capture_FinishAndStartPhotoFlag=1;///////////cocoguojia////////////////////////////                   
             }
             else
             {
                PRINTF_BEFORE2 printf("$$拍摄照片命令 命令出错=%d\r\n",FrameAllStruct->sendData[0]);
                 return 1;
             }           
          }  
          else
          {
               PRINTF_BEFORE2 printf("$$拍摄照片命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendDataLen=1;                                            //数据长度为1
          
          PRINTF_BEFORE1 printf("$$数据帧长度=1\r\n");                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
                                 
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x03;                                               //命令细分
          
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
       
     } 
     
     if((0x04==FrameAllStruct->cmdSet)&&(0x04==FrameAllStruct->cmdID))              //拍摄照片状态
     {
          PRINTF_BEFORE1 printf("$$拍摄照片状态命令\r\n");
      
          if(0==FrameAllStruct->sendDataLen)  
          {
            ; 
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$拍摄照片状态命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=0x50;                                         //固定 cocoguojia
          FrameAllStruct->sendDataLen=2;                                            //数据长度为2
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x04;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
     if((0x04==FrameAllStruct->cmdSet)&&(0x05==FrameAllStruct->cmdID))              //设置拍摄照片模式
     {
          PRINTF_BEFORE1 printf("$$设置拍摄照片模式\r\n");
          if(5==FrameAllStruct->sendDataLen)  
          {

             photoModelSetTempTable[0]=FrameAllStruct->sendData[1];
             photoModelSetTempTable[1]=FrameAllStruct->sendData[2];
             photoModelSetTempTable[2]=FrameAllStruct->sendData[3];
             photoModelSetTempTable[3]=FrameAllStruct->sendData[4];
              
             if(0==FrameAllStruct->sendData[1])
             {
                 PRINTF_BEFORE1 printf("$$单次拍照\r\n");
             }
             else  if(1==FrameAllStruct->sendData[1])
             {
                 PRINTF_BEFORE1 printf("$$爆发拍照\r\n");
              
             }
             else  if(1==FrameAllStruct->sendData[1])
             {
                 PRINTF_BEFORE1 printf("$$间隔拍摄\r\n");
              
             }
             else
             {
                PRINTF_BEFORE1 printf("$$拍摄照片命令 命令出错=%d\r\n",FrameAllStruct->sendData[0]);
                 return 1;
             } 
             
             PRINTF_BEFORE1 printf("$$暴裂镜头数量=%d\r\n",FrameAllStruct->sendData[2]);
             PRINTF_BEFORE1 printf("$$定时拍摄照片数=%d\r\n",FrameAllStruct->sendData[3]);
             PRINTF_BEFORE1 printf("$$区间拍照的时间间隔=%d\r\n",FrameAllStruct->sendData[4]);
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$拍摄照片命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
       
          FrameAllStruct->sendDataLen=1;                                            //数据长度为1
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x05;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
      
     if((0x04==FrameAllStruct->cmdSet)&&(0x06==FrameAllStruct->cmdID))              //设置拍摄照片模式
     {
          PRINTF_BEFORE1 printf("$$获取拍照模式\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$拍摄照片命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0x00;                                            //返回码 执行成功 ACK
          
          
          FrameAllStruct->sendData[1]=photoModelSetTempTable[0];                            //单次拍照
          FrameAllStruct->sendData[2]=photoModelSetTempTable[1];                            //爆裂镜头的数量
          FrameAllStruct->sendData[3]=photoModelSetTempTable[2];                            //定时拍摄照片数
          FrameAllStruct->sendData[4]=photoModelSetTempTable[3];                            //区间拍照的时间间隔（ITUT8）
          //------------------------------------------------------------------------------------------
          //cocoguojia
         // FrameAllStruct->sendData[1]=0x01;                            //单次拍照
         // FrameAllStruct->sendData[2]=0x03;                            //爆裂镜头的数量
         // FrameAllStruct->sendData[3]=0xff;                            //定时拍摄照片数
         // FrameAllStruct->sendData[4]=0x05;                            //区间拍照的时间间隔（ITUT8）
          
          FrameAllStruct->sendData[5]=0x00;                            //大疆 添加多的
          FrameAllStruct->sendDataLen=6;            //数据长度为5
           //FrameAllStruct->sendDataLen=5;    
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x06;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
          
     if((0x04==FrameAllStruct->cmdSet)&&(0x07==FrameAllStruct->cmdID))              //设置拍摄照片模式
     {
          PRINTF_BEFORE1 printf("$$录制视频\r\n");
        
          if(1==FrameAllStruct->sendDataLen)  
          {
             if(0==FrameAllStruct->sendData[0])
             {
                 PRINTF_BEFORE1 printf("$$停止录制视频\r\n");
             }
             else  if(1==FrameAllStruct->sendData[0])
             {
                 PRINTF_BEFORE1 printf("$$开始录制视频\r\n");
              
             }
             else
             {
                PRINTF_BEFORE1 printf("$$录制视频命令 命令出错=%d\r\n",FrameAllStruct->sendData[0]);
                 return 1;
             }           
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$录制视频命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }                
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK      
          FrameAllStruct->sendDataLen=1;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x07;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
 
    
     if((0x04==FrameAllStruct->cmdSet)&&(0x08==FrameAllStruct->cmdID))              //设置拍摄照片模式
     {
          PRINTF_BEFORE1 printf("$$获取录像状态\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取录像状态命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }                
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK 
          FrameAllStruct->sendData[1]=0;                                            //固定为没有录像 
          FrameAllStruct->sendData[2]=0x08;                                         //固定为0 录像时间 
          FrameAllStruct->sendData[3]=0;                                            //固定为0 录像时间           
          FrameAllStruct->sendDataLen=4;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x08;                                               //命令细分
          
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     

     if((0x04==FrameAllStruct->cmdSet)&&(0x09==FrameAllStruct->cmdID))              //SD卡参数
     {
          PRINTF_BEFORE1 printf("$$获取SD卡参数\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取SD卡参数命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }                
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK 
          FrameAllStruct->sendData[1]=0x03;                                            //SD卡状态 验证
          FrameAllStruct->sendData[2]=0;                                            //
          
          FrameAllStruct->sendData[3]=0x00;                                         //  
          FrameAllStruct->sendData[4]=0x10;                                            //
          FrameAllStruct->sendData[5]=0;                                            //
          FrameAllStruct->sendData[6]=0;                                            //  

          FrameAllStruct->sendData[7]=0;                                         //剩余镜头  
          FrameAllStruct->sendData[8]=0x08;                                            //
          FrameAllStruct->sendData[9]=0;                                            //
          FrameAllStruct->sendData[10]=0;                                            //    


          FrameAllStruct->sendData[11]=0x64;                                         //其余镜头  
          FrameAllStruct->sendData[12]=0;                                            //
          FrameAllStruct->sendData[13]=0;                                            //
          FrameAllStruct->sendData[14]=0;                                            //   

            //剩余录像时间
          FrameAllStruct->sendData[15]=0x70;                                         //其余镜头  
          FrameAllStruct->sendData[16]=0x17;                                            //
          FrameAllStruct->sendData[17]=0;                                            //
          FrameAllStruct->sendData[18]=0;                                            //   
          
          FrameAllStruct->sendDataLen=19;                                            //数据长度为19
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
                        
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
                                
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x09;                                               //命令细分
           
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
      } 
 
    
     if((0x04==FrameAllStruct->cmdSet)&&(0x0A==FrameAllStruct->cmdID))        //相机命令中的 格式SD卡
     {
        PRINTF_BEFORE1 printf("$$相机命令集,大类=4,小类=0X0A,格式SD卡 目前程序没处理--###########--\r\n");
        return 1;
     }
     
    
       
     if((0x04==FrameAllStruct->cmdSet)&&(0x21==FrameAllStruct->cmdID))              //获取计量模式
     {
          PRINTF_BEFORE1 printf("$$获取计量模式\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取计量模式 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=2;                                            //平均计量
        
          FrameAllStruct->sendDataLen=2;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x21;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
      if((0x04==FrameAllStruct->cmdSet)&&(0x23==FrameAllStruct->cmdID))              //获取点计量区模式
     {
          PRINTF_BEFORE1 printf("$$获取点计量区模式\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取点计量区模式 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=0x46;                                            //测点位置坐标UIT8
        
          FrameAllStruct->sendDataLen=2;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x23;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
     
     if((0x04==FrameAllStruct->cmdSet)&&(0x31==FrameAllStruct->cmdID))              // 获取焦点模式
     {
          PRINTF_BEFORE1 printf("$$获取焦点模式\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取焦点模式数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
          
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=1;                                            //手动对焦
                                       
        
          FrameAllStruct->sendDataLen=2;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x31;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
      if((0x04==FrameAllStruct->cmdSet)&&(0x35==FrameAllStruct->cmdID))              // 获取数字缩放因子
     {
          PRINTF_BEFORE1 printf("$$获取数字缩放因子\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取数字缩放因子 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
          
          //02 06 00 20 61 
          FrameAllStruct->sendData[0]=0x02;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=0x06;                                      
          FrameAllStruct->sendData[2]=0x00;                              
          FrameAllStruct->sendData[3]=0x20; 
          FrameAllStruct->sendData[4]=0x61; 
          FrameAllStruct->sendDataLen=5;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x35;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
       if((0x04==FrameAllStruct->cmdSet)&&(0x36==FrameAllStruct->cmdID))           //获取光学变焦规范
     {
          PRINTF_BEFORE1 printf("$$获取光学变焦规范\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取光学变焦规范 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
          
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK 
          FrameAllStruct->sendData[1]=0x9a;                                         //00 9A 01 78 00 0A 00
          FrameAllStruct->sendData[2]=0x01;                          
          FrameAllStruct->sendData[3]=0x78;
          FrameAllStruct->sendData[4]=0x00;
          FrameAllStruct->sendData[5]=0x0a;
          FrameAllStruct->sendData[6]=0x00;
          FrameAllStruct->sendDataLen=7;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x36;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
     if((0x04==FrameAllStruct->cmdSet)&&(0x38==FrameAllStruct->cmdID))              // 获得光学变焦焦距
     {
          PRINTF_BEFORE1 printf("$$获得光学变焦焦距\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获得光学变焦焦距 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
          
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=0x78;                                         // 00 78 00 
          FrameAllStruct->sendData[2]=0x00;                                
          FrameAllStruct->sendDataLen=3;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x38;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
          
     if((0x04==FrameAllStruct->cmdSet)&&(0x39==FrameAllStruct->cmdID))              //获取光学缩放因子
     {
          PRINTF_BEFORE1 printf("$$获取光学缩放因子\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取光学缩放因子 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
          
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=0;                                            //浮动，因素。有效范围从1到三十
          FrameAllStruct->sendData[2]=0;
          FrameAllStruct->sendData[3]=0x80;          
          FrameAllStruct->sendData[4]=0x3f;                                         
        
          FrameAllStruct->sendDataLen=5;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x39;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
      if((0x04==FrameAllStruct->cmdSet)&&(0x43==FrameAllStruct->cmdID))              //获取点辅助设置
     {
          PRINTF_BEFORE1 printf("$$获取点辅助设置\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取点辅助设置 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
          
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=0x5b;                                            //00 5b
                                               
        
          FrameAllStruct->sendDataLen=2;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x43;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
     
      if((0x04==FrameAllStruct->cmdSet)&&(0x44==FrameAllStruct->cmdID))              //获取焦点环值上界
     {
          PRINTF_BEFORE1 printf("$$获取焦点环值上界\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取焦点环值上界 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
          
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=0xe8;                                            //00 E8 03
          FrameAllStruct->sendData[2]=0x03;
 
          FrameAllStruct->sendDataLen=3;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x44;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
     
     
     if((0x04==FrameAllStruct->cmdSet)&&(0x46==FrameAllStruct->cmdID))              //获取焦点环值
     {
          PRINTF_BEFORE1 printf("$$获取焦点环值\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取焦点环值 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
          
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=0xf4;                                            //00 F4 01
          FrameAllStruct->sendData[2]=0x01;
 
          FrameAllStruct->sendDataLen=3;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0x46;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
     
       //获取相机命令集支持状态
     if((0x04==FrameAllStruct->cmdSet)&&(0xFF==FrameAllStruct->cmdID))            
     {
          PRINTF_BEFORE1 printf("$$获取相机命令集支持状态命令\r\n");
 
          if(0==FrameAllStruct->sendDataLen)  
          {
                ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取相机命令集支持状态命令 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
            
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          
          FrameAllStruct->sendData[1]=0X01;                                         //仅支摄像机命令集
          //FrameAllStruct->sendData[1]=0X5d;                                //仅支摄像机命令集 //第6为要为1 主要 大疆协议没说
          FrameAllStruct->sendDataLen=2;                                            //数据长度为2
          
          PRINTF_BEFORE1 printf("$$数据帧长度=1\r\n");                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x04;                                              //命令大类
          FrameAllStruct->cmdID=0xFF;                                               //命令细分
          

          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复   
                 
     } 
    
     
     /*
      if((0x05==FrameAllStruct->cmdSet)&&(0xff==FrameAllStruct->cmdID))              //获取万向节命令集支持状态
     {
          PRINTF_BEFORE1 printf("$$获取万向节命令集支持状态\r\n");
        
          if(0==FrameAllStruct->sendDataLen)  
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取万向节命令集支持状态 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
          
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=0x65;                                            //00 65
   
 
          FrameAllStruct->sendDataLen=2;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x05;                                              //命令大类
          FrameAllStruct->cmdID=0xff;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     
    
     
     if((0x05==FrameAllStruct->cmdSet)&&(0x01==FrameAllStruct->cmdID))              //获取万向节状态
     {
          PRINTF_BEFORE1 printf("$$获取万向节状态\r\n");
        
          if(1==FrameAllStruct->sendDataLen)  //数据帧长度是一  大疆和协议对不起来  cocoguojia
          {
            ;
          }  
          else
          {
               PRINTF_BEFORE1 printf("$$获取万向节状态 数据帧长度出错=%d\r\n",FrameAllStruct->sendDataLen);
               return 1;
          }               
           
          memset(FrameAllStruct->sendData,0,255);                                   //这里得清零 因为如果Enc有加密需求 那么填补自动就是0  
          
          FrameAllStruct->sendData[0]=0;                                            //返回码 执行成功 ACK
          FrameAllStruct->sendData[1]=0x01;                                          //00 01 00 00 00 00 00 00 00
          FrameAllStruct->sendData[2]=0x00; 
          FrameAllStruct->sendData[3]=0x00; 
          FrameAllStruct->sendData[4]=0x00; 
          FrameAllStruct->sendData[5]=0x00; 
          FrameAllStruct->sendData[6]=0x00; 
          FrameAllStruct->sendData[7]=0x00; 
          FrameAllStruct->sendData[8]=0x00; 
   

          FrameAllStruct->sendDataLen=9;                                            //数据长度为5
          
          PRINTF_BEFORE1 printf("$$数据帧长度= %d\r\n",FrameAllStruct->sendDataLen);                                
         
          FrameAllStruct->ackType_Ack=0;                                            //不用回ACK
          FrameAllStruct->ackType_Type=1;                                           //ACK包
         
                
          FrameAllStruct->paddingEnc_padding=0;                                     //填充包为0
         
          if(1==FrameAllStruct->paddingEnc_Enc)                                     //这里如果为1 就说明接收的信息为加密 但在之前处理已经解密了 所以在这只说明曾经加密了
          {
            FrameAllStruct->paddingEnc_Enc=1;                                       //加密
          }
          else
          {
            FrameAllStruct->paddingEnc_Enc=0;                                       //不加密
          }
         
                         
          FrameAllStruct->cmdSet=0x05;                                              //命令大类
          FrameAllStruct->cmdID=0x01;                                               //命令细分
          
  
          Bsp_Usart3SendOfPsdkFrame(FrameAllStruct);                                //收到ID验证命令 所以发送回复
     } 
     */
      
     return 0;
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//二级内部函数

/*
//-----------------------------------------------------------------------------------------------------------------------------
//功能:根据接到的16字节随机数据和用户信息USER_APP_KEY 进行MD5算法得出秘钥写进指针uint8_t *pKey里
//形式参数:  uint8_t *pKey=存放MD5计算结果秘钥的指针 结果16字节长度 
//          uint8_t *randomData=随机数 16字节
static void PsdkProtoUpper_CalTransferKey(uint8_t *pKey,uint8_t *randomData)
{
    uint8_t *pCal;
    MD5_CTX md5Ctx;  
    uint8_t *calBuf = pvPortMalloc(strlen(USER_APP_KEY)+17);
 
   //key + random = md5
    pCal = calBuf;
    memcpy(pCal, USER_APP_KEY, strlen(USER_APP_KEY));  
    memcpy(pCal+strlen(USER_APP_KEY), &randomData, 16);
    
    md5_init(&md5Ctx);//md5秘钥初始化 先将  MD5_CTX md5Ctx 结构体赋值
    md5_update(&md5Ctx, calBuf,strlen(USER_APP_KEY)+16);//&md5Ctx 刚初始化后的用于生产秘钥的结构体的指针, md5InputData=信息指针, pCal - md5InputData=信息长度
    md5_final(&md5Ctx, pKey);//再进行MD5算法 生产秘保存在 pKey里

    vPortFree(calBuf);//释放 缓存
}
*/

typedef struct {
    uint8_t ackCode;    /*!< Specifies PSDK command ack code.
                             This parameter can be any value of ::E_PsdkCmdAckCode. */
    uint8_t md5[16];    /*!< MD5 value calculated by payload. */
} T_LoadStateIdentityVerifyAck;

static uint8_t *Str_PutStrToBuff(uint8_t *pBuff, char *str, uint32_t maxStrLen);

/**
 * @brief Payload identity verify.
 * @param protHandle Pointer to PSDK protocol handle.
 * @param inProtInfo Pointer to PSDK protocol information of data package.
 * @param inProtData Pointer to data field of package.
 * @return PSDK function process state.
 
 
 inProtData 16字节随机数  inProtInfo APP_KEY 33字节长度
 */
static void PsdkPayloadStateCmd_IdentityVerify(uint8_t *outdatapr,uint8_t *inProtData)
{
    uint8_t calBuf[33 + 17];
    uint8_t *pCal;
    MD5_CTX md5;

    memset(calBuf,0,33+17);
    //key + random = md5
    pCal = calBuf;
    pCal = Str_PutStrToBuff(pCal, USER_APP_KEY, strlen(USER_APP_KEY));
    pCal = Str_PutStrToBuff(pCal, (char *) inProtData, 16);
    
   // PRINTF_BEFORE1 cocoguojia_usartNPutTableForNum(1,calBuf,33+17);
   
    md5_init(&md5);
    md5_update(&md5, calBuf, pCal - calBuf);
    md5_final(&md5, outdatapr);
    
}

static uint8_t *Str_PutStrToBuff(uint8_t *pBuff, char *str, uint32_t maxStrLen)
{
    uint32_t len;
    if (strlen(str) > maxStrLen) {
        len = maxStrLen;
    } else {
        len = strlen(str);
    }

    memcpy(pBuff, str, len);

    return (pBuff + len);
}




//--------------------------------------------------------------------------------------------------
//功能:按照格式要求发送数据给 大疆环形接口板
//形式参数:PSDK 数据格式结构体指针
static uint8_t Bsp_Usart3SendOfPsdkFrame(T_PsdkProtFrameAllStruct *FrameAllStruct)
{  
     uint16_t LenTemp=0;
    
     uint8_t *sendPtr = pvPortMalloc(255);
    
     uint8_t *sendBufPtr =sendPtr;
     uint8_t *sendBufPtrTemp =sendPtr;
    
     memset(sendPtr,0,255);

     FrameAllStruct->sof=0XAA;
    
     FrameAllStruct->lenVer_Ver=0;
     
     FrameAllStruct->ackType=FrameAllStruct->ackType_Type;
     FrameAllStruct->ackType<<=5;
     FrameAllStruct->ackType+=FrameAllStruct->ackType_Ack; 
   
     FrameAllStruct->reserve=0;
    
     FrameAllStruct->lenVer_len=(PSDK_PROTHEADERLEN+FrameAllStruct->sendDataLen)+PSDK_PROTHEADECRC32RLEN;
    
     PRINTF_BEFORE1 printf("$$整理数据准备发送阶段\r\n");
    
    //确定一下  FrameAllStruct->paddingEnc_padding   长度
    if (0 == FrameAllStruct->paddingEnc_Enc)        //不加密
    {
       LenTemp=FrameAllStruct->sendDataLen;
    } 
    else if (1 == FrameAllStruct->paddingEnc_Enc)   //加密
    {
         PRINTF_BEFORE1 printf("$$数据帧加密中\r\n");
      
        if (FrameAllStruct->sendDataLen % 16 == 0) 
        {
            ;
        }
        else 
        {
            if(FrameAllStruct->sendDataLen>241)
            {
                return 1;
            }
            else
            {    
                LenTemp= (uint16_t) ((FrameAllStruct->sendDataLen / 16 + 1) * 16);
            }
            
        }
        
        FrameAllStruct->paddingEnc_padding = (uint8_t) (LenTemp - FrameAllStruct->sendDataLen);
        
        PRINTF_BEFORE1 printf("$$数据帧加密后填补的长度=%d\r\n",  FrameAllStruct->paddingEnc_padding);
        
        FrameAllStruct->lenVer_len+=FrameAllStruct->paddingEnc_padding;
        
        PRINTF_BEFORE1 printf("$$要发送的数据总长度=%d\r\n", FrameAllStruct->lenVer_len);
        
        PRINTF_BEFORE1 printf("$$数据帧加密前为:\r\n");
        PRINTF_BEFORE1 printf("$$######################\r\n");
        PRINTF_BEFORE1 cocoguojia_usartNPutTableForNum(1,(&FrameAllStruct->sendData[0]),LenTemp);
        PRINTF_BEFORE1 printf("\r\n######################\r\n");
    } 
    else 
    {
        return 2;
    }
    

    
    if (1 == FrameAllStruct->paddingEnc_Enc)   //加密
    {
        PRINTF_BEFORE1 printf("$$执行数据帧加密 密钥为:\r\n");
        
         PRINTF_BEFORE1 printf("$$######################\r\n");
         PRINTF_BEFORE1 cocoguojia_usartNPutTableForNum(1,g_keyTable,16);
         PRINTF_BEFORE1 printf("\r\n######################\r\n");        

         PsdkAes_Encrypt((const uint8_t *)FrameAllStruct->sendData,(uint8_t *)(sendBufPtrTemp+PSDK_PROTHEADERLEN), (LenTemp),(uint8_t const*)g_keyTable,(uint16_t)(128));
    }
    else
    {
        if(0!=FrameAllStruct->sendDataLen)
        {
            memcpy(sendPtr+PSDK_PROTHEADERLEN,&(FrameAllStruct->sendData[0]),(FrameAllStruct->sendDataLen+FrameAllStruct->paddingEnc_padding));
        }
        else
        {
            ;
        }  
    }    

     FrameAllStruct->lenVer=FrameAllStruct->lenVer_Ver;
     FrameAllStruct->lenVer<<=10;
     FrameAllStruct->lenVer+=FrameAllStruct->lenVer_len;
     
     
     FrameAllStruct->paddingEnc=FrameAllStruct->paddingEnc_Enc;
     FrameAllStruct->paddingEnc<<=5;   
     FrameAllStruct->paddingEnc+=FrameAllStruct->paddingEnc_padding;
      
     
     *sendBufPtr=FrameAllStruct->sof;
     sendBufPtr++;
     
     *sendBufPtr=(uint8_t)(FrameAllStruct->lenVer);
     sendBufPtr++;
     *sendBufPtr=(uint8_t)((FrameAllStruct->lenVer)>>8);
     sendBufPtr++;
     
     *sendBufPtr=FrameAllStruct->ackType;
     sendBufPtr++;
     
     *sendBufPtr=FrameAllStruct->paddingEnc;
     sendBufPtr++;
     
     *sendBufPtr=FrameAllStruct->reserve;
     sendBufPtr++;
     
     *sendBufPtr=FrameAllStruct->cmdSet;
     sendBufPtr++;
     
     *sendBufPtr=FrameAllStruct->cmdID;
     sendBufPtr++;
     
     *sendBufPtr=(uint8_t )(FrameAllStruct->seqNum);
     sendBufPtr++; 
     *sendBufPtr=(uint8_t )((FrameAllStruct->seqNum)>>8);
     sendBufPtr++;
     PRINTF_BEFORE1 printf("$$seqNum=%d\r\n",FrameAllStruct->seqNum);     
     
     FrameAllStruct->crc16  = PsdkCrc_Crc16(sendPtr, PSDK_PROTHEADEBEFORERLEN);
     PRINTF_BEFORE1 printf("$$crc16=%04X\r\n", FrameAllStruct->crc16);
     
     *sendBufPtr=(uint8_t)(FrameAllStruct->crc16);
     sendBufPtr++;  
     *sendBufPtr=(uint8_t)((FrameAllStruct->crc16)>>8);
     
     sendBufPtr+=LenTemp;
     
     FrameAllStruct->crc32  = PsdkCrc_Crc32(sendBufPtrTemp,(PSDK_PROTHEADERLEN+LenTemp));
     PRINTF_BEFORE1 printf("$$参加CRC32校验的字节长度为=%d\r\n", PSDK_PROTHEADERLEN+LenTemp);
     PRINTF_BEFORE1 printf("$$crc32=%08X\r\n", FrameAllStruct->crc32);
     sendBufPtr++;  
     *sendBufPtr=(uint8_t)(FrameAllStruct->crc32);
     sendBufPtr++;  
     *sendBufPtr=(uint8_t)((FrameAllStruct->crc32)>>8);
     sendBufPtr++;
     *sendBufPtr=(uint8_t)((FrameAllStruct->crc32)>>16);
     sendBufPtr++;
     *sendBufPtr=(uint8_t)((FrameAllStruct->crc32)>>24);
         
     PRINTF_BEFORE1 printf("$$发送字节长度=%d\r\n",FrameAllStruct->lenVer_len);
    
     PRINTF_BEFORE1 printf("$$##################\r\n");
     PRINTF_BEFORE1 cocoguojia_usartNPutTableForNum(1,sendPtr,FrameAllStruct->lenVer_len);                           
     PRINTF_BEFORE1 printf("\r\n##################\r\n--------\r\n");
       
     Bsp_Usart3SendArray(sendPtr,FrameAllStruct->lenVer_len);
    
     vPortFree(sendPtr);     //释放 缓存

     return 0; 
}






