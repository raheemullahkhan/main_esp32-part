#include<mytasks.h>
#include <Arduino.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<controllpanel.h>
#include <strain_guage.h>
#include <motor.h>
#include<bluetooth.h>
#include<pattern.h>
extern bool reed_error;
bool motor_scale_debug=0;
bool guage_debug=0;
int high_pressure_signal=15;
extern uint32_t step_count;
 TaskHandle_t  motor_handle=NULL;
extern int scale_setting;
extern int speed_setting;
extern int pressure_setting;
 extern bool puause_flag;
extern bool homing;
extern bool pause_blink;


void motortask(void * p)
{

  while(1)
  {

 if(homing==1)
{
    enable_motor();
    set_up_direction();
}
  


     if(scale_setting>0&&pressure_setting>0&&speed_setting>0&&puause_flag==0&&pause_blink==0&&homing==0&&reed_error==0)
  
      {
   
       // run_pattern(1);
       run_sawtooth();
       delay(6000);


    }
  }
}

void setup() {
  Serial.begin(2000000);
  bluetooth_init();
 wifi_eeprom_initialize();
   pinMode(high_pressure_signal,INPUT_PULLUP);
  motor_init();
set_down_direction();
  step_count=0;    
 xTaskCreatePinnedToCore(motortask,"motor_running",3040,NULL,1,&motor_handle,1);
  initialize_all_modules();
  make_x_y_int();
}


void loop() {
  int a=3;
Task1(&a);
}

