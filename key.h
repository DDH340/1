#ifndef __KEY_H 
#define __KEY_H   
#include "sys.h"  
 
/*直接通过操作库函读取IO*/ 
#define KEY0   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4 
#define KEY1   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) //PE3  
#define KEY2   GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2 
#define WK_UP      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) //PA0 
 
#define KEY0_PRES  1 
#define KEY1_PRES 2 
#define KEY2_PRES 3 
#define WKUP_PRES   4 
 
void KEY_Init(void); //IO初始化
u8 KEY_Scan(u8);    //按键扫描函数  
#endif
