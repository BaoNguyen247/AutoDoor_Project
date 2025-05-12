    #include "lcd_module.h"
    #include "Prog_manager.h"
    #include <string.h>

    I2C_HandleTypeDef hi2c3;

    #define LCD_I2C_ADDRESS (0x27 << 1) // Địa chỉ 7-bit, dịch trái 1 bit
    // Định nghĩa lệnh và dữ liệu cho LCD1602
    #define LCD_CLEAR           0x01
    #define LCD_HOME            0x02
    #define LCD_ENTRY_MODE      0x06
    #define LCD_DISPLAY_ON      0x0C
    #define LCD_FUNCTION_SET    0x28 // 4-bit, 2 dòng, font 5x8
    #define LCD_SET_DDRAM       0x80

    // Bit điều khiển PCF8574
    #define LCD_RS      0x01 // Register Select (0: command, 1: data)
    #define LCD_RW      0x02 // Read/Write (không dùng, để 0)
    #define LCD_EN      0x04 // Enable
    #define LCD_BL      0x08 // Backlight (1: bật)


    // Hàm gửi 4 bit tới LCD qua PCF8574
    void lcd_send_nibble(uint8_t nibble, uint8_t rs) {
      uint8_t data = (nibble << 4); // Dữ liệu 4 bit cao
      data |= (rs ? LCD_RS : 0);    // Chọn RS (command/data)
      data |= LCD_BL;               // Bật backlight
      data |= LCD_EN;               // Bật EN
      HAL_I2C_Master_Transmit(&hi2c3, LCD_I2C_ADDRESS, &data, 1, HAL_MAX_DELAY);
      HAL_Delay(1);                // Đợi EN
      data &= ~LCD_EN;              // Tắt EN
      HAL_I2C_Master_Transmit(&hi2c3, LCD_I2C_ADDRESS, &data, 1, HAL_MAX_DELAY);
      HAL_Delay(50);                // Đợi lệnh hoàn tất
    }

    // Hàm gửi 1 byte (gửi hai nibble)
    void lcd_send_byte(uint8_t byte, uint8_t rs) {
      lcd_send_nibble(byte >> 4, rs); // Gửi nibble cao
      lcd_send_nibble(byte & 0x0F, rs); // Gửi nibble thấp
    }

    // Hàm khởi tạo LCD
    void lcd_init(void) {
      HAL_Delay(50); // Đợi LCD ổn định
      lcd_send_nibble(0x03, 0); // Khởi tạo chế độ 8-bit
      HAL_Delay(5);
      lcd_send_nibble(0x03, 0);
      HAL_Delay(1);
      lcd_send_nibble(0x03, 0);
      lcd_send_nibble(0x02, 0); // Chuyển sang chế độ 4-bit
      lcd_send_byte(LCD_FUNCTION_SET, 0); // 4-bit, 2 dòng
      lcd_send_byte(LCD_DISPLAY_ON, 0);   // Bật hiển thị, tắt con trỏ
      lcd_send_byte(LCD_CLEAR, 0);        // Xóa màn hình
      HAL_Delay(2);
      lcd_send_byte(LCD_ENTRY_MODE, 0);   // Chế độ nhập dữ liệu
      // lcd_set_cursor(0, 0);
      // lcd_print("Distance:");
      // lcd_set_cursor(0, 11); // Đặt con trỏ tại hàng 0, cột 11
      // lcd_print("PIR:");
      // lcd_set_cursor(1, 0); // Đặt con trỏ tại hàng 1, cột 0
    }

    // Hàm gửi lệnh tới LCD
    void lcd_command(uint8_t cmd) {
      lcd_send_byte(cmd, 0);
    }

    // Hàm gửi dữ liệu (ký tự) tới LCD
    void lcd_data(uint8_t data) {
      lcd_send_byte(data, 1);
    }

    // Hàm xuất chuỗi ký tự
    void lcd_print(const char *str) {
      while (*str) {
          lcd_data(*str++);
      }
    }
    // Hàm đặt vị trí con trỏ
    void lcd_set_cursor(uint8_t row, uint8_t col) {
      uint8_t address = (row == 0) ? (0x00 + col) : (0x40 + col);
      lcd_command(LCD_SET_DDRAM | address);
    }

    void MX_I2C3_Init(void)
    {

      /* USER CODE BEGIN I2C3_Init 0 */

      /* USER CODE END I2C3_Init 0 */

      /* USER CODE BEGIN I2C3_Init 1 */

      /* USER CODE END I2C3_Init 1 */
      hi2c3.Instance = I2C3;
      hi2c3.Init.Timing = 0x00000E14;
      hi2c3.Init.OwnAddress1 = 0;
      hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
      hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
      hi2c3.Init.OwnAddress2 = 0;
      hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
      hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
      hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
      if (HAL_I2C_Init(&hi2c3) != HAL_OK)
      {
        Error_Handler();
      }

      /** Configure Analogue filter
      */
      if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
      {
        Error_Handler();
      }

      /** Configure Digital filter
      */
      if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
      {
        Error_Handler();
      }
      /* USER CODE BEGIN I2C3_Init 2 */

      /* USER CODE END I2C3_Init 2 */

    }