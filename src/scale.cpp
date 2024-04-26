#include<scale.h>
uint8_t scale_data[11]={1,2,3,4,5,6,7,8,9,10,11};
uint8_t motor_scale_indx=0;
bool allowed_to_count_after_limit_crossed;
int force_measured=0;
uint8_t duty_cycle;
bool clockwise;
bool allowed_to_detect_object=1;
bool object_detected=0;
int absoulte_count;
bool offset_with_out_pressure=0;
int force_allowed[11]={ 15,20,25,30,35,40,45,50,55,60,65};
int32_t corrected_force;
bool reverse_direction_limit_crossed;
ADS1220_WE ads = ADS1220_WE(ADS1220_CS_PIN, ADS1220_DRDY_PIN);

void compare_force()
{
if((abs(force_measured)-abs(corrected_force))>=force_allowed[duty_cycle]&&allowed_to_detect_object)
{
      object_detected=0;
      absoulte_count=0;
    //  Serial.println("object_detected");
 }
}
void scale_init()
{
Serial.println("scale_initialize");
  if(!ads.init()){
    Serial.println("ADS1220 is not connected!");
    while(1);
  }
  ads.setDataRate(ADS1220_DR_LVL_6);
  ads.setOperatingMode( ADS1220_TURBO_MODE); 
}
void read_scale()  
{
   ads.setAvddAvssAsVrefAndCalibrate();
 
 
 ads.setCompareChannels(ADS1220_MUX_0_1);
 force_measured= ads.getRawData();
}


void pressure_calculation()
{
  bar_motion=0;
  allowed_to_detect_object=0;
  reverse_direction_limit_crossed=0;
  bool clockwise=1;
 if(clockwise)
  absoulte_count++;
 else
  absoulte_count--;
if(absoulte_count==0)
{
  allowed_to_count_after_limit_crossed=1;
  reverse_direction_limit_crossed=1;
  offset_with_out_pressure=1;
  bar_motion=1;
  object_detected=0;
}                         
if(abs(absoulte_count)==200&&offset_with_out_pressure) 
{
  bool waste=0;
}        
}