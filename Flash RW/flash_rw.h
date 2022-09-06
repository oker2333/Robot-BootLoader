#ifndef FLASH_RW_H
#define FLASH_RW_H

#include <stdint.h>

void FlashWrite(uint16_t len,uint8_t *data,uint32_t addr_start);
void FlashRead(uint16_t len,uint8_t *outdata,uint32_t addr_start);
void FlashErase(uint32_t start, uint32_t end);

#endif
