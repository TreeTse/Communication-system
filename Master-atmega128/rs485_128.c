#include "main.h"

#define JIESHOU PORTD &= ~BIT(4) 
#define FASONG PORTD |= BIT(4) 
//RS485接收和发送使能
#define address 0x98

uint data ,datalh[3],num = 0 ;


void usart1_init()
{
 	 DDRD &= ~BIT(2) ;
	 DDRD |= BIT(3)|BIT(4) ;
	 UCSR1B |= (1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1);//|(UCSZ2) ;
	 UBRR1L = 51 ;
	 UCSR1C |= BIT(7)|(1<<UCSZ10)|(1<<UCSZ11) ;
	 JIESHOU ;
}

void usart1_send(uchar data) 
{
     FASONG ;
	 while ( !( UCSR1A & (1<<UDRE1))) ;
	 UDR1 = data;
	 while(!(UCSR1A&(1<<TXC1)));
}

void usart1_send9(uint data) //发送16位数 
{
     uchar ldata = data , hdata = data>>8 ; //低八位数据和高一位数据
	 usart1_send(ldata);//第一次送地址  第二次送类型
	 delay_ms(15) ;
	 usart1_send(hdata) ;
}

void usart1_send_str(uchar str[])   //发送数组指令
{
     uchar i ;
	 for(i=0;str[i]!='\0';i++)
	    usart1_send9(str[i]) ;
}