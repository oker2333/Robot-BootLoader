#include <stdio.h>
#include "gd32f30x_libopt.h"
#include "bsp.h"
#include "iap.h"

/*���OTA��������*/

int main(void)
{
		__disable_irq();
		usart_config(460800);
	  
		Jump_to_APP();
}
