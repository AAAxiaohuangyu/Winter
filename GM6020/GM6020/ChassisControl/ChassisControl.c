#include "ChassisControl.h"

chassis_status_type chassis_status = {0};

uint8_t transfer_near_completed_flag = 0;

void transfer_near(void)
{
    chassis_status.chassis_angle_actual = (GM6020[0].angle + GM6020[0].angle + GM6020[0].angle) / 3.0f;
    if (transfer_near_completed_flag)
    {
        uint8_t mode_flag = 2;
        float error_naturalized = (float)fmod(chassis_status.chassis_angle_target - chassis_status.chassis_angle_actual, 360.0f);
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

        transfer_near_completed_flag = 0;
    }
}

uint8_t pid_mode_flag = 0;
static uint64_t trans_condition = 0;

void swerve(void)
{
    chassis_status.chassis_speed_actual = (M3508[0].speed + M3508[0].speed + M3508[0].speed) / 3.0f;
    chassis_status.chassis_angle_target_swerve = chassis_status.chassis_angle_actual;

    chassis_status.chassis_speed_target_swerve = chassis_status.chassis_speed_target_near * (float)fabs(cos(pi * (double)(chassis_status.chassis_angle_actual - chassis_status.chassis_angle_target_near) / 180.0));

    if (fabsf(chassis_status.chassis_speed_target_near) - fabsf(chassis_status.chassis_speed_target_swerve) >= 900)
    {
        if ((double)fabsf(chassis_status.chassis_speed_target_swerve) * speed_trans_constant <= chassis_angle_swerve_speedmax)
        {
            chassis_status.chassis_speed_target_swerve = (float)(((double)chassis_angle_swerve_speedmax / speed_trans_constant) * (double)(chassis_status.chassis_speed_target_near / fabsf(chassis_status.chassis_speed_target_near)));
        }
    }
    chassis_status.chassis_angle_target_swerve = chassis_status.chassis_angle_target_near;

    if (chassis_status.chassis_angle_target_near - chassis_status.chassis_angle_actual <= chassis_angle_swerve_anglemax && chassis_status.chassis_angle_target_near - chassis_status.chassis_angle_actual >= -chassis_angle_swerve_anglemax)
    {
        trans_condition++;
        if (trans_condition >= 120)
        {
            pid_mode_flag = 2;
        }
        else
        {
            pid_mode_flag = 0;
        }
    }
    else if ((double)chassis_status.chassis_speed_actual * speed_trans_constant <= chassis_angle_swerve_speedmax && (double)chassis_status.chassis_speed_actual * speed_trans_constant >= -chassis_angle_swerve_speedmax)
    {
        pid_mode_flag = 0;
        trans_condition = 0;
    }
    else
    {
        pid_mode_flag = 1;
        trans_condition = 0;
    }
}

void motor_status_allocate(void)
{
    uint8_t i = 0;
    for (i = 0; i < 3; i++)
    {
        GM6020_locationpid0[i].target = chassis_status.chassis_angle_target_swerve;
        GM6020_locationpid2[i].target = chassis_status.chassis_angle_target_swerve;
        GM6020_speedpid1[i].target = speed_parameter / (fabsf(chassis_status.chassis_speed_actual) + 1000.0f) * ((chassis_status.chassis_angle_target_swerve - chassis_status.chassis_angle_actual) / fabsf(chassis_status.chassis_angle_target_swerve - chassis_status.chassis_angle_actual));
        M3508_speedpid[i].target = chassis_status.chassis_speed_target_swerve;
    }
    M3508_speedpidcontrol();
    GM6020_locationpidcontrol();
}

void chassis_status_update(void *ptr)
{
    while (1)
    {
        transfer_near();
        swerve();
        motor_status_allocate();
        osDelay(3);
    }
}
