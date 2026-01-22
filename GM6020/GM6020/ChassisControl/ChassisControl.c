#include "ChassisControl.h"

chassis_status_type chassis_status = {0};

void transfer_near()
{
    uint8_t mode_flag = 2;
    chassis_status.chassis_angle_actual = (GM6020[0].angle + GM6020[1].angle + GM6020[2].angle + GM6020[3].angle) / 4.0f;
    float error_naturalized = fmod(chassis_status.chassis_angle_target - chassis_status.chassis_angle_actual, 360.0f);
    if (error_naturalized <= -180.0f)
    {
        error_naturalized += 360.0f;
    }
    else if (error_naturalized >= 180.0f)
    {
        error_naturalized -= 360.0f;
    }

    if (error_naturalized > 90.0f)
    {
        if (turn > -2)
            mode_flag = 0;
        else
            mode_flag = 2;
    }
    else if (error_naturalized < -90.0f)
    {
        if (turn < 2)
            mode_flag = 1;
        else
            mode_flag = 2;
    }
    else
    {
        if (error_naturalized > 0 && turn >= 2)
            mode_flag = 0;
        else if (error_naturalized < 0 && turn <= -2)
            mode_flag = 1;
        else
            mode_flag = 2;
    }

    switch (mode_flag)
    {
    case 0:
    {
        chassis_status.chassis_angle_target_near = chassis_status.chassis_angle_actual + error_naturalized - 180.f;
        chassis_status.chassis_speed_target_near = -chassis_status.chassis_speed_target;
        break;
    }
    case 1:
    {
        chassis_status.chassis_angle_target_near = chassis_status.chassis_angle_actual + error_naturalized + 180.f;
        chassis_status.chassis_speed_target_near = -chassis_status.chassis_speed_target;
        break;
    }
    case 2:
    {
        chassis_status.chassis_angle_target_near = chassis_status.chassis_angle_actual + error_naturalized;
        chassis_status.chassis_speed_target_near = chassis_status.chassis_speed_target;
        break;
    }
    }
}

float chassis_control_parameter = 10;

void swerve()
{
    chassis_status.chassis_speed_actual = (GM6020[0].speed + GM6020[1].speed + GM6020[2].speed + GM6020[3].speed);
    chassis_status.chassis_angle_target_swerve = chassis_status.chassis_angle_actual;

    chassis_status.chassis_speed_target_swerve = chassis_status.chassis_speed_target_near * fabs(cos(pi * (double)(chassis_status.chassis_angle_actual - chassis_status.chassis_angle_target_near) / 180));

    if (chassis_status.chassis_speed_actual <= chassis_angle_swerve_speedmax && chassis_status.chassis_speed_actual >= -chassis_angle_swerve_speedmax)
    {
        chassis_status.chassis_angle_target_swerve = chassis_status.chassis_angle_target_near;
    }
    else if (chassis_status.chassis_angle_target_near - chassis_status.chassis_angle_actual <= chassis_angle_swerve_anglemax && chassis_status.chassis_angle_target_near - chassis_status.chassis_angle_actual >= -chassis_angle_swerve_anglemax)
    {
        chassis_status.chassis_angle_target_swerve = chassis_status.chassis_angle_target_near;
    }
    else
    {
        chassis_status.chassis_angle_target_swerve += (1 - (log(chassis_status.chassis_speed_actual - chassis_angle_swerve_speedmax) / log(chassis_control_parameter))) * (chassis_status.chassis_angle_target - chassis_status.chassis_angle_actual);
    }
}
