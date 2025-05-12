#include "uart1_module.h"
#include "Prog_manager.h"
#include <string.h>
#include <stdio.h>
#include <sensor_module.h>
#include <update_module.h>
UART_HandleTypeDef huart1;

static uint8_t showDis = 0;
static uint8_t showPir = 0;
static uint8_t changepass = 0;
static uint8_t changepass_state = 0;
static uint8_t pccommand[1];
uint8_t usepassword[4] = {'2', '4', '7', '0'};   
uint8_t temppassword[4];
uint8_t lock_system = 0; 
uint8_t try = 5; // Biến đếm số lần nhập sai mật khẩu
uint8_t input = 0;
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}


void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}


void UART_Update(void)
{
  if (lock_system == 0){
    if (showDis == 0 && showPir == 0 && changepass == 0)
    {
      HAL_UART_Receive_IT(&huart1, pccommand, 1);
    }else if (showDis == 0 && showPir == 1 && changepass == 0)
    {
      if (human == true)
      {
        printf("PIR: Yes\n\r");
      }
      else
      {
        printf("PIR: No\n\r");
      }
      showPir = 0;
    }
    if (showDis == 1 && showPir == 0 && changepass == 0)
    {
      printf("Distance: %.2f cm\n\r", distance);
      showDis = 0;
    }
    else if (showDis == 0 && showPir == 0 && changepass == 1)
    {
      if(changepass_state == 1){
        HAL_UART_Receive_IT(&huart1, temppassword, 4);
      }
    }
    if (try <= 0)
    {
      printf("Ban da nhap sai qua so lan, he thong da bi khoa\n\r");
      lock_system = 1;
      changepass = 0;
      changepass_state = 0;
    }
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  if (changepass == 0){
    if(pccommand[0] == 'a')
    {
      showDis = 1;
      showPir = 0;
      changepass = 0;
      changepass_state = 0;
    }
    else if(pccommand[0] == 'b')
    {
      showDis = 0;
      showPir = 1;
      changepass = 0;
      changepass_state = 0;
    }
    else if(pccommand[0] == 'p')
    {
      showDis = 0;
      showPir = 0;
      changepass = 1;
      changepass_state = 1;
      printf("Nhap mat khau hien tai: \n\r");
    }
}else if (changepass == 1){
    if (changepass_state == 1){
      if (temppassword[0] == usepassword[0] && temppassword[1] == usepassword[1] &&temppassword[2] == usepassword[2] && temppassword[3] == usepassword[3])
      {
        printf("Mat khau trung khop, vui long dat mat khau moi\n\r");
        changepass_state = 2;
        memset(temppassword, 0, sizeof(temppassword));
        HAL_UART_Receive_IT(&huart1, temppassword, 4);
      }else{
        try--;        
        printf("Mat khau khong dung, con %d lan\n\r", try);
        memset(temppassword, 0, sizeof(temppassword));
        changepass = 0;
        changepass_state = 0;  
      }
    }else{
      usepassword[0] = temppassword[0];
      usepassword[1] = temppassword[1];
      usepassword[2] = temppassword[2];
      usepassword[3] = temppassword[3];
      printf("Mat khau moi da duoc cap nhat thanh cong\n\r");
      memset(temppassword, 0, sizeof(temppassword));
      changepass = 0;
      changepass_state = 0;
      try = 5;
      input = 0;
      HAL_UART_Receive_IT(&huart1, pccommand, 1);
    }
}
}