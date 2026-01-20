#ifndef pid_h
#define pid_h


#include "GM6020driver.h"
#include "cmsis_os2.h"

typedef struct{
    float Kp;
    float Ki;
    float Kd;
    float target;
    float actual;
    float error0;
    float error1;
    float error2;
    float output;
    float outputmax;
}pidtype;

extern pidtype speedpid[];
extern pidtype locationpid[];
extern int16_t location_pid_voltage_output0[];
extern int16_t speed_pid_voltage_output0[4];

void PID_location(pidtype *piddata);
void PID_speed(pidtype *piddate);
void locationpidcontrol(void *ptr);
void speedpidcontrol(void *ptr);
void pidinit(void);

#endif
