
#ifndef mytasks_h
#define mytasks_h
#include <Arduino.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<controllpanel.h>
extern TaskHandle_t Task1_Handle;
extern TaskHandle_t Task2_Handle;
void Task1(void *pvParameters);
void Task2(void *pvParameters);
void initialize_all_modules(void);

#endif