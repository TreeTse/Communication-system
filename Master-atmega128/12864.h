#ifndef _12864_h
#define _12864_h

#define NUM(x) num##x   //这个宏定义表示连接前后

#define uchar unsigned char 
#define uint unsigned int 

#define en_set PORTA |= BIT(3)	
#define en_clr PORTA &= ~BIT(3) 
#define en_out DDRA |= BIT(3)   

#define rw_set PORTA |= BIT(4)	
#define rw_clr PORTA &= ~BIT(4) 
#define rw_out DDRA |= BIT(4)   
#define rw_in  DDRA &=~BIT(4)   

#define rs_set PORTA |= BIT(5)	
#define rs_clr PORTA &= ~BIT(5) 
#define rs_out DDRA |= BIT(5)   

#define cs1_set PORTA |= BIT(6)	
#define cs1_clr PORTA &= ~BIT(6) 
#define cs1_out DDRA |= BIT(6)   

#define cs2_set PORTA |= BIT(7)	
#define cs2_clr PORTA &= ~BIT(7) 
#define cs2_out DDRA |= BIT(7)

void lcd12864_init(void);
void lcd_write_ch(uchar x,uchar y,const uchar *ch);
void lcd_write_hanzi(uchar x,uchar y,const uchar *dat);
void lcd_write_str_hang(uchar x,uchar y,const uchar *ch,uchar n);			//12864写一行字符函数
void lcd_write_ch_fb(uchar x,uchar y,const uchar *ch);	  	  	 		  //12864写一个字母或数字反白函数
void lcd_write_str_fb(uchar x,uchar y,const uchar *ch,uchar n);      //12864写一行字母或数字反白函数
void lcd_write_hanzi_fb(uchar x,uchar y,const uchar *date);	 			  //12864写一个汉字反白显示函数
void lcd_write_hanzi_hang_fb(uchar x,uchar y,const uchar *date,uchar n);  //12864写一行汉字反白显示函数
void lcd_write_hanzi_hanzi_hang(uchar x,uchar y,const uchar *date,uchar n); //12864写一行汉字函数
void lcd_init(void);
void lcd_clear_csr(void);
void lcd_clear_hang(uchar x);	   		   //12864清除一行显示函数


#endif