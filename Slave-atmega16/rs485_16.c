#include <iom16v.h>
#include <macros.h>
#define uchar unsigned char 
#define uint unsigned int 
#define JIESHOU PORTD &= ~BIT(2) 
#define FASONG PORTD |= BIT(2) 


#define address 0x48

uint data ,datalh[3],num = 0 ;

//从机1地址为1 
void delay_ms(uint n)
{
 	 uint i , j ;
	 for(i=0;i<n;i++)
	 for(j=0;j<1141;j++);
}

void usart_init()
{
 	 DDRD &= ~BIT(0) ;
	 DDRD |= BIT(1)|BIT(2) ;
	 UCSRA |= (1<<MPCM) ;
	 UCSRB |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);//|(UCSZ2) ;
	 UBRRL = 51 ;
	 UCSRC |= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1) ;
	 JIESHOU ;
}

#pragma interrupt_handler receive:12 
void receive()
{
 	 while ( !(UCSRA & (1<<RXC)) );
	 datalh[num] = UDR ;
	 num++ ;
	 if(num==2) 
	     num = 0 ;
	 if((UCSRA&(1<<MPCM))==1)
	 	 if(datalh[1]==1)
            if(datalh[0]==address)   //如果mpcm为1 , 接收地址 判断地址是否自己的 
	    	{
			    UCSRA &= ~(1<<MPCM) ;
		    }
			else UCSRA |= (1<<MPCM) ;
	 if((UCSRA&(1<<MPCM))==0)
           if(datalh[1]==0) //如果接收到数据帧
	       {
		     PORTB = 0x01 ;//测试用的语句
		   }
	 	   else if(datalh[1]==1)
	       {
		     UCSRA |= (1<<MPCM) ;
		   }
}

void usart_send(uchar data) 
{
      FASONG ;
	  while ( !( UCSRA & (1<<UDRE))) ;
	  UCSRB |= (1<<TXB8);
	  UDR = data;
	  /* 将数据放入缓冲器，发送数据 */
	  while(!(UCSRA&(1<<TXC)));
	  JIESHOU ;
}

void usart_send9(uint data)
{
 	  uchar ldata = data , hdata = data>>8 ; //低八位数据和高一位数据
	  usart_send(ldata);//第一次送地址  第二次送类型
	  delay_ms(15) ;
	  usart_send(hdata) ;
}