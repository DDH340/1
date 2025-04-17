#include "sys.h" 
#include "delay.h" 
#include "LED.h" 
#include "stm32f4xx.h"
#include "time.h"
#include "key.h"

u8 key;
u8 tim3_config_flag = 0; // 0: ģʽ1; 1: ģʽ2
void TIM3_Int_Init(uint16_t arr, uint16_t psc);
int main(void) { 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // �����ж����ȼ�����
    delay_init(168);     // ��ʼ����ʱ��������Ƶ168MHz��
    LED_Init();          // ��ʼ��LED
    KEY_Init();          // ��ʼ������
    TIM3_Int_Init(2000-1, 8400-1);  // ��ʱ����ʼ����Ĭ��500ms��
    
    while(1) {
    key = KEY_Scan(0); // ɨ�谴������֧��������
    if (key) {
        switch (key) {
            case WKUP_PRES: // WKUP ��������
                // �л���ʱ������
                if (tim3_config_flag == 0) {
                    // �л���ģʽ2
                    TIM3_Int_Init(10000 - 1, 8400 - 1); // ARR=9999, PSC=8399
                    tim3_config_flag = 1;
                } else {
                    // �л���ģʽ1
                    TIM3_Int_Init(2000 - 1, 8400 - 1); // ARR=1999, PSC=8399
                    tim3_config_flag = 0;
                }
            
        }
    } else {
        delay_ms(10); // �ް���ʱ��ʱ
    }
				//LED0=!LED0;
     // delay_ms(200);//��ʱ 200ms
    }
	}


