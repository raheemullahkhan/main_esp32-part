#ifndef eepromwifi_h
#define eepromwifi_h

#include <Arduino.h>

extern int pattern[10][1600];
extern int pattern_stop[10][1];
extern int max_step[10];
void max_step_size(int j);
void max_size_of_all_pattern();
 void wifi_eeprom_initialize(void);
void read_all_full_pattern(void);
void handle_server(void);
#endif
