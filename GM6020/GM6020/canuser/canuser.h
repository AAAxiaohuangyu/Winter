#include <fdcan.h>

extern FDCAN_RxHeaderTypeDef Rxheaderbuff; // 信息头结构体
extern uint8_t canrxdatabuff[];
extern uint8_t Rxfifo0_flag;

void FDCAN_Filter_init(void);
void FDCAN_SendStd(uint16_t std_id, uint8_t *data, uint8_t len);
