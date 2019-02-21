#include "code.h"
#include "main.h"

/*//open_flag 为开机成功标志    error_num为错误次数  i为显示错误信息设置*/
extern uint v , row  ,quanlity  ,open_flag  ,error_num  ,i  ,number ; 
extern uchar code[],code_in[],star[] ,code1[],numb[10][16] ;
/*xiugai为修改密码的标志   receive为usart0接收到的字符数组*/
extern uchar shezhinaozhong[],xiugai , receive[] ;  
extern uint number1 ;             //修改密码用到的数字是number1

void diyici_code()
{
     reset_receive() ;
 	 i = 0 ;
 	 quanlity = 0 ;   
 	 row = 0 ;
 	 error_num =0;
 	 while(1)  //第一个while循环为第一次开机输入密码
 	 {
 	    v = ad_get() ;
 		number = keynum_judge(v) ;    //输入的数字
 		if(number!=50&&number!=CONFIRM)  
     	{
	 	   code_in[quanlity++] = number+48 ; //数字转换成对应的字符,quanlity代表了密码长度的递增
	 	   lcd_write_ch(2,row++,numb[number]) ;  //star 改为numb
	 	   delay_ms(260) ;
	    }
		if (number==CONFIRM)
        {
		   if(!strcmp(code,code_in))  //密码匹配了
		   open_flag = 1 ; //开启标志
		   else 
	       {
		     error_num++ ;  //错误次数加一 
			 memset(code_in,'\0',50) ;  //清空数组 
			 quanlity = 0 ;//数组从0开始
			 row = 0 ;
		   }
	     }
		 if(open_flag==1)   //如果开机成功了 
		 {
		    open_flag = 0 ;
			quanlity = 0 ;
			row = 0 ;
			break ;
		 }	
		 else
		    switch(error_num)
		    {
			   case 1 :if(i==0)  error1()  ; i = 1 ; break ;
			   case 2 :if(i==1)  error2() ; i = 2 ; break ;
			   case 3 : error3() ; error_num = 0 ; i = 0 ;break ;
			   default : break ;
			}
       }
	   in_success() ;
	   error_num = 0 ;
}         //第一次开机 ！


void xiugai_code()  //number是实实在在的数字 不是字符！
{
     while(1)
	 {
	   v = ad_get() ;
	   number1 = keynum1_judge(v) ;
	   if(number1!=50&&number1!=XIUGAIWANCHENG&&number1!=70)  
       {
	      code1[quanlity++] = number1+48 ; //数字转换成对应的字符 
	 	  lcd_write_ch(2,row++,numb[number1]) ;
	 	  delay_ms(260) ;
	   }
	   if(number1==XIUGAIWANCHENG)
       {
	      error_num = 0  ;
	 	  memset(code,'\0',50) ;
	 	  strcpy(code,code1);   //替换修改后的密码
	 	  EEPROM_RESET();
	 	  EEPROM_WriteString(code1,0x00) ;  //写入修改后的密码
		  memset(code_in,'\0',50) ;
	 	  memset(code1,'\0',50) ; 
	 	  xiugai = 2 ;
	 	  break ;
	   }
	   else if(number1==70)   //返回主菜单  本次修改密码取消
       {
	      xiugai = 0 ;	
		  home1() ;
		  quanlity = 0 ;
		  row = 0 ;
		  memset(code_in,'\0',50) ;
		  memset(code1,'\0',50) ;
		  break ;
	   }
	   else if(number1==63) //不返回主菜单 重新输入密码 
       {
	      lcd_clear_hang(2);
		  quanlity = 0 ;
		  row = 0 ;
		  memset(code_in,'\0',50) ;
		  memset(code1,'\0',50) ;
	   }
    }
    error_num = 0 ;
}