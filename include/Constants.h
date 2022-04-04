

// modes for discharger
#define STOP 1
#define RUNNING 2
#define PAUSE 3
#define CHARGE_OVER 4
#define ENDING_CHARGE 5

// Mode for settings of the discharger
#define MAIN_MENU 0
#define SET_CHARGING_TIME 1
#define SET_OK 2

#define LED_BLINK_TIME 300 // time in milisec

#define SW_DEBOUNCE_TIME 50 // Time in milisec for debounce switches

#define ARDUINO_REF_VOLTAGE 5.0     //Voltage on ref pin 
#define MIN_FULL_CELL_VOLTAGE 2.45  //MIn cell voltage when battery is fulll
#define MAX_CELL_VOLTAGE 2.7        // Threshold cell voltage for relay off (in volts),also error for cell voltage
#define MAX_CELL_VOLTAGE_TIME 1000    // Time delay on for flag of the cell voltage max in milisec

#define MIN_CHARGING_TIME 0.1  //Minimal charging time in hours
#define MAX_CHARGING_TIME 50.0 //Minimal charging time in hours

#define RESET_TIMEOUT 5000 // Software Reset timeout in milisec