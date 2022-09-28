
//include all the libraries
#include <LedControl.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <RtcDS1307.h>
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
int varMin = 0;
int varHr = 0;
int varSec = 0;
int vardd = 0;
int varmm = 0;
int varyy = 0;
int hours = 0;
int Minutes = 0;
int time_min = 0;
int time_hours = 0;

int alarmMin;
int alarmHr;
int alarmHr_dig1;
int alarmHr_dig2;
int alarmMin_dig1;
int alarmMin_dig2;
  
//declare functions
const RtcDateTime& checkRtc();
void checkAlarm();
void checkButtons(const RtcDateTime& dt);
void declareTime(const RtcDateTime& dt);
void setAlarm();
void printDateTime(const RtcDateTime& dt);
void printAlarmTime();

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

RtcDateTime now;

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
    alarmState == digitalRead(alarmButton);

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
RtcDateTime now = checkRtc();
Serial.print("test rtc");
 //checkAlarm();
  checkButtons(now);
}

const RtcDateTime& checkRtc()
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
  return now;
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
void checkButtons(const RtcDateTime& dt){
  int clicks = 0;
  int clickState = 1;
  
  //determine amount of set button clicks within time frame
  if (setState == LOW){
    int buttonRef = millis();
    while(millis()-buttonRef<2000){
      if (setState == LOW && clickState == HIGH){
         clicks = !clicks;
         clickState = LOW;
      }
      else if (setState == HIGH && clickState == LOW){
        clickState = HIGH;
      }
  }
  //set time if clicks == 0
  if (clicks == 0){
    declareTime(now);
  }
  //set alarm if clicks == 1
  else if (clicks == 1){
    setAlarm();
  }
}
}
void declareTime(const RtcDateTime& dt){
  int lastState = 1;
  int up_state = 1; 
  int down_state = 1;
  
  while (setState == HIGH){
  //toggle timeSelect to change hrs/minutes
    int timeSelect;
    alarmState = digitalRead(alarmButton);
    if (alarmState == LOW && lastState == HIGH){
      timeSelect = !timeSelect;
      lastState = LOW;
    }
    else if (alarmState == HIGH && lastState == LOW){ 
      lastState = HIGH;                          
    }
    //blink 7-seg display 
    //timeSelect == 1, so hours are changing
    if (timeSelect == 1){
     while ((upState == HIGH) && (downState == HIGH) && (setState == HIGH)){
        lc.clearDisplay(0);
        delay(100);
  
      //print time--double check
      printDateTime(now);
      
      delay(200);
     }
     if (setState == LOW){
      return;
     }
     //decrease hour by one if down button is pressed
     if (downState == LOW && down_state == HIGH){
        Serial.println("hour down button pressed");
        down_state = LOW;
        varHr = dt.Hour();
        varMin = dt.Minute();
        varSec = dt.Second();
        vardd = dt.Day();
        varmm = dt.Month();
        varyy = dt.Year();
        varHr--;   //subtract one hour from the current time
     }
     else if (downState == HIGH && down_state == LOW){
      down_state = HIGH;
     }
     //increase hour by one if up button is pressed
     if (upState == LOW && up_state == HIGH){
        Serial.println("hour up button pressed");
        up_state = LOW;
        varHr = dt.Hour();
        varMin = dt.Minute();
        varSec = dt.Second();
        vardd = dt.Day();
        varmm = dt.Month();
        varyy = dt.Year();
        varHr++;   //add one hour to the current time
      }
     else if (upState == HIGH && up_state == LOW){
      up_state = HIGH;
     }
    }
    //blink lcd 
    //timeSelect == 0, so minutes are changing
    if (timeSelect == 0){
      while((upState == HIGH) && (downState == HIGH) && (setState == HIGH)){
        lcd.clear();
        delay(100);
  
        //print time--double check
        printDateTime(now);
  
        delay(200);
      }
     if (setState == LOW){
      return;
     }
      //decrease minutes by one
     if (downState == LOW && down_state == HIGH){
        Serial.println("minutes down button pressed");
        down_state = LOW;
        varHr = dt.Hour();
        varMin = dt.Minute();
        varSec = dt.Second();
        vardd = dt.Day();
        varmm = dt.Month();
        varyy = dt.Year();
        varMin--;   //subtract one minute from the current time
     }
     else if (downState == HIGH && down_state == LOW){
      down_state = HIGH;
     }
     //increase minutes by 1
     if (upState == LOW && up_state == HIGH){
        Serial.println("minutes up button pressed");
        up_state = LOW;
        varHr = dt.Hour();
        varMin = dt.Minute();
        varSec = dt.Second();
        vardd = dt.Day();
        varmm = dt.Month();
        varyy = dt.Year();
        varMin++;   //add one minute to the current time
      }
     else if (upState == HIGH && up_state == LOW){
      up_state = HIGH;
     }
}
}
      RtcDateTime dateTime(varyy, varmm, vardd, varHr, varMin, varSec);
      Rtc.SetDateTime(dateTime);
}

void setAlarm(){
  int lastState = 1;
  int down_state = 1;
  int up_state = 1;
  
  while (setState == HIGH){
  
  //set preliminary alarm time
  alarmHr = 12;
  alarmMin = 59;
  
  int timeSelect;
  alarmState = digitalRead(alarmButton);
  
  if (alarmState == LOW && lastState == HIGH){
    timeSelect = !timeSelect;
    lastState = LOW;
  }
  else if (alarmState == HIGH && lastState == LOW){ 
    lastState = HIGH;                    
}
  //timeSelect == 1 controls hours
  if (timeSelect == 1){
    while((upState == HIGH) && (downState == HIGH) && (setState == HIGH)){
      lc.clearDisplay(0);
      delay(100);
      printAlarmTime();
    }
     if (setState == LOW){
      return;
     }
    //decrease alarm hrs by 1
    if (downState == LOW && down_state == HIGH){
      down_state = LOW;
      alarmHr--;
      printAlarmTime();
    }
    else if (downState == HIGH && down_state == LOW){
      down_state = HIGH;
    }
    //increase alarm hrs by 1
    if (upState == LOW && up_state == HIGH){
      up_state = LOW;
      alarmHr++;
      printAlarmTime();
    }
    else if (upState == HIGH && up_state == LOW){
      up_state == HIGH;
    }
  }
  //timeSelect == 0 controls minutes
  if (timeSelect == 0){
    while((upState == HIGH) && (downState == HIGH) && (setState == HIGH)){
      //blink lcd time
      lcd.clear();
      delay(100);
      printAlarmTime();
    }
     if (setState == LOW){
      return;
     }
    //decrease alarm mins by 1
    if (downState == LOW && down_state == HIGH){
      down_state = LOW;
      alarmMin --;
      printAlarmTime();
    }
    else if (downState == HIGH && down_state == LOW){
      down_state == HIGH;
    }
    //increase alarm mins by 1
    if (upState == LOW && up_state == HIGH){
      up_state = LOW;
      alarmMin++;
      printAlarmTime();
    }
    else if (upState == HIGH && up_state == LOW){
      up_state = HIGH;
    }
  }
}
}
void printAlarmTime(){
      //break time into digits
      alarmHr_dig1 = (alarmHr/10U)%10;
      alarmHr_dig2 = (alarmHr/1U)%10;
      alarmMin_dig1 = (alarmMin/10U)%10;
      alarmMin_dig2 = (alarmMin/1U)%10;

      //print on 7-seg
      lc.setDigit(0,0,alarmHr_dig1,false);
      lc.setDigit(0,1,alarmHr_dig2,false);

      //print on lcd 
      lcd.setCursor(5,0);
      for (int i = 1; i <= alarmMin_dig1; i++){
        lcd.print(char(255)); 
      }
      lcd.setCursor(4,1); 
      for (int i = 1; i <= alarmMin_dig2; i++){
        lcd.print(char(255)); 
      }
}
#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{ 
    char datestring[20];
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
