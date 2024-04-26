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
;

void compare_force()
{
if((abs(force_measured)-abs(corrected_force))>=force_allowed[duty_cycle]&&allowed_to_detect_object)
{
      object_detected=0;
      absoulte_count=0;
    //  Serial.println("object_detected");
 }
}




