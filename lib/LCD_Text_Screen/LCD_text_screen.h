#ifndef LCD_H
#define LCD_H

//#define LCD_I2C //Comment this if you are using "clasic" lcd 16x2 in 4 pin mode

#include "Arduino.h"
#ifdef LCD_I2C
#include "LiquidCrystal_I2C.h"
#else
#include "LiquidCrystal.h"
#endif

class LCDTextScreen
{
private:
    int numOfCharaters; // LCD charaters
    int numOfRaws;      // LCD row num
    int lcd_address;    // LCD address
    int _rs, _en, _d4, _d5, _d6, _d7;

#ifdef LCD_I2C
    // Set the LCD
    LiquidCrystal_I2C _lcd = LiquidCrystal_I2C(lcd_address, numOfCharaters, numOfRaws);
#else
    LiquidCrystal _lcd = LiquidCrystal(_rs, _en, _d4, _d5, _d6, _d7);
#endif
    // Make sure the string size is allowed
    String Check_String_Atributes(String);

    // Check , center and print one row of LCD
    void LCD_print_One_ROW(String);

    // Determin how many caracters to print before
    void PREfix_print(String);

    // Determin how many caracters to print after
    void POSTfix_print(String);

    // Center string to LCD row
    int Center_String_to_ROW(String);

public:
     #ifdef LCD_I2C
    // Constructor
    LCDTextScreen(int address, int raw_numb, int char_numb);
    #else
    LCDTextScreen(int _rs_pin, int _en_pin, int _d4_pin, int _d5_pin, int _d6_pin, int _d7_pin);
    #endif 

    // Lcd init
    void LCD_init(void);

    // Write info on LCD screen (all 4 rows)
    void LCD_write(String raw1 = " ", String raw2 = " ", String raw3 = " ", String raw4 = " ");
};

#endif