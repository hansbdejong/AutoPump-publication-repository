#include <SD.h>
#include <SPI.h>
#include <Wire.h>              //for the Real Time Clock
#include <Sodaq_DS3231.h>      //for the Real Time Clock

/*
 * Variables to modify; 
 * month (1-12), day (1-31), hour(0-23), minute (0-59), second (0-59)
 * Enter time in seconds e.g. 900 = 15 minutes
 * Enter filename between quotes for SD card
 */
int startTime[] = {2, 14, 18, 58, 0};
int EventInterval = 30; 
char FileName[] = "PumpLogTest2.txt";

//Do not modify below this line!

int realEventInterval = EventInterval - 1; //Correction for sample interval while loop, experimentally determined

//Date variables
DateTime now;
int RTCmonth;              //Real Time Clock
int RTCday;
int RTChour;
int RTCminute;
int RTCsecond;
int startMonth;            //declared month, day, hour, or minute to sample
int startDay;
int startHour;
int startMinute;
int startSecond;

//index variables
int startTimeIndex = 0;    //pointer to declared month, day, hour, or minute 

//relays
int relayOn = 0;
int relayOff = 1;

//Pumps
int sampleValve[] = {2, 3}; // 2 = Lower Pump, 3 = Upper Pump

//for SD card
File myFile;
int pinCS = 10; // Pin 10 on Arduino Uno

void setup() 
{

  //for SD card
  pinMode(pinCS, OUTPUT);
 
  //begin
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  SD.begin();

  //Write to file log
  myFile = SD.open(FileName, FILE_WRITE);
  myFile.println("******************Parameter Header****************");
  myFile.print("Cycle time for both pumps = ");
  myFile.print(EventInterval);
  myFile.println(" seconds");
  myFile.print("Lower Pump = 1");
  myFile.print(' ');
  myFile.println("Upper Pump = 2");
  myFile.println("**************************************************");
  myFile.close(); // close the file

  //Starts the pump at the programmed time
  
  //Start on the correct month
  startMonth = startTime[startTimeIndex];
  now = rtc.now();
  RTCmonth = now.month();
  while (RTCmonth != startMonth)
  {
    delay(100);
    now = rtc.now();
    RTCmonth = now.month();
  }                        
  startTimeIndex++;

  //Start on the correct day
  startDay = startTime[startTimeIndex];
  now = rtc.now();
  RTCday = now.date();      
  while (RTCday!=startDay)
  {
    delay(100);
    now = rtc.now();
    RTCday = now.date();
  }                
  startTimeIndex++;
  
  //Start on the correct hour
  startHour=startTime[startTimeIndex]; 
  now = rtc.now();    
  RTChour = now.hour(); 
  while (RTChour!=startHour)
  {
    delay(100);
    now = rtc.now();    
    RTChour = now.hour();
  }           
  startTimeIndex++;
  
  //Start on the correct minute
  startMinute=startTime[startTimeIndex]; 
  now = rtc.now();
  RTCminute = now.minute();
  while (RTCminute < startMinute)
  {
    delay(100);        
    now = rtc.now();
    RTCminute = now.minute();
  }
  startTimeIndex ++;
  
  //Start on the correct second
  startSecond=startTime[startTimeIndex]; 
  now = rtc.now();
  RTCsecond = now.minute();
  while (RTCsecond < startSecond)
  {
    delay(100);        
    now = rtc.now();
    RTCsecond = now.second();
  }
} 

void loop()
{ 
  //loop twice - bottom pump then top pump
  for (int i = 0; i < 2; ++i)
  {  
    DateTime sampleStartTime = rtc.now();
    pinMode (sampleValve[i], OUTPUT);
    digitalWrite (sampleValve[i], relayOff);
    now = rtc.now(); //get the current date-time
    
    //Log start of pump to file
    myFile = SD.open(FileName, FILE_WRITE);
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.date(), DEC);
    myFile.print(' ');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(' ');
    myFile.print(i+1);
    myFile.println("_Pump on");
    myFile.close(); 
    
    //Keep pump on for programmed amount of time (realEventInterval)  
    while (rtc.now().get() - sampleStartTime.get() <= realEventInterval) 
    {   
      digitalWrite(sampleValve[i], relayOn); // Turn on pump
    } 
    digitalWrite(sampleValve[i], relayOff); // Turn off pump

    //Log end of pump to file
    now = rtc.now(); //get the current date-time     
    myFile = SD.open(FileName, FILE_WRITE);
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.date(), DEC);
    myFile.print(' ');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(' ');
    myFile.print(i+1);
    myFile.println("_Pump off");
    myFile.close();   
  } 
}

/*
 * GPIO for the RTC and SD CARD reader - these hardwired outputs cannot be modified.
 * 
 * CLOCK required pinouts
 * VCC -> Arduino 5V
 * GND -> Arduino GND
 * SCL -> SCL or A5
 * SDA -> SDA or A4
 * 
 * SD CARD required pinouts
 * VCC -> Arduino 5V
 * GND -> Arduino GND
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 * CS -> 10
 * 
 * Code written by H. DeJong and D. Mucciarone 
 */


