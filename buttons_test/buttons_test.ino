//initalize buttons
const int downButton = 3;
const int upButton = 4;
const int setButton = 5;
const int alarmButton = 12;
int downState = 0;
int upState = 0;
int setState = 0;
int alarmState = 0;

void setup() {
  Serial.begin(57600);
  //setup buttons
  pinMode(downButton, INPUT);
  pinMode(upButton, INPUT);
  pinMode(alarmButton, INPUT);
  pinMode(setButton, INPUT);   
}

void loop() {
  //read the states of each button
  downState == digitalRead(downButton);
  upState == digitalRead(upButton);
  alarmState == digitalRead(alarmButton);
  setState == digitalRead(setButton);

  
  if (downState == LOW){
    Serial.println("down button pressed");
  }
  if (upState == LOW){
    Serial.println("up button pressed");
  }
  if (alarmState == LOW){
    Serial.println("alarm button pressed");
  }
  if (setState == LOW){
    Serial.println("set button pressed");
  }
}
