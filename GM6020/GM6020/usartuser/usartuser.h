#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "pid.h"
#include "cmsis_os2.h"
#include "ChassisControl.h"
#include "dmimudriver.h"

extern uint8_t usart1rxdatabuff[];
extern uint8_t message_flag;

#define channel 4

void command(void*ptr);
void showmotordata(void*ptr);
