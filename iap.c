#include "iap.h"
#include "iap_config.h"
#include "gd32f30x_libopt.h"

fmc_state_enum flash_write_word(uint32_t address, uint32_t data)
{
	  fmc_state_enum state;
		
    fmc_unlock();
		state = fmc_word_program(address, data);
	
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
	
	  fmc_lock();
		
		return state;
}

uint32_t APP_Jump_Address(void)
{
	 uint32_t jump_address = *((uint32_t*)APP_ADDR_ADDRESS);
	
	 if((jump_address == APP_A_ADDRESS) || (jump_address == APP_B_ADDRESS))
	 {
		  return jump_address;
	 }
	 printf("APP_ADDR doesn't exist,the default address is APP_A_ADDRESS\r\n");
	 flash_write_word(APP_ADDR_ADDRESS,APP_A_ADDRESS);		//���ڵ�һ��ota����ʱ��A���������жϵ�ǰ����������ַ
	 
	 return APP_A_ADDRESS;			//������¼��app addressδд�룬����ֱ����ת��A����
}

int32_t Jump_to_APP(void)
{
		uint32_t jump_addr = APP_Jump_Address();
		printf("jump_addr = 0x%x\r\n",jump_addr);
	  
		if (0x20000000 == ((*(volatile uint32_t*)jump_addr) & 0x2FFE0000))
		{
			  printf("Now Jump To APP\r\n");

				Jump_To_ADDR_t Jump_To_Application = (Jump_To_ADDR_t)(*(volatile uint32_t*)(jump_addr + 4));
			
				uint32_t offset = *((volatile uint32_t*)APP_ADDR_ADDRESS) - FLASH_BASE_ADDR;
				nvic_vector_table_set(NVIC_VECTTAB_FLASH, offset);
			  
				__set_MSP(*(volatile uint32_t*) jump_addr);
				Jump_To_Application();
		}
		return 1;
}
