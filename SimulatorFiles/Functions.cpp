#include "Functions.h"

Timers t1;
Timers t2;
Timers t3;
Timers t4;
Timers t5;

Edge e1;
Edge e2;
Edge e3;

#ifdef LCD_I2C
// LCD set
LCDTextScreen lcd(0x27, 16, 2);
#else

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
#define rs 12
#define en 11 
#define d4 5
#define d5 4
#define d6 3
#define d7 2
LCDTextScreen lcd(rs, en, d4, d5, d6, d7);
#endif

// Current state of the charger
byte state_of_charger = 1;

bool settings_ok = false; // Flag for successful settings of input parameters

bool ok_switch = false;  // State of the OK_PIN
bool UP_switch = false;  // State of the UP pin switch
bool DWN_switch = false; // State of the DWN pin switch

bool ok_pressed = false;  // Button ok pressed flag
bool up_pressed = false;  // Button cancel pressed flag
bool dwn_pressed = false; // Button dwn pressed flag

bool max_cell_voltage = false; // Flag for max cell voltage

bool charging_over = false; // Flag for charging over

float cell_voltage = 0; // current battery cell voltage

//===================================================================================== TO DO =========================================================
unsigned long starting_charge_time = 0; // Start time of charging
unsigned long total_charge_time = 0;    // Total time of charging in hours
int seconds, hours, minutes;
int remaining_hours = 0; // time left until the end of charge - in hours
int remaining_min = 0;   // time left until the end of charge - in min
int remaining_sec = 0;   // time left until the end of charge - in sec

bool time_ok = false; //flag time set ok
// Set IO pins
void set_IO(void)

{ //========= INPUT pins ============
    pinMode(VOLTAGE_PIN, INPUT);
    pinMode(OK_PIN, INPUT_PULLUP);
    pinMode(DWN_PIN, INPUT_PULLUP);
    pinMode(UP_PIN, INPUT_PULLUP);
    // ======== OUT pins ===========
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(RELAY, OUTPUT);
}

// Set communication and LCD
void set_LCD_and_comms(void)
{
    Serial.begin(115200);
   // lcd.LiquidCrystal_I2C()
   lcd.LCD_init();
}

// Read state of the switches and other inputs
void read_inputs(void)
{
    ok_switch = !digitalRead(OK_PIN);
    UP_switch = !digitalRead(UP_PIN);
    DWN_switch = !digitalRead(DWN_PIN);

    ok_pressed = e1.fTrigDebounce(ok_switch, SW_DEBOUNCE_TIME);
    up_pressed = e2.fTrigDebounce(UP_switch, SW_DEBOUNCE_TIME);
    dwn_pressed = e3.fTrigDebounce(DWN_switch, SW_DEBOUNCE_TIME);

    max_cell_voltage = messure_cell_voltage(); // Read cell voltage
}

// Change discharger current mode
void change_charger_mode(void)
{
    switch (state_of_charger)
    {
    case STOP:
        if (ok_pressed && settings_ok && time_ok)
        {
            state_of_charger = RUNNING; // Go to running mode
        }
        break;
    case RUNNING:
        if (ok_pressed)
        {
            state_of_charger = PAUSE; // Go to pause mode
        }
        else if (max_cell_voltage)
        {
            state_of_charger = ENDING_CHARGE; // Go to "Charge over" mode
        }
        else if (charging_over)
        {
            state_of_charger = CHARGE_OVER; // Go to "Charge over" mode
        }
        break;
    case PAUSE:
        if (ok_pressed)
        {
            state_of_charger = RUNNING; // Go to running mode
        }
        break;
    case ENDING_CHARGE:

        if (charging_over)
        {
            state_of_charger = CHARGE_OVER;
        }

        break;
    case CHARGE_OVER:
        //======================================= TO DO IF NEEDED ==========================================
        break;
    }
    if (up_pressed || dwn_pressed)
    {
        state_of_charger = STOP; // Go to stop mode
        settings_ok = false;     // reset settings flag
    }
}

// messure cell voltage
bool messure_cell_voltage(void)
{

    int read_value = analogRead(VOLTAGE_PIN);
    cell_voltage = read_value * (ARDUINO_REF_VOLTAGE / 1024.0);

    bool result = t1.delayOn(cell_voltage >= MAX_CELL_VOLTAGE, MAX_CELL_VOLTAGE_TIME);

    return result;
}

// Handling led indications
void led_handler(void)
{
    bool led_pulse = t1.on_off_Timer(true, LED_BLINK_TIME);

    switch (state_of_charger)
    {
    case STOP: // Only red led  working
        digitalWrite(LED_RED, false);
        digitalWrite(LED_GREEN, false);
        break;
    case RUNNING: // Only green led only working
        digitalWrite(LED_RED, false);
        digitalWrite(LED_GREEN, true);
        break;
    case PAUSE: // Only green led only is blinking
        digitalWrite(LED_RED, false);
        digitalWrite(LED_GREEN, led_pulse);
        break;
    case ENDING_CHARGE: 
         digitalWrite(LED_RED, led_pulse);
        digitalWrite(LED_GREEN, true);
        break;
    case CHARGE_OVER: // Charge over, bouth leds are blinking alternately
        digitalWrite(LED_RED, !led_pulse);
        digitalWrite(LED_GREEN, led_pulse);
        break;
    default:
        break;
    }
}

// LCD handling text and menus
void lcd_handler(void)
{
    bool pulseForBlinkChar = t2.on_off_Timer(true, 500);
    bool pulseForTextChange = t3.on_off_Timer(true, 2500);
    int static speed_time = 100;

    bool speed_up_1 = t4.impulsTimer(t4.delayOn(UP_switch || DWN_switch, speed_time), speed_time);   // speed up 1 variable
    bool speed_up_2 = t5.impulsTimer(t5.delayOn(UP_switch || DWN_switch, 3000),100); // speed up 2 variable
    if (speed_up_2)
    {   
        speed_time = speed_time - 10;
    }
    else if (!UP_switch && !DWN_switch)
    {
        speed_time = 100;
    }
    speed_time = constrain(speed_time,1,100) ;

    //=====================================================
    String char_for_settings = " ";

    if (pulseForBlinkChar)
    {
        char_for_settings = char(255); // char for full field of the lcd 1 char
    }
    else
    {
        char_for_settings = "h";
    }

    static byte setting_menu = MAIN_MENU;

    // mapping value range for charging time
    // float lcd_charging_time = map(float(pot_Value), 0.0, 1023.0, MIN_CARGING_TIME, MAX_CARGING_TIME); // mapping value charging time
    switch (state_of_charger)
    {
    case STOP:
    {
        if (cell_voltage > MAX_CELL_VOLTAGE)
        {
            if (pulseForTextChange)
            {
                lcd.LCD_write(F("Napon celije je"), F("iznad maximuma!"));
            }
            else
            {
                lcd.LCD_write(F("Iskljuci"), F("bateriju !"));
            }
            setting_menu = MAIN_MENU; // Go back to main menu
            settings_ok = false;      // cancle settings
        }
        else
        {
            //==================== Scrolling through menus ===================
            if (ok_pressed)
            {
                setting_menu++; // menu up
            }
            else if (UP_switch && DWN_switch && setting_menu > MAIN_MENU)
            {
                setting_menu--; // menu down
            }
            setting_menu = constrain(setting_menu, MAIN_MENU, SET_OK);
            //==================================================================
            switch (setting_menu)
            {
            case MAIN_MENU:
                if (pulseForTextChange)
                {
                    lcd.LCD_write(F("Za podesavanja"), F("pritisni OK"));
                }
                else
                {
                    lcd.LCD_write(F("Napon celije je"), String(cell_voltage) + "V");
                }
                break;
            case SET_CHARGING_TIME: // Set charging timr value

                 if (up_pressed ||(speed_up_1 && UP_switch))
                {
                    minutes++; // value up
                    // Serial.print("up");
                     //Serial.println(minutes);

                }
                else if (dwn_pressed || (speed_up_1 && DWN_switch))
                {
                    minutes--; // value dwn
                    // Serial.print("dwn");
                   // Serial.println(minutes);

                }

                if(minutes >59)
                {
                    hours++;
                }
                else if  (minutes <0)
                {
                    hours--;
                }

                minutes = constrain(minutes, 0, 59);
                hours = constrain(hours, 0, 50);

                if (hours == 50 && minutes > 0)
                {
                    minutes = 0;
                }
                // total_charge_time = constrain(total_charge_time, MIN_CHARGING_TIME, MAX_CHARGING_TIME);

                lcd.LCD_write(F("Podesi vreme"), "punjenja " + String(hours) + ":" + String(minutes) + char_for_settings);

                break;
            case SET_OK:
                if(time_ok)
                {
                settings_ok = true;
                lcd.LCD_write(F("Podeseno!Stisni"), F("OK za start"));
                }
                else
                {
                    lcd.LCD_write(F("Vreme mora biti"), F("vece od 1min!"));
                }
                break;
            }
        }
        break;
    }
    case RUNNING:
    {
        lcd.LCD_write("Puni se,Vc=" + String(cell_voltage) + "V", "Vreme=" + String(remaining_hours) + ":" + String(remaining_min) + ":" + String(remaining_sec));
        setting_menu = MAIN_MENU;
    }
    break;
    case PAUSE:
        lcd.LCD_write(F("PAUZIRANO,stisni"), F("OK za nastavak"));
        setting_menu = MAIN_MENU;
        break;
    case CHARGE_OVER:
        if (pulseForTextChange)
        {
            lcd.LCD_write(F("Kraj Punjenja!"), "Vc=" + String(cell_voltage) + "V");
        }
        else
        {
            lcd.LCD_write(F("Pritisni < ili >"), F("taster za nazad"));
        }
        setting_menu = MAIN_MENU;
        break;
    case ENDING_CHARGE:
        if (pulseForTextChange)
        {
            lcd.LCD_write(F("Dopunjavanje BAT."), "je u toku...");
        }
        else
        {
            lcd.LCD_write("Napon Vc=" + String(cell_voltage) + "V", "Vreme=" + String(remaining_hours) + ":" + String(remaining_min)+ ":" + String(remaining_sec));
        }
        break;
    default:
        break;
    }
}

// Calulation of the discharge duration
void calculate_charge_time(void)
{
    unsigned long remaining_milisec;
    unsigned long current_time = millis(); // Current time
    static int last_State = state_of_charger;
   
    total_charge_time = hours * 3600000 + minutes * 60000; // convert hours and min to milisec

    if (total_charge_time > 59000) //set time is > of 1 sec
    {
        time_ok = true;
    }
    else
    {
       time_ok = false;
    }

    if (state_of_charger == RUNNING || state_of_charger == ENDING_CHARGE)
    {
        remaining_milisec = total_charge_time - (millis() - starting_charge_time);

    }
    else
    {
        remaining_milisec = 0;
    }
    


//3600000 milliseconds in an hour
remaining_hours = remaining_milisec / 3600000;
remaining_milisec = remaining_milisec - 3600000 * remaining_hours;
//60000 milliseconds in a minute
remaining_min = remaining_milisec / 60000;
remaining_milisec = remaining_milisec - 60000 * remaining_min;

//1000 milliseconds in a second
remaining_sec = remaining_milisec / 1000;
remaining_milisec = remaining_milisec - 1000 * remaining_sec;

      //Debug remaining time
   // Serial.println((String)remaining_hours + ":" + (String)remaining_min + ":" + (String)remaining_sec); // Dubug
    if (state_of_charger == STOP)
    {
        // Reset all times
        charging_over = false;
        starting_charge_time = 0;
    }
    else
    {
        if (state_of_charger == RUNNING && last_State == STOP) // Transition from stop to running mode
        {
            starting_charge_time = millis(); // set start time
        }
        else if (current_time - starting_charge_time >= total_charge_time)
        {
            charging_over = true;
        }
        else if (state_of_charger == PAUSE && last_State == RUNNING)
        {
            //============================================================================= TO DO =====================================
        }
    }

    last_State = state_of_charger;
}

// OUTPUT Relay on / off handling
void relay_handler(void)
{
    switch (state_of_charger)
    {
    case RUNNING:
        digitalWrite(RELAY, true);
        break;
    case ENDING_CHARGE:
        if (cell_voltage <= MIN_FULL_CELL_VOLTAGE)
        {
            digitalWrite(RELAY, true);
        }
        else if (cell_voltage >= MAX_CELL_VOLTAGE)
        {
            digitalWrite(RELAY, false);
        }
        break;
    default:
        digitalWrite(RELAY, false);
        break;
    }
}

// SW Reset function
void (*resetFunc)(void) = 0; // declare reset function @ address 0

// Software reset of the device
void software_reset(void)
{
    // Delay for reset
    bool reset = t2.delayOn((UP_switch && DWN_switch), RESET_TIMEOUT);

    if (reset)
    {
        // Call function @ address 0  to reset controller
        resetFunc();
    }
}

// float map
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//SW verison message
void version_message(void)
{  
     String version = "Software version 0.2";
    Serial.println(version);
}