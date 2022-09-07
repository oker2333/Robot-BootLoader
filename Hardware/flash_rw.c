#include "flash_rw.h"
#include "iap_config.h"

fmc_state_enum flash_write_word(uint32_t address, uint32_t data)
{
	  fmc_state_enum state;
		
    fmc_unlock();
	  fmc_page_erase((address / FMC_PAGE_SIZE)*FMC_PAGE_SIZE);
		state = fmc_word_program(address, data);
	
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
	
	  fmc_lock();
		
		return state;
}

fmc_state_enum flash_write_dword(uint32_t address, uint16_t data)
{
	  fmc_state_enum state;
	
    fmc_unlock();
		state = fmc_halfword_program(address, data);

    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
	
	  fmc_lock();
		
		return state;
}

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
