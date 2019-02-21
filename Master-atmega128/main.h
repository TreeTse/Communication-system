#ifndef _main_h
#define _main_h

#include <iom128v.h>
#include <macros.h>

#define uchar unsigned char
#define uint unsigned int
//
#define CONFIRM 20

//修改完成
#define XIUGAIWANCHENG 109

#define SHOW_TIME 1
#define CHANGE_TIME 2
#define ALARM 3
#define SHOW_MOTOR 5
#define SHOW_LED 6
#define WRITE_SUSHU 7
//菜单页码
#define PAGE1 1
#define PAGE2 2

#define MOTOR_RUN 1
#define LED_RUN 2
#define LED_STOP 0
#define SUSHU 3

#define YEAR 1
#define MONTH 2
#define DAY 3
#define HOUR 4
#define MINUTE 5
#define SECOND 6


//延时函数
void delay_us(uint x);
void delay(uint x);
void key1() ;    //按键判断函数
void updatetime() ;    //时间更新函数

#endif
