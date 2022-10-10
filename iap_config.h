#ifndef IAP_CONFIG_H_
#define IAP_CONFIG_H_

#define FMC_PAGE_NUM					((uint16_t)0x80U)
#define FMC_PAGE_SIZE					((uint32_t)0x00000800U)

#define FLASH_TOTAL_SIZE 			((uint32_t)0x40000U)

#define FLASH_BASE_ADDR 			((uint32_t)0x08000000U)

#define IAP_ADDRESS 					((uint32_t)0x08000000U)
#define APP_ADDRESS 					((uint32_t)0x08004000U)
#define APP_BACKUP_ADDRESS 		((uint32_t)0x08021000U)
#define APP_SIZE_ADDRESS			((uint32_t)0x0803E000U)
#define INFO_ADDRESS 					((uint32_t)0x0803E800U)
#define FLASH_END_ADDR 				((uint32_t)0x0803FFFFU)

#define IAP_FLASH_SIZE 				((uint32_t)(APP_ADDRESS - IAP_ADDRESS))
#define APP_FLASH_SIZE 				((uint32_t)(APP_BACKUP_ADDRESS - APP_ADDRESS))
#define APP_BACKUP_FLASH_SIZE	((uint32_t)(APP_SIZE_ADDRESS - APP_BACKUP_ADDRESS))
#define APP_SIZE_FLASH_SIZE 	((uint32_t)(INFO_ADDRESS - APP_SIZE_ADDRESS))
#define INFO_FLASH_SIZE 			((uint32_t)(FLASH_END_ADDR - INFO_ADDRESS + 1))

#endif
