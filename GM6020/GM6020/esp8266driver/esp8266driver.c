#include "esp8266driver.h"

uint8_t usart3rxdatabuff[64] = {0};

void esp8266_init(void){
    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)"AT+CWMODE=2\r\n", strlen("AT+CWMODE=2\r\n"));
    HAL_Delay(300);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)"AT+CWSAP=\"esp8266\",\"mypassword123\",5,3\r\n", strlen("AT+CWSAP=\"esp8266\",\"mypassword123\",5,3\r\n"));
    HAL_Delay(300);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)"AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"));
    HAL_Delay(300);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)"AT+CIPSERVER=1,8080\r\n", strlen("AT+CIPSERVER=1,8080\r\n"));
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        usart1rxdatabuff[Size] = '\0';
        message_flag = 1;
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, usart1rxdatabuff, 64);
    }

    if(huart->Instance == USART3){
        if (usart3rxdatabuff[0] == 0x0D && usart3rxdatabuff[1] == 0x0A && usart3rxdatabuff[2] == 0x2B && usart3rxdatabuff[3] == 0x49 && usart3rxdatabuff[4] == 0x50 && usart3rxdatabuff[5] == 0x44)
        {
            uint16_t temp = 0;
            memcpy(&temp, &usart3rxdatabuff[11], 2);
            chassis_status.chassis_angle_target = (float)temp;
            pid_mode_flag = 1;
            transfer_near_completed_flag = 1;
        }

            HAL_UARTEx_ReceiveToIdle_DMA(&huart3, usart3rxdatabuff, 64);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        __HAL_UART_CLEAR_FEFLAG(huart);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, usart3rxdatabuff, 64);
    }
}
