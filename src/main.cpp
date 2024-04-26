#include<mytasks.h>
#include <Arduino.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<controllpanel.h>
#include <strain_guage.h>
#include <motor.h>
#include<bluetooth.h>
#include <math.h>
extern bool reed_error;

bool motor_scale_debug=0;
bool kepad_debug=0;
bool guage_debug=0;
int high_pressure_signal=15;
int exterme_signal=2;

extern uint32_t step_count;
 TaskHandle_t  motor_handle=NULL;

extern uint8_t scale_setting;
extern uint8_t speed_setting;
extern uint8_t pressure_setting;
 extern bool puause_flag;
extern bool homing;
extern bool pause_blink;
int time_duration[11]={30,27,24,21,18,15,12,9,6,4,100};
int scale_distance[11]={10,20,30,40,50,60,70,80,90,100,100};
void motortask(void * p)
{
  while(1)
  {

 if(homing==1)
{

    enable_motor();
  set_up_direction();
}
    int duration=time_duration[speed_setting];
    //Serial.println(duration);
        double result = (scale_data[scale_setting] / 80.0) * 100;

    // Round to the nearest integer
    int scale_steps = (int)round(result);
   // Serial.println(scale_steps);


    
      
      //  Serial.println(scale_setting);

     if(scale_setting>0&&pressure_setting>0&&speed_setting>0&&puause_flag==0&&pause_blink==0&&homing==0&&reed_error==0)
  
      {
      
        generate_steps(scale_steps, duration);
      bluetooth_send(41);// noting extreme position
        changeDirection();
        generate_steps(scale_steps, duration);
        bluetooth_send(41);
        changeDirection();
      // vTaskDelay(pdMS_TO_TICKS(1));
      

    }
  }
}

void setup() {
  Serial.begin(500000);
  bluetooth_init();
 wifi_eeprom_initialize();
   pinMode(high_pressure_signal,INPUT_PULLUP);
  motor_init();
set_up_direction();
  step_count=0;    
 xTaskCreatePinnedToCore(motortask,"motor_running",3040,NULL,1,&motor_handle,1);
  initialize_all_modules();
xTaskCreatePinnedToCore(Task1, "Task1", 30000, NULL, 1, NULL,0);


}


void loop() {



}

/*
#include "BluetoothSerial.h"
#include<mytasks.h>
#include <Arduino.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<controllpanel.h>
#include <strain_guage.h>
#include <motor.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please enable it in your build environment.
#endif
bool motor_scale_debug=1;
bool kepad_debug;
bool guage_debug=1;
int high_pressure_signal=15;
int exterme_signal=2;
extern uint32_t step_count;
 TaskHandle_t Taskh1=NULL;

extern uint8_t scale_setting;
extern uint8_t speed_setting;
extern uint8_t pressure_setting;
 extern bool puause_flag;
extern bool homing;
extern bool pause_blink;

void motortask(void * p)
{
  while(1)
  {

//if (homing==1)
{

    enable_motor();
  //set_up_direction();
}
    int duration=9;

    
      
      //  Serial.println(scale_setting);

  //    if(scale_setting>0&&pressure_setting>0&&speed_setting>0&&puause_flag==0&&pause_blink==0&&homing==0)
  
      {
      
        generate_steps(80, duration);
       // bluetooth_send_eco(41);
        changeDirection();
        generate_steps(80, duration);
        i2c_send(41);
        changeDirection();
       //vTaskDelay(pdMS_TO_TICKS(1));
      

    }
  }
}
BluetoothSerial SerialBT; // Bluetooth Serial Object for client



void setup() {

motor_init();
set_up_direction();
SerialBT.begin(20000000);
  step_count=0;    
  
    xTaskCreatePinnedToCore(motortask,"motor_running",3040,NULL,1,NULL,1);


  
  initialize_all_modules();
xTaskCreatePinnedToCore(Task1, "Task1", 30000, NULL, 1, NULL,0);

}

void loop() {
  if (SerialBT.connected()) {

    SerialBT.write('f'); // Send via Bluetooth
    
    delay(13); // Adjust delay to control message frequency
    if (SerialBT.available()) {
    
    char c = SerialBT.read(); 
       delay(10); // Small delay to give time for buffer handling
    Serial.println(c);

    // Introduce a delay to avoid high-frequency looping
 
  }

  } else {
    Serial.println("Disconnected from the Bluetooth server.");
    delay(1000); // Delay before attempting reconnection
  }
}



/*
#include<controllpanel.h>

void setup() {
  Serial.begin(115200);
  scale_init();  
  wifi_eeprom_initialize();
 timer_initialize();
 motor_initialize();
   read_all_full_pattern();
   max_size_of_all_pattern();
  allowed_restart=0;
    initialize_button_pins();
  initialize_turn_off_all_leds();
  controlpanel_start_stage();
}


void loop() {
handle_server();
  load_buttons_flipflop();
 save_if_a_pin_pressed();
execute_the_event_if_button_is_pressed_then_clear_it();
time_depended_function();
  digitalWrite(LED_OE,LOW);
  do_saved_actions_on_leds();
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
Serial.println(force_measured);
    
}*/










