#ifndef pid_h
#define pid_h


#include "motordriver.h"
#include "cmsis_os2.h"

typedef struct{
    float Kp;
    float Ki;
    float Kd;
    float target;
    float actual;
    float error0;
    float error1;
    float error_sum;
    float output;
}pidtype;

extern pidtype GM6020_speedpid[];
extern pidtype GM6020_locationpid[];
extern pidtype M3508_speedpid[];
extern int32_t location_pid_voltage_output0[];
extern int32_t speed_pid_voltage_output0[];

void PID_location(pidtype *piddata);
void PID_speed(pidtype *piddate);
void GM6020_locationpidcontrol(void);
void M3508_speedpidcontrol(void);
void pidinit(void);

#endif
