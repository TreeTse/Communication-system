#include "TWI.h"

extern dis_code[] ;
extern uchar minute,second,hour,year,month,day,week ;
extern uchar beijingshijian[] , nian[] , yue[] , ri[] , numb[][16] , 
	         shi[] , fen[] , miao[],xingqi[],shuzi[][32],kongge[],shezhinaozhong[] ;
//这些数组都是中文
extern uchar min,sec,ho , ye , mon , da ;   //缺字母的为闹钟的 
extern uchar mi  ,s  ,h  ,y , mo , d ; 

void delayms(uint n)
{
 	 uint i , j ;
	 for(i=0;i<n;i++)
	   for(j=0;j<1141;j++);
}

void seg_display(uchar min , uchar sec)
{
 	 PORTB |= BIT(0);
	 PORTB &= BIT(0) ;
	 PORTA = dis_code[min/10];
	 delayms(2) ;
	 PORTB |= BIT(1);
	 PORTB &= BIT(1) ;
	 PORTA = dis_code[min%10];
	 delayms(2) ;

	 PORTB |= BIT(2);
	 PORTB &= BIT(2) ;
	 PORTA = dis_code[sec/10];
	 delayms(2) ;

	 PORTB |= BIT(3);
	 PORTB &= BIT(3) ;
	 PORTA = dis_code[sec%10];
	 delayms(2) ;
}

void TWI_init(void)
{  
    TWCR=(1<<TWEA)|(1<<TWEN);
    TWBR=32;
    TWSR=0;
}

uchar TWI_Write(uchar Wdata,uchar RegAddress)
{
   DDRD|=(1<<PD0)|(1<<PD1);
   Start();      		 	   //TWI启动
   Wait();
   if(Satus()!=START) 
   return FALSE;    
   
   Write8Bit(WD_DS1307_ADDR); //写 AT24C02C地址和写方式
   Wait();
   if(Satus()!=MT_SLA_ACK) 
   return FALSE;     
      
   Write8Bit(RegAddress);  	   //写 AT24C02C相应寄存器地址
   Wait();
   if(Satus()!=MT_DATA_ACK) 
   return FALSE;        
  
   Write8Bit(Wdata);     	   //写数据到 AT24C02C相应寄存器
   Wait();
   if(Satus()!=MT_DATA_ACK) 
   return FALSE;         
   
   Stop();        	 		   //TWI停止
   delayms(10);   		   	   //延时
   return TRUE;
}


uchar TWI_Read(uchar RegAddress)
{
   uchar temp;
   Start();	   			  	   //TWI启动
   Wait();
   if (Satus()!=START) 
      return FALSE;         
   Write8Bit(WD_DS1307_ADDR); //写 AT24C02C地址和写方式
   Wait(); 
   if (Satus()!=MT_SLA_ACK) 
      return FALSE;        
   Write8Bit(RegAddress);      //写 AT24C02C相应寄存器地址
   Wait();
   if (Satus()!=MT_DATA_ACK) 
      return FALSE;
    
   Start();            	       //TWI重新启动
   Wait();
   if (Satus()!=RE_START)  
      return FALSE;
    
   Write8Bit(RD_DS1307_ADDR); //写 AT24C02C地址和读方式
   Wait();
   if(Satus()!=MR_SLA_ACK)  
      return FALSE;      
    
   TWI_READ();          	   //启动主TWI读方式
   Wait();
   /*读完一个数停止接收数据,主机接收到不再接收应答信号,如果继续接收到应答信号
   说明主机继续接收数据则为错误*/
   if(Satus()!=MR_DATA_NOACK)												 																		 
      return FALSE;     
   temp=TWDR;        		   //读取 TWI接收数据
   Stop();          		   //TWI停止
   return temp;
}

uchar BCD_change(uchar n)  //BCD转化成十进制
{
   n = (n>>4)*10+(n&0x0f) ;
   return n ;
}

uchar change_BCD(uchar n )  //十进制转化成BCD
{
   n = (n/10)*16+n%10 ;
   return n ;
}

uchar read_time(uchar address)
{
   uchar time ;
   time = TWI_Read(address) ;
   time = BCD_change(time) ;
   return time ;
}

uchar show_time() 
{    
    second = read_time(0x00) ;
	minute = read_time(0x01) ;
	hour = read_time(0x02) ;
	week = TWI_Read(0x03) ;
	day = read_time(0x04) ;
	month = read_time(0x05) ;
	year = read_time(0x06) ;
	lcd_write_hanzi_hanzi_hang(0,0,beijingshijian,8) ;
	
	lcd_write_hanzi(1,2,nian);    //第二行是日期       
	lcd_write_hanzi(1,4,yue);
	lcd_write_hanzi(1,6,ri);
	lcd_write_ch(1,0,numb[2]);
	lcd_write_ch(1,1,numb[0]);
	lcd_write_ch(1,2,numb[year/10]);
	lcd_write_ch(1,3,numb[year%10]);
	lcd_write_ch(1,6,numb[month/10]);
	lcd_write_ch(1,7,numb[month%10]);
	lcd_write_ch(1,10,numb[day/10]);
	lcd_write_ch(1,11,numb[day%10]);
	
	lcd_write_hanzi(2,1,shi);      //第三行是时间
	lcd_write_hanzi(2,3,fen);
	lcd_write_hanzi(2,5,miao);
	lcd_write_ch(2,0,numb[hour/10]);
	lcd_write_ch(2,1,numb[hour%10]);
	lcd_write_ch(2,4,numb[minute/10]);
	lcd_write_ch(2,5,numb[minute%10]);
	lcd_write_ch(2,8,numb[second/10]);
	lcd_write_ch(2,9,numb[second%10]);
	
	lcd_write_hanzi_hanzi_hang(3,3,xingqi,2) ;//第四行是星期
	lcd_write_hanzi(3,5,shuzi[week]); 
}

void shanshuo_nian()  //年闪烁
{
    lcd_write_ch(1,2,numb[year/10]);
	lcd_write_ch(1,3,numb[year%10]);
	delay_ms(100) ;
	lcd_write_ch(1,2,kongge);
	lcd_write_ch(1,3,kongge);
	delay_ms(100) ;
}

void shanshuo_yue()  //月闪烁 
{
 	lcd_write_ch(1,6,numb[month/10]);
	lcd_write_ch(1,7,numb[month%10]);
	delay_ms(100) ;
	lcd_write_ch(1,6,kongge);
	lcd_write_ch(1,7,kongge);
	delay_ms(100) ;
}

void shanshuo_ri()  //日闪烁
{
    lcd_write_ch(1,10,numb[day/10]);
	lcd_write_ch(1,11,numb[day%10]);
	delay_ms(100) ;
	lcd_write_ch(1,10,kongge);
	lcd_write_ch(1,11,kongge);
	delay_ms(100) ;
}

void shanshuo_shi() //时闪烁
{
    lcd_write_ch(2,0,numb[hour/10]);
	lcd_write_ch(2,1,numb[hour%10]);
	delay_ms(100) ;
	lcd_write_ch(2,0,kongge);
	lcd_write_ch(2,1,kongge);
	delay_ms(100) ;
}

void shanshuo_fen()  //分闪烁
{
    lcd_write_ch(2,4,numb[minute/10]);
	lcd_write_ch(2,5,numb[minute%10]);
	delay_ms(100) ;
	lcd_write_ch(2,4,kongge);
	lcd_write_ch(2,5,kongge);
	delay_ms(100) ;
}

void shanshuo_miao() //秒闪烁
{
    lcd_write_ch(2,8,numb[second/10]);
	lcd_write_ch(2,9,numb[second%10]);
	delay_ms(100) ;
	lcd_write_ch(2,8,kongge);
	lcd_write_ch(2,9,kongge);
	delay_ms(100) ;
}

void shanshuo_ye()  //年闪烁
{
    lcd_write_ch(1,2,numb[ye/10]);
	lcd_write_ch(1,3,numb[ye%10]);
	delay_ms(100) ;
	lcd_write_ch(1,2,kongge);
	lcd_write_ch(1,3,kongge);
	delay_ms(100) ;
}

void shanshuo_mon()  //月闪烁 
{
    lcd_write_ch(1,6,numb[mon/10]);
	lcd_write_ch(1,7,numb[mon%10]);
	delay_ms(100) ;
	lcd_write_ch(1,6,kongge);
	lcd_write_ch(1,7,kongge);
	delay_ms(100) ;
}

void shanshuo_da()  //日闪烁
{
    lcd_write_ch(1,10,numb[da/10]);
	lcd_write_ch(1,11,numb[da%10]);
	delay_ms(100) ;
	lcd_write_ch(1,10,kongge);
	lcd_write_ch(1,11,kongge);
	delay_ms(100) ;
}

void shanshuo_ho() //时闪烁
{
    lcd_write_ch(2,0,numb[ho/10]);
	lcd_write_ch(2,1,numb[ho%10]);
	delay_ms(100) ;
	lcd_write_ch(2,0,kongge);
	lcd_write_ch(2,1,kongge);
	delay_ms(100) ;
}

void shanshuo_min()  //分闪烁
{
    lcd_write_ch(2,4,numb[min/10]);
	lcd_write_ch(2,5,numb[min%10]);
	delay_ms(100) ;
	lcd_write_ch(2,4,kongge);
	lcd_write_ch(2,5,kongge);
	delay_ms(100) ;
}

void shanshuo_sec() //秒闪烁
{
    lcd_write_ch(2,8,numb[sec/10]);
	lcd_write_ch(2,9,numb[sec%10]);
	delay_ms(100) ;
	lcd_write_ch(2,8,kongge);
	lcd_write_ch(2,9,kongge);
	delay_ms(100) ;
}

void write_time() //写修改后的时间
{
    year = change_BCD(year) ;
	month = change_BCD(month) ;
	day = change_BCD(day) ; 
	hour = change_BCD(hour) ;
	minute = change_BCD(minute) ;
	second = change_BCD(second) ;
	TWI_Write(second,0x00) ;
	TWI_Write(minute,0x01) ;
	TWI_Write(hour,0x02) ;
	TWI_Write(day,0x04) ;
	TWI_Write(month,0x05) ;
	TWI_Write(year,0x06) ;
} 

void show_alarm() 
{
	lcd_write_hanzi_hanzi_hang(0,0,shezhinaozhong,8) ;
	lcd_write_hanzi(1,2,nian);    //第二行是日期       
	lcd_write_hanzi(1,4,yue);
	lcd_write_hanzi(1,6,ri);
	lcd_write_ch(1,0,numb[2]);
	lcd_write_ch(1,1,numb[0]);
	lcd_write_ch(1,2,numb[ye/10]);
	lcd_write_ch(1,3,numb[ye%10]);
	lcd_write_ch(1,6,numb[mon/10]);
	lcd_write_ch(1,7,numb[mon%10]);
	lcd_write_ch(1,10,numb[da/10]);
	lcd_write_ch(1,11,numb[da%10]);
	
	lcd_write_hanzi(2,1,shi);      //第三行是时间
	lcd_write_hanzi(2,3,fen);
	lcd_write_hanzi(2,5,miao);
	lcd_write_ch(2,0,numb[ho/10]);
	lcd_write_ch(2,1,numb[ho%10]);
	lcd_write_ch(2,4,numb[min/10]);
	lcd_write_ch(2,5,numb[min%10]);
	lcd_write_ch(2,8,numb[sec/10]);
	lcd_write_ch(2,9,numb[sec%10]);
}