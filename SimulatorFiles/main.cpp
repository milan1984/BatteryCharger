#include "Functions.h"

void setup()
{
    set_IO();
    set_LCD_and_comms();
    version_message();
}

void loop()
{
    read_inputs();              // Read switches and other inputs
    change_charger_mode();   // Change mode of the charger
    calculate_charge_time(); // Calculate total digcharging time
    
    lcd_handler();              // LCD menus and text

    led_handler();   // LEDs handler
    relay_handler(); // Relay on/off

    software_reset(); // Software reset of the device
}