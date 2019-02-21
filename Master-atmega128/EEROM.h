#ifndef EEROM_H_
#define EEROM_H_

#include <iom128v.h>
#include <macros.h>

#define uchar unsigned char
#define uint unsigned int 


void EEPROM_WriteByte(uchar data, uchar address);
uchar EEPROM_ReadByte(uchar address);
void EEPROM_WriteString(uchar *s,uchar address);


#endif