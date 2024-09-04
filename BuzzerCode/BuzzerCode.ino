int buzzerPin = 11;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  // nothing to setup
}

void loop() {
  analogWrite(buzzerPin, 256); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  analogWrite(buzzerPin, 0);     // Stop sound...
  delay(1000);
}