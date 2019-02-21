/*LCD12864液晶屏程序*/
#include<iom128v.h>
#include<macros.h>
#include"12864.h"
#include"main.h"

void lcd12864_init(void)
{
     DDRC=0xFF;		 //12864数据口，置为输出
 	 PORTC=0xFF;
 	 en_out;   //置EN输出
 	 rw_out;   //置rw输出
 	 rs_out;   //置rs输出
 	 cs1_out;//置cs1输出
 	 cs2_out;//置cs2输出 
}

/*12864写命令函数*/
void lcd_write_com(uchar com)
{
	rw_clr;			   //写操作
	delay_us(3);
	rs_clr;			  //写数据	
	PORTC=com;
	en_set;
	delay_us(3);
	en_clr;
}

/*12864写数据函数*/
void lcd_write_date(uchar date)
{
	rw_clr;	    //写操作
	delay_us(3);  
	rs_set;   //写数据
	PORTC=date;
	en_set;
	delay_us(3);
	en_clr;
}

/*
12864确定位置数据函数
页地址：x范围0-7
列地址：y范围0-63
*/
void lcd_setxy(uchar x,uchar y)
{
	lcd_write_com(0xb8+x);
	lcd_write_com(0x40+y);
}

/*
12864写一个字母或数字函数
页地址：x范围0-3
列地址：y范围0-15
*/ 
void lcd_write_ch(uchar x,uchar y,const uchar *ch)
{
	uchar i,j;
	if(y<8)			//判断列地址，如果小于8列，显示在左屏
	{
		cs1_clr;	//	CS1=0;
	    cs2_set;	//  CS2=1;
	}
	else 			//判断列地址，如果大于8列，显示在右屏
	{
		cs1_set;	//  CS1=1;
		cs2_clr;	//  CS2=0;
		y=y-8;
	}
	for(i=0;i<2;i++)
	{
	 	 /*x*2+i的意思是每个字符占用两页，一共可以显示4行（一共8页）*/
		 /*y*8 的意思是每次写入的列都是8的整数倍*/
		 lcd_setxy(x*2+i,y*8);	  			
		 for(j=0;j<8;j++)
		 {
		 	 lcd_write_date(ch[i*8+j]) ;   	//一共16个字节
			 delay_us(1);
		 }
	}
	cs1_clr;		 //    CS1=0;
	cs2_clr;		 //    CS2=0;
}

/*
12864写一个汉字函数
页地址：x范围0-3
列地址：y范围0-7
*/ 
void lcd_write_hanzi(uchar x,uchar y,const uchar *dat)
{
	uchar i,j;
	if(y<4)
	{
		cs1_clr;		   //CS1=0;
		cs2_set;    	   //CS2=1;
	}
	else 
	{
        cs1_set;		//CS1=1;
		cs2_clr;		//CS2=0;
		y-=4;
	}

	for(i=0;i<2;i++)
	{
		lcd_setxy(x*2+i,y*16);	  // x*2+i的意思是每个汉字占用两页，一共可以显示4行（一共8页）
		delay_us(1);			  //每次写入都是16的倍数列
		for(j=0;j<16;j++)
		{
			lcd_write_date(dat[i*16+j]) ;   	//一共16个字节
			delay_us(3);
		}	
	}
	cs1_clr;			//CS1=0;
	cs2_clr;			//CS2=0;
}

/***
******12864写一行字符函数
******页地址：x范围0-3
******列地址：y范围0-15
******字符数:0-15
***/
void lcd_write_str_hang(uchar x,uchar y,const uchar *ch,uchar n)
{
	uchar i;
	for(i=0;i<n;i++)
	{
		lcd_write_ch(x,y,ch+16*i);	//指针相加
		y++;
		delay_us(3);	
	}
}


/*
12864写一行汉字函数
页地址：x范围0-3
列地址：y范围0-7
字符数:0-7*/
void lcd_write_hanzi_hanzi_hang(uchar x,uchar y,const uchar *date,uchar n)
{
	uchar i;
	for(i=0;i<n;i++)
	{
		lcd_write_hanzi(x,y,date+i*32);
		y++;	
		delay_us(3);
	}
}

/*
12864写一个字母或数字反白函数
页地址：x范围0-3
列地址：y范围0-15
*/ 
void lcd_write_ch_fb(uchar x,uchar y,const uchar *ch)
{
	uchar i,j;
	if(y<8)
	{
		cs1_clr;		   //CS1=0;
		cs2_set;		   //CS2=1;
	}
	else 
	{
		cs1_set;		   //CS1=1;
		cs2_clr;		   //CS2=0;
		y-=8;
	}
	
	for(i=0;i<2;i++)
	{
		lcd_setxy(x*2+i,y*8);		
		{
			for(j=0;j<8;j++)
			{
				lcd_write_date(255-ch[i*8+j]) ;   	//一共16个字节	
				delay_us(3);
			}
		}
	}
	cs1_clr; 				//CS1=0;
	cs2_clr;				//CS2=0;	
}

/*
12864写一行字母或数字反白函数
页地址：x范围0-3
列地址：y范围0-15
*/ 
void lcd_write_str_fb(uchar x,uchar y,const uchar *ch,uchar n)
{
    uchar i;
 	for(i=0;i<n;i++)
 	{
       lcd_write_ch_fb(x,y,ch+i*16);
	   y++;
	   delay_us(3);
    }
}
/*
12864写一个汉字反白显示函数
页地址：x范围0-3
列地址：y范围0-7
字符数: n范围0-7
*/
void lcd_write_hanzi_fb(uchar x,uchar y,const uchar *date)
{
	uchar i,j;
	if(y<4)
	{
		cs1_clr;			  //CS1=0;
		cs2_set;			  //CS2=1;
	}
	else
	{
		cs1_set;			  //CS1=1;
		cs2_clr;			  //CS2=0;
		y-=4;
	}

	for(i=0;i<2;i++)
	{
		lcd_setxy(x*2+i,y*16);	
		for(j=0;j<16;j++)
		{
			lcd_write_date(255-date[i*16+j]) ;   	//一共16个字节	
			delay_us(3);
		}	
	}
	cs1_clr; 				//CS1=0;
	cs2_clr;				//CS2=0;
}

/*
12864写一行汉字反白显示函数
页地址：x范围0-3
列地址：y范围0-7
字符数: n范围0-7
*/
void lcd_write_hanzi_hang_fb(uchar x,uchar y,const uchar *date,uchar n)
{
	uchar i;
	for(i=0;i<n;i++)
	{
	 	lcd_write_hanzi_fb(x,y,date+i*32);
		y++;
		delay_us(3);
	}
}

/*
12864初始化函数
*/
void lcd_init(void)
{
	lcd_write_com(0x3f);   	//开显示开关
	lcd_write_com(0xc0);	//LCD行初始地址，共64行
	lcd_write_com(0xb8);	//LCD页初始地址	 共8页
	lcd_write_com(0x40);	//LCD列初始地址，共64列
}

/*
12864清屏函数
*/
void lcd_clear_csr(void)
{
	uchar i,j;
//	CS1=0;
	cs1_clr;
//  CS2=1;
	cs2_set;
	for(i=0;i<8;i++)		 		 //清除左屏
	{
		lcd_setxy(i,0);
		for(j=0;j<64;j++)
		{
			lcd_write_date(0) ;
			delay_us(1);
		}
		lcd_setxy(0,0);	
	}
	//CS1=1;
	cs1_set;					   		 //清除右屏
	//CS2=0;
	cs2_clr;
	for(i=0;i<8;i++)
	{
		lcd_setxy(i,0);
		for(j=0;j<64;j++)
		{
			lcd_write_date(0) ;
			delay_us(1);
		}
		lcd_setxy(0,0);	
	}
}

/*
12864清除一行显示函数
	x :0-3
*/
void lcd_clear_hang(uchar x)
{
	uchar i,j;
	cs1_clr;  //CS1=0;		//左屏
	cs2_set;  //CS2=1;
	for(i=0;i<2;i++)
	{
		lcd_setxy(x*2+i,0);
		delay_us(2);
		for(j=0;j<64;j++)
		{
			lcd_write_date(0);
			delay_us(2);
		}	
	}

	cs1_set;			//CS1=1;  右屏
	cs2_clr;			//CS2=0;
	for(i=0;i<2;i++)
	{
		lcd_setxy(x*2+i,0);
		delay_us(2);
		for(j=0;j<64;j++)
		{
			lcd_write_date(0);
			delay_us(2);
		}	
	}
}