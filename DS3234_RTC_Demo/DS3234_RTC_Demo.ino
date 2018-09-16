/******************************************************************************
DS3234_RTC_Demo.ino
Jim Lindblom @ SparkFun Electronics
original creation date: October 2, 2016
https://github.com/sparkfun/SparkFun_DS3234_RTC_Arduino_Library

Configures, sets, and reads from the DS3234 real-time clock (RTC).

Resources:
SPI.h - Arduino SPI Library

Development environment specifics:
Arduino 1.6.8
SparkFun RedBoard
SparkFun Real Time Clock Module (v14)
*****************************************************************************/
#include <SPI.h>
#include <SparkFunDS3234RTC.h>


//ard pin 10 - 1
//ard pin 12 - 19
//ard pin 13 - 18
//ard pin 11 - 17
//ard pin


//////////////////////////////////
// Configurable Pin Definitions //
//////////////////////////////////
#define DS13074_CS_PIN 10 // DeadOn RTC Chip-select pin

void setup() 
{
  // Use the serial monitor to view time/date output
  Serial.begin(9600);
  rtc.begin(DS13074_CS_PIN);
  
  delay(50);
  
  rtc.update();
  delay(50);
  rtc.autoTime();
  //rtc.setTime(5, 10, 1, 2, 1, 05, 18);
  delay(50);
  rtc.update();
}

void loop() 
{
  static int8_t lastSecond = -1;
  
  // Call rtc.update() to update all rtc.seconds(), rtc.minutes(),
  // etc. return functions.
  rtc.update();

  delay(50);

  if (rtc.second() != lastSecond) // If the second has changed
  {
    printTime(); // Print the new time
    
    lastSecond = rtc.second(); // Update lastSecond value
  } 

  delay(50);
}

void printTime()
{
  Serial.print(String(rtc.hour()) + ":"); // Print hour
  if (rtc.minute() < 10)
    Serial.print('0'); // Print leading '0' for minute
  Serial.print(String(rtc.minute()) + ":"); // Print minute
  if (rtc.second() < 10)
    Serial.print('0'); // Print leading '0' for second
  Serial.print(String(rtc.second())); // Print second

  if (rtc.is12Hour()) // If we're in 12-hour mode
  {
    // Use rtc.pm() to read the AM/PM state of the hour
    if (rtc.pm()) Serial.print(" PM"); // Returns true if PM
    else Serial.print(" AM");
  }
  
  Serial.print(" | ");

  // Few options for printing the day, pick one:
  Serial.print(rtc.dayStr()); // Print day string
  Serial.print(" - ");
  Serial.print(String(rtc.month()) + "/" +   // Print month
                 String(rtc.date()) + "/");  // Print date
  Serial.println(String(rtc.year()));        // Print year
}
