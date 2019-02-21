/*主程序*/
#include<iom128v.h>
#include<macros.h>
#include "12864.h"
#include "font.h"
#include "usart.h"
#include "TWI.h"
#include "delay.h"
#include "EEROM.h"
#include "home.h"
#include "ADC.h"
#include "alarm.h"

//number2是素数

extern uchar dis_code[10] ;//= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; //共阳数码管段码
extern uchar code[50] ;
uchar code_in[50],code1[50],code2[50] ; //输入的密码
/*//open_flag 为开机成功标志    error_num为错误次数  i为显示错误信息设置*/
uint v , row = 0 ,quanlity = 0 ,open_flag = 0 ,error_num = 0 ,i = 0 , k1= 0 ;
uint number ,number1 ;
uchar minute,second,hour,year,month,day,week,about_clock , about_alarm ,change_what,
      change_next , change_over,k ,about_motor = 0 ;//英文全称的为时钟的
uchar min=35,sec=40,ho=15 , ye=19 , mon=2 , da=19 , cishu = 0;   //缺字母的为闹钟的
uchar page ; //page为菜单页码  分第一页菜单和第二页菜单
uchar xiugai,function,input_num = 50  ;
uint number2 ;
uchar receive[20],geshu ;
uchar mi = 0 ,s  ,h=18  ,y=17 , mo=2 , d =22 , about_led = 0 ,about_sushu;
uchar angel = 90 ;


void updatetime()
{
    second = read_time(0x00) ;
	  minute = read_time(0x01) ;
	  hour = read_time(0x02) ;
	  week = TWI_Read(0x03) ;
	  day = read_time(0x04) ;
	  month = read_time(0x05) ;
	  year = read_time(0x06) ;
}

void key1()
{
    v = ad_get() ;
	function = key_judge(v) ;
	switch(about_clock)
	{
		case 0 : if(page==PAGE1)
			   	 {
					 home1();
					 page = 0 ;
				 }
			     else if(page==PAGE2)
				 {
					home2() ;
					page = 0;
			     }
				 change_what = 0 ;
				 break ;
		case SHOW_TIME :k = SHOW_TIME ;
			 		   	show_time() ;
						break ;
		case CHANGE_TIME :if(k!=SHOW_TIME)
		                  {
							 lcd_clear_csr();
							 show_time();
						  }
						  k=SHOW_TIME ;
						  switch(change_what)
		                  {
							 case YEAR : shanshuo_nian() ;lcd_write_ch(2,8,numb[second/10]);
							             lcd_write_ch(2,9,numb[second%10]); break ;  //闪烁完了 又要显示回来  否则会空
							 case MONTH : shanshuo_yue() ; lcd_write_ch(1,2,numb[year/10]);
							 	          lcd_write_ch(1,3,numb[year%10]);break ;
							 case DAY : shanshuo_ri() ;lcd_write_ch(1,6,numb[month/10]);
							 	  	  	lcd_write_ch(1,7,numb[month%10]); break ;
							 case HOUR : shanshuo_shi() ; lcd_write_ch(1,10,numb[day/10]);
							 	  	   	 lcd_write_ch(1,11,numb[day%10]); break ;
							 case MINUTE : shanshuo_fen() ;lcd_write_ch(2,0,numb[hour/10]);
							 	           lcd_write_ch(2,1,numb[hour%10]); break ;
							 case SECOND : shanshuo_miao() ; lcd_write_ch(2,4,numb[minute/10]);
							 	           lcd_write_ch(2,5,numb[minute%10]);break ;
						  }
						  break ;
		case ALARM 	: show_alarm() ;
		              switch(change_what)
		              {
							  case YEAR : shanshuo_ye() ;
							    		  lcd_write_ch(2,8,numb[sec/10]);
										  lcd_write_ch(2,9,numb[sec%10]);
										  break ;  //闪烁完了 又要显示回来  否则会空
							  case MONTH :
							   			  shanshuo_mon() ;
										  lcd_write_ch(1,2,numb[ye/10]);
										  lcd_write_ch(1,3,numb[ye%10]);
										  break ;
							  case DAY :  shanshuo_da() ;
							   			  lcd_write_ch(1,6,numb[mon/10]);
										  lcd_write_ch(1,7,numb[mon%10]);
										  break ;
							  case HOUR : shanshuo_ho() ;
							   			  lcd_write_ch(1,10,numb[da/10]);
										  lcd_write_ch(1,11,numb[da%10]);
										  break ;
							  case MINUTE : shanshuo_min() ;
							   			   	lcd_write_ch(2,0,numb[ho/10]);
											lcd_write_ch(2,1,numb[ho%10]);
											break ;
							  case SECOND : shanshuo_sec() ;
							   			   	lcd_write_ch(2,4,numb[min/10]);
											lcd_write_ch(2,5,numb[min%10]);
											break ;
					   }
					   break ;
		case SHOW_MOTOR	: zhuandong() ; break ;
		case SHOW_LED : if(about_sushu!=1)
		         	    liushuideng() ;
						break ;
		case WRITE_SUSHU :sushuxieru(); break ;
    }
	/*设置了闹钟 要开启它才会开启  当闹钟开启时 按下关闭 那么闹钟就关闭了*/
	switch(about_alarm)
	{
		case ALARM_ON : alarm_on() ;  break ;
		case ALARM_OFF : PORTE &= ~BIT(2) ; break ;
    }
	if(about_motor == MOTOR_RUN)
	{
		if(receive[0] == '*'&&(receive[1]>=48&&receive[1]<=49)&&(receive[2]>=48&&receive[2]<=57)&&(receive[3]>=48&&receive[3]<=57))
		{
		    angel = (receive[1]-48)*100+(receive[2]-48)*10+(receive[3]-48);
		    reset_receive() ;
			usart1_send9((65<<8)|angel) ;
			reset_receive() ;
		 }
     }
	 if(about_sushu == 1&&(about_clock == 0||about_clock == SHOW_LED))  //如果已经写入了素数且在主菜单
	 {
		    if(receive[0] == 'R'&&(receive[1]>=48&&receive[1]<=57)&&(receive[2]>=48&&receive[2]<=57))
		    {
			   if(((receive[1]-48)*10+(receive[2]-48))>=95)
			   {
			       bushisushu() ;
			   	   reset_receive() ;
			   }
			   else
			   {
			 	    number2 = zhishu[(receive[1]-48)*10+(receive[2]-48)] ;
					shisushu() ;reset_receive() ;k1=0;}
			   }
		     }
	  else if(about_sushu == 0 ) ;
	  {
		  if(receive[0] == 'R'&&(receive[1]>=48&&receive[1]<=57)&&(receive[2]>=48&&receive[2]<=57))
		  reset_receive() ;
	  }
}

void xiugai_mima()
{
    if(xiugai == 0)
    {
	   key1() ;
	   usart0_judge() ;
	}
	else if(xiugai==1)
	{
	   xiugai1() ;
	   xiugai_code() ;
	}
	else if(xiugai==2)
	{
    	first_open() ;
    	error_num = 0 ;
		i = 0 ;
		read_code() ;
		diyici_code() ;
    	xiugai = 0 ;
	}
}

void main(void)     //主函数
{
    usart0_init();  //USART0初始化
		ADC_init() ;    //第一次开机时使能ADC
		lcd12864_init();//lcd12864端口初始化
		lcd_init();     //lcd12864初始化
		lcd_clear_csr();//清屏
		first_open() ; //开机界面显示
		read_code() ; //从EEPROM读取密码
		diyici_code() ;//第一次开机
		row = 0 ;
		//点了修改时间  没有按下确认 不能修改闹钟
		timer1_init();
		usart1_init() ;  //USART1初始化
		usart1_send9(0x00) ;
		alarm_init() ;
 		while(1)
    {
		   updatetime() ;   //时间的更新
		   xiugai_mima() ;
	  }
}
