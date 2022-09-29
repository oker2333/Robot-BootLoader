#include "iap.h"
#include "iap_config.h"
#include "gd32f30x_libopt.h"

uint32_t APP_Download_Address(void)
{
	uint32_t download_addr = 0x00;
	uint32_t cur_addr = *((uint32_t*)APP_ADDR_ADDRESS);
	if(cur_addr == APP_A_ADDRESS)
	{
		 download_addr = APP_B_ADDRESS;
	}
	else if(cur_addr == APP_B_ADDRESS)
	{
		 download_addr = APP_A_ADDRESS;
	}
	else
	{
		 download_addr = APP_A_ADDRESS;
	}
	return download_addr;
}

uint32_t APP_Jump_Address(void)
{
	 uint32_t jump_address = IAP_ADDRESS;
	 uint32_t download_addr = APP_Download_Address();
	 
	 if(download_addr == APP_A_ADDRESS)
	 {
		  jump_address = APP_B_ADDRESS;
	 }
	 else
	 {
		   jump_address = APP_A_ADDRESS;
	 }
	
	 return jump_address;
}

int32_t Jump_to_APP(void)
{
		uint32_t jump_addr = APP_Jump_Address();
		printf("jump_addr = 0x%x\n",jump_addr);
	  
		if (0x20000000 == ((*(volatile uint32_t*)jump_addr) & 0x2FFE0000))
		{
			  printf("Jump to APP\r\n");

				Jump_To_ADDR_t Jump_To_Application = (Jump_To_ADDR_t)(*(volatile uint32_t*)(jump_addr + 4));
			
				uint32_t offset = *((volatile uint32_t*)APP_ADDR_ADDRESS) - FLASH_BASE_ADDR;
				nvic_vector_table_set(NVIC_VECTTAB_FLASH, offset);
			  
				__set_MSP(*(volatile uint32_t*) jump_addr);
				Jump_To_Application();
		}
		return 1;
}
