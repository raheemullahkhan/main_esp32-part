#include<mytasks.h>
#include<scale.h>
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;
void Task1(void *pvParameters) {
  (void) pvParameters;

  while (1) {
   // Serial.println( scale_steps);
    handle_server();
  load_buttons_flipflop();
 save_if_a_pin_pressed();
execute_the_event_if_button_is_pressed_then_clear_it();
time_depended_function();
 // digitalWrite(LED_OE,LOW);
  do_saved_actions_on_leds();
  if(pause_blink&&allowed_to_enter_pause_blink)
pause_blink_function();

 /* 
 if(motor_on)
 digitalWrite(enablePin,HIGH);
 else
 digitalWrite(enablePin,LOW);
 if(on_off>700)
 on_off=1;
if(pause_blink&&allowed_to_enter_pause_blink)
pause_blink_function();
read_scale();  
compare_force();   
//Serial.println(force_measured-corrected_force);*/
   // vTaskDelay(pdMS_TO_TICKS(10)); // Delay for 1 second
  }
}



void initialize_all_modules(void)
{
    
  //wifi_eeprom_initialize();

  // read_all_full_pattern();
  //max_size_of_all_pattern();
  allowed_restart=0;
    initialize_button_pins();
  initialize_turn_off_all_leds();
  controlpanel_start_stage(); 
}