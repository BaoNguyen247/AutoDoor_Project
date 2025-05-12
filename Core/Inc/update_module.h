#ifndef __UPDATE_MODULE_H
#define __UPDATE_MODULE_H

#include "stdbool.h"
#include "stdint.h"
void UpdatePIRstatus(void);
void UpdateDistance(void);
void UpdateProg(void);
void UpdatephysicKeypad(void);
void UpdateAction(void);
extern volatile float distance; // Biến toàn cục để lưu khoảng cách từ cảm biến siêu âm
extern volatile uint32_t nextTimeclose; // Biến toàn cục để lưu thời gian tương lai sau 5 phút
extern volatile bool human; // Biến toàn cục để lưu trạng thái cảm biến PIR
extern volatile bool move; // Biến toàn cục để lưu trạng thai1 chuyen dong cam bien sieu am
extern uint8_t usepassword[4];  

#endif /* __UPDATE_MODULE_H */