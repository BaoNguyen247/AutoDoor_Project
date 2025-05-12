#ifndef PROG_MANAGER_H
#define PROG_MANAGER_H
#include "stm32h7xx_hal.h"
void Error_Handler(void);
void StartProgram(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
#endif /* PROG_MANAGER_H */
