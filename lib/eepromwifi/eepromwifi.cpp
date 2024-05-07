#include<eepromwifi.h>
#include<EEPROM.h>
uint8_t pattern_y[10][100];
void eeprom_write(int address,byte number)
{
     EEPROM.write(address,number);
  if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed");
    } else {
      Serial.println("ERROR! EEPROM commit failed");
    }
}






#include <vector>
std::vector<int> parseCommaSeparatedStringToInts(const String &input)
{
    std::vector<int> result;
    String remaining = input;
    int delimiterIndex;

    while ((delimiterIndex = remaining.indexOf(',')) != -1)
    {
        String token = remaining.substring(0, delimiterIndex);
        remaining = remaining.substring(delimiterIndex + 1);

        try
        {
            int value = token.toInt();
            result.push_back(value);
        }
        catch (...)
        {
            // Handle parsing errors, if necessary
        }
    }

    if (!remaining.isEmpty())
    {
        try
        {
            int value = remaining.toInt();
            result.push_back(value);
        }
        catch (...)
        {
            // Handle parsing errors, if necessary
        }
    }

    return result;
}
#include <WiFi.h>
#include <WebServer.h>
const char *apSSID = "ESP32_AP";        // SSID of the Access Point
const char *apPassword = "password123"; // Password for the Access Point
WebServer server(80);
#define NUM_PATTERNS 10
#define DATA_SIZE 500
int temp_data[DATA_SIZE];
int patternIndex = -1;
int currentPattern = -1;
int currentDataIndex = 0;
void handleRoot()
{
    String html = "<html><body>";

    html += "<h2>Select a pattern:</h2>";
    for (int i = 0; i < NUM_PATTERNS; i++)
    {
        html += "<a href='/pattern?index=" + String(i) + "'>Pattern " + String(i + 1) + "</a><br>";
    }

    html += "</body></html>";
    server.send(200, "text/html", html);
}
void handlePattern()

{
    patternIndex = server.arg("index").toInt();
    server.sendHeader("Location", "/datapoints", true);
    server.send(302, "text/plain", "Selected Pattern " + String(patternIndex + 1) + ". Redirecting to input data points.");
}
void handleDataPoints()
{
    if (patternIndex == -1)
    {
        server.send(200, "text/plain", "Please select a pattern first.");
        return;
    }

    currentPattern = patternIndex;
    currentDataIndex = 0;

    String html = "<html><body>";
    html += "<h2>Input Data Points for Pattern " + String(currentPattern + 1) + "</h2>";
    html += "<form method='POST' action='/data'>";
    html += "<input type='text' name='data'>";
    html += "<input type='submit' value='Send Data'>";
    html += "</form>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}
#include<Wire.h>
#define EEPROM_ADDRESS 0x50
int readDataFromEEPROM(int pattern, int dataIndex)
{
    int startAddress = pattern *128;
    int address = startAddress + (dataIndex * sizeof(int));

    Wire.beginTransmission(EEPROM_ADDRESS); // Start communication with EEPROM
    Wire.write((byte)(address >> 8));       // MSB of the address
    Wire.write((byte)(address & 0xFF));     // LSB of the address
    Wire.endTransmission();

    Wire.requestFrom(EEPROM_ADDRESS, sizeof(int));

    int highByte = 0;
    int lowByte = 0;

    if (Wire.available() >= sizeof(int))
    {
        highByte = Wire.read(); // Read the high byte
        lowByte = Wire.read();  // Read the low byte
    }

    int value = (highByte << 8) | lowByte; // Combine high and low bytes
    return value;
}
void storePatternInEEPROM(int data[], int number,int pattern_number)
{
int startAddress; // Declare startAddress outside of the switch statement

switch (pattern_number)
{
    case 0:
        startAddress =0;
        break;
    case 1:
        startAddress = 100;
        break;
    case 2:
        startAddress = 200;
        break;
    case 3:
        startAddress = 300;
        break;
    case 4:
        startAddress = 400;
        break;
    case 5:
        startAddress = 500;
        break;
    case 6:
        startAddress = 600;
        break;
    case 7:
        startAddress = 700;
        break;
    case 8:
        startAddress = 800;
        break;
    case 9:
        startAddress = 900;
        break;
    // Add a default case if needed
    default:
        // Handle an invalid pattern_number here
        break;
}

// Now you can use the startAddress variable outside of the switch statement

    Serial.print("pattern number is ");
    Serial.println(pattern_number);
    Serial.print(" starting addres");
    Serial.println(startAddress);
    int l=0;
    for (int i = startAddress; i < startAddress+32;i++)
        {
        eeprom_write(i,data[l++]);
        
        }
 for (int  i = startAddress; i <32+startAddress; i++)
    {
        Serial.print(EEPROM.read(i));
        Serial.print(',');
    }

   /* Wire.beginTransmission(EEPROM_ADDRESS);
    Wire.write((byte)(startAddress >> 8));   // MSB
    Wire.write((byte)(startAddress & 0xFF)); // LSB
  

    for (int i = 0; i < 32;i++)
    {
        Wire.write(data[i] >> 8);   // Send the high byte
        Wire.write(data[i] & 0xFF); // Send the low byte
        Wire.write(2000 >> 8);
        Wire.write(2000 & 0xFF);
       
    }
    Wire.endTransmission();*/




    delay(100); // Wait for write operation to complete

    
  /*  for (uint16_t i = 0; i <32; i++)
    {
        Serial.print(readDataFromEEPROM(number, i));
        Serial.print(',');
    }*/
}
void handleData()
{
    String data = server.arg("data");
    data.concat("-1000,-1000,-1000");
    Serial.println(data);

    if (currentPattern == -1)
    {
        server.send(200, "text/plain", "Please select a pattern first.");
        return;
    }

    Serial.println();
    Serial.println(data);

    // Parse the received comma-separated string into individual data points
    std::vector<int> dataPoints = parseCommaSeparatedStringToInts(data);

   if (!dataPoints.empty())
    {

        
        int i=0;
        int page=0;
 while (i < dataPoints.size() && currentDataIndex+1 < DATA_SIZE)
  {

       int k=0;
        for (int j = i; j <i+ 32; j++)
        {
            temp_data[k++] = dataPoints[j];    
        }
        
storePatternInEEPROM(temp_data,page,currentPattern);
        page++;
        i=i+32;
 }
          
    


        currentPattern = -1; // Reset current pattern
        server.send(200, "text/plain", "Pattern " + String(currentPattern + 1) + " received and stored.");
    }
    else
    {
        server.send(200, "text/plain", "No data received.");
    }
}

void read_full_pattern(int patter_number, int start_address)
{
    Serial.print("pattern number is");
    Serial.println(patter_number);
    int t;
    for(int i=start_address;i<start_address+100;i++) 
    {
        pattern_y[patter_number][t++]=EEPROM.read(i);
    } 
    
    for (t=0;t<100;t++)
    {
        Serial.print(pattern_y[patter_number][t]);
        Serial.println(',');
    }
   /*int pattern_second=0;
    bool end_condition=0;  
    long a;  
    for(int page=page_number;page<100+page_number;page++)
   { 

    
    for(int i=0;i<32;i++)
    {
     if(end_condition)
      break;

      a=readDataFromEEPROM(page, i);
      
      if(a>3300)
      a=a-65536;
      if(a==-1000)
      {
        end_condition=1;
        
      }
      
       
      pattern[number][pattern_second++]=a;
      pattern_stop[number][0]=pattern_second;
    }
   }
Serial.println("pattern");
Serial.println(number);
 pattern_second=0;  
  //for (uint32_t i=0;i<20*32;i++)
  for (uint32_t i=0;i<pattern_stop[number][0];i++)
  {
    Serial.print(pattern[number][pattern_second++]);
    Serial.print(',');
  }*/
}
void read_all_full_pattern()
{
for (int i=0;i<3;i++)
{
read_full_pattern(i,i*100);
}
}
void handle_server(void)
{ server.handleClient();
}
 void wifi_eeprom_initialize()
  {
    EEPROM.begin(4096);
   WiFi.softAP(apSSID, apPassword);
    IPAddress apIP(192, 168, 4, 1);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));


    server.on("/", HTTP_GET, handleRoot);
    server.on("/pattern", HTTP_GET, handlePattern);
    server.on("/datapoints", HTTP_GET, handleDataPoints);
    server.on("/data", HTTP_POST, handleData);
    server.begin();
     Serial.println("Access Point IP address: " + WiFi.softAPIP().toString());
 //  Wire.begin(); // Initialize I2C communication
 read_all_full_pattern();
   }


