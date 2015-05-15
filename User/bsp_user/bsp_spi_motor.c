#include <includes.h>
#include "globalavr.h"

/*
*********************************************************************************************************
*
*	ģ������ : AD7192�Ĵ���������غ���
*	˵    �� : 
        AD5722_SendByte-----------SPI���ֽڷ��ͽ���
	           - ����˵������������SPIͨ��ʱ���ڷ��ͺͽ���ʱ��ͬʱ���е�,����������һ���ֽڵ����ݺ�ҲӦ�����ܵ�һ���ֽڵ�����
	             ��1��stm32�ȵȴ��ѷ��͵������Ƿ�����ɣ����û�з�����ɣ����ҽ���ѭ��200�Σ����ʾ���ʹ��󣬷����յ���ֵΪ0;
	             ��2�����������ɣ�stm32��SPI1���߷���TxData
	             ��3��stm32�ٵȴ����յ������Ƿ������ɣ����û�н�����ɣ����ҽ���ѭ��200�Σ����ʾ���մ����򷵻�ֵ0
	             ��4�������������ˣ��򷵻�STm32��ȡ�����µ�����  
       AD5722_RegEdit------------��AD5722ָ����һ����д������8λ
       AD5722_Write-------д�Ĵ�������
       AD5722_Read------���Ĵ�������

*	�޸ļ�¼ :
*		�汾��  ����           ����    ˵��
*		v1.0    2014.12.15      ����     
*
*	Copyright (C), NJUST
*
*********************************************************************************************************
*/
unsigned char MotorChange(unsigned int val)
{
	long int cstr = 0;	
	CPU_SR_ALLOC();
	if(val > 0xFFFFFF)
	{
		return 1;
	}
	//ConfigAD5754R();
	
	CPU_CRITICAL_ENTER(); 
	
	cstr = DAC_Register | DAC_Channel_A | (long int)val;		//VoutA=4.97v
	WriteToAD5754RViaSpi(&cstr);
	cstr = DAC_Register | DAC_Channel_B | (long int)val;		//VoutB=3.32V
	WriteToAD5754RViaSpi(&cstr);
	cstr = Power_Control_Register | PowerUp_ALL;   
	WriteToAD5754RViaSpi(&cstr);	
	
	CPU_CRITICAL_EXIT();
	
	MotorCurOutput = val;
	return 0;
}

unsigned char AD5722_SendByte(unsigned char byte)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){};
  SPI_I2S_SendData(SPI2, byte);
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){};
  return SPI_I2S_ReceiveData(SPI2);
}

void AD5722_RegEdit(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte)
{
	unsigned char WriteData;
  unsigned char i;
		
	for(i=0; i<NumberOfByte; i++)
 	{
	 	WriteData = *(WriteBuffer + i);
		*(ReadBuffer + i) = AD5722_SendByte(WriteData);
	}  
}

void AD5722_Write(unsigned char code)
{
	unsigned char WriteBuf[1];
	unsigned char ReadBuf[1];

	GPIO_ResetBits(GPIOB, GPIO_Pin_12);      //�޸�
  	WriteBuf[0] = code;	 	
    AD5722_RegEdit(WriteBuf, ReadBuf, 1);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);    //�޸�
}

void WriteToAD5754RViaSpi(long int *RegisterData)
{	
	unsigned char WriteBuf[3];
	unsigned char ReadBuf[3];
	
	WriteBuf[0]= *RegisterData>>16;
	WriteBuf[1]= *RegisterData>>8;
  WriteBuf[2]= *RegisterData;

	
	SET_SYNC();
	__nop();__nop();__nop();
	CLR_SYNC();	 /* ���SYNC�ź� */
	__nop();
	

  AD5722_RegEdit(WriteBuf, ReadBuf, 3);

	__nop();__nop();__nop();
		
//	ValueToWrite <<= 1;	//Rotate data
//	Delay(5);

	SET_SYNC();
  __nop();__nop();__nop();

}

void ConfigAD5754R(void)
{
	int i;
	long int *p;
	long int ins[2] = {0, 0};

	ins[0] = Output_Range_Select_Register | Range2_Select | DAC_Channel_ALL;
	ins[1] = Power_Control_Register | PowerUp_ALL;
	p = ins;

	for(i=0; i<2; i++)
	{ 
		WriteToAD5754RViaSpi(p);
		__nop();__nop();__nop();__nop();__nop();   /* NOTE */
		p++;
	}
}

void bsp_InitSPI2(void)
{
	bsp_InitSPI2_GPIO();
	InitSPI2();
}

void bsp_InitSPI2_GPIO(void)
{
	/*��ʼ��GPIO�ܽţ�����GPIO�ĳ�ʼ���ṹ*/	 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;    
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	/*Ƭѡ�ø�*/	
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	
	/* ����MSK��MISO��MOSI���ù��� */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	/*Ƭѡ�ø�*/	

	/* ����MSK��MISO��MOSI���Ź��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void InitSPI2(void)
{
	/*��ʼ��SPI�ܽţ�����SPI�ĳ�ʼ���ṹ*/	 
	SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	
	
	/* �Ƚ�ֹSPI  */
	SPI_Cmd(SPI2, DISABLE);		
	/*����SPI1�Ĳ���SPI�ķ��򡢹���ģʽ������֡��ʽ��CPOL��CPHA��NSS�������Ӳ����SPIʱ�ӡ����ݵĴ���λ���Լ�CRC*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_ERR, ENABLE);
	SPI_Cmd(SPI2, ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void SPI2_IRQHandler(void)
{
	OSIntEnter();
	if(SPI_I2S_GetITStatus(SPI2, SPI_IT_CRCERR) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI2, SPI_IT_CRCERR);
		MotorCommErr++;
	}
	
	if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_OVR) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_OVR);
		MotorCommErr++;
	}	
	
	if(SPI_I2S_GetITStatus(SPI2, SPI_IT_MODF) == SET)
	{
		SPI_I2S_ClearITPendingBit(SPI2, SPI_IT_MODF);
		MotorCommErr++;
	}
	OSIntExit();
}

