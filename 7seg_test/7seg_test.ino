//increments 7-seg display

#include "LedControl.h"
LedControl lc=LedControl(11,13,10,1);
unsigned long delaytime=450;

void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
}

void loop() {  
  for(int i=0;i<9999;i++) {
    lc.setDigit(0,0,((int)i/10*10-(int)i/100*100)/10,false);
    lc.setDigit(0,1,i-(int)i/10*10,false);
    delay(delaytime);
    //only two digits are needed 
/*
    lc.setDigit(0,2,((int)i/10*10-(int)i/100*100)/10,false);
    lc.setDigit(0,3,i-(int)i/10*10,false);
    delay(delaytime);
*/
  }
}
