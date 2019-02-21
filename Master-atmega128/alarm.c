/*闹钟功能实现*/
#include "alarm.h"
#include "main.h"
#define END 0                   //end代表0   为结束标志
#define PAUSE 1                 //pause 代表1  为暂停标志

unsigned char z;                //全局变量z代表简谱中的第几个音
unsigned int Two_tiger[][2]=    //二维数组 {频率,时间（毫秒）}
{	  // 这是<<两只老虎>>
{262,200},{294,200},{330,200},{262,200},{262,200},   //{1,1}即{PAUSE,PAUSE}不是频率 而是停顿一下
{262,200},{294,200},{330,200},{262,200},
{262,200}, {330,500},{349,500},{392,500},{392,200},
{330,500},{349,500},{392,500},{392,200},{392,300},
{440,300},{392,300},{349,300},{330,300},{262,200},
{392,200},{392,300},{440,300},{392,300},{349,300},
{330,300},{262,200},{392,200},{262,300},{392,300},
{262,300} ,{392,200},{262,300},{392,300},{262,300},{END,END}};//{0,0}即{END,END}表示结束

extern uchar minute,second,hour,year,month,day;
extern uchar min,sec,ho , ye , mon , da ,naozhong[] , gai[] ;   //缺字母的为闹钟的 
extern uchar mi  ,s  ,h ,y , mo , d  ; //存放闹钟的数据 
extern uchar wancheng[],function1[],shijian[] , hello[],shengxiao[] ,qvxiao[] ; //这些是显示的中文


void timer1_init()              //定时器1初始化函数
{ 
  	 /*选择模式14的快速PWM模式  不分频   比较比配时清零OCA1*/
	 TCCR1A |= (1<<WGM11)|(1<<COM1A1)|(1<<COM1A0) ;    //
	 TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS10); 
}

/*音乐hz函数:根据简谱上每个音的频率计算出 ICR1的值(TOP值) */
void music_hz(unsigned char i)          
{
	unsigned int x , num ;
	x=Two_tiger[i][0];
	if(x!=END &&x!=PAUSE)              
	{
		num=(2000000/x)-1;           //求赫兹
		ICR1=num;                    //ICR1值
		OCR1A = num/2 ;              //50%占空比
	}
	else 
	{  
		TCCR1A=0x00;               //OC1A 为低电平的普通IO口 不发出声音
	}
}



void music_play()           //音乐播放函数
{

    if(Two_tiger[z][1] != END)                    //不是结束标志 END 的情况下
    {   
	   if(Two_tiger[z][1] != PAUSE )               //如果不是暂停标志 PAUSE
	   {
		  TCCR1A |= (1<<WGM11)|(1<<COM1A1)|(1<<COM1A0) ;   //定时器为快速PWM 模式 
		  music_hz(z);                       
		  delay_ms(Two_tiger[z][1]);
		  z++;
	   }
	   else 			               //如果是暂停标志PAUSE 
	   {
		  TCCR1A=0x00;                 //OC1A 为低电平的普通IO口 不发出声音
		  delay_ms(200) ;              //延时一小下
		  z++;                         //转入下一个音符或者开始暂停标志
	   }		
    }
	else                                         //如果是结束标志END
    {
	    TCCR1A=0x00;                      //OC1A 为低电平的普通IO口 不发出声音
		delay_ms(1000) ;                      //延时长一点时间
		z=0 ;                                 //z变为0  又重新开始播放
	}
}


void alarm_init() //闹钟初始
{
     DDRE |= BIT(2) ;
	 DDRB |= BIT(5) ; 
	 PORTB &= ~BIT(5) ;
}

void alarm_on()  //闹钟打开
{
if(year==ye)
    if(month == mon)
	    if(day == da) 
		    if(hour == ho) 
			    if(minute == min )
				    if(sec==second)
					    PORTE |= BIT(2) ;
}



void alarm_gai_wancheng()  //闹钟修改完成
{
     lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(1,0,naozhong,2);
	 lcd_write_hanzi_hanzi_hang(1,2,gai,2);
	 lcd_write_hanzi_hanzi_hang(1,4,wancheng,2);
	 delay_ms(1500) ;
	 lcd_clear_csr();
	 home1() ;
}

void clock_gai_wancheng() //时钟修改完成
{
     lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(1,0,shijian,2);
	 lcd_write_hanzi_hanzi_hang(1,2,gai,2);
	 lcd_write_hanzi_hanzi_hang(1,4,wancheng,2);
	 delay_ms(1500) ;
	 lcd_clear_csr();
	 home1() ;
}

void code_gai_wancheng()  //密码修改完成
{
     lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(1,0,hello,2);
	 lcd_write_hanzi_hanzi_hang(1,2,gai,2);
	 lcd_write_hanzi_hanzi_hang(1,4,wancheng,2);
	 delay_ms(1500) ;
}

void naozhongshengxiao()     //闹钟生效
{
     lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(0,0,naozhong,2);
	 lcd_write_hanzi_hanzi_hang(0,2,shengxiao,2);
	 //lcd_write_hanzi_hanzi_hang(0,4,wancheng,2);
	 delay_ms(1500) ;
	 lcd_clear_csr();
	 home1() ;
}

void naozhongqvxiao()            //闹钟取消 
{
     lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(0,0,naozhong,2);
	 lcd_write_hanzi_hanzi_hang(0,2,qvxiao,2);
	 //lcd_write_hanzi_hanzi_hang(0,4,wancheng,2);
	 delay_ms(1500) ;
	 lcd_clear_csr();
	 home1() ;
}