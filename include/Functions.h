#include "Arduino.h"
#include "LCD_text_screen.h"
#include "IO_list.h"
#include "Constants.h"
#include "Timers.h"
#include "Edge.h"



//SW verison message
void version_message(void);

//Set IO pins
void set_IO(void);
 
//Set communication and LCD
void set_LCD_and_comms(void);

//Read state of the switches and other inputs
void read_inputs(void);

//Change discharger current mode
void change_charger_mode (void);

//messure cell voltage
bool messure_cell_voltage (void);

//Handling led indications
void led_handler (void);

//LCD handling text and menus
void lcd_handler(void);

//Calulation of the discharge duration
void calculate_charge_time(void);

//OUTPUT Relay on / off handling
void relay_handler(void);

// Software reset of the device
void software_reset(void);

//Float numb mapping
long map(long , long , long , long , long );