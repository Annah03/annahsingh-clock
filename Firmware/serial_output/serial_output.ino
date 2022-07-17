

//initalize button
int pushButton = 2;

//setup routine with reset
void setup() {
  //9600 bits per second
  Serial.begin(9600);
  //pushButton is an input
  pinMode(pushButton, INPUT);
}

//button loop read and print button state
void loop() {
  int buttonState = digitalRead(pushButton);
  Serial.println(buttonState);
  delay(1000);
}
