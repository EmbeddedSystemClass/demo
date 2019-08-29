 
#include "u2rece_task.h"
#include "psdk_debug.h" 
 //cocoguojia say:
 //RUN_TASK 此任务负责具体的逻辑 配合一些关键的状态标志 控制系统的走向
 //在系统状态里面 才能改变系统状态标志

 

uint8_t 	g_camAllStateFlag=0;						//2=相机有开的有关的 0=相机全体都关机 1=相机全体都开机


uint8_t 	g_keyEnNum=0;								//按键被按下的次数 累计到255就会回0
uint32_t 	g_oledStateFlag=0;							//0表示不运行

uint16_t  	g_tim5PlauseUs=0;							//检测到的PWM脉宽宽度
uint16_t  	g_tim5TriggerPlauseUs=0;					//触发时 检测到的PWM脉宽宽度
uint16_t  	g_tim5CatchNum=0;							//捕获并且被触发的有效总次数


uint8_t 	g_capture_FinishAndStartPhotoFlag=0;		//捕捉结束并且要进行拍照流程标志 1=开始拍照流程


uint8_t  	g_camPowerFlagTable[6]={0,0,0,0,0,0};       //5个相机的开关机标志


uint16_t  	g_photoPicNum[6]={0,0,0,0,0,0};         	//5个相机分别的照片数


uint8_t  	g_camRtkFlagTable[6]={0,0,0,0,0,0};         //5个相机的热靴有效标志


uint8_t  	g_readingFlag=0;							//1=正在读中  0=正常工作状态
uint8_t  	g_lastReadingFlag=0;						//上次状态 1=正在读中  0=正常工作状态

uint8_t     g_setModelFlag=0;							//1=进入到设置模式


uint8_t     g_signalOrAutoModelFlag=2;						//2=signal模式   4=auto 2s  6=auto 3s
//uint8_t   g_rtkModelFlag=0;	                                //1=rtk 0=非rtk



uint16_t    g_ttlCurrentNum=0;							//当前触发计数
uint16_t    g_ttlTriggerNum=0;							//触发是的触发计数



uint8_t     g_currentRowNum=2;						    //当前行 Num 当前行 OLED的当前行 范围=2 4 6  2=signal模式   4=auto 2s  6=auto 3s

uint8_t 	g_camTaskCanTakeOverFlag=0;					//cam任务可以接管标志 1=cam任务可以接管 
uint32_t 	g_offOrOnTureStateFlag=SYSTEMSTATE_GOTOSHUT;
uint8_t  	g_camLoseFlagTable[6]={0,0,0,0,0,0};        //5个相机的存在标志 即不丢失 上电后会检查一次

uint8_t     g_temperatureDelayLastFlag=2;               //温度延迟标志 0=上次是从小到大使能风扇  1=上次是从大到小关闭风扇  2=风扇功能暂时关闭  注意开机后计时5分子后才准开风扇 关机了就时间清零


uint8_t     g_theTurelostNumTable[6]={0,0,0,0,0,0};     //实际丢片数

uint8_t     g_ppsRtkHealthNum=0;    //ppsRtk定位质量数值


uint8_t g_9999Flag=0;//1=utc获取精准定位不成功

unsigned portBASE_TYPE uxHighWaterMarku2rece; 


//-------------------------------------------------------------------------
//任务函数
void  U2RECE_TASK (void const * argument)
{ 
    uint8_t  tableCamNum=0;//相机开关机状态
    uint8_t  tableCamPowerState=0;//相机开关机状态
    uint16_t tablePhotoNum=0;//照片的张数
        
    uint8_t dat;
    uint8_t n;
    int res=0;

	  uxHighWaterMarku2rece=uxTaskGetStackHighWaterMark( U2RECE_TASK_Handle );    //查询大小  
	  PSDK_LOG_DEBUG("u2rece_TASK  %d\r\n", uxHighWaterMarku2rece );
	while (1) 
	{
       res=0;
       res=cocoguojia_usartReadDataForlen(2,&dat,1);
       if(0<res)
       {

            if('@'==dat)
            {
                n=1;
            }
            switch(n)
            {
                case 0:
                break;
                
                case 1:  
                n++;
                break;                  
                
                case 2:
                if(('1'<=dat)&&('5'>=dat))
                {
                    tableCamNum=dat-'0';
                    n++;
                }
                else
                {
                    n=0;
                }    
                break;
                
                case 3:
                if(('1'<=dat)&&('2'>=dat))
                {
                    if('1'==dat)
                    {
                        tableCamPowerState=1;
                    }
                    else
                    {
                         tableCamPowerState=0;
                    }  
                    n++;
                }
                else
                {
                    n=0;
                }    
                break;
                
                case 4:
                if(('0'<=dat)&&('5'>=dat))
                {
                    tablePhotoNum=dat-'0';
                    n++;
                }
                else
                {
                    n=0;
                }    
                break;
                
                case 5:
                if(('0'<=dat)&&('9'>=dat))
                {
                    tablePhotoNum*=10;
                    tablePhotoNum+=(dat-'0');
                    n++;
                }
                else
                {
                    n=0;
                }    
                break;
                
                case 6:
                if(('0'<=dat)&&('9'>=dat))
                {
                    tablePhotoNum*=10;
                    tablePhotoNum+=(dat-'0');
                    n++;
                }
                else
                {
                    n=0;
                }    
                break;
                
                case 7:
                if(('0'<=dat)&&('9'>=dat))
                {
                    tablePhotoNum*=10;
                    tablePhotoNum+=(dat-'0');
                    n++;
                }
                else
                {
                    n=0;
                }    
                break;
                
                case 8:
                if(('0'<=dat)&&('9'>=dat))
                {  
                    g_theTurelostNumTable[tableCamNum]=dat-'0';//在这记录每个相机的丢片真实数                                             
                    n++;
                }
                else
                {
                    n=0;
                }    
                break;
                
                case 9:
                if('#'==dat)
                {   
                    g_camPowerFlagTable[tableCamNum]=tableCamPowerState;      
                    g_photoPicNum[tableCamNum]=tablePhotoNum;                             
                    g_camLoseFlagTable[tableCamNum]=1;      
                }
                n=0;             
                break;
                    
                
                default:
                n=0;
                break;
            }
        }
        else
        {
            osDelay(1);
        }
    }
}
