
#include<controllpanel.h>
#include<Arduino.h>
#include<stdio.h>
#include<bluetooth.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<motor.h>

bool reed_error=0;
bool wait_click_event=0;
 extern TaskHandle_t  motor_handle;
bool finding_refrence=0;
bool motor_on=0;
bool temp_pause=0;
uint8_t multiple_program_idx=0;
bool moving_direction;
bool prev_direction;
int  object_detected_and_count=0; 
bool  allowed_reverse_direction=0;
bool direction_at_touching;
bool allowed_to_detect_touching; 
uint32_t speed_data[11]={200,180,160,140,120,100,80,60,40,30,20};
uint8_t motor_force_indx=0;
bool allowed_to_enter_pause_blink=0;
int scale_setting=0;
int speed_setting=0;
int pressure_setting=0;
bool allowed_restart;
bool comming_from_off_condition=0;
bool pause_blink=0;
bool button_more_pressed_allowed=1;
uint32_t uinvesal_previous_time=0;
uint8_t NONE=0;
uint32_t temp_pause_time;
bool first_blink=1;
uint8_t pause1=0;
bool one_blinked=0;
  uint32_t time_one_time_blink_interval;
  bool time_program=0;
bool time_scale=0;
bool time_pressure=0;
bool time_speed=0;
bool new_program_selected=0;
bool scale_setected=0;
bool pressure_selected=0;
bool speed_selected=0;
uint8_t temperary_program_number=0;
uint8_t temperary_prev_event=0;
uint32_t time_for_multiple_pattern_clear=0;
uint8_t prev_id;
uint8_t prev_type;
uint8_t program_id;
bool flag_for_single_program;
bool comming_from_off_for_homing=0;

bool mention_time_from_refrence_time_passed (unsigned int _time, uint32_t *refrence_time) 
{
  uint32_t a;
  static const uint32_t MAX = 0xFFFFFFFF;  // Value at which wrap around occurs.
  if (millis() >= *refrence_time)
     a=millis()-*refrence_time;
  else 
    a= MAX - millis()+ *refrence_time+ 1;
  if(a>=_time)
    {
      *refrence_time=millis();
      return 1;
    }
  return 0;
}
int BTN_PL = 32 ,  //white Parallel load(button shift register).
BTN_IN = 35,// yellow
ST_CP = 33,// blackwhite
NBR_SWITCH = 24;
uint8_t temp=0;
enum SYSTEM_CONSTANTS 
{    
  OFF = 0,ON = 1,SS2 = 33/* blackwhite*/,SS_BUTTON = 32/*white*/, Buttons_IN = 35/*yellow*/, CLK = 27, /* orage*/                                             
};
// int LED_OE = 15;/* pink*/gnd

enum LED_CONSTANTS
 {  ledreplaced=14,/* pink*/ LED_LE = 25,/*blue*/ LED_OUT = 26,/*green*/  BLINK = 2, FAST_BLINK, BLINK_INTERVAL = 800,
    FAST_BLINK_INTERVAL=50, NUMBER_LED = 24,
};
class LED 
{
  uint8_t state = OFF;
  uint8_t lampState = OFF;
   uint32_t lastStateChange = 0;
  public:
  //int task_time=0;
  uint8_t getState() {
  switch (state) {
      case BLINK:  // Normal blink => Toggle on/off every BLINK_INTERVAL.
      {
        if(mention_time_from_refrence_time_passed(BLINK_INTERVAL,&lastStateChange))
        lampState=1-lampState;
        return lampState;
      }
      case FAST_BLINK:  // Fast blink => Toggle on/off every call.
      {
         if(mention_time_from_refrence_time_passed(FAST_BLINK_INTERVAL,&lastStateChange))
        lampState=1-lampState;
        return lampState;
      }
      default:
        return lampState;
    }
  }
  void setState(uint8_t deviceState) {
    state = deviceState;
    if (state == BLINK) {
      lastStateChange = millis();
    }
    lampState = (state == OFF) ? OFF : ON;  // Blinking starts in the ON state.
  }
  uint8_t ultimate_state_for_pause()
  {
    return state;
  }
} static LEDS[NUMBER_LED];

void clear_all_leds_states()
{
  for (int k=0;k<NUMBER_LED;k++)
    LEDS[k].setState(OFF);
  
}
void give_pulse()
{
  digitalWrite(CLK, LOW);
  digitalWrite(CLK, HIGH); 
}  
void initialize_turn_off_all_leds() {
  pinMode(LED_LE, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(LED_OUT, OUTPUT);
 // pinMode(LED_OE, OUTPUT);
  //digitalWrite(LED_OE, HIGH);
  digitalWrite(LED_LE, LOW);
  int i;
  for(i=0;i<NUMBER_LED;i++) 
    LEDS[i].setState(OFF);
  LEDS[1].setState(ON);

}

void pause_blink_function()
 {
 allowed_to_enter_pause_blink=0;
       LEDS[18].setState(BLINK); // blink
       LEDS[19].setState(BLINK);
 }
 void do_saved_actions_on_leds()
 {
  if(!pause_blink)
  {
  if (puause_flag)
  {
   LEDS[18].setState(ON);
  LEDS[19].setState(ON);
  }
  else
  {
   LEDS[18].setState(OFF);
  LEDS[19].setState(OFF);
  }}
  for(uint8_t i = 0; i < NUMBER_LED; i++) {
    digitalWrite(LED_OUT, LEDS[i].getState());
    give_pulse();

  }
  digitalWrite(LED_LE,HIGH); //latches the data shifted out
  delayMicroseconds(1);
  digitalWrite(LED_LE,LOW);
  if (wait_click_event)
  {
    Serial.println("wait for click event");
    delay(1000);
    wait_click_event=0;
  }
 }
 void do_pause_actions_on_leds()
{
  
  LEDS[18].setState(ON);
  LEDS[19].setState(ON);

  motor_on=0;
  uint8_t tem_array[NUMBER_LED];
  for (uint8_t i=0;i<NUMBER_LED;i++)
  {motor_on=1;
    if(LEDS[i].ultimate_state_for_pause()==BLINK||LEDS[i].ultimate_state_for_pause()==ON)
    tem_array[i]=ON;
    else
    tem_array[i]=OFF;

  }
   for(uint8_t i = 0; i < NUMBER_LED; i++) {
    digitalWrite(LED_OUT, tem_array[i]);
    give_pulse();

  }
  digitalWrite(LED_LE,HIGH); //latches the data shifted out
  delayMicroseconds(1);
  digitalWrite(LED_LE,LOW);
 }



 enum button_type
 {
  KEYPAD = 0,SCALE,PRESSURE,SPEED,POWER,STRAP,PAUSE,             
};
enum button_event
{
  no_event=0, clicked,hold,long_hold,extra_long,
};

class Buttons
{
  int buttonState = LOW;
  unsigned long startTime = 0;
  unsigned long endTime = 0;
  bool button_active_detected=0;
  bool event=0;
  bool not_clicked=0;
  
public:
      uint32_t sensitivity_time;
       uint8_t name;
       int type;
       uint8_t clickedcount=0;
      unsigned long buttonPressedTime = 0;
    button_event B_event;

  Buttons(int _type,uint8_t _name, uint32_t _sensitivity_time)
  {
    sensitivity_time = _sensitivity_time;
    name = _name;
    type=_type;
    B_event=no_event;
    Serial.println(name);
    Serial.println(sensitivity_time);
  }
  void display();
  void which_action_noted();
 
  bool readButton();
};

// Define the button array as a global variable
Buttons button[24] = {

Buttons(KEYPAD, 4, 1000),                                                        //     0 | Keypad #4.
Buttons(SCALE, 1, 0),                                                       //     1 | Scale increase. Hold=500ms.
Buttons(SCALE, 0, 0),                                                       //     2 | Scale decrease. Hold=500ms.
Buttons(KEYPAD, 3, 1000),                                                        //     3 | Keypad #3.
Buttons(KEYPAD, 2, 1000),                                                        //     4 | Keypad #2.
Buttons(KEYPAD, 1, 1000),                                                        //     5 | Keypad #1.
Buttons(STRAP, 0, 0),                                                         //     6 | Strap.
Buttons(POWER, 0, 1000),                                                         //     7 | Power.
Buttons(SPEED, 0, 0),                                                       //     8 | Speed decrease. Hold=500ms.
Buttons(KEYPAD, 9, 1000),                                                        //     9 | Keypad #9.
Buttons(KEYPAD, 8, 1000),                                                        //    10 | Keypad #8.
Buttons(KEYPAD, 7, 1000),                                                        //    11 | Keypad #7.
Buttons(PRESSURE, 1, 0),                                                    //    12 | Pressure increase. Hold=500ms.
Buttons(PRESSURE, 0, 0),                                                    //    13 | Pressure decrease. Hold=500ms.
Buttons(KEYPAD, 6, 1000),                                                        //    14 | Keypad #6.
Buttons(KEYPAD, 5, 1000),                                                        //    15 | Keypad #5.
Buttons(0,0,0),Buttons(0,0,0),Buttons(0,0,0),Buttons(0,0,0),Buttons(0,0,0),   // 16-20 | Unused.
Buttons(SCALE, 1, 0),                                                         //    21 | Pause(right).
Buttons(PAUSE, 1, 0),                                                         //    22 | Pause(left).
Buttons(SPEED, 1, 0),                                                       //    23 | Speed increase. Hold=500ms.
};  


void Buttons::display()
{
  Serial.print(type);
  Serial.print("  ");
  Serial.print(name);
  Serial.print("  ");
  Serial.println(B_event);
}
uint8_t extra_change=0;
uint8_t prev_extra=0;
void Buttons::which_action_noted()
{  
    if ((millis()-startTime)>sensitivity_time)
    { 
      extra_change++;
        Serial.println(" extra long");
      button_active_detected = 0;
      button_more_pressed_allowed=0;
      ///which_action_noted();m
     Serial.println( clickedcount++);
      
      endTime = millis();
      buttonPressedTime = endTime - startTime;
      B_event=extra_long;
    
    }
    if (((millis()-startTime)>800)&& ((millis()-startTime)<=sensitivity_time))
    {
      Serial.println("long_hold"); 
      endTime = millis();
      buttonPressedTime = endTime - startTime;
      B_event=long_hold;
    }
   
    if (((millis()-startTime)>500)&& ((millis()-startTime)<=800))
    {
 
      Serial.println("hold");
      
      endTime = millis();
      buttonPressedTime = endTime - startTime;
      B_event=hold;
    }
     if ((millis()-startTime)<200)
    {
      Serial.println("clicked");
      
      endTime = millis();
      buttonPressedTime = endTime - startTime;
     B_event=clicked;
    }
  prev_type=type;
 if(B_event==clicked&&type!=KEYPAD&&type!=POWER)
  {
    wait_click_event=1;
    
  }
  display();
}
bool Buttons::readButton() {
  int newButtonState = digitalRead(BTN_IN);
  if(newButtonState)
uinvesal_previous_time=millis();
    if (newButtonState != buttonState)
   {
    change_botton_occur_for_motor=1;
  
    Serial.println("button change occur");
    vTaskSuspend(motor_handle);
    uinvesal_previous_time=millis();
    
    delay(10); // debounce delay
    newButtonState = digitalRead(BTN_IN);

    if (newButtonState == HIGH) 
    {
      button_active_detected = 1;
      startTime = millis();
      //Serial.println(temp++); this can measure the number of state
    } 
    else 
    {
      
      button_active_detected = 0;
      button_more_pressed_allowed=1;
      endTime = millis();
      buttonPressedTime = endTime - startTime;
      buttonState=newButtonState;
      return 1;
    }
  }
  if (button_active_detected&&button_more_pressed_allowed) {
        Serial.println(millis() - startTime);
    which_action_noted();

  }

  buttonState = newButtonState;
  return 0;
}

void give_pulse_next_button()
{
  digitalWrite(CLK, LOW);
    digitalWrite(CLK, HIGH);
    
}  



void initialize_button_pins(void)
{
  pinMode(BTN_IN, INPUT);
  pinMode(CLK, OUTPUT);
  pinMode(BTN_PL, OUTPUT);
  pinMode(ST_CP, OUTPUT);
}

void load_buttons_flipflop()
{
  digitalWrite(ST_CP, LOW);
  digitalWrite(ST_CP, HIGH);
  digitalWrite(BTN_PL, LOW);
  digitalWrite(BTN_PL, HIGH);
}
void save_if_a_pin_pressed()
{
  

  uint8_t i;

  for (i = 0; i < 24; i++)
  {
     if(button[i].readButton())
     {
     break;
     }
      give_pulse_next_button();
 
  }

}




struct ControlSetting {                                                        // Control panel component.
  uint8_t setting;                                                             // Control setting value.
  uint8_t *lEDIds;                                                             // Associated LEDs.
};

class CPState {  // Defines the operations required for all control panel states.
  public:
   virtual void execute(uint8_t event, uint8_t type, uint8_t id) {
    Serial.print("CPState.execute: event = ");Serial.print(event);Serial.print(", type = ");Serial.print(type);Serial.print(", id = ");Serial.println(id);
   }
};

class CPOff: public CPState 
{
public:
  void begin();
  void execute(uint8_t, uint8_t, uint8_t);
} static CP_OFF;

class CPSelectProgram: public CPState 
{
public:
  void begin();
  void execute(uint8_t, uint8_t, uint8_t);
} static CP_SEL_PGM;
class CPSingleProgramSelected: public CPState 
{
public:
  void begin();
  void execute(uint8_t, uint8_t, uint8_t);
} static CP_SINGLE_PGM;

class CPMultipleProgramSelected: public CPState 
{
public:
  void begin();
  void execute(uint8_t, uint8_t, uint8_t);
} static CP_MULTIPLE_PGM;


class CPSelectScale: public CPState 
{
public:
  void begin();
  void execute(uint8_t, uint8_t, uint8_t);
} static CP_SEL_SCALE;

class CPSelectPressure: public CPState 
{
public:
  void begin();
  void execute(uint8_t, uint8_t, uint8_t);
} static CP_SEL_PRESSURE;

class CPSelectSpeed : public CPState 
{
public:
  void begin();
  void execute(uint8_t, uint8_t, uint8_t);
} static CP_SEL_SPEED;

class CPReedError: public CPState {
public:
  void begin();
  void execute(uint8_t, uint8_t, uint8_t);
} static CP_Reed_error;

class CPMotorPause: public CPState 
{
public:
  void begin();
  void execute(uint8_t, uint8_t, uint8_t);
} static CP_MOTOR_
;
//======================================================================================================================
struct ControlPanel 
{  

  CPState *currentState = &CP_OFF;
   ControlSetting power = { NONE, (uint8_t[]) { 0 } };
   ControlSetting program = { NONE, (uint8_t[]) { 2, 3, 4, 7, 8, 9, 12, 13, 14 } };
  ControlSetting scale = { NONE, (uint8_t[]) { 5, 6, 23 } };
   ControlSetting pressure = { NONE, (uint8_t[]) { 10, 11, 22 } };
   ControlSetting speed = { NONE, (uint8_t[]) { 15, 16, 21 } };
  ControlSetting*scale_pointer=&scale;
  ControlSetting*pressure_pointer=&pressure;
  ControlSetting*speed_pointer=&speed;

  ControlPanel() {                   // Set the timing for button "hold" events:
    CP_OFF.begin();                  // The control panel is off.
  }


  void setOff(uint8_t lEDs[], uint8_t len)
   {
    while (len) {
      LEDS[lEDs[--len]].setState(OFF);
    }
  }

 void setBlink(uint8_t lEDs[], uint8_t len)
    {
    while (len) {
      LEDS[lEDs[--len]].setState(BLINK);
    }
  }
  
  void adjust_single_program(uint8_t programNumber)
   {
   // program.setting=programNumber;
      multiple_program_selected_flag=0;
    setOff(program.lEDIds, 9); 
    LEDS[program.lEDIds[programNumber - 1]].setState(ON); 

  }
void adjust_multiple_program_led_on( uint8_t *multiple_program) 
{
       uint8_t j=0;
       Serial.print("the programs on are");
       multiple_program_selected_flag=1;
       for(j=0;j<9;j++)
       {
        if(multiple_program_array[j])
        {
         
        LEDS[program.lEDIds[multiple_program[j]- 1]].setState(ON); 
        Serial.print(multiple_program_array[j]);
        }
      
       }
       Serial.println();
        Serial.print("the programs offed are");
       for(j=0;j<9;j++)
       {
        if(!multiple_program_array[j])
        {
        LEDS[program.lEDIds[multiple_program[j]- 1]].setState(OFF); 
        Serial.print(multiple_program_array[j]);
        
        }
      
       }
       Serial.println();
       uinvesal_previous_time=millis(); 
        time_one_time_blink_interval=millis();
  }

void adjust_multiple_leds_small_off( uint8_t *multiple_program)
{
   uint8_t j=0;  

       for(j=0;j<9;j++)
       {
        LEDS[program.lEDIds[multiple_program[j]- 1]].setState(OFF); 
       }
    
       Serial.println();
      // uinvesal_previous_time=millis(); 
     one_blinked=1;
}


   void adjustSetting(ControlSetting *ctl, uint8_t value) 
   {
      if ((unsigned long)ctl == (unsigned long)scale_pointer)
       {
         scale_setected=1;    
       }
     if ((unsigned long)ctl == (unsigned long)pressure_pointer)
      {
        pressure_selected=1;
      }
    if ((unsigned long)ctl == (unsigned long)speed_pointer)
    {
      speed_selected=1;
    }
  if(ctl->setting&&speed_selected)
    {
      //Serial.println("speed ffffffff");
      LEDS[15].setState(ON);
    }


    if (ctl->setting <10&& value&&extra_change!=prev_extra)
    {

      //if(extra_change!=1)
      ctl->setting++;    // Only increase if setting is not at max.
      //prev_extra=extra_change;
     
    }
    if (ctl->setting && !value&&extra_change!=prev_extra) 
    {

      ctl->setting--;
    //  if(ctl->setting==0)
    //  extra_change=0;
     // prev_extra=extra_change;  
      //puause_flag=1;
    }
    Serial.print("seeting");
    Serial.println(ctl->setting);


 
    if(ctl->setting>=1&&ctl->setting<5)
    {
       setOff(ctl->lEDIds, 3);    
        LEDS[ctl->lEDIds[0]].setState(ON);
    }
     if(ctl->setting>=5&&ctl->setting<10)
    {
       setOff(ctl->lEDIds, 3);  
       LEDS[ctl->lEDIds[0]].setState(ON);  
    LEDS[ctl->lEDIds[2]].setState(ON);
    }
    if(ctl->setting>=10)
    {
      LEDS[ctl->lEDIds[1]].setState(ON);  
       LEDS[ctl->lEDIds[0]].setState(ON);  
    LEDS[ctl->lEDIds[2]].setState(ON);

    }
//*******************************motor data  index selection
     if ((unsigned long)ctl == (unsigned long)scale_pointer)
   {
     motor_scale_indx= ctl->setting;
     if(ctl->setting==0)
     scale_zero=1;
     if(ctl->setting>0)
     scale_zero=0;
     Serial.print("motor scale indx");
     Serial.println(scale_data[ctl->setting]);
     scale_setting=ctl->setting;
     motor_scale_indx=ctl->setting;

     
     //scaling=1;
    // do_scaling();
   }
    if ((unsigned long)ctl == (unsigned long)pressure_pointer)
    {
     duty_cycle=ctl->setting;
     pressure_setting=duty_cycle;
     //i2c_send(pressure_setting);
     bluetooth_send(pressure_setting);
     if(ctl->setting==0)
     force_zero=1;
     if(ctl->setting>0)
      force_zero=0;
     Serial.print("pressure");
     Serial.println(duty_cycle);
     char buffer[10];
  

  
    }
    if ((unsigned long)ctl == (unsigned long)speed_pointer)
    {
     Serial.print("motor speed data");
     if(ctl->setting==0)
     speed_zero=1;
     if(ctl->setting>0)
     speed_zero=0;

    Serial.println(speed_data[ctl->setting]);
    speed_setting=ctl->setting;
    // timerAlarmWrite(timer, speed_data[ctl->setting], true); 
    }
    //**************************************motor data index selection ended
       if(comming_from_off_condition&&!scale_zero&&!force_zero&&!speed_zero)
     {
      pause_blink=1;
      Serial.println("pause_blink");
    puause_flag=1;
      comming_from_off_condition=0;
     }
  }
} static CTL_PANEL;


void multiple_program_array_filling(uint8_t id)
{
   {  
    new_program_selected=1;
    multiple_program_selected_flag=1;
    
    
    uinvesal_previous_time=millis(); 
    Serial.print("multiple program");
    Serial.println(number_of_program_selected);
    bool pattern_exist=0;
    for(int k=0;k<number_of_program_selected;k++)
    if(multiple_program_array[k]==id)
    pattern_exist=1;

    
    if(!pattern_exist)
    {
      program_selected=0;
      multiple_program_array[number_of_program_selected]=id;
      number_of_program_selected++;

    }
    
    if(number_of_program_selected==10)
    number_of_program_selected=0;
  }
}
void do_pause_operation()
{

        if(extra_change!=prev_extra)
        {
          prev_extra=extra_change;
          Serial.println("pause occur on the system");
          if(comming_from_off_for_homing&&!scale_zero&&!force_zero&&!speed_zero)
          {
            Serial.println("start homeing");
            disable_motor();
            delay(10);
            enable_motor();
            delay(2);
            comming_from_off_for_homing=0;
            homing=1;
            
          
          }
          puause_flag=!puause_flag;
          pause_blink=0;

        }
      
}

void motor_pararmeter_setting(uint8_t event, uint8_t type, uint8_t id)
{
  if(event==clicked)
  {
  Serial.println("motor_paramter called"); 
if (type == SCALE)
   {      

    CTL_PANEL.adjustSetting(&CTL_PANEL.scale, id);
    time_pressure=1;
    uinvesal_previous_time=millis();        
 
  }
    if (type == SPEED) 
    {  
    CTL_PANEL.adjustSetting(&CTL_PANEL.speed,id);                             //   Adjust the speed setting.
    }
if (type == PRESSURE) {  
    CTL_PANEL.adjustSetting(&CTL_PANEL.pressure, id); 
    time_speed=1;
    uinvesal_previous_time=millis(); 

  }
  }
}
int c=0;
void select_single_multi_mode(uint8_t event,uint8_t id)
{
 if (event==clicked)
   { 
    Serial.print("the value of c is ");
    Serial.println(c++);
    program_id=id; 
    CP_SINGLE_PGM.begin();
    
  }
  

  if (event==hold) 
  { 
     program_id=id;
   //  multiple_program_array_filling(id);
    CP_MULTIPLE_PGM.begin();
  }

}
void clear_all_patterns()
{
  number_of_program_selected=0;
  for(int t=0;t<9;t++)
    multiple_program_array[t]=0;
}
// Define actions for control panel OFF state.
void CPOff::begin() {
  comming_from_off_condition=1;
  if(allowed_restart)
  ESP.restart();
  Serial.println("cpoff");
  CTL_PANEL.currentState = this;                                               // Set the control panel state to OFF.
  clear_all_leds_states(); 
  LEDS[1].setState(ON);                                               // Power LED is off.
  


 // BUTTONS[PWR_ID].holdInterval = 0;                                            // Power button only responds to "click".
  CTL_PANEL.program.setting = NONE;                                            // Program has not been selected.
  CTL_PANEL.scale.setting = NONE;                                              // Scale has not been selected.
  CTL_PANEL.pressure.setting = NONE;                                           // Pressure has not been selected.
  CTL_PANEL.speed.setting = NONE;                                              // Speed has not been selected.
   new_program_selected=0;
   scale_setected=0;
   pressure_selected=0;
   speed_selected=0;
  puause_flag=0;

    clear_all_patterns();
    motor_on=0;
    scale_zero=1;
    force_zero=1;
    speed_zero=1;
    allowed_to_enter_pause_blink=1;
    comming_from_off_for_homing=1;
   
}
void CPOff::execute(uint8_t event, uint8_t type, uint8_t id) 
{
  if (type == POWER&&event==extra_long) 
  {                                                         // If the power button was pressed:
    LEDS[0].setState(ON); 
    LEDS[1].setState(OFF);
    time_program=1;
    uinvesal_previous_time=millis();
  }
if(type==STRAP&&event==clicked)
  CP_Reed_error.begin();    

}



// Define actions for control panel CP_PROGRAM state.
void CPSelectProgram::begin() {
  Serial.println("selact a program");
  allowed_restart=1;
  CTL_PANEL.currentState = this;                                               // Set the control panel state to CP_PROGRAM.
  LEDS[0].setState(ON);                             // Power LED blinks slowly.
  CTL_PANEL.setBlink(CTL_PANEL.program.lEDIds, 9); 



}
void CPSelectProgram::execute(uint8_t event, uint8_t type, uint8_t id) {
  if(type==KEYPAD)
  select_single_multi_mode(event,id);
    motor_pararmeter_setting(event,type,id);
 if (type == POWER)                                                    // If the power button was pressed:
    CP_OFF.begin();                                                            //   Activate OFF.
 if(type==STRAP&&event==clicked)
  CP_Reed_error.begin();  
if(type==PAUSE) 
  do_pause_operation();




}

void CPSingleProgramSelected::begin()
{
  Serial.println("single program mode");
  CTL_PANEL.currentState = this; 
  CTL_PANEL.adjust_single_program(program_id);
  pattern_number=program_id-1;
  Serial.print("pattern number is ");
  Serial.println(pattern_number);
 new_program_selected=1;

  clear_all_patterns();
  multiple_program_idx=0;
  finding_refrence=1;

}


void CPSingleProgramSelected::execute(uint8_t event, uint8_t type, uint8_t id)
{
    motor_pararmeter_setting(event,type,id);
  if(type==KEYPAD)
  select_single_multi_mode(event,id);

  if (type == POWER)                                                    // If the power button was pressed:
    CP_OFF.begin();                                                            //   Activate OFF.
 if(type==STRAP&&event==clicked)
  CP_Reed_error.begin();  
if(type==PAUSE) 
  do_pause_operation();
  
  
}
void CPMultipleProgramSelected ::begin()
{
  Serial.println("MULTIPLE program mode");
  CTL_PANEL.currentState = this; 
  CTL_PANEL.adjust_multiple_program_led_on(multiple_program_array);
  new_program_selected=1;

  
  
 
}


void CPMultipleProgramSelected::execute(uint8_t event, uint8_t type, uint8_t id)
{
    motor_pararmeter_setting(event,type,id);
  if(type==KEYPAD&&event==clicked)
    {
      LEDS[ CTL_PANEL.program.lEDIds[id- 1]].setState(ON);
    flag_for_single_program=1;
    program_id=id;// for CP_SINGLE_PGM.begin()has no argument so we initilize it;
    }
  if(type==KEYPAD&&event==hold)
  {
    flag_for_single_program=0;
    CTL_PANEL.setOff(CTL_PANEL.program.lEDIds,9);
    LEDS[ CTL_PANEL.program.lEDIds[id- 1]].setState(ON);
    CTL_PANEL.adjust_multiple_program_led_on(multiple_program_array);// turn ON multiple arrary leds
  }
if(type==KEYPAD&&event==long_hold)
{
CTL_PANEL.adjust_multiple_leds_small_off(multiple_program_array);
LEDS[ CTL_PANEL.program.lEDIds[id- 1]].setState(OFF);


}
if(type==KEYPAD&&event==extra_long)
{
  multiple_program_array_filling(id);
  CTL_PANEL.adjust_multiple_program_led_on(multiple_program_array);
}

if (type == POWER)                                                    // If the power button was pressed:
    CP_OFF.begin();                                                            //   Activate OFF.
 if(type==STRAP&&event==clicked)
  CP_Reed_error.begin();  
if(type==PAUSE) 
  do_pause_operation();

}
void CPSelectScale::begin() {
  Serial.println("cpscale");
  if(!scale_setected) //scale selection mode has choice to press scale_setting but not yet don
  LEDS[6].setState(BLINK);
  CTL_PANEL.currentState = this;                                               // Set the control panel state to CP_SEL_SCALE.
 // CTL_PANEL.adjustSetting(&CTL_PANEL.scale, 0);  
  Serial.println("scale_select called");                          

}
void CPSelectScale::execute(uint8_t event, uint8_t type, uint8_t id)
 {
  if(type==KEYPAD)
  select_single_multi_mode(event,id);
    motor_pararmeter_setting(event,type,id);

 if (type == POWER) {                                                    // If the power button was pressed:
    CP_OFF.begin();                                                            //   Activate OFF.
  }
 if(type==STRAP&&event==clicked)
  CP_Reed_error.begin();  
if(type==PAUSE)
  do_pause_operation();
      
 
}

// Define actions for control panel CP_PRESSURE state.
void CPSelectPressure::begin() {
  Serial.println("pressure");
  if(!pressure_selected) //scale selection mode has choice to press scale_setting but not yet don
  LEDS[11].setState(BLINK);
  CTL_PANEL.currentState = this;                                               // Set the control panel state to CP_SEL_PRESSURE.
 // CTL_PANEL.adjustSetting(&CTL_PANEL.pressure, 0);                             // Decrease the pressure setting(sets the LEDs properly).
 Serial.println("pressure selected called");
  motor_on=1;
}
void CPSelectPressure::execute(uint8_t event, uint8_t type, uint8_t id) {

  if(type==KEYPAD)
  select_single_multi_mode(event,id);
    motor_pararmeter_setting(event,type,id);


 if (type == POWER) {                                                    // If the power button was pressed:
    CP_OFF.begin();                                                            //   Activate OFF.
  }
 if(type==STRAP&&event==clicked)
  CP_Reed_error.begin(); 
  if(type==PAUSE)
  do_pause_operation();  
}

// Define actions for control panel CP_SPEED state.
void CPSelectSpeed::begin() {
  Serial.println("speed begin ");
    
  if(!speed_selected)
  LEDS[16].setState(BLINK);
  CTL_PANEL.currentState = this;     
  
  //CTL_PANEL.adjustSetting(&CTL_PANEL.speed, 0); 
  Serial.println("speed selected called");
  motor_on=1;  
}
void CPSelectSpeed::execute(uint8_t event, uint8_t type, uint8_t id) 
{
   motor_pararmeter_setting(event,type,id);
  if(type==KEYPAD)
  select_single_multi_mode(event,id);

 if (type == POWER) {                                                    // If the power button was pressed:
    CP_OFF.begin();                                                            //   Activate OFF.
  }
 if(type==STRAP&&event==clicked)
  CP_Reed_error.begin(); 
  if(type==PAUSE)
  do_pause_operation();  
  
}


void CPReedError::begin() {
  CTL_PANEL.currentState = this;                                               // Set the control panel state to CP_MOTOR_ON.
  Serial.println("reed sonsor error");
  reed_error=1;
   time_program=0;
   puause_flag=0;
time_scale=0;
 time_pressure=0;
time_speed=0;

 clear_all_leds_states();
 LEDS[1].setState(FAST_BLINK);
 motor_on=0;
}
void CPReedError::execute(uint8_t event, uint8_t type, uint8_t id) {
  
  if (type == POWER&&event==clicked) {              
    CP_OFF.begin();                                                            //   Activate OFF.
  }
}






void execute_the_event_if_button_is_pressed_then_clear_it()
{
   int k=0;
  for(k=0;k<24;k++)
  {
    if(button[k].B_event!=no_event)
    {
      CTL_PANEL.currentState->execute(button[k].B_event, button[k].type, button[k].name);

    }

 

  }
  for(k=0;k<24;k++)
  {
  button[k].B_event=no_event;

  }

}
void time_depended_function()
{
if(homing&&mention_time_from_refrence_time_passed(2000,&uinvesal_previous_time))
{
  homing=0;
  Serial.println("end homing");
}
if(change_botton_occur_for_motor&&mention_time_from_refrence_time_passed(500,&uinvesal_previous_time))
{
  Serial.println("button change time end");
  vTaskResume(motor_handle);

  change_botton_occur_for_motor=0;
}
  if(temp_pause&&mention_time_from_refrence_time_passed(200,&temp_pause_time))
  temp_pause=0;
 uint32_t waiting_for_scale_setting;
 if(multiple_program_selected_flag)
 waiting_for_scale_setting=4000;
 else 
 waiting_for_scale_setting=2000;
if(flag_for_single_program&&mention_time_from_refrence_time_passed(50,&uinvesal_previous_time))
{
  flag_for_single_program=0;
  CP_SINGLE_PGM.begin();
}
  if(time_program&& mention_time_from_refrence_time_passed(1500,&uinvesal_previous_time))//
  {
    time_program=0;
    CP_SEL_PGM.begin();
  }
  if(new_program_selected&& mention_time_from_refrence_time_passed(waiting_for_scale_setting,&uinvesal_previous_time))
  {
    new_program_selected=0;
    CP_SEL_SCALE.begin();
  }
   if(time_pressure&& mention_time_from_refrence_time_passed(2000,&uinvesal_previous_time))
  {
    time_pressure=0;
    CP_SEL_PRESSURE.begin();
  }
   if(time_speed&& mention_time_from_refrence_time_passed(2000,&uinvesal_previous_time))
  {
    time_speed=0;
    CP_SEL_SPEED.begin();
  }
}
void controlpanel_start_stage(void)
{
     CP_OFF.begin();
}