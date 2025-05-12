#ifndef __ACTION_H
#define __ACTION_H
#include "stdbool.h"
#include "stdint.h"
void ChangeTime(void);
void ActionLight(void);
void ActionDoor(void);
extern volatile bool manualOpen;
extern volatile bool isManual;
extern volatile uint32_t stopTime;
#endif // __ACTION_H