#ifndef __UART1_MODULE_H
#define __UART1_MODULE_H
#include "Prog_manager.h"
extern UART_HandleTypeDef huart1;
void MX_USART1_UART_Init(void);
void UART_Update(void);
void CheckPassword(void);
extern uint8_t lock_system; 
extern uint8_t try; // Biến đếm số lần nhập sai mật khẩu
#endif