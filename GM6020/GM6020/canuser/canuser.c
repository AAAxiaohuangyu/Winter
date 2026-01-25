#include <canuser.h>
#include <string.h>

void FDCAN_Filter_init(void)
{
    FDCAN_FilterTypeDef sfilter;

    memset(&sfilter, 0, sizeof(sfilter));

    sfilter.IdType = FDCAN_STANDARD_ID;
    sfilter.FilterIndex = 0;
    sfilter.FilterType = FDCAN_FILTER_MASK;
    sfilter.FilterConfig = FDCAN_RX_FIFO0;
    sfilter.FilterID1 = 0x000; // id
    sfilter.FilterID2 = 0x000; // mask
    HAL_FDCAN_ConfigFilter(&hfdcan1, &sfilter);

    memset(&sfilter, 0, sizeof(sfilter));
}
void FDCAN_SendStd(uint16_t std_id, uint8_t *data, uint8_t len)
{
    FDCAN_TxHeaderTypeDef TxHeader = {0};

    if (len > 8)
        len = 8;

    TxHeader.Identifier = std_id;
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = (len == 8) ? FDCAN_DLC_BYTES_8 : (len == 7) ? FDCAN_DLC_BYTES_7
                                                       : (len == 6)   ? FDCAN_DLC_BYTES_6
                                                       : (len == 5)   ? FDCAN_DLC_BYTES_5
                                                       : (len == 4)   ? FDCAN_DLC_BYTES_4
                                                       : (len == 3)   ? FDCAN_DLC_BYTES_3
                                                       : (len == 2)   ? FDCAN_DLC_BYTES_2
                                                       : (len == 1)   ? FDCAN_DLC_BYTES_1
                                                                      : FDCAN_DLC_BYTES_0;

    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF; // Classic
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;  // Classic
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;

    HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, data);
}

FDCAN_RxHeaderTypeDef Rxheaderbuff;
uint8_t canrxdatabuff[8];
uint8_t Rxfifo0_flag = 0;

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &Rxheaderbuff, canrxdatabuff);
        Rxfifo0_flag = 1;
}
