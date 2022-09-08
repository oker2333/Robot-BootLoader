#include "gd32f30x_libopt.h"
#include "bsp.h"
#include "iap.h"

/*
	FLASH:256KB,start:0x8000000,size:0x40000
	RAM:48KB,start:0x20000000,size:0xC000
	CPU:120MHz
*/

/*
	IAP start:0x0800 0000  size:32KB	(0x8000 Bytes)
	APP start:0x0800 8000	 size:222KB	(0x37800 Bytes)
	INF start:0x0803 f800  size:2KB		(0x800 Bytes)
*/

int main(void)
{
	  nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
		__disable_irq();
		usart_config(115200);
		
		while(1){
			 switch(IAP_ReadFlag()){
				 case APPRUN_FLAG_DATA:
					 Jump_to_APP();
					 break;
				 case UPDATE_FLAG_DATA:
					 if(Download2Flash() > 0)
					 {
						  IAP_WriteFlag(APPRUN_FLAG_DATA);
					 }
					 break;
				 case UPLOAD_FLAG_DATA:
					 break;
				 default:
					 printf("No APP Data\n");
					 break;
				 
			 }
		}
}
