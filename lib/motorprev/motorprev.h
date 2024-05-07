#ifndef motor_h
#define motor_h
#include<Arduino.h>
extern bool bar_motion;
extern bool allowed_to_note_direction_after_touch;
extern bool obsticle_direction;
extern int outputPin ;  // black
extern int directionPin ;  //white
extern int enablePin ;    //blue
extern int hlfbPin ; 
extern bool motion_direction;
extern bool puause_flag;
 extern bool scale_zero;
extern bool force_zero;
 extern bool speed_zero;
extern bool scaling;
 extern bool prev_motion_direction;
extern uint32_t step_moved;
 extern uint32_t step_at_turning;
 extern int on_off;
 extern uint8_t pattern_number;
 extern bool multiple_program_selected_flag;
 extern uint8_t multiple_program_array[9];
 extern bool homing;
 extern bool change_botton_occur_for_motor;
 void  note_direction_after_touch();
void motor_initialize(void);
void cheack_direction_change();
void cheack_reverse_limit_cross_and_offset(void);
void do_motion(void);
void timer_initialize(void);
void move(void);
#endif