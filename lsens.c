#include "lsens.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "adc.h"
//初始化光敏传感器
void Lsens_Init(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能 GPIOF 时钟
 //先初始化 ADC3 通道 7IO 口
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PA7 通道 7
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
 GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化 
Adc_Init();//初始化 ADC3
}
//读取 Light Sens 的值
//0~100:0,最暗;100,最亮
u8 Lsens_Get_Val(void)
{
u32 temp_val=0;
u8 t;
for(t=0;t<LSENS_READ_TIMES;t++)
{
temp_val+=Get_Adc(ADC_Channel_5); //读取 ADC 值,通道 5
delay_ms(5);
}
temp_val/=LSENS_READ_TIMES;//得到平均值
if(temp_val>4000)temp_val=4000;
return (u8)(100-(temp_val/40));
}