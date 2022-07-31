

#define buzzer 2 //change to any output pin (not analog inputs) 

void setup() { //run this code once
  pinMode(buzzer, OUTPUT); //tell arduino the buzzer is an output device
}

void loop(){ //loop forever...
  
  for(int i=0; i<255; i++) { //do this 255 times
    analogWrite(buzzer, i); //raise the voltage sent out of the pin by 1
    delay(10); //wait 10 milliseconds to repeat 
  }

  for(int i=255; i>0; i--) { // do this 255 times
    analogWrite(buzzer, i); //lower the voltage sent out of the pin by 1
    delay(10); //wait 10 milliseconds to repeat
    
  }
}
