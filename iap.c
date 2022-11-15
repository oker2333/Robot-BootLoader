#include <stdio.h>
#include <stdint.h>
#include "iap.h"
#include "iap_config.h"
#include "gd32f30x_libopt.h"

uint32_t flash_read_word(uint32_t address)
{
	  return *((uint32_t*)address);
}

uint16_t flash_read_dword(uint32_t address)
{
	  return *((uint16_t*)address);
}

uint8_t flash_read_byte(uint32_t address)
{
	  return *((uint8_t*)address);
}

void flash_read_buffer(uint32_t address, uint8_t *data,uint32_t data_len)
{
	  uint32_t* ptr = (uint32_t*)data;
		for(int i = 0;i < data_len/4;i++){
				*ptr++ = (*((volatile uint32_t*)(uint32_t)address));
			  address += 4;
		}
		
		uint8_t* byte_ptr = (uint8_t*)ptr;
		
		for(int j = 0;j < data_len%4;j++)
		{
			 *byte_ptr++ = (*((volatile uint8_t*)(uint32_t)address++));
		}
}

uint32_t flash_write_buffer(uint32_t address, uint8_t *data,uint32_t data_len)
{
	  fmc_state_enum fmc_state;
	  uint32_t ret = 0;
	  uint8_t remainder = 0;
	  uint32_t write_value = 0;
	  uint16_t half_word = 0;
	  
	  fmc_unlock();

		remainder = address % 2;
	
		address -= remainder;
	
		for(int i = (address / FMC_PAGE_SIZE);i <= ((address + data_len) / FMC_PAGE_SIZE);i++)
		{
			 fmc_page_erase(i * FMC_PAGE_SIZE);
		}
		
	  if(remainder){
			  half_word = data[0] << 8;
			  fmc_state = fmc_halfword_program(address, half_word);
				
				fmc_flag_clear(FMC_FLAG_BANK0_END);
				fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
				fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
			
				if(fmc_state != FMC_READY){
				    ret = address;
					goto exit;
				}
			
			  address += 2;
			  data_len -= remainder;
			  data++;
		}
		
		for(int j = 0;j < data_len/4;j++)
		{
			  write_value = (data[4*j + 0] << 0) | (data[4*j + 1] << 8) | (data[4*j + 2] << 16) | (data[4*j + 3] << 24);
			  
				fmc_state = fmc_word_program(address, write_value);
			  address += 4;
			
				fmc_flag_clear(FMC_FLAG_BANK0_END);
				fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
				fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
			
				if(fmc_state != FMC_READY){
				    ret = address;
					goto exit;
				}
		}
		
		if(data_len%4 != 0)
		{
			 switch(data_len%4)
			 {
				 case 1:
					 write_value = data[data_len-1];
					 break;
				 case 2:
					 write_value = data[data_len-2] | (data[data_len-1] << 8);
					 break;
				 case 3:
					 write_value = data[data_len-3] | (data[data_len-2] << 8) | (data[data_len-1] << 16);
					 break;
				 default:
					 write_value = 0;
					 break;
			 }
				fmc_state = fmc_word_program(address, write_value);
			
				fmc_flag_clear(FMC_FLAG_BANK0_END);
				fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
				fmc_flag_clear(FMC_FLAG_BANK0_PGERR);

				if(fmc_state != FMC_READY){
				    ret = address;
					goto exit;
				}			 
		}
		
exit:
	  fmc_lock();
		return ret;
}

uint8_t Update_Init_Flag(void)
{
	uint32_t valid_flag = 0xAABBCCDD;
	uint32_t current_flag = flash_read_word(INFO_ADDRESS);
	if(current_flag != valid_flag)
	{
		 uint32_t jump_addr = APP_ADDRESS_A;
		 flash_write_buffer(JUMP_ADDR_ADDRESS, (uint8_t*)&jump_addr,sizeof(uint32_t));
		 flash_write_buffer(INFO_ADDRESS, (uint8_t*)&valid_flag,sizeof(uint32_t));
		 return 0;		//first init
	}
	return 1;
}

void Jump_to_APP(void)
{
	  Update_Init_Flag();
	
	  uint32_t jump_address = 0x00;
		flash_read_buffer(JUMP_ADDR_ADDRESS,(uint8_t*)&jump_address,sizeof(uint32_t));
		  
		if (0x20000000 == ((*(volatile uint32_t*)jump_address) & 0x2FFE0000))
		{
			  printf("Now Jump to Appication, Stack Address = %d\r\n",jump_address);		
				
				Jump_To_ADDR_t Jump_To_Application = (Jump_To_ADDR_t)(*(volatile uint32_t*)(jump_address + 4));
			
				uint32_t offset = jump_address - FLASH_BASE_ADDR;
				nvic_vector_table_set(NVIC_VECTTAB_FLASH, offset);
			  
				__set_MSP(*(volatile uint32_t*) jump_address);
				Jump_To_Application();
		}
		else
		{
			  printf("Invalid Stack Address = %d\r\n",jump_address);
		}
		while(1);
}
