#include "bluetooth.h"
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