#include "main.h"

#define JIESHOU PORTD &= ~BIT(2) 
#define FASONG PORTD |= BIT(2) 


#define address 0x56 

uint data ,datalh[3],num = 0 ;
uchar rdata ;
extern number , number_1 ,about_motor,angel,about_led , led1[] , k ;


void usart_init()
{
    DDRD &= ~BIT(0) ;
	DDRD |= BIT(1)|BIT(2) ;
	UCSR0A |= (1<<MPCM0) ;
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);//|(UCSZ2) ;
	UBRR0L = 51 ;
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01) ;
	JIESHOU ;
}

#pragma interrupt_handler receive:19 
void receive()
{
    while ( !(UCSR0A & (1<<RXC0)) );
	datalh[num] = UDR0 ;
	num++ ;
	if(num==2) 
	     num = 0 ;
    if(datalh[0]==address)    //接收到自己的地址，就清空MPCM0位
	{
		UCSR0A &= ~(1<<MPCM0) ;		
		PORTB = 0x01 ;
    }
	if((UCSR0A&(1<<MPCM0))==0)
        if(datalh[1]==0)
	    {		
		//PORTB = 0x01 ;
		}
}

void usart_send(uchar data) 
{
 	 FASONG ;
	 while ( !( UCSR0A & (1<<UDRE0))) ;
	 UDR0 = data;
	 /* 将数据放入缓冲器，发送数据 */
	 while(!(UCSR0A&(1<<TXC0)));
	 JIESHOU ;
}

void usart_send9(uint data)
{
     uchar ldata = data , hdata = data>>8 ; //低八位数据和高一位数据
	 usart_send(ldata);//第一次送地址  第二次送类型
	 delay_ms(15) ;
	 usart_send(hdata) ;
}

void sushu_judge() //素数判断写入函数
{
     uint i , j , sushu[100] ;
	 TWI_Write(1,0) ;
	 for(i=2;i<=500;i++)
     {
         for(j=2;j<i;j++)
         {
		    if(i%j==0)
        	break;
		 }
     	 if(j==i)
	     {
            sushu[number]= i;
			if(i<256)
	        {    
		       TWI_Write(i,number) ;
		       number_1 = number ;
			}
		    else
		    {
		       TWI_Write(i-256,number) ;
			}
	        number++ ;
		 }
     }
	 number = 0 ;
}

void usart0_judge() 
{
    if(datalh[1]==0&&datalh[0]==0xfe)       
	{						
	    TCCR1B |= (1<<CS11);
		about_motor = MOTOR_RUN ;	
		reset_datalh() ;
	}
	if(about_motor == MOTOR_RUN )      //舵机打开
	{
		if(datalh[1]=='A'&&(datalh[0]>=0&&datalh[0]<=180))
		angel = datalh[0] ;
		OCR1AH = ((uint)(angel/0.09+499)>>8)&0x00ff ;
        OCR1AL = (uint)(angel/0.09+499)&0x00ff ;
    }
	if(datalh[1]==0&&datalh[0]==0xfd)        //LED流动
	{
		about_led = LED_RUN ;
	}
	if(about_led == LED_RUN)              //LED停止
	{
		spi_send(led1[k++]);
	    if(k==9) k = 0 ;
		delay(50);
		if(datalh[1]=='S'&&datalh[0]=='T')
		{
		 	spi_send(0x0000);
			about_led = 0 ;
			reset_datalh() ;
		}
	}
	if(datalh[1]=='W'&&datalh[0]=='V')
	{
		//for(k=0;k<255;k++)
		//TWI_Write(0xff,k);
		sushu_judge() ;
	    reset_datalh() ;
	}
}