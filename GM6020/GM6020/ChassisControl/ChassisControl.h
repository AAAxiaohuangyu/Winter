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

#define chassis_angle_swerve_speedmax 1.0f
#define chassis_angle_swerve_anglemax 4.5f
#define pi 3.1415926535

extern chassis_status_type chassis_status;
