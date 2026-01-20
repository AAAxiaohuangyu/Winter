#include "usart.h"
#include "string.h"
#include "GM6020driver.h"
#include "stdio.h"
#include "pid.h"
#include "cmsis_os2.h"

extern uint8_t usartrxdatabuff[];
extern uint8_t message_flag;

#define channel 4

void command(void*ptr);
void showGM6020data(void*ptr);
