#include "iicuser.h"

uint8_t iic_rx_buff[8] = {0};
uint8_t iic_tx_buff[48] = {0};

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c,uint8_t TransferDirection,uint16_t AddrMatchCode)
{
    if(hi2c->Instance == I2C1){
        if (TransferDirection == I2C_DIRECTION_TRANSMIT)
        {
            HAL_I2C_Slave_Receive_DMA(hi2c, iic_rx_buff, sizeof(iic_rx_buff));
        }
        else if (TransferDirection == I2C_DIRECTION_RECEIVE)
        {
            memcpy(&iic_tx_buff[0], &dmimu_data.acceleration_x, 4);
            memcpy(&iic_tx_buff[4], &dmimu_data.acceleration_y, 4);
            memcpy(&iic_tx_buff[8], &dmimu_data.acceleration_z, 4);
            memcpy(&iic_tx_buff[12], &dmimu_data.angular_velocity_x, 4);
            memcpy(&iic_tx_buff[16], &dmimu_data.angular_velocity_y, 4);
            memcpy(&iic_tx_buff[20], &dmimu_data.angular_velocity_z, 4);
            memcpy(&iic_tx_buff[24], &dmimu_data.quaternion_w, 4);
            memcpy(&iic_tx_buff[28], &dmimu_data.quaternion_x, 4);
            memcpy(&iic_tx_buff[32], &dmimu_data.quaternion_y, 4);
            memcpy(&iic_tx_buff[36], &dmimu_data.quaternion_z, 4);
            memcpy(&iic_tx_buff[40], &odometer_chassis_position.x, 4);
            memcpy(&iic_tx_buff[44], &odometer_chassis_position.y, 4);
            HAL_I2C_Slave_Transmit_DMA(hi2c, iic_tx_buff, sizeof(iic_tx_buff));
        }
    }
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c){
    memcpy(&chassis_status.chassis_speed_target, &iic_rx_buff[0], 4);
    memcpy(&chassis_status.chassis_angle_target, &iic_rx_buff[4], 4);
    HAL_I2C_EnableListen_IT(&hi2c1);
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c){
    HAL_I2C_EnableListen_IT(&hi2c1);
}
