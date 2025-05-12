#include "action.h"
#include "update_module.h"
#include "Prog_manager.h"
#include "sensor_module.h"
#include "lcd_module.h"
#include "stdbool.h"
volatile uint32_t durationOpendoor = 5000;
volatile uint32_t stopAction = 0;
volatile uint32_t startAction = 0;
volatile bool manualOpen = false;
volatile bool isManual = false;
uint8_t preventLoop = 0;
uint8_t onetimeOpen = 0;
uint8_t onetimeClose = 0;
volatile uint32_t stopTime = 5;
static bool flaglight = false;
static uint32_t timeoff_light = 0;
static uint8_t change_state = 0;
void ChangeTime(void)
{
}
void ActionLight(void)
{
    if (signal_state == true){
        // Nếu có người, thực hiện hành động nào đó
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1); // Bật đèn LED
        change_state = 1;
    } else {
        if(flaglight == false){
            timeoff_light = HAL_GetTick() + 30000; // Thời gian tắt đèn LED sau 10 giây       
            change_state = 0;                 
            flaglight = true;
        }
        if(HAL_GetTick() >= timeoff_light){
            if(change_state == 0){
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0); // Tắt đèn LED    
                flaglight = false;
            }else{
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1); // Bật đèn LED
                flaglight = false;
            }
        }
    }
}
void OpenDoor(void)
{

    if (onetimeOpen == 0){
        if(preventLoop == 0){
            lcd_set_cursor(1,4);
            lcd_print("OPENING");
            if (move == 1){             
                startAction = HAL_GetTick(); // Lưu thời gian bắt đầu mở cửa
                stopAction = startAction + durationOpendoor; // Lưu thời gian dừng mở cửa
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1); // Bật động cơ mở cửa
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1); // Bật động cơ mở cửa
            }
            preventLoop = 1; // Ngăn chặn vòng lặp mở cửa liên tục
        }else{
                if (HAL_GetTick() >= stopAction) {
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0); // Tắt động cơ mở cửa
                    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0); // Bật động cơ mở cửa
                    preventLoop = 0; // Cho phép vòng lặp mở cửa tiếp theo
                    onetimeOpen = 1;
                    onetimeClose = 0; 
                }
        }
    }
}

void CloseDoor(void)
{
    if(onetimeClose == 0){   
    if(preventLoop == 0){
    lcd_set_cursor(1,4);
    lcd_print("CLOSING");          
        if (move == 0){
            startAction = HAL_GetTick(); // Lưu thời gian bắt đầu mở cửa
            stopAction = startAction + durationOpendoor; // Lưu thời gian dừng mở cửa
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 1); // Bật động cơ mở cửa
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1); // Bật động cơ mở cửa
        }
        preventLoop = 1; // Ngăn chặn vòng lặp mở cửa liên tục
    }else{
            if (HAL_GetTick() >= stopAction) {
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0); // Tắt động cơ mở cửa
                HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 0); // Tat động cơ mở cửa
                onetimeClose = 1; // Đặt lại biến onetimeClose về 1
                onetimeOpen = 0;
                preventLoop = 0; // Cho phép vòng lặp mở cửa tiếp theo
                lcd_set_cursor(1,4);
                lcd_print("-------");
            }
    }
    }
}
void ActionDoor(void)
{
    if(isManual == true){
        lcd_set_cursor(0,4);
        lcd_print("WELCOME ");
        if(manualOpen == true){
            move = 1; // Nếu biến manualOpen là true, thì move = 1
        }
        else{
            move = 0; // Nếu biến manualOpen là false, thì move = 0
        }
    }else{
        lcd_set_cursor(0,4);
        lcd_print("AUTODOOR");
        // distance2 = distance; // Đọc khoảng cách từ cảm biến siêu âm
        if(Ultra_ReadDistance() < 20)
        {
            nextTimeclose = HAL_GetTick() + durationOpendoor + (stopTime*1000); //Lưu thời gian hiện tại vào biến nextTimeclose
            move = 1;  
        }else{										
            if(HAL_GetTick() >=  nextTimeclose){		
            move = 0;						
            }
        }
    }
    if(move == 1){
                OpenDoor(); // Gọi hàm OpenDoor() để mở cửa
        } else {
                CloseDoor(); // Gọi hàm CloseDoor() để đóng cửa

    }
}