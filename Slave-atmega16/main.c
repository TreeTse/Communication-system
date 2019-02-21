#include "main.h"

uint  number = 1 ,number_1,k ;
uint sushu[250]  ;

void sushu_judge() 
{
  	 uint i , j ;
	 //TWI_Write(1,0) ;
	 for(i=2;i<=500;i++)
     {
        for(j=2;j<i;j++)
        {
		  if(i%j==0)
          break;
		}
        if(j==i)
	    {
            sushu[number]= i;
			if(i<256)
	        {    
		       TWI_Write(i,number) ;
		       number_1 = number ;
			}
		    else
		    {
		       TWI_Write(i-256,number) ;
			}
	        number++ ;
	     }
       }
}

void main()
{
 	 DDRB |= BIT(0) ;
	 //usart_init() ;
	 //SEI() ;
	 //sushu_judge() ;
	 //SEI() ;
	 while(1) ;
}