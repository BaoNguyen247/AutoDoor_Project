#ifndef __SENSOR_MODULE_H
#define __SENSOR_MODULE_H

#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
// Khai báo biến toàn cục
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;
extern volatile bool signal_state;
void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM5_Init(void);
float Ultra_ReadDistance(void);
bool  PIR_ReadStatus(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void StartTriggerPeriodic(void);
#endif /* ULTRASONIC_H */
