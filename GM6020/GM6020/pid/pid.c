#include <pid.h>

pidtype speedpid[8];
pidtype locationpid[8];
int32_t location_pid_voltage_output0[4];
int32_t speed_pid_voltage_output0[4];

void pidinit(void)
{
    locationpid[0].Kp = 24.3;
    locationpid[0].Ki = 0.31;
    locationpid[0].Kd = 14.1;
    locationpid[0].target = 0;
    locationpid[0].output = 0;

    speedpid[0].Kp = 26.2;
    speedpid[0].Ki = 0.023;
    speedpid[0].Kd = 1.61;
    speedpid[0].output = 0;
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

void locationpidcontrol(void *ptr)
{
    while (1)
    {
        int i = 0;
        for (i = 0; i < 4; i++)
        {
            locationpid[i].actual = GM6020[i].angle;
            PID_location(&locationpid[i]);
            speedpid[i].target = locationpid[i].output;
            speedpid[i].actual = GM6020[i].speed;
            PID_speed(&speedpid[i]);
            location_pid_voltage_output0[i] = (int32_t)speedpid[i].output;
            if (location_pid_voltage_output0[i] > 25000)
            {
                location_pid_voltage_output0[i] = 25000;
            }
            else if (location_pid_voltage_output0[i] < -25000)
            {
                location_pid_voltage_output0[i] = -25000;
            }
            GM6020_Voltage_Tx(0x1FF, (int16_t *)location_pid_voltage_output0);
        }
        osDelay(10);
    }
}

void PID_speed(pidtype *piddata)
{
    piddata->error1 = piddata->error0;
    piddata->error0 = piddata->target - piddata->actual;
    piddata->error_sum += piddata->error0;

    piddata->output = piddata->Kp * piddata->error0 + piddata->Ki * piddata->error_sum + piddata->Kd * (piddata->error0 - piddata->error1);
}

void speedpidcontrol(void *ptr)
{
    while (1)
    {
        int i = 0;
        for (i = 0; i < 4; i++)
        {
            speedpid[i].actual = GM6020[i].speed;
            PID_speed(&speedpid[i]);
            speed_pid_voltage_output0[i] = (int32_t)speedpid[i].output;
            if (speed_pid_voltage_output0[i] > 25000)
            {
                speed_pid_voltage_output0[i] = 25000;
            }
            else if (speed_pid_voltage_output0[i] < -25000)
            {
                speed_pid_voltage_output0[i] = -25000;
            }
            GM6020_Voltage_Tx(0x1FF, (int16_t *)speed_pid_voltage_output0);
        }
        osDelay(15);
    }
}
