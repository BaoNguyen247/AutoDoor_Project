    #ifndef __LCD_MODULE_H
    #define __LCD_MODULE_H
    #include <stdint.h>
    void MX_I2C3_Init(void);
    void lcd_send_nibble(uint8_t nibble, uint8_t rs);
    void lcd_send_byte(uint8_t byte, uint8_t rs);
    void lcd_init(void);
    void lcd_command(uint8_t cmd);
    void lcd_data(uint8_t data);
    void lcd_print(const char *str);
    void lcd_set_cursor(uint8_t row, uint8_t col);
    #endif 