/*#include "bluetooth.h"
#include <BluetoothSerial.h>

#include "BluetoothSerial.h"

// Check if Bluetooth is enabled
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please enable it in your build environment.
#endif

BluetoothSerial SerialBT; // Bluetooth Serial Object
void bluetooth_init(void)
{
    SerialBT.begin(20000000);
}
void bluetooth_send(int a )
{
 if (a >= 0 && a <= 255) {  // Ensure the integer is within byte range
        SerialBT.write((byte)a); // Send it as a byte
    }
}
void bluetooth_recive()
{
    
if (SerialBT.available()) {
  
    char c = SerialBT.read();
    Serial.println(c);
    delay(50); // Small delay to give time for buffer handling
  }
}
*/


#include<Arduino.h>
bool status;
#define pulse_pin_enable 2
#define pulse_pin 4
void bluetooth_init(void)
{
pinMode(pulse_pin_enable,OUTPUT);
  pinMode(pulse_pin,OUTPUT);
  digitalWrite(pulse_pin,LOW);
}
void bluetooth_send(int a )
{
digitalWrite(pulse_pin_enable,LOW);
  Serial.println("enable");
  delay(3);
  for(int i=0;i<a;i++)
  {
    digitalWrite(pulse_pin,HIGH);
    delay(1);
     digitalWrite(pulse_pin,LOW);
    delay(1);
    Serial.println(i);
  }

  digitalWrite(pulse_pin_enable,HIGH); 
}