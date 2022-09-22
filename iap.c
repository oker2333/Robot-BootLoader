#include "iap.h"

typedef void (*Jump_To_ADDR_t)(void);

int32_t Jump_to_APP(void)
{
	  /*
			向量表的起始地址是有要求的：必须先求出系统中共有多少个向量，再把这个数字
			向上增大到是 2 的整次幂，而起始地址必须对齐到后者的边界上。例如，如果一共有 32 个
			中断，则共有 32+16（系统异常） = 48 个向量，向上增大到 2 的整次幂后值为 64，因此地址
			地址必须能被 64*4=256 整除，从而合法的起始地址可以是：0x0, 0x100, 0x200 等。
		*/
		if (0x20000000 == ((*(volatile uint32_t*)APP_ADDRESS) & 0x2FFE0000))
		{
			  printf("Jump to APP\r\n");
				Jump_To_ADDR_t Jump_To_Application = (Jump_To_ADDR_t)(*(volatile uint32_t*)(APP_ADDRESS + 4));
				__set_MSP(*(volatile uint32_t*) APP_ADDRESS);
				Jump_To_Application();
		}
		return 1;
}

uint8_t buf_1k[1024] ={0};

int fputc(int ch, FILE *f)
{
	  (void)f;
    usart_data_transmit(IAP_Port, ch);
    while(RESET == usart_flag_get(IAP_Port, USART_FLAG_TBE));
	  return 0;
}

uint32_t IAP_ReadFlag(void)
{
	 return flash_read_word(IAP_FLAG_ADDR);
}

void IAP_WriteFlag(uint32_t flag)
{
	 flash_write_buffer(IAP_FLAG_ADDR, (uint8_t*)&flag,sizeof(uint32_t));
}

int32_t Download2Flash(void)
{
	  int32_t Size = 0;
		
	  printf("\n\r Waiting for the file to be sent ... (press 'a' to abort)\n\r");
	  Size = Ymodem_Receive();
		if (Size > 0)
		{
			printf("\n\r------------------- \n\r Programming Completed Successfully!\n\r Name: %s",FileName);
			printf("\n\r Size: 0x%x Bytes\r\n------------------- \r\n",Size);
		}
		else if (Size == -1)
		{
			printf("\n\rThe image size is higher than the allowed space memory!\n\r");
		}
		else if (Size == -2)
		{
			printf("\n\rVerification failed!\n\r");
		}
		else if (Size == -3)
		{
			printf("\r\nAborted by user.\n\r");
		}
		else
		{
			printf("\n\rFailed to receive the file!\n\r");
		}
		return Size;
}
