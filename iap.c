#include "iap.h"


int32_t Jump_to_APP(void)
{
	  /*
			���������ʼ��ַ����Ҫ��ģ����������ϵͳ�й��ж��ٸ��������ٰ��������
			���������� 2 �������ݣ�����ʼ��ַ������뵽���ߵı߽��ϡ����磬���һ���� 32 ��
			�жϣ����� 32+16��ϵͳ�쳣�� = 48 ���������������� 2 �������ݺ�ֵΪ 64����˵�ַ
			��ַ�����ܱ� 64*4=256 �������Ӷ��Ϸ�����ʼ��ַ�����ǣ�0x0, 0x100, 0x200 �ȡ�
		*/
		uint32_t jump_addr = APP_Jump_Address();
		printf("jump_addr = 0x%x\n",jump_addr);		//�����޸�APP��ʼ��ַ�޸�bug����APPֱ�����ص���ʼ��ַ��ʹ��IAP��ת
	  
		if (0x20000000 == ((*(volatile uint32_t*)jump_addr) & 0x2FFE0000))
		{
			  printf("Jump to APP\r\n");
			  Write_APP_Address();
			  //���Ǽ���ʱ�޸�bug
				Jump_To_ADDR_t Jump_To_Application = (Jump_To_ADDR_t)(*(volatile uint32_t*)(jump_addr + 4));
				__set_MSP(*(volatile uint32_t*) jump_addr);
				Jump_To_Application();
		}
		return 1;
}

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

uint32_t APP_Jump_Address(void)
{
	 uint32_t jump_address = IAP_ADDRESS;
	 uint32_t download_addr = APP_Download_Address();
	 
	 if(get_download_status())
	 {
		  return download_addr;
	 }
	 
	 if(download_addr == APP_A_ADDRESS)
	 {
		  jump_address = APP_B_ADDRESS;
	 }
	 else
	 {
		   jump_address = APP_A_ADDRESS;
	 }
	
	 return jump_address;
}

uint32_t APP_Download_Address(void)
{
	uint32_t download_addr = 0x00;
	uint32_t cur_addr = flash_read_word(APP_ADDR_ADDRESS);
	if(cur_addr == APP_A_ADDRESS)
	{
		 download_addr = APP_B_ADDRESS;
	}
	else if(cur_addr == APP_B_ADDRESS)
	{
		 download_addr = APP_A_ADDRESS;
	}
	else
	{
		 download_addr = APP_A_ADDRESS;
	}
	return download_addr;
}

void Write_APP_Address(void)
{
	 uint32_t jump_address = APP_Jump_Address();
	 flash_write_buffer(APP_ADDR_ADDRESS, (uint8_t*)jump_address,sizeof(uint32_t));
}

static int32_t download_status = 0;

void set_download_status(int32_t status)
{
	 download_status = status;
}

int32_t get_download_status(void)
{
	 return download_status;
}

int32_t Download2Flash(void)
{
	  int32_t Size = 0;
		uint32_t download_addr = APP_Download_Address();
		printf("download_addr = 0x%x\n",download_addr);
	
	  printf("\n\r Waiting for the file to be sent ... (press 'a' to abort)\n\r");
	  Size = Ymodem_Receive(download_addr,APP_FLASH_SIZE);
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
