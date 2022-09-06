#include "flash_rw.h"
#include "gd32f30x_fmc.h"
#include "iap_config.h"

void FlashWrite(uint16_t len,uint8_t *data,uint32_t addr_start)
{	
	uint16_t i = 0;
	uint32_t temp = 0;
	uint32_t addr = addr_start;
	
	fmc_state_enum fmc_state = FMC_READY;
	fmc_unlock();

	for(i=0;i<len/4;i++) 
	{
		temp = (data[0] << 0) + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);

		fmc_state = fmc_word_program(addr, temp);
		
		if(fmc_state != FMC_READY)
		{
			return;
		}
		
		fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
		
		data += 4;
		addr += 4;
	}
	
	if((len % 4) == 3)
	{
		temp = (data[0] << 0) + (data[1] << 8) + (data[2] << 16);
		temp = temp | 0xff000000;
		fmc_state = fmc_word_program(addr,temp);
		fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
	}
	else
	{
		if((len % 4) == 2)
		{
			temp = (data[0]<<0) + (data[1]<<8);
			temp = temp | 0xffff0000;
			fmc_state = fmc_word_program(addr,temp);
			fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
		}
		else
		{
			if((len % 4) == 1)
			{
				temp = (data[0]<<0);
				temp = temp | 0xffffff00 ;
				fmc_state = fmc_word_program(addr,temp);
				fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);
			}
		}
	}
	fmc_lock();
}

void FlashRead(uint16_t len,uint8_t *outdata,uint32_t addr_start)
{	
	uint32_t addr;
	uint16_t i;
	
	addr = addr_start;
	
	for(i=0;i<len;i++) 
	{
		*outdata = *(uint8_t*) addr;
		addr = addr + 1;
		outdata++;
	}
}

void FlashErase(uint32_t start, uint32_t end)
{
		uint32_t EraseCounter;
	
		fmc_state_enum fmc_state = FMC_READY;

	  /* unlock the flash program/erase controller */
    fmc_unlock();

    /* clear all pending flags */
		fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);

    /* erase the flash pages */
    while(start < end)
		{
			EraseCounter = start / FMC_PAGE_SIZE;
			fmc_state = fmc_page_erase(EraseCounter * FMC_PAGE_SIZE);
			if(fmc_state != FMC_READY)
			{
				return;
			}
			
			fmc_flag_clear(FMC_FLAG_BANK0_END | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_PGERR);

			start += FMC_PAGE_SIZE;
    }

    /* lock the main FMC after the erase operation */
    fmc_lock();
}
