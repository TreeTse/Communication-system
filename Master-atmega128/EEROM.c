/*EEPROM的读写*/
#include "main.h"

void EEPROM_WriteString(uchar *s,uchar address)
{
    while(*s)
	{
	    EEPROM_WriteByte(*s,address);
		delay_ms(5);
		s++;
		address++;
	}
}

void EEPROM_WriteByte(uchar data, uchar address)
{
	uchar sreg;
	sreg = SREG;
	SREG = ~0x80;
	while(EECR & (1<<EERE));
	EEARH = 0x00;
	EEARL = address;
	EEDR = data;
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
	
	SREG = sreg ;
}

uchar EEPROM_ReadByte(uchar address)
{
	uchar sreg;
	sreg = SREG;
	SREG = ~0x80;
	while(EECR & (1<<EERE));
	EEARH = 0x00;
	EEARL = address;
	EECR |= (1<<EERE);
	
	SREG = sreg ;
	return EEDR;
}

void EEPROM_RESET()
{
 	 uchar i ;
	 for(i=0;i<20;i++)
     {
	    EEPROM_WriteByte(0xFF,i);
		delay_ms(5) ;
	}
}