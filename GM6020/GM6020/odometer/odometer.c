#include "odometer.h"

odometer_chassis_position_type odometer_chassis_position = {0};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if (htim->Instance == TIM1)
    {
        HAL_IncTick();
    }
    
    else if(htim->Instance == TIM2){
        odometer_chassis_position.x += speed_trans_constant * (double)chassis_status.chassis_speed_actual * cos(pi * (double)chassis_status.chassis_angle_actual / 180.0) * 0.003;
        odometer_chassis_position.y += speed_trans_constant * (double)chassis_status.chassis_speed_actual * sin(pi * (double)chassis_status.chassis_angle_actual / 180.0) * 0.003;
    }
}
