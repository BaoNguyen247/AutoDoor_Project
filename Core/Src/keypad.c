#include "keypad.h"
#include "Prog_manager.h"
#include "gpio_module.h"
#include "stdio.h"
#include "stdint.h"
#include "action.h"
#include "stdbool.h"
GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
uint32_t previousMillis = 0;
uint32_t currentMillis = 0;
uint8_t keyPressed = 0;
uint8_t keyPush = 0;
uint8_t onetimeUpdate = 0; // Biến toàn cục để lưu giá trị phím nhấn
uint8_t changepassPhysical = 0; // Biến toàn cục để lưu giá trị phím nhấn
// Khởi tạo bàn phím: đặt các hàng (rows) ở mức cao
void keypad_init(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1); // R1
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);  // R2
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);  // R3
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1); // R4
}
void delay_us(uint32_t us) {
  for(volatile uint32_t i = 0; i < us; i++){
    __NOP();
  }

}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (keyPush == 0) {
    delay_us(300000); // Đợi 100us để tránh nhiễu
    /* Cấu hình các chân cột PG13, PC5, PC1, PA2 thành GPIO_INPUT */
    if(HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_9) == 1){
      
      manualOpen = !manualOpen;
      printf("Phim nhan: G9 \n\r");
    }else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == 1){
      isManual = !isManual;
      printf("Phim nhan: A6 \n\r");
    }else{
    GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructPrivate.Pull = GPIO_PULLDOWN;
    GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;

    // Cấu hình PG13 (C1)
    GPIO_InitStructPrivate.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructPrivate);

    // Cấu hình PC5, PC1 (C2, C3)
    GPIO_InitStructPrivate.Pin = GPIO_PIN_5 | GPIO_PIN_1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructPrivate);

    // Cấu hình PA2 (C4)
    GPIO_InitStructPrivate.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructPrivate);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
    if(GPIO_Pin == GPIO_PIN_13 && HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_13)) // C1 = PG13
    {
      keyPressed = 49; // Giá trị ASCII của 1
    }
    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)) // C2 = PC5
    {
      keyPressed = 50; // Giá trị ASCII của 2
    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) // C3 = PC1
    {
      keyPressed = 51; // Giá trị ASCII của 3
    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) // C4 = PA2
    {
      keyPressed = 65; // Giá trị ASCII của A
    }

    // Quét hàng 2 (R2 = PD6)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
    if(GPIO_Pin == GPIO_PIN_13 && HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_13)) // C1 = PG13
    {
      keyPressed = 52; // Giá trị ASCII của 4
    }
    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)) // C2 = PC5
    {
      keyPressed = 53; // Giá trị ASCII của 5
    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) // C3 = PC1
    {
      keyPressed = 54; // Giá trị ASCII của 6
    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) // C4 = PA2
    {
      keyPressed = 66; // Giá trị ASCII của B
    }

    // Quét hàng 3 (R3 = PB4)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 0);
    if(GPIO_Pin == GPIO_PIN_13 && HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_13)) // C1 = PG13
    {
      keyPressed = 55; // Giá trị ASCII của 7
    }
    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)) // C2 = PC5
    {
      keyPressed = 56; // Giá trị ASCII của 8
    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) // C3 = PC1
    {
      keyPressed = 57; // Giá trị ASCII của 9
    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) // C4 = PA2
    {
      keyPressed = 67; // Giá trị ASCII của C
    }

    // Quét hàng 4 (R4 = PG14)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);
    if(GPIO_Pin == GPIO_PIN_13 && HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_13)) // C1 = PG13
    {
      keyPressed = 42; // Giá trị ASCII của *
    }
    else if(GPIO_Pin == GPIO_PIN_5 && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)) // C2 = PC5
    {
      keyPressed = 48; // Giá trị ASCII của 0 
    }
    else if(GPIO_Pin == GPIO_PIN_1 && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) // C3 = PC1
    {
      keyPressed = 35; // Giá trị ASCII của #
    }
    else if(GPIO_Pin == GPIO_PIN_2 && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) // C4 = PA2
    {
      keyPressed = 68; // Giá trị ASCII của D
    }

    // In phím nhấn để gỡ lỗi
    if (keyPressed != 0) {
        printf("Phim nhan: %c\n\r", keyPressed);
    }
    if (keyPressed == 42){
      changepassPhysical = 1; // Nếu phím nhấn là *, thì biến changepassPhysical = 1
    }
    onetimeUpdate = 1;   // Đặt biến oneTimeUpdate = 1 để cập nhật một lần  
    // Đặt lại tất cả hàng về mức cao
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, 1);

    GPIO_InitStructPrivate.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructPrivate.Pull = GPIO_PULLDOWN;
   // Cấu hình PG13 (C1)
    GPIO_InitStructPrivate.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructPrivate);
    // Cấu hình PC5, PC1 (C2, C3)
    GPIO_InitStructPrivate.Pin = GPIO_PIN_5 | GPIO_PIN_1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructPrivate);
    // Cấu hình PA2 (C4)
    GPIO_InitStructPrivate.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructPrivate);
  }
  } keyPush = 1;
    delay_us(2000);  
}