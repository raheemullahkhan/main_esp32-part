#ifndef strain_guage_H
#define strain_guage_H
#include <Arduino.h>
#include <SPI.h>
#define real 1
#define imagnary 0

void take_value_after_two_step_of_direction_change();
bool object_detected_between_extremes(bool,uint8_t,uint8_t );

void update_ads_dataRtos(void);

#endif
