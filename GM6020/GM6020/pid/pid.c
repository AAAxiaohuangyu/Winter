#include "pid.h"

pidtype GM6020_speedpid0[8];
pidtype GM6020_speedpid1[8];
pidtype GM6020_speedpid2[8];
pidtype GM6020_locationpid0[8];
pidtype GM6020_locationpid2[8];
pidtype M3508_speedpid[8];
int32_t GM6020_location_pid_voltage_output0[4];
int32_t M3508_speed_pid_current_output0[4];

void pidinit(void)
{
    GM6020_locationpid0[0].Kp = 38.6;
    GM6020_locationpid0[0].Ki = 0.09;
    GM6020_locationpid0[0].Kd = 13;
    GM6020_locationpid0[0].target = 0;

    GM6020_speedpid0[0].Kp = 6.94;
    GM6020_speedpid0[0].Ki = 0.01;
    GM6020_speedpid0[0].Kd = 1.9;
    GM6020_speedpid0[0].output = 0;

    GM6020_speedpid1[0].Kp = 200;
    GM6020_speedpid1[0].Ki = 10;
    GM6020_speedpid1[0].Kd = 1;

    GM6020_locationpid2[0].Kp = 60;
    GM6020_locationpid2[0].Ki = 0.25;
    GM6020_locationpid2[0].Kd = 16;
    GM6020_locationpid2[0].target = 0;

    GM6020_speedpid2[0].Kp = 75;
    GM6020_speedpid2[0].Ki = 0.02;
    GM6020_speedpid2[0].Kd = 17;
    GM6020_speedpid2[0].output = 0;

    M3508_speedpid[0].Kp = 6.57;
    M3508_speedpid[0].Ki = 0.008;
    M3508_speedpid[0].Kd = 15.48;
}

void PID_location_GM6020(pidtype *piddata)
{
    piddata->error1 = piddata->error0;
    piddata->error0 = piddata->target - piddata->actual;

    if (piddata->error0 <= 4.5f && piddata->error0 >= -4.5f)
    {
        piddata->error_sum += piddata->error0;
        piddata->output = piddata->Kp * piddata->error0 + piddata->Ki * piddata->error_sum + piddata->Kd * (piddata->error0 - piddata->error1);

        if (piddata->error0 <= 0.25f && piddata->error0 >= -0.25f)
        {
            piddata->error_sum /= 1.008f;
        }
    }
    else
    {
        piddata->output = piddata->Kp * piddata->error0 + piddata->Kd * (piddata->error0 - piddata->error1);
        piddata->error_sum = piddata->error_sum / 1.1f;
    }
}

void PID_speed_GM6020(pidtype *piddata)
{
    piddata->error1 = piddata->error0;
    piddata->error0 = piddata->target - piddata->actual;
    piddata->error_sum += piddata->error0;

    if (piddata->error_sum >= 125000)
    {
        piddata->error_sum = 125000;
    }
    else if (piddata->error_sum <= -125000)
    {
        piddata->error_sum = -125000;
    }

    piddata->output = piddata->Kp * piddata->error0 + piddata->Ki * piddata->error_sum + piddata->Kd * (piddata->error0 - piddata->error1);
}

void GM6020_locationpidcontrol(void)
{
    int i = 0;
    if (pid_mode_flag == 0)
    {
        for (i = 0; i < 3; i++)
        {
            GM6020_locationpid0[i].actual = GM6020[i].angle;
            PID_location_GM6020(&GM6020_locationpid0[i]);
            GM6020_speedpid0[i].target = GM6020_locationpid0[i].output;
            GM6020_speedpid0[i].actual = GM6020[i].speed;
            PID_speed_GM6020(&GM6020_speedpid0[i]);
            GM6020_location_pid_voltage_output0[i] = (int32_t)(GM6020_speedpid0[i].output);
            if (GM6020_location_pid_voltage_output0[i] > 25000)
            {
                GM6020_location_pid_voltage_output0[i] = 25000;
            }
            else if (GM6020_location_pid_voltage_output0[i] < -25000)
            {
                GM6020_location_pid_voltage_output0[i] = -25000;
            }

            GM6020_speedpid1[i].error_sum = 0;
            GM6020_locationpid2[i].error_sum = 0;
            GM6020_speedpid2[i].error_sum = 0;
        }
    }
    else if (pid_mode_flag == 1)
    {
        for (i = 0; i < 3; i++)
        {
            GM6020_speedpid1[i].actual = GM6020[i].speed;
            PID_speed_GM6020(&GM6020_speedpid1[i]);
            GM6020_location_pid_voltage_output0[i] = (int32_t)(GM6020_speedpid1[i].output);
            if (GM6020_location_pid_voltage_output0[i] > 25000)
            {
                GM6020_location_pid_voltage_output0[i] = 25000;
            }
            else if (GM6020_location_pid_voltage_output0[i] < -25000)
            {
                GM6020_location_pid_voltage_output0[i] = -25000;
            }

            GM6020_locationpid0[i].error_sum = 0;
            GM6020_speedpid0[i].error_sum = 0;
            GM6020_locationpid2[i].error_sum = 0;
            GM6020_speedpid2[i].error_sum = 0;
        }
    }

    else if (pid_mode_flag == 2)
    {
        for (i = 0; i < 3; i++)
        {
            GM6020_locationpid2[i].actual = GM6020[i].angle;
            PID_location_GM6020(&GM6020_locationpid2[i]);
            GM6020_speedpid2[i].target = GM6020_locationpid2[i].output;
            GM6020_speedpid2[i].actual = GM6020[i].speed;
            PID_speed_GM6020(&GM6020_speedpid2[i]);
            GM6020_location_pid_voltage_output0[i] = (int32_t)(GM6020_speedpid2[i].output);

            if (GM6020_location_pid_voltage_output0[i] > 25000)
            {
                GM6020_location_pid_voltage_output0[i] = 25000;
            }
            else if (GM6020_location_pid_voltage_output0[i] < -25000)
            {
                GM6020_location_pid_voltage_output0[i] = -25000;
            }

            GM6020_locationpid0[i].error_sum = 0;
            GM6020_speedpid0[i].error_sum = 0;
            GM6020_speedpid1[i].error_sum = 0;
        }
    }
    GM6020_Voltage_Tx(0x1FF, (int16_t *)GM6020_location_pid_voltage_output0);
}

void PID_speed_M3508(pidtype *piddata)
{
    piddata->error1 = piddata->error0;
    piddata->error0 = piddata->target - piddata->actual;
    piddata->error_sum += piddata->error0;

    if (piddata->error_sum >= 125000)
    {
        piddata->error_sum = 125000;
    }
    else if (piddata->error_sum <= -125000)
    {
        piddata->error_sum = -125000;
    }

    piddata->output = piddata->Kp * piddata->error0 + piddata->Ki * piddata->error_sum + piddata->Kd * (piddata->error0 - piddata->error1);

    if (piddata->error0 - piddata->error1 > -120 && piddata->error0 - piddata->error1 < 120 && (piddata->target - piddata->actual < -100 || piddata->target - piddata->actual > 100))
    {
        piddata->output *= 1.2f;
    }
}

void M3508_speedpidcontrol(void)
{
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        M3508_speedpid[i].actual = M3508[i].speed;
        PID_speed_M3508(&M3508_speedpid[i]);
        M3508_speed_pid_current_output0[i] = (int32_t)(M3508_speedpid[i].output);
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
