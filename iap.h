#ifndef IAP_H
#define IAP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "flash_rw.h"
#include "ymodem.h"
#include "iap_config.h"

#define IAP_FLAG_ADDR ((uint32_t)(0x0803F800U))

#define APPRUN_FLAG_DATA      0x5A5A   //APP����Ҫ���κδ���ֱ������״̬
#define UPDATE_FLAG_DATA      0xEEEE   //���ر�־������
#define UPLOAD_FLAG_DATA      0xDDDD   //�ϴ���־������

typedef void (*Jump_To_ADDR_t)(void);


extern char FileName[FILE_NAME_LENGTH];

uint32_t IAP_ReadFlag(void);
void IAP_WriteFlag(uint32_t flag);

uint32_t APP_Download_Address(void);
void Write_APP_Address(void);

void set_download_status(int32_t status);
int32_t get_download_status(void);

uint32_t APP_Jump_Address(void);

int32_t Download2Flash(void);

int32_t Jump_to_APP(void);

#endif
