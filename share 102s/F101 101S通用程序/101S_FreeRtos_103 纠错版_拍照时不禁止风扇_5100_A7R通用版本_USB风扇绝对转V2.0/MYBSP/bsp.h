

#ifndef __BSP__H
#define __BSP__H



#define PRINTF_BEFORE
//#define PRINTF_BEFORE  if(0==1)

//------------------------------------------------------------------------------
//������״̬���
#define READ_ENING			(0==PBin(10))
#define READ_NOING			(1==PBin(10))


//------------------------------------------------------------------------------
//������
#define FMQIO_OFF    	PAout(11)=0
#define FMQ_ON			cocoguojia_tim1Enable()
#define FMQ_OFF    		cocoguojia_tim1Disable()


//------------------------------------------------------------------------------
//����
#define KEY_ENING			(0==PBin(11))

//------------------------------------------------------------------------------
//KEY LED
#define KEY_LED_ON			PBout(12)=0
#define KEY_LED_OFF    		PBout(12)=1



//------------------------------------------------------------------------------
//FAN
#define FAN_ON			PBout(8)=1
#define FAN_OFF    		PBout(8)=0


//------------------------------------------------------------------------------
//ָʾ��
#define LEDPOINT_ON			PAout(1)=0
#define LEDPOINT_OFF    	PAout(1)=1



//------------------------------------------------------------------------------
//TTLʹ�ܺ� �ܽ�״̬���
#define READTTL_IS0			(0==PAin(0))
#define READTTL_IS1			(1==PAin(0))


/* ���� */
#define PHOTO1_EN 		PAout(12)=0                     //���޸�
#define PHOTO1_NO 		PAout(12)=0
#define PHOTO2_EN 		PAout(12)=0                     //���޸�
#define PHOTO2_NO 		PAout(12)=0
#define PHOTO3_EN 		PAout(12)=0                     //���޸�
#define PHOTO3_NO 		PAout(12)=0
#define PHOTO4_EN 		PAout(12)=0                     //���޸�
#define PHOTO4_NO 		PAout(12)=0
#define PHOTO5_EN 		PAout(12)=0                     //���޸�
#define PHOTO5_NO 		PAout(12)=0

#define PHOTOALL_EN 		PAout(12)=0                     //���޸�
#define PHOTOALL_NO 		PAout(12)=0





/* ���ػ� */
#define POWER_EN 		PAout(12)=0                     //���޸�
#define POWER_NO 		PAout(12)=0


/* OK���� */
#define OK_EN 			PAout(12)=0                     //���޸�
#define OK_NO 			PAout(12)=0  



extern void BSP_Init(void);


#endif




