#include "LED.h"
#include "exti.h"
#include "beep.h"
#include "key.h" 
#include "delay.h" 
#include "stm32f4xx.h"
void EXTIX_Init(void)
{
NVIC_InitTypeDef NVIC_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
KEY_Init(); //按键对应的 IO 口初始化
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能 SYSCFG 时钟
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 连接线 2
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE3 连接线 3
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE4 连接线 4
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 连接线 0
 /* 配置 EXTI_Line0 */
 EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能 LINE0
 EXTI_Init(&EXTI_InitStructure);
/* 配置 EXTI_Line2,3,4 */
EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
 EXTI_Init(&EXTI_InitStructure);//配置
NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断 0
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级 0
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
 NVIC_Init(&NVIC_InitStructure);//配置 NVIC
NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断 2
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级 3
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
 NVIC_Init(&NVIC_InitStructure);//配置 NVIC
NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断 3
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级 2
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
 NVIC_Init(&NVIC_InitStructure);//配置 NVIC
NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//外部中断 4
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级 1
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
 NVIC_Init(&NVIC_InitStructure);//配置 NVIC 
}
//外部中断 0 服务程序
void EXTI0_IRQHandler(void)
{ 
delay_ms(10); //消抖
if(WK_UP==1)
{ BEEP=!BEEP; //蜂鸣器翻转
}
EXTI_ClearITPendingBit(EXTI_Line0); //清除 LINE0 上的中断标志位
}
//外部中断 2 服务程序
void EXTI2_IRQHandler(void)
{ 
delay_ms(10); //消抖
if(KEY2==0) 
{ LED0=!LED0; 
}
EXTI_ClearITPendingBit(EXTI_Line2);//清除 LINE2 上的中断标志位
}
//外部中断 3 服务程序
void EXTI3_IRQHandler(void)
{ 
delay_ms(10); //消抖
if(KEY1==0)
{ LED1=!LED1;
}
EXTI_ClearITPendingBit(EXTI_Line3); //清除 LINE3 上的中断标志位 
}
//外部中断 4 服务程序
void EXTI4_IRQHandler(void)
{ 
delay_ms(10); //消抖
if(KEY0==0)
{ LED0=!LED0;
LED1=!LED1;
}
EXTI_ClearITPendingBit(EXTI_Line4);//清除 LINE4 上的中断标志位 
} 
//外部中断初始化程序
//初始化 PE2~4,PA0 为中断输入.
