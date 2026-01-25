#include "ChassisControl.h"

typedef struct{
    float x;
    float y;
} odometer_chassis_position_type;

extern odometer_chassis_position_type odometer_chassis_position;

#define speed_trans_constant  0.000284947965//速度单位转换常量,转换到m/s
