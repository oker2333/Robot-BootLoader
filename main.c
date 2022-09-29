#include <stdio.h>
#include "gd32f30x_libopt.h"
#include "bsp.h"
#include "iap.h"


/*
	FLASH:256KB,start:0x8000000,size:0x40000
	RAM:48KB,start:0x20000000,size:0xC000
	CPU:120MHz
*/

/*
	IAP				 start:0x08000000(134217728)    size:16KB		(16384 Bytes)  (0x4000)
	APP A 		 start:0x08004000(134234112)		size:116KB	(118784 Bytes) (0x1D000)
	APP B 	   start:0x08021000(134352896)		size:116KB	(118784 Bytes) (0x1D000)
	FLAG   		 start:0x0803E000(134471680)		size:2KB		(2048 Bytes)	 (0x800)
	APP ADDR	 start:0x0803E800(134473728)		size:2KB		(2048 Bytes)	 (0x800)
	INFO			 start:0x0803F000(134475776)		size:4KB		(4096 Bytes)	 (0x1000)
*/


int main(void)
{
	  nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
		__disable_irq();
		usart_config(460800);
	  
		Jump_to_APP();
}
