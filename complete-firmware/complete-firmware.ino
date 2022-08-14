//include all the libraries
#include "LedControl.h"
#include <LiquidCrystal.h>
#include <Wire.h>
#include <RtcDS1307.h>
#include <DS1307RTC.h>
RtcDS1307<TwoWire> Rtc(Wire);

//initalize global variables
unsigned min_dig1;
unsigned min_dig2;
unsigned long delaytime = 450;
unsigned long timeReference = 0;
unsigned hour_dig1;
unsigned hour_dig2;
unsigned long buttonTime = 0;
unsigned long timeLimit;

//initalize buttons
const int downButton = 3;
const int upButton = 4;
const int setButton = 5;
const int alarmButton = 12;
int downState = 0;
int upState = 0;
int setState = 0;
int alarmState = 0; 
int buttonPresses = 0;
int buzzer = 2;

//RTC variables
int hours = 0;
int Minutes = 0;
int time_min = 0;
int time_hours = 0;

int alarmMin;
int alarmHr;

// for the 16x2 LCD
#define rs A0 
#define en A1 
#define d4 6 
#define d5 7  
#define d6 8 
#define d7 9

//for the alarm
#define buzzer 2

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LedControl lc=LedControl(11,13,10,1);

void setup() {
    // setup buttons and alarm
    pinMode(downButton, INPUT);
    pinMode(upButton, INPUT);
    pinMode(alarmButton, INPUT);
    pinMode(setButton, INPUT); 
    pinMode(buzzer, OUTPUT);

    //Button States
    downState == digitalRead(downButton);
    upState == digitalRead(upButton);
    setState == digitalRead(setButton);
    alarmState == digitalRead(setButton);

    //setup lcd
    lcd.begin(16, 2);
    
    //setup 7-seg
    lc.shutdown(0,false);
    lc.setIntensity(0,8);
    lc.clearDisplay(0);

    //setup rtc
    Serial.begin(57600);

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin(); 
    delay(100);
        
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        if (Rtc.LastError() != 0)
        {
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        }
        else
        {
            Serial.println("RTC lost confidence in the DateTime!");
            Rtc.SetDateTime(compiled);
        }
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    //clear RTC 
    Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);   
    timeReference = millis();
}

void loop() {
checkRtc();
checkAlarm();
checkButtons();
}

void checkRtc()
{
  if (millis()-timeReference>=2000){
    if (!Rtc.IsDateTimeValid()) 
  {
      if (Rtc.LastError() != 0)
      {
          Serial.print("RTC communications error = ");
          Serial.println(Rtc.LastError());
      }
      else
      {
          Serial.println("RTC lost confidence in the DateTime!");
      }
  }

  RtcDateTime now = Rtc.GetDateTime();

  printDateTime(now);
  Serial.println();

  delay(100);  
}
}
//check if the alarm should be turned on
void checkAlarm()
{
  if ((alarmHr == hours) && (alarmMin == Minutes)){
    unsigned long alarmTime = millis();
    //run alarm
    while (millis()-alarmTime < 60000){
          int alarmStatus = 1;
//potential issue 255 times might be less than one minute
    if ((alarmStatus == 1) && (millis()-alarmTime<6000)){
      for(int i=0; i<255; i++) { //do this 255 times
        analogWrite(buzzer, i); //raise the voltage sent out of the pin by 1
        delay(5000); //wait 5 seconds to repeat 
        }
      for(int i=255; i>0; i--) { // do this 255 times
        analogWrite(buzzer, i); //lower the voltage sent out of the pin by 1
        delay(5000); //wait 5 seconds to repeat
        }  
    }
    else if ((alarmStatus == 1) && (alarmState == LOW)){
      noTone(buzzer);
      alarmStatus = 0;
    }
    else if ((alarmStatus==1)&&(millis()-alarmTime>=6000)){
      noTone(buzzer);
      alarmStatus = 0;
    }
  }
}
}
void checkButtons(){
  int clicks = 0;
  if (setState == LOW){
    int buttonRef = millis();
    while(millis()-buttonRef<2000){
      if (setState == LOW){
         clicks = 1;
      }
      else if (setState == HIGH){
        clicks = 0;
      }
    }
  }
  if (clicks == 0){
    declareTime();
  }
  else if (clicks == 1){
    setAlarm();
  }
}

void declareTime(){
  int lastState = alarmState;
  int timeSelect;
  alarmState = digitalRead(alarmButton);
  if (alarmState == LOW && lastState == HIGH){
    timeSelect = 1;
  }
  else {
    timeSelect = 0;                           
  }
  //blink 7-seg display 
  if (timeSelect == 1){
   while ((upState == HIGH) && (downState == HIGH)){
      LedControl.clearDisplay(0);
      delay(100);

    //print 7-seg time
        snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            varmm = dt.Month(),
            vardd = dt.Day(),
            varyy = dt.Year(),
            varHr = dt.Hour(),
            varMin = dt.Minute(),
            varSec = dt.Second() );
    hours = dt.Hour();
    hour_dig1 = (hours/10U)%10;
    hour_dig2 = (hours/1U)%10;
    delay(200);
   }
  }
  if (timeSelect == 0){
    
  }
}
void setAlarm(){

}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{ 
    char datestring[20];
    String meridiem = " am"; 
    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);

    lcd.clear();         
    Minutes = dt.Minute(); 
    min_dig1 = (Minutes/10U)%10;
    min_dig2 = (Minutes/1U)%10;
 
    lcd.setCursor(5,0);
    for (int i = 1; i <= min_dig1; i++){
      lcd.print(char(255)); 
    }
    lcd.setCursor(4,1); 
        for (int i = 1; i <= min_dig2; i++){
      lcd.print(char(255)); 
    } 
    
    //print hours on 7-seg 
    hours = dt.Hour();
    hour_dig1 = (hours/10U)%10;
    hour_dig2 = (hours/1U)%10;

    lc.setDigit(0,0,hour_dig1,false);
    lc.setDigit(0,1,hour_dig2,false);
    
    delay(1000);
}
