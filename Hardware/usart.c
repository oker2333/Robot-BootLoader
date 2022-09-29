#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "gd32f30x_libopt.h"

int fputc(int ch, FILE *f)
{
	  (void)f;
    usart_data_transmit(USART0, ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
	  return 0;
}

void usart_config(uint32_t baudval)
{
		rcu_periph_clock_enable(RCU_GPIOA);	//enable GPIO clock, PA9/PA10
		rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_USART0);	//enable USART clock
	
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);	//PA9--TX0
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);	//PA10--RX0

    usart_deinit(USART0);
    usart_baudrate_set(USART0, baudval);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}

