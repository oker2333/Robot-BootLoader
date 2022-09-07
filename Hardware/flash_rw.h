#ifndef FLASH_RW_H
#define FLASH_RW_H

#include <stdint.h>
#include "gd32f30x_fmc.h"

fmc_state_enum flash_write_word(uint32_t address, uint32_t data);
fmc_state_enum flash_write_dword(uint32_t address, uint16_t data);

uint32_t flash_read_word(uint32_t address);
uint16_t flash_read_dword(uint32_t address);
uint8_t flash_read_byte(uint32_t address);

#endif
