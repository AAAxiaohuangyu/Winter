#ifndef pid_h
#define pid_h


#include "motordriver.h"
#include "cmsis_os2.h"
#include "ChassisControl.h"

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

extern pidtype GM6020_speedpid0[];
extern pidtype GM6020_speedpid1[];
extern pidtype GM6020_speedpid2[];
extern pidtype GM6020_locationpid0[];
extern pidtype GM6020_locationpid2[];
extern pidtype M3508_speedpid[];
extern int32_t location_pid_voltage_output0[];
extern int32_t speed_pid_voltage_output0[];

void PID_location_GM6020(pidtype *piddata);
void PID_speed_GM6020(pidtype *piddata);
void GM6020_locationpidcontrol(void);
void M3508_speedpidcontrol(void);
void PID_speed_M3508(pidtype *piddata);
void pidinit(void);

#endif
