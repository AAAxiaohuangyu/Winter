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

#define proportion 0.01f
#define chassis_angle_swerve_speedmax 0.2f
#define chassis_angle_swerve_anglemax 1.5f
#define pi 3.1415926535
#define speed_trans_constant 0.000284947965 // 速度单位转换常量,转换到m/s
#define speed_parameter 50000.0f

extern chassis_status_type chassis_status;
extern uint8_t transfer_near_completed_flag;
extern uint8_t pid_mode_flag; //0:快速转向,1:慢速转向,2:保持角度

void transfer_near(void);
void swerve(void);
void motor_status_allocate(void);
void chassis_status_update(void *ptr);

#endif
