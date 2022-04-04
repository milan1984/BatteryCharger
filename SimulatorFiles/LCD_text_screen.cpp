#include "LCD_text_screen.h"

#define LCD_BLANK_SPACE " " // One blank space for LCD screen

#ifdef LCD_I2C
LCDTextScreen::LCDTextScreen(int address, int char_numb, int raw_numb)
{

    numOfCharaters = char_numb; // LCD charaters
    numOfRaws = raw_numb;       // LCD row num
    lcd_address = address;      // LCD address

    _lcd = LiquidCrystal_I2C(lcd_address, numOfCharaters, numOfRaws);
}
#else
LCDTextScreen::LCDTextScreen(int _rs_pin, int _en_pin, int _d4_pin, int _d5_pin, int _d6_pin, int _d7_pin)
{
    _rs = _rs_pin;
    _en = _en_pin;
    _d4 = _d4_pin;
    _d5 = _d5_pin;
    _d6 = _d6_pin;
    _d7 = _d7_pin;

    _lcd = LiquidCrystal(_rs, _en, _d4, _d5, _d6, _d7);
}
#endif
// Write info on LCD screen (all 4 rows)
void LCDTextScreen::LCD_write(String row_1_String, String row_2_String, String row_3_String, String row_4_String)
{
    // lcd.clear();
    switch (numOfRaws)
    {
    case 1:
        //=========  LINE 1  ========
        _lcd.setCursor(0, 0);
        LCD_print_One_ROW(row_1_String);
        break;
    case 2:
        //=========  LINE 1  ========
        _lcd.setCursor(0, 0);
        LCD_print_One_ROW(row_1_String);
        //=========  LINE 2  ========
        _lcd.setCursor(0, 1);
        LCD_print_One_ROW(row_2_String);
        break;
    case 3:
        //=========  LINE 1  ========
        _lcd.setCursor(0, 0);
        LCD_print_One_ROW(row_1_String);
        //=========  LINE 2  ========
        _lcd.setCursor(0, 1);
        LCD_print_One_ROW(row_2_String);
        //=========  LINE 3  ========
        _lcd.setCursor(0, 2);
        LCD_print_One_ROW(row_3_String);
        break;
    case 4:
        //=========  LINE 1  ========
        _lcd.setCursor(0, 0);
        LCD_print_One_ROW(row_1_String);
        //=========  LINE 2  ========
        _lcd.setCursor(0, 1);
        LCD_print_One_ROW(row_2_String);
        //=========  LINE 3  ========
        _lcd.setCursor(0, 2);
        LCD_print_One_ROW(row_3_String);
        //=========  LINE 4  ========
        _lcd.setCursor(0, 3);
        LCD_print_One_ROW(row_4_String);
        break;
    default:
        break;
    }
}
// Check , center and print one row of LCD
void LCDTextScreen::LCD_print_One_ROW(String text_for_lcd)
{
    String for_print = Check_String_Atributes(text_for_lcd);
    PREfix_print(for_print);
    _lcd.print(for_print);
    POSTfix_print(for_print);
}
// Make sure the string size is allowed
String LCDTextScreen::Check_String_Atributes(String string_to_check)
{
    int string_to_check_Lenght = string_to_check.length();

    // if the string is bigger than allowed
    if (string_to_check_Lenght > numOfCharaters)
    {
        // return "** String to long **";
        // Return resized string
        return string_to_check.substring(0, numOfCharaters);
    }
    else
    {
        return string_to_check;
    }
}
// Determin how many caracters to print before
void LCDTextScreen::PREfix_print(String _text)
{
    for (int i = 0; i < Center_String_to_ROW(_text); i++)
    {
        _lcd.print(LCD_BLANK_SPACE);
    }
}
// Determin how many caracters to print before
void LCDTextScreen::POSTfix_print(String _text)
{
    int String_lenght = _text.length();
    int String_start_pos = Center_String_to_ROW(_text);
    int character_number = numOfCharaters - (String_start_pos + String_lenght);

    for (int i = 0; i < character_number; i++)
    {
        _lcd.print(LCD_BLANK_SPACE);
    }
}

// Center string to LCD row (returns starting position of string on LCD row)
int LCDTextScreen::Center_String_to_ROW(String _string_to_process)
{
    int numOfChar = _string_to_process.length();
    int Starting_position = 0;

    // If string is like max numb of LCD char in one row or greater then max number
    if ((numOfCharaters - numOfChar == 0) || (numOfCharaters - numOfChar < 0))
    {
        Starting_position = 0;
    }
    else
    {
        int free_places = numOfCharaters - numOfChar;
        Starting_position = (free_places / 2);
    }

    return Starting_position;
}

// Lcd init
void LCDTextScreen::LCD_init(void)
{    
    #ifdef LCD_I2C
    _lcd.begin();
    #else
    _lcd.begin(16,2);
    #endif
    _lcd.clear();
}