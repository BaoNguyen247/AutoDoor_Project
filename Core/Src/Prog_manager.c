#include "Prog_manager.h"
#include "stdio.h"
#include <string.h>
#include "math.h"
#include "sensor_module.h"
#include "lcd_module.h"
#include "gpio_module.h"
#include "uart1_module.h"
#include "update_module.h"
#include "keypad.h"
static void MX_NVIC_Init(void);
void StartProgram(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C3_Init(); 
  MX_USART1_UART_Init();  
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_TIM5_Init();
  MX_NVIC_Init();
  keypad_init();
  StartTriggerPeriodic();
  lcd_init();
  while(1){
    UpdateProg();
    HAL_Delay(500); // Delay 1 giây giữa các lần cập nhật
  }
}


void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
static void MX_NVIC_Init(void)
{
  HAL_NVIC_SetPriority(TIM2_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
  HAL_NVIC_SetPriority(TIM5_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(TIM5_IRQn);
}
