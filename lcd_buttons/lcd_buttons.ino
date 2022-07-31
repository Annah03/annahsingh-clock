
/*
 * Connections
 * connect SDA of the RTC with A4
 * connect SCL of the RTC with A5
 */
 
#include <LiquidCrystal.h>
#include <Wire.h>
#include <RtcDS1307.h>
#include <DS1307RTC.h>
RtcDS1307<TwoWire> Rtc(Wire);

unsigned min_dig1;
unsigned min_dig2;

//initalize buttons
const int downButton = 3;
const int upButton = 4;
int downState = 0;
int upState = 0;

//RTC
int varMin = 0;
int varHr = 0;
int varSec = 0;
int vardd = 0;
int varmm = 0;
int varyy = 0;
int hours = 0;
int Minutes = 0;

// for the 16x2 LCD
#define rs A0 
#define en A1 
#define d4 6 
#define d5 7  
#define d6 8 
#define d7 9

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup () 
{   //setup buttons
    pinMode(downButton, INPUT);
    pinMode(upButton, INPUT);
    
    //setup lcd
    lcd.begin(16, 2);

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
}

void loop () 
{   
    delay(500);
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

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{ 
    char datestring[20];
    String meridiem = " am"; 
    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            varmm = dt.Month(),
            vardd = dt.Day(),
            varyy = dt.Year(),
            varHr = dt.Hour(),
            varMin = dt.Minute(),
            varSec = dt.Second() );
    Serial.print(datestring);
    Serial.print(meridiem);
    Serial.print(" ");
    
  downState = digitalRead(downButton);
  upState = digitalRead(upButton);
  Serial.println(downState);
  
  if (downState == LOW){
        Serial.println("down button pressed");
        varHr = dt.Hour();
        varMin = dt.Minute();
        varSec = dt.Second();
        vardd = dt.Day();
        varmm = dt.Month();
        varyy = dt.Year();
        varMin--;   //subtract one minute from the current tine
        RtcDateTime dateTime(varyy, varmm, vardd, varHr, varMin, varSec);
        Rtc.SetDateTime(dateTime);
  }
    lcd.clear();         
    Minutes = varMin; 
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
    delay(0);
}
