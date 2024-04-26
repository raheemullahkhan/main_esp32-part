#ifndef controllpanel_h
#define controllpanel_h
#include<Arduino.h>
#include<SPI.h>
#include<scale.h>
#include<eepromwifi.h>
#include<motorprev.h>
extern bool motor_on;
extern bool pause_blink;
extern bool allowed_to_enter_pause_blink;
extern bool allowed_restart;

extern int  LED_OE;
void i2c_send(uint8_t );
void controlpanel_start_stage(void);
void initialize_button_pins(void);
 void initialize_turn_off_all_leds(void);
 void load_buttons_flipflop(void);
 void save_if_a_pin_pressed(void);
 void execute_the_event_if_button_is_pressed_then_clear_it(void);
void time_depended_function(void);
 void do_saved_actions_on_leds(void);
void pause_blink_function(void);

#endif
