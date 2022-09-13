#ifndef IAP_H
#define IAP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "flash_rw.h"
#include "ymodem.h"
#include "iap_config.h"

#define IAP_FLAG_ADDR (uint32_t)0x0803F800U

#define APPRUN_FLAG_DATA      0x5A5A   //APP不需要做任何处理，直接运行状态
#define UPDATE_FLAG_DATA      0xEEEE   //下载标志的数据
#define UPLOAD_FLAG_DATA      0xDDDD   //上传标志的数据

extern char FileName[FILE_NAME_LENGTH];

uint32_t IAP_ReadFlag(void);
void IAP_WriteFlag(uint32_t flag);

int32_t Download2Flash(void);

int32_t Jump_to_APP(void);

#endif
