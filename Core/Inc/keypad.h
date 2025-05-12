#ifndef __KEYPAD_H
#define __KEYPAD_H
#include "stdint.h"
void keypad_init(void);
void delay_us(uint32_t us);
extern uint8_t keyPressed; // Biến toàn cục để lưu giá trị phím nhấn
extern uint8_t keyPush;
extern uint8_t changepassPhysical;
extern uint8_t onetimeUpdate ; // Biến toàn cục để lưu giá trị phím nhấn
#endif /* __KEYPAD_H */