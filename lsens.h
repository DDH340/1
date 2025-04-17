#ifndef __LSENS_H
#define __LSENS_H
#include "sys.h"
void Lsens_Init(void); 
u8 Lsens_Get_Val(void);
#ifdef HIGH_PRECISION_MODE
    #define LSENS_READ_TIMES   20u  // ???????20?
#else
    #define LSENS_READ_TIMES   10u  // ??????10?
#endif
#endif
