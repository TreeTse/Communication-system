/*此c文件的函数主要实现的是串口的指令控制*/
#include "main.h"
#include "main.h"
#include "alarm.h"

extern uchar receive[20],geshu,change_what , about_clock ,about_alarm , k , page;
extern uchar minute,second,hour,year,month,day ;
extern uchar  min,sec,ho , ye , mon , da , cishu , xiugai , input_num , number , code[] ; 
extern uchar y , mo , d , h , mi , s ,about_motor , about_led , about_sushu;

//usart初始化函数
void usart0_init() 
{
 	 DDRE &= ~BIT(0) ;           //PE0为接收
	 DDRE |= BIT(1) ;            //PE1为发送
	 PORTE |= BIT(1)|BIT(0) ;
	 UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0) ;  //接收中断  发送 接收使能
	 UCSR0C |= BIT(7)|(1<<UCSZ00)|(1<<UCSZ01) ;//8数据位 1停止位
	 UBRR0L = 51 ;             //9600波特率
	 SEI() ;                   //开启全局中断
}

//usart接收函数
#pragma interrupt_handler usart0_receive:19
void usart0_receive ()        
{
 	 uchar data  ;
	 while(!(UCSR0A&(1<<RXC0)));
	 data = UDR0  ;
	 receive[geshu] = data ;
	 input_num = data ;
	 usart0_send(receive[geshu]) ;
	 geshu++ ;
}

//主菜单的输入指令函数
void usart0_send(uchar data)    
{
 	 while(!(UCSR0A&(1<<UDRE0)));
	 UDR0 = data ;
	 while(!(UCSR0A&(1<<TXC0)));
}

//清空数组 
void reset_receive()  
{
 	 memset(receive,'\0',20) ;
	 geshu = 0 ;
}

void usart0_judge() 
{
 	 uchar p ;
	 if(!strcmp(receive,"homepage"))   //主页
     {
	  		ye = y ;
			mon = mo ;
			da = d ;
			ho = h ;
			min = mi ;
			sec = s ;
			reset_receive() ;
			about_clock = 0 ;
			change_what = 0 ;
			k = 0 ;
			page = PAGE1 ;
	}
	else if(!strcmp(receive,"showtime"))  //显示时间指令
    {
	 	    reset_receive() ;
			if(change_what == 0 ) //change_what为需要改变年、月、日时所需要使用的     
				 if(about_clock!=SHOW_TIME)   //其他时候about_clock 都是等于0
		    	 {
	    	        about_clock = SHOW_TIME  ;  
	    	   		lcd_clear_csr(); 
		         }
	}
	else if (!strcmp(receive,"changetime"))    //修改时间指令
    {
	   reset_receive() ;
	   if(change_what == 0&&about_clock!=SHOW_TIME)
	   {
	      about_clock = CHANGE_TIME ;
	      change_what = YEAR ;
	   }
	}
	else if(!strcmp(receive,"setalarm"))   //设置闹钟
    {
	 	 reset_receive() ;
		 delay_ms(100) ;
		 if(change_what == 0 ) 
		 {
		     about_clock = ALARM ;
		 	 change_what = YEAR ;        //先是年修改
		 	 lcd_clear_csr(); 
		 }
	}
	else if(!strcmp(receive,"jia"))   // 加
    {
	 	 reset_receive() ;
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
	}
	else if(!strcmp(receive,"other"))   //下一项要改变的参数
    {
	      reset_receive() ;
		  if(about_clock == CHANGE_TIME||about_clock == ALARM)
		  {
	          change_what++ ;
	   		  delay_ms(200) ;
	    	  if(change_what==7)
	    	  change_what = 1 ;
		  }
	}
	else if(!strcmp(receive,"jian"))  //减
    {
	      reset_receive() ;
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
	  }
	
	 else if(!strcmp(receive,"wancheng"))  //修改完成指令
     {
	     reset_receive() ;
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
		     write_time() ;
			 about_clock = 0 ;
			 home1();
			 k=0;
			 change_what = 0 ;
			 alarm_gai_wancheng() ;
		  }
	 }
	 else if(!strcmp(receive,"naozhongkai"))  //闹钟开启指令
   	 {
	        reset_receive() ;
			if(about_clock == 0)
	    	{   
	    	  about_alarm = ALARM_ON ;
	    	  naozhongshengxiao() ;
		    }
	 }
	 else if(!strcmp(receive,"naozhongqv")) //闹钟取消指令
     {
	  	  reset_receive() ;
		  if(about_clock==0)
	      {
	        about_alarm = ALARM_OFF ;
	    	naozhongqvxiao() ;
		  }
	 }
	 else if(!strcmp(receive,"next"))   //显示下一个页面指令
     {
	     reset_receive() ;
		 cishu++ ;
		 if(about_clock == 0 ) 
		    if(cishu%2==1)
	          page = PAGE1 ;
	     else 
		    page = PAGE2 ;
		 delay_ms(270) ;
	}
	else if(!strcmp(receive,"xiugaimima"))   //修改密码指令
    {
	    reset_receive() ;
		delay_ms(260) ;
		//about_alarm = ALARM_ON ;
		if(about_clock==0) 
		  if(change_what == 0 )
	         xiugai = 1 ;
	}
	else if(!strcmp(receive,"zhuandong"))  //转动舵机指令
    {
	    reset_receive() ;
		delay_ms(200) ;
		if(about_clock == 0 ) 
		about_clock = SHOW_MOTOR  ;
		//delay_ms(200) ;
		about_motor = MOTOR_RUN ;
		usart1_send9(0x156) ;
		usart1_send9(0xfe) ;  //这个代表转动
		delay_ms(100) ;
	}
	else if(!strcmp(receive,"liushuideng"))	  //开启流水灯指令
    {
	    reset_receive() ;
		if( about_clock == 0 ) 
		about_clock = SHOW_LED ;
		delay_ms(200) ;
		about_clock = SHOW_LED ;
		about_led = LED_RUN ;
		usart1_send9(0x156) ;
		usart1_send9(0xfd) ;
	}
	else if(!strcmp(receive,"stop"))    //停止流水灯指令
    {
	    reset_receive() ;
		if(about_led == LED_RUN) 
    	{
		    usart1_send9(('S'<<8)|'T');
			liushuidengqvxiao();
			about_led = LED_STOP ;
		}
	}
	else if(!strcmp(receive,"sushuxieru"))   //写入素数指令
    {
	 	 reset_receive() ;
		 if(about_clock ==0&&about_sushu!=1)  //在主菜单 且素数没有写入时候
	     {
	         about_clock = WRITE_SUSHU ;    //就显示素数写入完成
			 about_sushu = 1 ;
		 }
		 usart1_send9(('W'<<8)|'V');  //从机执行写入函数 
	}
	
	for(p=0;p<20;p++)   //错误处理
    {
	   if(receive[p]=='#')
	   {
	   	 reset_receive() ;
	     break;
	   }
	}
}
