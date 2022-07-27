
int buzzer=2;
void setup() {
   Serial.begin(9600);
   pinMode(A1,OUTPUT);
}

void loop() {
   delay(360000);
   tone(buzzer,200,6000);
   Serial.println("Alarm on");
   delay(1000);
   noTone(buzzer);
   Serial.println("Alarm off");
}
