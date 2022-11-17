#include <stdio.h>
#include "gd32f30x_libopt.h"
#include "bsp.h"
#include "iap.h"
#include "iap_config.h"

/*详见OTA升级方案*/

int main(void)
{
		__disable_irq();
		usart_config(3000000);
	
    printf("bootloader V%d.%d is running\r\n",MAJOR_VERSION,MINOR_VERSION);
	
		Jump_to_APP();
}
