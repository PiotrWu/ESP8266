#include <ESP8266WiFi.h>
#include <Wire.h>
 
const int sampleWindow = 50;                              // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
 
#define SENSOR_PIN A0
#define PIN_QUIET D3
#define PIN_MODERATE D4
#define PIN_LOUD D5
 

void setup ()  
{   
  pinMode (SENSOR_PIN, INPUT); // Set the signal pin as input  
  pinMode(PIN_QUIET, OUTPUT);
  pinMode(PIN_MODERATE, OUTPUT);
  pinMode(PIN_LOUD, OUTPUT); 
 
  digitalWrite(PIN_QUIET, LOW);
  digitalWrite(PIN_MODERATE, LOW);
  digitalWrite(PIN_LOUD, LOW);
  
  Serial.begin(115200);
}  
 
   
void loop ()  
{ 
   unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(SENSOR_PIN);                    //get reading from microphone
      if (sample < 1024)                                  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // save just the min levels
         }
      }
   }
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   int db = map(peakToPeak,20,900,49.5,90);             //calibrate for deciBels
   Serial.print(db);
   Serial.println("Db");
  if (db <= 60)
  {
    Serial.println("Level: Quiet");
    digitalWrite(PIN_QUIET, HIGH);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, LOW);
  }
  else if (db > 60 && db<85)
  {
    Serial.println("Level: Moderate");
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, HIGH);
    digitalWrite(PIN_LOUD, LOW);
  }
  else if (db>=85)
  {
    Serial.println("Level: High");
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, HIGH);
 
  }
  delay(2000);      // thingspeak needs minimum 15 sec delay between updates.
}