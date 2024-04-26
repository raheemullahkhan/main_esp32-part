#include<strain_guage.h>
extern bool motor_scale_debug;
extern int high_pressure_signal;
long strain_guage_rtos;
bool extreme_taking_data=0;
double force_alloweded=2000;
extern bool guage_debug;
#define free_step_limit 8
double value_at_extreme=0;
void take_value_after_two_step_of_direction_change()
{
  extreme_taking_data=1;
  /*value_at_extreme=ads.getRawData();
  Serial.println(value_at_extreme);
  ads.start();
  extreme_taking_data=0;*/
}
bool object_detected_between_extremes(bool complex_flag,uint8_t free_step,uint8_t step_count) 
{
  bool a;
  double result= strain_guage_rtos;
  //Serial.println(result);
  if(complex_flag==imagnary)
  {
   // Serial.println("imagnary collision not allowed");
    a=0;
  }
  
  if (complex_flag==real&&free_step>9)
      {
         // if(abs(abs(result)-abs(value_at_extreme))>force_alloweded)
         if(result==0)
          {
            if(motor_scale_debug)
            Serial.println("collision detected");
            a= 1;
          }
            else
            {
              //Serial.println("collision not detected");
                  a= 0;
            }
    }
  if (complex_flag==real&&free_step<9)
 {
   a=0;
   extreme_taking_data=1;
    //value_at_extreme=ads.getRawData();   
   //Serial.print("real_free_steps");
   //Serial.println(free_step);
   }
   if (complex_flag==real&&free_step==9)
 {
   
    extreme_taking_data=1; 

   }
   if(abs(step_count)<3)// donot detect collision -3<stepcount<3
   a=0;


return a;

}
void update_ads_dataRtos(void)
{
    
/* while(!Serial2.available())  
 delayMicroseconds(1); 
 if (Serial2.available()) {
    // Read the incoming string until a newline character is received
    String incomingString = Serial2.readStringUntil('\n');
    // Convert the string to a long integer
    if(extreme_taking_data)
    {
       value_at_extreme = atol(incomingString.c_str());
       if(motor_scale_debug)
      { 
        Serial.println("my extreme");
       Serial.println(value_at_extreme);
      }
      extreme_taking_data=0;
    }
    else
        {
            strain_guage_rtos = atol(incomingString.c_str());
           if(guage_debug) 
          { 
            //Serial.print("updated");
          
        Serial.println(strain_guage_rtos);
          }
        }
    
  }*/
              strain_guage_rtos = digitalRead(high_pressure_signal);
           if(guage_debug) 
          { 
            Serial.print("updated");
          
        Serial.println(strain_guage_rtos);
        }

}