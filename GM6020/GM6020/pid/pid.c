#include <pid.h>

pidtype speedpid[8];
pidtype locationpid[8];
int16_t location_pid_voltage_output0[4];
int16_t speed_pid_voltage_output0[4];

void pidinit(void)
{
    locationpid[0].Kp = 31;
    locationpid[0].Ki = 1.4;
    locationpid[0].Kd = 9;
    locationpid[0].outputmax = 2160;

    speedpid[0].Kp = 30;
    speedpid[0].Ki = 11;
    speedpid[0].Kd = 11;
    speedpid[0].outputmax = 25000;
}

void PID_location(pidtype *piddata)
{
    piddata->error2 = piddata->error1;
    piddata->error1 = piddata->error0;
    piddata->error0 = piddata->target - piddata->actual;

    if (piddata->error0 >= 50 && piddata->error0 <= -50)
    {
        piddata->output += piddata->Kp * (piddata->error0 - piddata->error1) + piddata->Kd * (piddata->error0 - 2 * piddata->error1 + piddata->error2);
    }
    else
    {
        piddata->output += piddata->Kp * (piddata->error0 - piddata->error1) + piddata->Ki * piddata->error0 + piddata->Kd * (piddata->error0 - 2 * piddata->error1 + piddata->error2);
    }

    if (piddata->output >= piddata->outputmax)
    {
        piddata->output = piddata->outputmax;
    }
    if (piddata->output <= -piddata->outputmax)
    {
        piddata->output = -piddata->outputmax;
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
            PID_speed(&speedpid[i]);
            location_pid_voltage_output0[i] = (int16_t)speedpid[i].output;
            GM6020_Voltage_Tx(0x1FF, location_pid_voltage_output0);
        }
        osDelay(5);
    }
}

void PID_speed(pidtype *piddata)
{
    piddata->error2 = piddata->error1;
    piddata->error1 = piddata->error0;
    piddata->error0 = piddata->target - piddata->actual;

    piddata->output += piddata->Kp * (piddata->error0 - piddata->error1) + piddata->Ki * piddata->error0 + piddata->Kd * (piddata->error0 - 2 * piddata->error1 + piddata->error2);

    if (piddata->output >= piddata->outputmax)
    {
        piddata->output = piddata->outputmax;
    }
    if (piddata->output <= -piddata->outputmax)
    {
        piddata->output = -piddata->outputmax;
    }
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
            speed_pid_voltage_output0[i] = (int16_t)speedpid[i].output;
            GM6020_Voltage_Tx(0x1FF, speed_pid_voltage_output0);
        }
        osDelay(15);
    }
}
