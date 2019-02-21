#ifndef _ADC_h
#define ADC.h

#include "main.h"
#define CONFIRM 20

void delay_ms(uint);
void ADC_init() ;
uint ad_get();
uint keynum_judge(uint);//输入密码数字
uint keynum1_judge(uint v) ;//修改密码用的
uint key_judge(uint v) ; //主菜单功能函数

#endif 