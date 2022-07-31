//initalize buttons
const int downButton = 3;
const int upButton = 4;
const int setButton = 5;
const int alarmButton = 12;

void setup() {
  
  Serial.begin(57600);
  Serial.println();

  //setup buttons
  pinMode(downButton, INPUT);
  pinMode(upButton, INPUT);
  pinMode(alarmButton, INPUT);
  pinMode(setButton, INPUT);  
}

void loop() {
//read the states of each button
  int downState = digitalRead(downButton);
  int upState = digitalRead(upButton);
  int alarmState = digitalRead(alarmButton);
  int setState = digitalRead(setButton);

//serial print if button is pressed
  if (downState == LOW){
    Serial.println("down button pressed");
  }
  if (upState == LOW){
    Serial.println("up button pressed");
  }
  */
  if (alarmState == LOW){
    Serial.println("alarm button pressed");
  }
  if (setState == LOW){
    Serial.println("set button pressed");
  }
  
}
