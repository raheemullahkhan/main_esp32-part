#include<motorprev.h>
#include<scale.h>
#include<eepromwifi.h>
int count_after_reverse_limit_crossed;
bool allowed_to_note_direction_after_touch;
bool obsticle_direction;
bool puause_flag=0;
bool scale_zero;
bool force_zero;
bool speed_zero;
bool scaling=0;

 int outputPin = 15;  // black
 int directionPin = 0;  //wh
  int enablePin = 2;    //blue
 int hlfbPin = 33;      // green

 bool bar_motion=1;
bool motion_direction;
bool prev_motion_direction;
uint32_t step_moved=0;
uint32_t step_at_turning=0;
bool do_pwm=1;
static int pwm_count = 0;
bool both_on_off_occured=0;
int on_off=0;
int stepcompleted=0;
uint8_t pattern_number=0;
int pattern_running=0;
int idx=0;
int current_position=0;
int stepgiven=0;
int pulse_per_revolution=3;
hw_timer_t *timer = NULL;
const int timerInterruptInterval = 30; // 1000 microseconds = 1 millisecond
bool multiple_program_selected_flag=0;
uint8_t multiple_program_array[9]={1,2,3,4,5,6,7,8,9};
int round_count=0;
uint8_t number_of_program_selected=0;
uint8_t program_selected=0;
bool homing=0;
bool change_botton_occur_for_motor=0;

void do_motion()
            {
              cheack_direction_change();
              if(object_detected&&do_pwm) 
              {
                pressure_calculation();
              }
              if(!allowed_to_detect_object&&allowed_to_note_direction_after_touch)
                   note_direction_after_touch();
              cheack_reverse_limit_cross_and_offset();
               
                if(pwm_count<2)
                  {
                      if(do_pwm&&bar_motion) 
                            digitalWrite(outputPin,HIGH);
                        both_on_off_occured=0;
                  }
                else
                    {
                      if(do_pwm&&bar_motion) 
                            {
                              digitalWrite(outputPin,LOW); 
                              if(motion_direction)
                                step_moved++;
                              else
                                step_moved--;
                              both_on_off_occured=1;
                            }                           
                     
                    }
                
                if(pwm_count>=2)
                    {
                    on_off++;
                    pwm_count=0;
            
            }
 } 


void  note_direction_after_touch()
    {
      allowed_to_note_direction_after_touch=0;
      obsticle_direction=motion_direction;
    }

void cheack_reverse_limit_cross_and_offset()
{
  if(allowed_to_count_after_limit_crossed)
  {
    count_after_reverse_limit_crossed++;
  }
  if(count_after_reverse_limit_crossed>=9000)
  {
    count_after_reverse_limit_crossed=0;
    allowed_to_count_after_limit_crossed=0;
    allowed_to_detect_object=1;
  }
}
 void motor_initialize()
  {
  pinMode(directionPin, OUTPUT);
  pinMode(outputPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(hlfbPin, INPUT_PULLUP);
  motion_direction=0;
  digitalWrite(directionPin, motion_direction);   // Set initial direction
  digitalWrite(enablePin,LOW);   
  }
void cheack_direction_change()
{
if(prev_motion_direction!=motion_direction)
{
  step_at_turning=step_moved;
  prev_motion_direction=motion_direction;
  corrected_force=force_measured;
}
}
  void take_new_data() {
                stepcompleted=0;
                int diff=abs(pattern[pattern_running][idx])-current_position; 
                current_position=abs(pattern[pattern_running][idx++]); 
               
                if(diff>0)
                    {
                      motion_direction=1;
                        
                      digitalWrite(directionPin,motion_direction);
                      stepgiven=abs(diff);
                      clockwise=1;
                    }
                else
                  {
          
                    motion_direction=0;
                      digitalWrite(directionPin, motion_direction);
                      stepgiven=abs(diff);
                      clockwise=0;
                  }

              if(pattern[pattern_running][idx]==-1000)//change idx and pattern number from here
                  {
                    step_moved=50;
                    absoulte_count=0;
                    idx=0;
                     pulse_per_revolution=scale_data[motor_scale_indx];
                    if(multiple_program_selected_flag)  
                      {   
                            pattern_running=multiple_program_array[program_selected]-1;
                            program_selected++;
                           if(program_selected==number_of_program_selected)
                              program_selected=0;
                      }
                    else
                    {
                      pattern_running=pattern_number;
                    }

                  }
            
            }
void onTimerInterrupt() 
          {


if(!puause_flag&&!scale_zero&&!force_zero&&!speed_zero&&!scaling&&!homing&&!change_botton_occur_for_motor)
          {
              pwm_count++;
              do_motion();
            //if(on_off==scale_data[motor_scale_indx]-1)// how much count is one step 
              if(on_off==pulse_per_revolution)
                  {
                    stepcompleted++;
                    on_off=0;       
                  }
            if(stepcompleted==max_step[pattern_running])// if 6 step is completed take next data;
               {
                stepcompleted=0;
                 round_count++;
                if(round_count==5);
                      round_count=0;
               if(round_count==0)
                   take_new_data();

               }
              if(stepcompleted<stepgiven)
              {
                do_pwm=1;
              }
              else
              do_pwm=0;
          }
       }





void timer_initialize()
{
timer = timerBegin(0, 80, true); // Timer 0, Prescaler 80, Count Up
  timerAttachInterrupt(timer, &onTimerInterrupt, true);
  timerAlarmWrite(timer, timerInterruptInterval, true);
  timerAlarmEnable(timer);
  }
 