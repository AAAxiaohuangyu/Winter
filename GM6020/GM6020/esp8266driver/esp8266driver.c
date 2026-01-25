#include "esp8266driver.h"

uint8_t usart3rxdatabuff[64];

void esp8266_init(void){
    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)"AT+CWMODE=2\r\n", strlen("AT+CWMODE=2\r\n"));
    HAL_Delay(100);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)"AT+CWSAP=\"esp8266\",\"mypassword123\",5,3\r\n", strlen("AT+CWSAP=\"esp8266\",\"mypassword123\",5,3\r\n"));
    HAL_Delay(200);
    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)"AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"));
    HAL_Delay(200);
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
