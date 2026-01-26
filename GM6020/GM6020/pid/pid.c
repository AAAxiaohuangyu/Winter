#include "pid.h"

pidtype GM6020_speedpid[8];
pidtype GM6020_locationpid[8];
pidtype M3508_speedpid[8];
int32_t GM6020_location_pid_voltage_output0[4];
int32_t M3508_speed_pid_current_output0[4];

void pidinit(void)
{
    GM6020_locationpid[0].Kp = 30.3;
    GM6020_locationpid[0].Ki = 0.05;
    GM6020_locationpid[0].Kd = 18.1;
    GM6020_locationpid[0].target = 0;
    GM6020_locationpid[0].output = 0;

    GM6020_speedpid[0].Kp = 25.2;
    GM6020_speedpid[0].Ki = 0.01;
    GM6020_speedpid[0].Kd = 1.11;
    GM6020_speedpid[0].output = 0;

    M3508_speedpid[0].Kp = 0.86;
    M3508_speedpid[0].Ki = 0.0013;
    M3508_speedpid[0].Kd = 9.48;
}

void PID_location(pidtype *piddata)
{
    piddata->error1 = piddata->error0;
    piddata->error0 = piddata->target - piddata->actual;

    if (piddata->error0 <= 10 && piddata->error0 >= -10)
    {
        piddata->error_sum += piddata->error0;
        piddata->output = piddata->Kp * piddata->error0 + piddata->Ki * piddata->error_sum + piddata->Kd * (piddata->error0 - piddata->error1);
    }
    else
    {
        piddata->output = piddata->Kp * piddata->error0 + piddata->Kd * (piddata->error0 - piddata->error1);
        piddata->error_sum = piddata->error_sum / 1.2f;
    }
}

void GM6020_locationpidcontrol(void)
{
        int i = 0;
        for (i = 0; i < 3; i++)
        {
            GM6020_locationpid[i].actual = GM6020[i].angle;
            PID_location(&GM6020_locationpid[i]);
            GM6020_speedpid[i].target = GM6020_locationpid[i].output;
            GM6020_speedpid[i].actual = GM6020[i].speed;
            PID_speed(&GM6020_speedpid[i]);
            GM6020_location_pid_voltage_output0[i] = (int32_t)GM6020_speedpid[i].output;
            if (GM6020_location_pid_voltage_output0[i] > 25000)
            {
                GM6020_location_pid_voltage_output0[i] = 25000;
            }
            else if (GM6020_location_pid_voltage_output0[i] < -25000)
            {
                GM6020_location_pid_voltage_output0[i] = -25000;
            }
        }
        GM6020_Voltage_Tx(0x1FF, (int16_t *)GM6020_location_pid_voltage_output0);
}

void PID_speed(pidtype *piddata)
{
    piddata->error1 = piddata->error0;
    piddata->error0 = piddata->target - piddata->actual;
    piddata->error_sum += piddata->error0;

    if(piddata->error_sum >= 125000){
        piddata->error_sum = 125000;
    }
    else if(piddata->error_sum <= -125000){
        piddata->error_sum = -125000;
    }

    piddata->output = piddata->Kp * piddata->error0 + piddata->Ki * piddata->error_sum + piddata->Kd * (piddata->error0 - piddata->error1);
}

void M3508_speedpidcontrol(void)
{
        int i = 0;
        for (i = 0; i < 3; i++)
        {
            M3508_speedpid[i].actual = M3508[i].speed;
            PID_speed(&M3508_speedpid[i]);
            M3508_speed_pid_current_output0[i] = (int32_t)M3508_speedpid[i].output;
            if (M3508_speed_pid_current_output0[i] > 16384)
            {
                M3508_speed_pid_current_output0[i] = 16384;
            }
            else if (M3508_speed_pid_current_output0[i] < -16384)
            {
                M3508_speed_pid_current_output0[i] = -16384;
            }
        }
        M3508_Current_Tx(0x200, (int16_t *)M3508_speed_pid_current_output0);
}
