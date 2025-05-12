#include "update_module.h"
#include "Prog_manager.h"
#include "sensor_module.h"
#include "stdio.h"
#include "stdint.h"
#include "uart1_module.h"
#include "keypad.h"

volatile uint32_t nextTimeclose;	
volatile float distance = 30;
volatile bool human = false;
volatile bool move = false; 
static uint8_t countpass = 0;
static uint8_t temppass[5] = {0, 0, 0, 0, 0};  

void UpdatePIRstatus(void){
    human = PIR_ReadStatus(); // Đọc trạng thái cảm biến PIR
}

void UpdateDistance(void){
    distance = Ultra_ReadDistance(); // Đọc khoảng cách từ cảm biến siêu âm
}


void UpdateAction(void){
//    distance1 = distance; // Đọc khoảng cách từ cảm biến siêu âm
    ActionLight(); // Gọi hàm ActionLight() để thực hiện hành động dựa trên trạng thái cảm biến
    ActionDoor(); // Gọi hàm ActionDoor() để thực hiện hành động dựa trên khoảng cách
}

UpdatephysicKeypad(void){
    if (countpass < 5){
        if(changepassPhysical == 1){
            if(onetimeUpdate == 1){
            temppass[countpass] = keyPressed; // Lưu giá trị phím nhấn vào mảng temppass
            printf("temp[%d] = %d\n\r", countpass,temppass[countpass]);
            countpass++; // Tăng biến đếm lên 1
            onetimeUpdate = 0; // Đặt lại biến oneTimeUpdate về 0
        }   
    }
    }else{
        if(temppass[4] == usepassword[3] && temppass[1] == usepassword[0] && temppass[2] == usepassword[1] && temppass[3] == usepassword[2]){
            printf("Mat khau dung\n\r"); // Nếu mật khẩu đúng
            try = 5; // Đặt lại biến đếm số lần nhập sai mật khẩu về 5
            ChangeTime(); // Gọi hàm ChangeTime() để thay đổi thời gian

        }
        else
        {
            printf("Mat khau sai\n\r"); // Nếu mật khẩu sai
            try--; // Giảm biến đếm số lần nhập sai mật khẩu
        }
        countpass = 0; // Đặt lại biến đếm nếu vượt quá 4
        changepassPhysical = 0; // Đặt lại biến changepassPhysical về 0
    }
    if(try == 0){
        lock_system = 1;
    }
}
void UpdateProg(void){
    // Chương trình sẽ được cập nhật ở đây
    // Ví dụ: cập nhật trạng thái của các cảm biến, hiển thị lên LCD, gửi qua UART, v.v.
    UpdateDistance(); // Cập nhật khoảng cách
    UpdatePIRstatus(); // Cập nhật trạng thái cảm biến PIR
    UART_Update(); // Cập nhật UART
    UpdatephysicKeypad(); // Cập nhật bàn phím vật lý
    UpdateAction(); // Cập nhật hành động dựa trên trạng thái cảm biến
    keyPush = 0;
}