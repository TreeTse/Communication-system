#include "delay.h"
#include "main.h"

void delay_ms(uint n)
{
     uint i,j;
	 for(i=0;i<n;i++)
	    for(j=0;j<1141;j++);
}

void delay_us(uint x)
{
     while(x--)
 	 {
  	    asm("nop");
 	 }
}

void delay(uint x)
{
     uint m,n;
 	 for(m=x;m>0;m--)
 	   for(n=1320;n>0;n--);
}