#ifndef GM6020driver_h
#define GM6020driver_h
#include "canuser.h"

typedef struct
{
    float angle;
    int16_t speed;
    float current;
    uint8_t temperature;
} GM6020datatype;

extern GM6020datatype GM6020[];
extern int16_t GM6020current0[];
extern int16_t GM6020current1[];

void GM6020_Current_Tx(uint16_t id, int16_t current[]);
void GM6020_Rx(uint16_t id, uint8_t Rxbuff[]);
void GM6020_Voltage_Tx(uint16_t id, int16_t Voltage[]);
void GM6020data(void *ptr);

#endif
