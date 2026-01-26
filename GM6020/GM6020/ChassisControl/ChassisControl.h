#ifndef ChassisControl_h
#define ChassisControl_h

#include "pid.h"
#include "math.h"

typedef struct{
    float chassis_angle_actual;
    float chassis_speed_actual;
    float chassis_angle_target;
    float chassis_speed_target;
    float chassis_angle_target_near;
    float chassis_speed_target_near;
    float chassis_angle_target_swerve;
    float chassis_speed_target_swerve;
}chassis_status_type;

#define proportion 0.1f
#define chassis_control_parameter 5000.0f
#define chassis_angle_swerve_speedmax 1.0f
#define chassis_angle_swerve_anglemax 4.5f
#define pi 3.1415926535
#define speed_trans_constant 0.000284947965 // 速度单位转换常量,转换到m/s

extern chassis_status_type chassis_status;

void transfer_near(void);
void swerve(void);
void motor_status_allocate(void);
void chassis_status_update(void *ptr);

#endif
