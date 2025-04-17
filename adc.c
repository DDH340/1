#include "adc.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "lsens.h"

//初始化 ADC
void Adc_Init(void)
{ 
 GPIO_InitTypeDef GPIO_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
ADC_InitTypeDef ADC_InitStructure;
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能 PA 时钟
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);//使能 ADC1 时钟
 //先初始化 IO 口
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;// 下拉
 GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;// 下拉
 GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;// 下拉
 GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
 
RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE); //ADC1 复位
RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE); //复位结束

 ADC_TempSensorVrefintCmd(ENABLE);//使能内部温度传感器

 ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
 ADC_CommonInitStructure.ADC_TwoSamplingDelay =ADC_TwoSamplingDelay_5Cycles;
 ADC_CommonInitStructure.ADC_DMAAccessMode =ADC_DMAAccessMode_Disabled;
 ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
 ADC_CommonInit(&ADC_CommonInitStructure);
 ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12 位模式
 ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式
 ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
 ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
ADC_InitStructure.ADC_NbrOfConversion = 1;//1 个转换在规则序列中
 ADC_Init(ADC1, &ADC_InitStructure);
ADC_Cmd(ADC3, ENABLE);//开启 AD 转换器
}
//得到温度值
//返回值:温度值(扩大了 100 倍,单位:℃.)
short Get_Temprate(void)
{
u32 adcx; short result;
double temperate;
adcx=Get_Adc_Average(ADC_Channel_16,20); //读取通道 16,20 次取平均
temperate=(float)adcx*(3.3/4096); //电压值
temperate=(temperate-0.76)/0.0025+25; //转换为温度值
result=temperate*=100; //扩大 100 倍.
return result;
}


u16 Get_Adc(u8 ch) 
{
//设置指定 ADC 的规则组通道，一个序列，采样时间
ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_480Cycles ); 
ADC_SoftwareStartConv(ADC3); //使能指定的 ADC1 的软件转换启动功能
while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//等待转换结束
return ADC_GetConversionValue(ADC3); //返回最近一次 ADC1 规则组的转换结果
}
//获取通道 ch 的转换值，取 times 次,然后平均
//ch:通道编号 times:获取次数
//返回值:通道 ch 的 times 次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
u32 temp_val=0; u8 t;
for(t=0;t<times;t++)
{
temp_val+=Get_Adc(ch); 
	delay_ms(5);
}
return temp_val/times;
}
