/*
 * Connections
 * connect SDA of the RTC with A4
 * connect SCL of the RTC with A5
 */
 
#include <LiquidCrystal.h>
#include <Wire.h>
#include <RtcDS1307.h>
RtcDS1307<TwoWire> Rtc(Wire);

 
unsigned min_dig1;
int Minutes;
unsigned min_dig2;

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
{
    Serial.begin(57600);

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    //--------RTC SETUP ------------
    // if you are using ESP-01 then uncomment the line below to reset the pins to
    // the available pins for SDA, SCL
    // Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL

    lcd.begin(16, 2);
    Rtc.Begin();
    delay(3000);
    

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        if (Rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
            // what the number means
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            Serial.println("RTC lost confidence in the DateTime!");
            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

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

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low); 
}

void loop () 
{
    if (!Rtc.IsDateTimeValid()) 
    {
        if (Rtc.LastError() != 0)
        {
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
            Serial.print("RTC communications error = ");
            Serial.println(Rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) the battery on the device is low or even missing and the power line was disconnected
            Serial.println("RTC lost confidence in the DateTime!");
        }
    }

    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
    Serial.println();

    delay(10000); // ten seconds
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{ 
    char datestring[20];
    String meridiem = " am"; 
    int hour = dt.Hour();
    if (hour>12)
              {
                hour = hour-12;
                meridiem = " pm";
              }
    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            hour,
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
    Serial.print(meridiem);

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
    delay(1000);
}
