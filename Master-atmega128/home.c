#include "main.h"
#include "home.h"
#include "EEPROM.h"

extern uchar kaiji[],hello[],mistake1_0[],mistake1_1[],input_code[],
	   		 mistake2_0[],mistake3_0[],mistake3_1[],welcome[] ;//开机输入密码提示字符
extern uchar menu[] , function1[], function2[] , function3[],function4[],
	         function5[] , function6[],zhuandongdushu[] ,numb[][16] ,kongge[];
uchar code[50]  ; //这个数组为密码存放的
extern uchar shezhinaozhong[] , hello[] ,angel , qvxiao[] ,about_clock , 
             sushu_num[] , bu[] , gaiwei[]  , shi1[] , xieru[],su_num[] ;
extern uchar liushui[]; //这些数组都是 中文
extern uint number2 ;

void first_open()     //输入密码时lcd显示的
{
     lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(0,0,kaiji,8);  //您好，请输入开机
	 lcd_write_hanzi_hanzi_hang(1,0,hello,3);  //密码
}

void error1()
{
     lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(0,0,mistake1_0,8); //输入错误,还有两
	 lcd_write_hanzi_hanzi_hang(1,0,mistake1_1,4);//次机会!
	 delay_ms(1000);
	 lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(0,0,input_code,8);//请重新输入密码:
}

void error2() 
{
     lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(0,0,mistake2_0,8); //输入错误,还有一次机会
	 lcd_write_hanzi_hanzi_hang(1,0,mistake1_1,4);
	 delay_ms(1000);
	 lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(0,0,input_code,8);//请重新输入密码:
}

void error3()
{
     lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(0,0,mistake3_0,8); //您已输入错误三次
	 lcd_write_hanzi_hanzi_hang(1,0,mistake3_1,6);//请等待5秒！
	 delay_ms(5000);
	 lcd_clear_csr();
	 first_open() ;
}

void in_success() //输入密码成功
{
     lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(0,0,welcome,3); //欢迎......
	 delay_ms(1000) ;
	 lcd_clear_csr();
	 lcd_write_hanzi_hanzi_hang(0,3,menu,2); //菜单
	 lcd_write_hanzi_hanzi_hang(1,0,function1,6);//显示时间
	 lcd_write_hanzi_hanzi_hang(2,0,function2,6);//修改时间
	 lcd_write_hanzi_hanzi_hang(3,0,function3,6);//修改密码
}

void home1()
{
    lcd_clear_csr();
	lcd_write_hanzi_hanzi_hang(0,3,menu,2); //菜单
	lcd_write_hanzi_hanzi_hang(1,0,function1,6);//显示时间
	lcd_write_hanzi_hanzi_hang(2,0,function2,6);//修改时间
	lcd_write_hanzi_hanzi_hang(3,0,function3,6);//修改密码
}

void home2()
{
    lcd_clear_csr();
	lcd_write_hanzi_hanzi_hang(0,0,function4,6);//显示时间
	lcd_write_hanzi_hanzi_hang(1,0,function5,6);//修改时间
	lcd_write_hanzi_hanzi_hang(2,0,function6,6);//修改密码
	lcd_write_hanzi_hanzi_hang(3,0,sushu_num,6);
}

void read_code()  //从EEPROM读取密码
{
    uchar i ;
	for(i=0;EEPROM_ReadByte(i)!=0xff;i++)
	{	
        code[i] = EEPROM_ReadByte(i) ;
	}
}

void xiugai1()
{
    lcd_clear_csr();
	lcd_write_hanzi_hanzi_hang(0,0,shezhinaozhong,5);
	lcd_write_hanzi_hanzi_hang(0,5,hello,3);
}

void zhuandong()
{
    lcd_write_hanzi_hanzi_hang(0,0,zhuandongdushu,5);
	lcd_write_ch(0,10,numb[angel/100]);
	lcd_write_ch(0,11,numb[angel%100/10]);
	lcd_write_ch(0,12,numb[angel%10]);
	lcd_clear_hang(1);
	lcd_clear_hang(2);
	lcd_clear_hang(3);
}

void liushuideng()
{
 	lcd_write_hanzi_hanzi_hang(0,0,liushui,8);
	lcd_clear_hang(1);
	lcd_clear_hang(2);
	lcd_clear_hang(3);
}

void liushuidengqvxiao()
{
    lcd_clear_csr();
	lcd_write_hanzi_hanzi_hang(0,0,liushui,3);
	lcd_write_hanzi_hanzi_hang(0,3,qvxiao,2);
	delay_ms(700) ;
	about_clock = 0 ;
	home1() ;
}


void sushuxieru()
{
    lcd_clear_csr();
	lcd_write_hanzi_hanzi_hang(0,0,su_num,2);
	lcd_write_hanzi_hanzi_hang(0,2,xieru,2);
	delay_ms(700) ;
	about_clock = 0 ;
	home1() ;
}

void bushisushu()
{
    lcd_clear_hang(0);
	lcd_clear_hang(1);
	lcd_clear_hang(2);
	lcd_clear_hang(3);
	lcd_write_hanzi_hanzi_hang(0,0,gaiwei,2);
	lcd_write_hanzi_hanzi_hang(0,2,bu,1);
	lcd_write_hanzi_hanzi_hang(0,3,shi1,1);
	lcd_write_hanzi_hanzi_hang(0,4,su_num,2);
}

void shisushu()
{
    lcd_write_hanzi_hanzi_hang(0,0,gaiwei,2);
	lcd_write_hanzi_hanzi_hang(0,2,shi1,1);
	lcd_write_hanzi_hanzi_hang(0,3,su_num,2);
	lcd_write_hanzi_hanzi_hang(0,5,kongge,1);
	lcd_write_ch(0,12,numb[number2/100]);
	lcd_write_ch(0,13,numb[number2%100/10]);
	lcd_write_ch(0,14,numb[number2%10]);
	lcd_clear_hang(1);
	lcd_clear_hang(2);
	lcd_clear_hang(3);
}