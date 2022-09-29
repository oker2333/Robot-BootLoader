#ifndef IAP_H
#define IAP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef void (*Jump_To_ADDR_t)(void);

int32_t Jump_to_APP(void);

#endif
