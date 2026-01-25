#ifndef motordriver_h
#define motordriver_h
#include "canuser.h"

typedef struct
{
    float angle;
    int16_t speed;
    float current;
    uint8_t temperature;
} motordatatype;

extern motordatatype GM6020[];
extern motordatatype M3508[];
extern int32_t turn;

void GM6020_Rx(uint16_t id, uint8_t Rxbuff[]);
void GM6020_Voltage_Tx(uint16_t id, int16_t Voltage[]);
void M3508_Rx(uint16_t id, uint8_t Rxbuff[]);
void M3508_Current_Tx(uint16_t id, int16_t current[]);
void motor_data_update(void *ptr);

#endif
