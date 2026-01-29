#ifndef dmimudriver_h
#define dmimudriver_h

#include "usb_host.h"
#include "usbh_cdc.h"

typedef struct{
    float acceleration_x;
    float acceleration_y;
    float acceleration_z;
    float angular_velocity_x;
    float angular_velocity_y;
    float angular_velocity_z;
    float quaternion_w;
    float quaternion_x;
    float quaternion_y;
    float quaternion_z;
} dmimu_data_type;

extern dmimu_data_type dmimu_data;
extern USBH_HandleTypeDef hUsbHostHS;

void dmimu_init(void);
void dmimu_update(void *ptr);
uint16_t Get_CRC16(uint8_t *ptr, uint16_t len);

#endif
