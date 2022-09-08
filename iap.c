#include "iap.h"

typedef void (*Jump_To_ADDR_t)(void);

void Jump_to_APP(void)
{
		if (0x20000000 == ((*(volatile uint32_t*)APP_ADDRESS) & 0x2FFE0000))
		{
				Jump_To_ADDR_t Jump_To_Application = (Jump_To_ADDR_t)(*(volatile uint32_t*)(APP_ADDRESS + 4));
				__set_MSP(*(volatile uint32_t*) APP_ADDRESS);
				Jump_To_Application();
		}
}

static void Int2Str(char* str, int32_t intnum)
{
  uint32_t i, Div = 1000000000, j = 0, Status = 0;

  for (i = 0; i < 10; i++)
  {
    str[j++] = (intnum / Div) + 48;

    intnum = intnum % Div;
    Div /= 10;
    if ((str[j-1] == '0') & (Status == 0))
    {
      j = 0;
    }
    else
    {
      Status++;
    }
  }
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

void Download2Flash(void)
{
	  char Number[10] = {0};
	  int32_t Size = 0;
	
	  printf("\n\r Waiting for the file to be sent ... (press 'a' to abort)\n\r");
	  Size = Ymodem_Receive(&buf_1k[0],APP_ADDRESS);
		if (Size > 0)
		{
			printf("-------------------\n");
			printf("\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
			printf(FileName);
			Int2Str(Number, Size);
			printf("\n\r Size: ");
			printf(Number);
			printf(" Bytes\r\n");
		}
		else if (Size == -1)
		{
			printf("\n\n\rThe image size is higher than the allowed space memory!\n\r");
		}
		else if (Size == -2)
		{
			printf("\n\n\rVerification failed!\n\r");
		}
		else if (Size == -3)
		{
			printf("\r\n\nAborted by user.\n\r");
		}
		else
		{
			printf("\n\rFailed to receive the file!\n\r");
		}
}
