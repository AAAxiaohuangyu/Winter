#include "usartuser.h"

uint8_t usartrxdatabuff[64] = {0};
uint8_t message_flag = 0;

void command(void*ptr)
{
    char tittle[6] = {0};
    char subtittle[5] = {0};
    int txid;
    int txdata;
    while (1)
    {
        if (message_flag)
        {
            if (strncmp((char *)usartrxdatabuff, "GM6020",6) == 0)
            {
                sscanf((char *)usartrxdatabuff, "%s %s %d %d",tittle,subtittle, &txid, &txdata);
                if (strncmp(subtittle, "speed", 5) == 0)
                {
                    if (txid >= 1 && txid <= 8)
                    {
                        speedpid[txid - 1].target = (float)txdata;
                    }
                }
                else if (strncmp(subtittle, "location", 8) == 0)
                {
                    if (txid >= 1 && txid <= 8)
                    {
                        locationpid[txid - 1].target = (float)txdata;
                    }
                }
                
            }
            memset(usartrxdatabuff, 0, sizeof(usartrxdatabuff));
            message_flag = 0;
        }
    }
}

void showGM6020data(void*ptr)
{
    while (1)
    {
        /*char txbuff[100] = {0};
        sprintf(txbuff, "motor1,angle:%f,speed:%d,current:%f,tempurature:%d,test:%f", GM6020[0].angle, GM6020[0].speed, GM6020[0].current, GM6020[0].temperature, speedpid[0].output);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)txbuff, sizeof(txbuff));*/

        uint8_t package[channel * 4 + 4];
        memcpy(&package[4 * 0], &locationpid[0].target, 4);
        memcpy(&package[4 * 1], &locationpid[0].actual, 4);
        memcpy(&package[4 * 2], &speedpid[0].target, 4);
        memcpy(&package[4 * 3], &speedpid[0].actual, 4);
        package[channel * 4 + 0] = 0x00;
        package[channel * 4 + 1] = 0x00;
        package[channel * 4 + 2] = 0x80;
        package[channel * 4 + 3] = 0x7f;
        HAL_UART_Transmit_DMA(&huart1, package, sizeof(package));
        osDelay(40);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        usartrxdatabuff[Size] = '\0';
        message_flag = 1;
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, usartrxdatabuff, 64);
    }
}
