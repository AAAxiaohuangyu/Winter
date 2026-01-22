#include "GM6020driver.h"

GM6020datatype GM6020[8];
int16_t GM6020current0[4] = {0};
int16_t GM6020current1[4] = {0};
int32_t turn = 0;

void GM6020_Current_Tx(uint16_t id, int16_t current[])
{
    uint8_t Txbuff[8] = {0};
    uint8_t i = 0;
    for (i = 0; i < 4; i++)
    {
        Txbuff[2 * i] = (current[i] >> 8) & 0xFF;
        Txbuff[2 * i + 1] = current[i] & 0xFF;
    }
    FDCAN_SendStd(id, Txbuff, 8);
}

void GM6020_Voltage_Tx(uint16_t id, int16_t Voltage[])
{
    uint8_t Txbuff[8] = {0};
    uint8_t i = 0;
    for (i = 0; i < 4; i++)
    {
        Txbuff[2 * i] = (Voltage[i] >> 8) & 0xFF;
        Txbuff[2 * i + 1] = Voltage[i] & 0xFF;
    }
    FDCAN_SendStd(id, Txbuff, 8);
}

void GM6020_Rx(uint16_t id, uint8_t Rxbuff[])
{
    static uint16_t angle_raw = 0;
    static uint16_t angle_raw0 = 0;
    int16_t current_raw = 0;
    angle_raw0 = angle_raw;
    angle_raw = (Rxbuff[0] << 8) | Rxbuff[1];
    if (angle_raw - angle_raw0 >= 4096)
    {
        turn--;
    }
    if (angle_raw - angle_raw0 <= -4096)
    {
        turn++;
    }
    GM6020[id - 0x205].angle = (angle_raw / 8191.0f) * 360.0f + 360.0f * turn;
    GM6020[id - 0x205].speed = (Rxbuff[2] << 8) | Rxbuff[3];
    current_raw = (Rxbuff[4] << 8) | Rxbuff[5];
    GM6020[id - 0x205].current = (current_raw / 16384.0f) * 3.0f;
    GM6020[id - 0x205].temperature = Rxbuff[6];
}

void GM6020data(void*ptr){
    while (1)
    {
        if(Rxfifo0_flag){
            GM6020_Rx(Rxheaderbuff.Identifier, canrxdatabuff);
            Rxfifo0_flag = 0;
        }

    }
    
}
