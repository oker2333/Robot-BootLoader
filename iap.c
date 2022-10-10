#include <stdio.h>
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

void Write_APP_Size(uint32_t File_Bytes)
{
	 flash_write_buffer(APP_SIZE_ADDRESS, (uint8_t*)&File_Bytes,sizeof(uint32_t));
}

uint8_t Update_Init_Flag(void)
{
	uint32_t Valid_Flag = 0xAABBCCDD;
	uint32_t Current_Flag = flash_read_word(INFO_ADDRESS);
	if(Current_Flag != Valid_Flag)
	{
		 Write_APP_Size(0);
		 flash_write_buffer(INFO_ADDRESS, (uint8_t*)&Valid_Flag,sizeof(uint32_t));
		 return 0;		//first init
	}
	return 1;
}

int32_t Jump_to_APP(void)
{
		if (0x20000000 == ((*(volatile uint32_t*)APP_ADDRESS) & 0x2FFE0000))
		{
			  uint8_t succeed = 1;
			  uint8_t init_flag = Update_Init_Flag();
			  
			  uint32_t file_size = flash_read_word(APP_SIZE_ADDRESS);
			
				while((init_flag != 0) && (file_size <= APP_FLASH_SIZE) && (file_size > 0))
				{
					 flash_write_buffer(APP_ADDRESS, (uint8_t*)APP_BACKUP_ADDRESS,file_size);
					
					 for(int i = 0;i < file_size;i++)
					 {
							if(*((volatile uint8_t *)(APP_ADDRESS+i)) != *((volatile uint8_t *)(APP_BACKUP_ADDRESS+i)))
							{
								 printf("app data check doesn't pass\r\n");
								 succeed = 0;
								 break;
							}
					 }
					 
					 if(succeed){
						  printf("OTA Upgrade Succeed\r\n");
							Write_APP_Size(0);
					 }
					 file_size = flash_read_word(APP_SIZE_ADDRESS);
				}
				
			  printf("Now Jump to Appication\r\n");		
				
				Jump_To_ADDR_t Jump_To_Application = (Jump_To_ADDR_t)(*(volatile uint32_t*)(APP_ADDRESS + 4));
			
				uint32_t offset = APP_ADDRESS - FLASH_BASE_ADDR;
				nvic_vector_table_set(NVIC_VECTTAB_FLASH, offset);
			  
				__set_MSP(*(volatile uint32_t*) APP_ADDRESS);
				Jump_To_Application();
		}
		
		printf("Invalid Stack Address\r\n");
		
		return 1;
}
