#ifndef _USART_h
#define _USART_h


void usart0_init() ;
void usart0_send(uchar) ;
void reset_receive() ;//清空输入的receive数组
void usart0_judge() ; //主菜单的usart指令选择功能

#endif