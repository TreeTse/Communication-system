#include "main.h"


unsigned int led1[] = {(BIT(7)<<8)|BIT(0),(BIT(6)<<8)|BIT(1),(BIT(5)<<8)|BIT(2),
(BIT(4)<<8)|BIT(3),(BIT(3)<<8)|BIT(4),(BIT(2)<<8)|BIT(5),(BIT(1)<<8)|BIT(6),
(BIT(0)<<8)|BIT(7)};

uchar  number , number_1 ;
extern uint data ,datalh[3],num ;
extern uchar rdata ;
extern angel ;
uchar k=0 , k1 = 0 ;
uchar about_motor,about_led , about_sushu;


void reset_datalh()  //清零datalh函数
{
  	 memset(datalh,'\0',3) ;
}

void main()
{    	
	usart_init() ;
	SEI() ;
	//sushu_judge() ;
	//TWI_Write(0x08,0x01) ;
	DDRB |= BIT(0) ; // 测试的LED
	DDRB |= BIT(1) ; //舵机PWM
	timer1_init() ;
 	spi_init();
	while(1)
	{ 
	     usart0_judge() ;
	}
}