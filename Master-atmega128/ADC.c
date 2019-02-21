/*AD键盘的实现*/
#include "ADC.h"
#include "main.h"
#include "delay.h"
#include "alarm.h"


//共阳数码管段码
uchar dis_code[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; 
extern uchar about_clock ,change_what , change_next , change_over , about_alarm;
extern uchar minute,second,hour,year,month,day,week ,k,k1; 
extern uchar min,sec,ho,ye,mon,da;   //缺字母的为闹钟的 实际闹钟
extern uchar page,cishu,xiugai;   //页码  翻页次数   修改密码
extern uchar mi  ,s  ,h ,y , mo , d  ; //存放闹钟的数据 
extern uchar receive[] ,about_motor ,about_led , about_sushu;
extern uchar quanlity , code_in[] ,row ,code1[];

//ADC初始化
void ADC_init() 
{
    DDRF &= ~BIT(0) ; //AD0
	ADMUX |= (1<<REFS0) ;  //参考电压选择
	ADCSRA |= (1<<ADEN)|(1<<ADSC) ; //ADC使能并开始转换
}

uint ad_get()
{
   uint i ;
   ADCSRA |= (1<<ADSC) ;   //ADC开始转换
   while(!(ADCSRA&(1<<ADIF)));   
     i = ADC ;
   ADCSRA &= ~(1<<ADSC) ;   
   return i ;
}


uint keynum_judge(uint v)  //AD键盘键码判断函数
{
     uchar p ;
	 if(v>=900||strcmp(receive,"0")==0)
     {
	   reset_receive() ;
	   return 0 ; 
	 }
     else if(v>=800||strcmp(receive,"1")==0)
     {
	   reset_receive() ;
	   return 1 ;
	 }
	 else if(v>=750||strcmp(receive,"2")==0)
     {
	   reset_receive() ;
	   return 2 ;
	 }
	 else if(v>=700||strcmp(receive,"3")==0)
     {
	   reset_receive() ;
	   return 3 ;
	 }
	 else if(v>=650||strcmp(receive,"4")==0)
     {
	   reset_receive() ;
	   return 4 ;
	 }
	 else if(v>=625||strcmp(receive,"5")==0)
     {
	   reset_receive() ;
	   return 5 ;
	 }
	 else if(v>=600||strcmp(receive,"6")==0)
     {
	   reset_receive() ;
	   return 6  ;
	 }
	 else if(v>=550||strcmp(receive,"7")==0)
     {
	   reset_receive() ;
	   return 7  ;
	 }
	 else if(v>=525||strcmp(receive,"8")==0)
     {
	   reset_receive() ;
	   return 8 ;
	 }
	 else if(v>=500)
     {
	   delay_ms(270) ; 
	   return 50 ;
	 }
	 else if(v>=480||strcmp(receive,"9")==0)
     { 
	   reset_receive() ;
	   return 9 ;
	 }
	 else if(v>=460||strcmp(receive,"confirm")==0)
     {
	   reset_receive() ;
	   return CONFIRM ;
	 }
	 else if(v>=440) 
     {
	   return 50 ;
	 }
	 else if(v>=420||strcmp(receive,"reset")==0)   //输入想修改的话可以按下这个
     {
	   reset_receive() ;
	   quanlity = 0 ; 
       row = 0 ;
	   lcd_clear_hang(2);
	   memset(code_in,'\0',50);
	   return 50 ;
	 }
 	 else 
     {
	   for(p=0;p<20;p++)
	   if(receive[p]=='#')
	   reset_receive();
	   return 50 ;
	 }
}

 //修改密码函数
uint keynum1_judge(uint v) 
{
 	 uchar p ;
	 if(v>=900||strcmp(receive,"0")==0)
     {
	    reset_receive() ;
		return 0 ; 
	 }
	 else if(v>=800||strcmp(receive,"1")==0)
     {
	    reset_receive() ;
		return 1 ;
	 }
	 else if(v>=750||strcmp(receive,"2")==0)
     {
	    reset_receive() ;
	    return  2 ;
	 }
 	 else if(v>=700||strcmp(receive,"3")==0)
     {
	    reset_receive() ;
		return 3 ;
	 }
	 else if(v>=650||strcmp(receive,"4")==0)
     {
	    reset_receive() ;
		return 4 ;
	 }
	 else if(v>=625||strcmp(receive,"5")==0)
     {
	    reset_receive() ;
		return 5 ;
	 }
	 else if(v>=600||strcmp(receive,"6")==0)
     {
	    reset_receive() ;
	    return 6  ;
	 }
	 else if(v>=550||strcmp(receive,"7")==0)
     {
	    reset_receive() ;
		return 7  ;
	 }
	 else if(v>=525||strcmp(receive,"8")==0)
     {
	    reset_receive() ;
		return 8 ;
	 }
	 else if(v>=500)
     {
	    return 50 ;
	 }
	 else if(v>=480||strcmp(receive,"9")==0)
     {
	    reset_receive() ;
		return 9 ;
	 }
	 else if(v>=460||strcmp(receive,"xiugaiwancheng")==0)
     {
	     reset_receive() ;
		 delay_ms(270) ;
		 code_gai_wancheng() ;
		 return XIUGAIWANCHENG ;	
	 }
	 else if(v>=440||strcmp(receive,"homepage")==0)
     {
	  	  reset_receive() ;
		  delay_ms(270) ;
		  return 70 ;
	 }
	 else if(v>=420||strcmp(receive,"reset")==0)
     {
	      reset_receive() ;
		  quanlity = 0 ; 
   		  row = 0 ;
		  memset(code_in,'\0',50) ;
		  memset(code1,'\0',50) ;
		  lcd_clear_hang(2);
		  return 50 ;
	 }
	 else 
     {
	  	  for(p=0;p<20;p++)
		  if(receive[p]=='#')
		  reset_receive();
		  return 50 ;
	 }
}



uint key_judge(uint v)  //输入判断函数
{
 	 if(v>=900)        //显示主页
     {
	    about_clock = 0 ;
		change_what = 0 ;
		k = 0 ;
		page = PAGE1 ;
		return 0 ;	
	  }
	  else if(v>=800)  //显示时间功能 
      {
	     if(change_what == 0 )            //其他时候about_clock 都是等于0 的
		 if(about_clock!=SHOW_TIME)
		 {
	        about_clock = SHOW_TIME  ; 
	    	lcd_clear_csr(); 
		 }
	     return 1 ;
	   } 
	   else if(v>=750)  //修改时间功能
       {
	        if(change_what == 0&&about_clock!=SHOW_TIME)
			{
	    	    about_clock = CHANGE_TIME ;
	    		change_what = YEAR ;
			}
			return 2 ;
	    }
		else if(v>=700)     //设置闹钟功能
    	{
	        if(change_what == 0 ) 
			{
			 	about_clock = ALARM ;
				change_what = YEAR ; //先是年修改
				lcd_clear_csr(); }
				return 3 ;
			}
		else if(v>=650)   //密码修改已经放在翻页的下面
    	{
		   return 4 ;
		}
		else if(v>=625)
   		{
		   if(about_clock == 0 ) 
		   about_clock = SHOW_MOTOR  ;
		   delay_ms(200) ;
		   about_motor = MOTOR_RUN ;
		   usart1_send9(0x156) ;
		   usart1_send9(0xfe) ;
		   return 5 ;
	     }
		 else if(v>=600)       //流水灯开关功能
    	 {
		     if( about_clock == 0 )  //在主菜单的情况下
			 about_clock = SHOW_LED ;
			 if(about_led == LED_STOP)
	    	 {
	            about_led = LED_RUN ;
	    		usart1_send9(0x156) ;
	    		usart1_send9(0xfd) ;
				delay_ms(200) ;
		     }
			 else if(about_led == LED_RUN)
	    	 {
			    about_led = LED_STOP ;
				usart1_send9(('S'<<8)|'T');
				delay_ms(200) ;
				//在主菜单且流水灯正在流动的情况下
				if((about_clock == 0||about_clock==SHOW_LED) )     
				liushuidengqvxiao() ;
			  }
			  return 6  ;
 	      }
		  else if(v>=550)
    	  {
		     if(about_sushu!=1)
			 usart1_send9(('W'<<8)|'V'); //从机执行写入函数 
			 if(about_clock ==0&&about_sushu!=1)  //在主菜单 且素数没有写入时候
	    	 {
	   		     about_clock = WRITE_SUSHU ;    //就显示素数写入完成 
				 about_sushu = 1 ;
			 }
			 delay_ms(200) ;
			 return 7  ;
		   }
		   else if(v>=525)  //菜单翻页浏览
    	   {
		     cishu++ ;
			 if(about_clock == 0 ) //如果在菜单界面
			    if(cishu%2==1)
				    page = PAGE1 ;
			 else 
			 	 page = PAGE2 ;
			 delay_ms(300) ;
			 return 8 ;
		   }
		   else if(v>=500)  //闹钟开启
    	   {
		     if(about_clock == 0)
			 {
			     naozhongshengxiao() ;
			 }
			 about_alarm = ALARM_ON ;
			 return 9 ;
			}
			else if(v>=480)   //闹钟关闭
    		{
			   if(about_clock==0)
			   {
			   	   about_alarm = ALARM_OFF ;
				   naozhongqvxiao() ;
			   }
			   return 10 ; //CONFIRM
			 }
			 else if(v>=460)       //修改密码
    		 {
			    delay_ms(260) ;
				//about_alarm = ALARM_ON ;
				if(about_clock==0) 
				  if(change_what == 0)
				     xiugai = 1 ;
				return 11 ;
			 }
			 else if(v>=440) 
    		 {
			    if(about_clock == CHANGE_TIME||about_clock == ALARM)
				{
	    		    change_what++ ;
	    			delay_ms(200) ;
	    			if(change_what==7)
	    			   change_what = 1 ;
			    }
				return 12 ;  //下一项
			  }
			  else if(v>=420)
    		  {
			     if(about_clock == CHANGE_TIME ) 
				 switch(change_what)
	    		 {
				    case YEAR : year++ ; break ;
					case MONTH : month++ ; break ;
					case DAY : day++ ; break ;
					case HOUR : hour++ ; break ;
					case MINUTE : minute++ ; break ;
					case SECOND : second++ ; break ;
				  }
				  else if (about_clock == ALARM) 
				  switch(change_what)
	    		  {
				     case YEAR : ye++ ; break ;
					 case MONTH : mon++ ; break ;
					 case DAY : da++ ; break ;
					 case HOUR : ho++ ; break ;
					 case MINUTE : min++ ; break ;
					 case SECOND : sec++ ; break ;
				   }
				   return 13 ;  //加一 
				}
				else if(v>=400)
   				{
				     if(about_clock == CHANGE_TIME ) 
					 switch(change_what)
	    			 {
					    case YEAR : year-- ; break ;
						case MONTH : month-- ; break ;
						case DAY : day-- ; break ;
						case HOUR : hour-- ; break ;
						case MINUTE : minute-- ; break ;
						case SECOND : second-- ; break ;
		             }
					 else if (about_clock == ALARM) 
					 switch(change_what)
	    			 {
					    case YEAR : ye-- ; break ;
						case MONTH : mon-- ; break ;
						case DAY : da-- ; break ;
						case HOUR : ho-- ; break ;
						case MINUTE : min-- ; break ;
						case SECOND : sec-- ; break ;
					  }
					  return 14 ; //减一
				}
				else if(v>=390)
    			{
				      //change_over = CONFIRM ;  //修改完成
					  if(about_clock == CHANGE_TIME)
	    			  {
					     write_time() ;
						 about_clock = 0 ;
						 home1();
						 k=0;
						 change_what = 0 ;
						 clock_gai_wancheng() ;
					   }
					   else if(about_clock == ALARM )
	    			   {
					      //write_alarm() ;
						  about_clock = 0 ;
						  home1();
						  k=0;
						  change_what = 0 ;
						  alarm_gai_wancheng() ;
						}
						return 15 ;
	              }
				  else return 50 ;
}

/*void seg_adc_display(uint data)
{
 	   v = 1.0*data/1024*5*1000 ;
	   PORTD |= BIT(3) ;
	   PORTD &= BIT(3) ;
	   PORTC = dis_code[v%10];
	   delay_ms(4) ;

	   PORTD |= BIT(2) ;
	   PORTD &= BIT(2) ;
	   PORTC = dis_code[v/10%10];
	   delay_ms(4) ;

	   PORTD |= BIT(1) ;
	   PORTD &= BIT(1) ;
	   PORTC = dis_code[v/100%10];
	   delay_ms(4) ;

	   PORTD |= BIT(0) ;
	   PORTD &= BIT(0) ;
	   PORTC = dis_code[v/1000];
	   delay_ms(4) ;

	   PORTD |= BIT(0) ;
	   PORTD &= BIT(0) ;
	   PORTC = 0x7f ;
	   delay_ms(4) ;
}*/