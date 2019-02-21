#include "main.h"
uint angel = 90 ;

void timer1_init()            //定时器1初始化函数
{
      TCCR1A |= (1<<WGM11)|(1<<COM1A1);
	  TCCR1B |= (1<<WGM12)|(1<<WGM13); 
	  /*//8分频 8M晶振 比较匹配时置低 周期为20ms 频率为50HZ 先不打开定时器*/
	  
	  ICR1H = 0x4E ;
	  ICR1L = 0x1F ;
	  //OCR1AH = 0x04 ;     //499+2000*angel/180
	  //OCR1AL = 0xE2 ;
	  OCR1AH = ((uint)(499+2000*angel/180.0)>>8)&0x00ff ;
	  OCR1AL = (uint)((499+2000*angel/180.0))&0x00ff ;
}


/*motor_display()//499+2000*angel/180           角度显示函数
{
      OCR1AH = ((uint)(angel/0.09+499)>>8)&0x00ff ;
	  OCR1AL = (uint)(angel/0.09+499)&0x00ff ;
}*/
