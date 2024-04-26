#ifndef scale_h
#define scale_h
#include<Arduino.h>
#include<ADS1220_WE.h>
#include<motorprev.h>
extern hw_timer_t *timer;
extern uint8_t motor_scale_indx;
extern uint8_t number_of_program_selected;
extern uint8_t program_selected;
extern uint8_t scale_data[11];
 extern bool allowed_to_count_after_limit_crossed;
extern bool clockwise;
extern bool reverse_direction_limit_crossed;
extern bool offset_with_out_pressure;
extern int force_measured;
 extern uint8_t duty_cycle;
 extern bool allowed_to_detect_object;
 extern bool object_detected;
extern int absoulte_count;
 extern int force_allowed[11];
 extern int32_t corrected_force;
#define ADS1220_CS_PIN    5 // chip select pin
#define ADS1220_DRDY_PIN  34 // data ready pin

void scale_init(void);
void read_scale(void);
void compare_force(void);
void pressure_calculation(void);


#endif