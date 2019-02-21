#include "spi.h"
#include "macros.h"


void spi_init(void)
{
 	DDRC|=BIT(0)|BIT(1)|BIT(2);
}

void spi_send(uint data)
{
 	uchar  i,num;
	for(i=0;i<16;i++)
	{
	 	num=((data>>i)&1);
		if(num)
		{
		 	DS_H;
		}
		else
		{
		 	DS_L;
		}
		SHCP_L;
		SHCP_H;
		STCP_L;
		STCP_H;
	}

}