#include "usartuser.h"

uint8_t usart1rxdatabuff[64] = {0};
uint8_t message_flag = 0;

void command(void*ptr)
{
    char tittle[5] = {0};
    char subtittle[5] = {0};
    char tritittle[8] = {0};
    int txdata1;
    int txdata2;
    while (1)
    {
        if (message_flag)
        {
            if (strncmp((char *)usart1rxdatabuff, "motor",5) == 0)
            {
                sscanf((char *)usart1rxdatabuff, "%s %s %d %s %d", tittle, subtittle, &txdata1, tritittle, &txdata2);
                if (strncmp(subtittle, "speed", 5) == 0 && strncmp(tritittle, "location", 8) == 0)
                {
                    pid_mode_flag = 1;
                    chassis_status.chassis_speed_target = (float)txdata1;
                    chassis_status.chassis_angle_target = (float)txdata2;
                    transfer_near_completed_flag = 1;
                }
                
            }

            else if (strncmp((char *)usart1rxdatabuff, "dmimu_init", 10) == 0)
            {
                dmimu_init();
            }

            memset(usart1rxdatabuff, 0, sizeof(usart1rxdatabuff));
            message_flag = 0;
        }
    }
}

void showmotordata(void*ptr)
{
    while (1)
    {
        /*char txbuff[100] = {0};
        sprintf(txbuff, "motor1,angle:%f,speed:%d,current:%f,tempurature:%d,test:%f", GM6020[0].angle, GM6020[0].speed, GM6020[0].current, GM6020[0].temperature, speedpid[0].output);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)txbuff, sizeof(txbuff));*/

        uint8_t package[channel * 4 + 4];
        memcpy(&package[4 * 0], &chassis_status.chassis_angle_target, 4);
        memcpy(&package[4 * 1], &chassis_status.chassis_angle_actual, 4);
        memcpy(&package[4 * 2], &M3508_speedpid[0].target, 4);
        memcpy(&package[4 * 3], &M3508_speedpid[0].actual, 4);
        package[channel * 4 + 0] = 0x00;
        package[channel * 4 + 1] = 0x00;
        package[channel * 4 + 2] = 0x80;
        package[channel * 4 + 3] = 0x7f;
        HAL_UART_Transmit_DMA(&huart1, package, sizeof(package));
        osDelay(40);
    }
}

