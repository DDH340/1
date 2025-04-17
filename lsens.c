#include "lsens.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "adc.h"
//��ʼ������������
void Lsens_Init(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ�� GPIOF ʱ��
 //�ȳ�ʼ�� ADC3 ͨ�� 7IO ��
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PA7 ͨ�� 7
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
 GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ�� 
Adc_Init();//��ʼ�� ADC3
}
//��ȡ Light Sens ��ֵ
//0~100:0,�;100,����
u8 Lsens_Get_Val(void)
{
u32 temp_val=0;
u8 t;
for(t=0;t<LSENS_READ_TIMES;t++)
{
temp_val+=Get_Adc(ADC_Channel_5); //��ȡ ADC ֵ,ͨ�� 5
delay_ms(5);
}
temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ
if(temp_val>4000)temp_val=4000;
return (u8)(100-(temp_val/40));
}