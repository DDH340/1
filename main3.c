#include "sys.h" 
#include "delay.h" 
#include "LED.h" 
#include "stm32f4xx.h"
#include "time.h"
#include "key.h"

u8 key;
u8 tim3_config_flag = 0; // 0: 模式1; 1: 模式2
void TIM3_Int_Init(uint16_t arr, uint16_t psc);
int main(void) { 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 设置中断优先级分组
    delay_init(168);     // 初始化延时函数（主频168MHz）
    LED_Init();          // 初始化LED
    KEY_Init();          // 初始化按键
    TIM3_Int_Init(2000-1, 8400-1);  // 定时器初始化（默认500ms）
    
    while(1) {
    key = KEY_Scan(0); // 扫描按键（不支持连按）
    if (key) {
        switch (key) {
            case WKUP_PRES: // WKUP 按键按下
                // 切换定时器配置
                if (tim3_config_flag == 0) {
                    // 切换到模式2
                    TIM3_Int_Init(10000 - 1, 8400 - 1); // ARR=9999, PSC=8399
                    tim3_config_flag = 1;
                } else {
                    // 切换回模式1
                    TIM3_Int_Init(2000 - 1, 8400 - 1); // ARR=1999, PSC=8399
                    tim3_config_flag = 0;
                }
            
        }
    } else {
        delay_ms(10); // 无按键时延时
    }
				//LED0=!LED0;
     // delay_ms(200);//延时 200ms
    }
	}


