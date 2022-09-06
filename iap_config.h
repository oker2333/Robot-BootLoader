#ifndef IAP_CONFIG_H_
#define IAP_CONFIG_H_

/* 
	256KB FLASH
	#define FMC_PAGE_NUM           	((uint16_t)0x80U)
	#define FMC_PAGE_SIZE           ((uint16_t)0x800U)
	#define FMC_WRITE_START_ADDR    ((uint32_t)0x08000000U)
	#define FMC_WRITE_END_ADDR      ((uint32_t)0x0803FFFFU)
*/

#define IAP_OFFSET ((uint32_t)0x00000000U)
#define APP_OFFSET ((uint32_t)0x00008000U)
#define INF_OFFSET ((uint32_t)0x0003F800U)

#endif
