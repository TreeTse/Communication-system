#ifndef	_spi_h
#define _spi_h

#include <iom48v.h>
#define uchar unsigned char 
#define uint unsigned int 


#define	 SHCP_H	   (PORTC|=0X02)
#define	 SHCP_L	   (PORTC&=~0X02)

#define	 STCP_H	   (PORTC|=0X01)
#define	 STCP_L	   (PORTC&=~0X01)

#define	 DS_H	   (PORTC|=0X04)
#define	 DS_L	   (PORTC&=~0X04)


void spi_init(void);
void spi_send(uint data);



#endif