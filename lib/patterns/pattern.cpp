#include "pattern.h"
#include<motor.h>
#include<bluetooth.h>
#include <math.h>
#include<Arduino.h>
#include<controllpanel.h>
int  scale_steps;
int time_duration[11]={70,70,70,21,18,15,12,9,6,4,100};
int scale_distance[11]={10,20,30,40,50,60,70,80,90,100,100};
extern int scale_setting;
extern int scale_setting;
extern int speed_setting;
float x[100]={0.0000000,0.2500000,0.5000000,0.7500000,1.0000000,1.2500000,1.5000000,1.7500000,2.0000000,3.2500000,3.5000000,3.7500000,4.0000000,4.2500000,4.5000000,4.7500000,4.9958000,5.5082998,6.0000000,6.2500000,6.5000000,
2};
float y[100]={ 0.0000000,0.7500000,0.0000000,1.5000000,0.0000000,2.2500000,0.0000000,3.0000000,0.0000000,5.0000000,1.5000000,4.5000000,0.7500000,3.7500000,0.0000000,3.0000000,0.0500000,1.5000000,0.0000000,0.7500000,0.0000000,-10
};
int xtime[100];
int ystep[100];
int yscale[100];
void make_x_y_int(void)
{
    for (int i=0;i<100;i++)
    {
     xtime[i]=x[i]*10000;
     Serial.print(xtime[i]);
     Serial.print("\t");
     ystep[i]=y[i]*50;
     Serial.println(ystep[i]) ;
    }

}
void update_scale_steps()
{
    double result = (scale_distance[scale_setting] / 80.0) * 100;
     scale_steps = (int)(result);
}
 void update_scale_parameter()

 {
    int runned=ystep[0];
  int next =ystep[1];
Serial.print(" update serial parameters     tththth");
Serial.println(scale_setting);

int i=0;
int next_stop=0;
while (next_stop!=-500)
{
  runned=ystep[i];
  next=ystep [i+1];
  next_stop=ystep[i+2];
  Serial.print(ystep[i]);
  Serial.print("\t");
   double result =(scale_distance[scale_setting]*ystep[i])/100;
    scale_steps = (int)(result);
  Serial.println(scale_steps);
  yscale[i]=scale_steps;
  i++;
  //delay(100);
}

//delay(50);  
  

 }
void run_pattern(int i)
{
     int duration=time_duration[speed_setting]; 
             generate_steps(scale_steps, duration);
      bluetooth_send(13);// noting extreme position
      
      
      changeDirection();
        generate_steps(scale_steps, duration);
        bluetooth_send(13);
      changeDirection();
        update_scale_steps();

}
void run_sawtooth()
{
 update_scale_parameter();
  int runned=ystep[0];
  int next =ystep[1];

int i=0;
int next_stop=0;
while (next_stop!=-500)
{
  runned=ystep[i];
  next=ystep [i+1];
  next_stop=ystep[i+2];
    int duration=xtime[i+1]-xtime[i]; 
  if(next>runned)
    {
      set_up_direction();
      int step_time=duration/(next-runned);
       

          //   generate_steps(next-runned, step_time);
          generate_steps(yscale[i+1]-yscale[i], step_time);
          
             // bluetooth_send(13);// noting extreme position
    }
  else
      {

        set_down_direction();
          int step_time= duration/(runned-next);
        // generate_steps(runned-next, step_time);
        generate_steps(yscale[i]-yscale[i+1],step_time);
       // bluetooth_send(13);// noting extreme position
      }
  i++;

}

}