#include <stdio.h>
#include "gd32f30x_libopt.h"
#include "bsp.h"
#include "iap.h"

/*详见OTA升级方案*/

int main(void)
{
		__disable_irq();
		usart_config(3000000);
	  
		Jump_to_APP();
}
