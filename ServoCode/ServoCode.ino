#include <Servo.h>

Servo myservo;  // create a servo object

void setup() {
  myservo.attach(11);  // attaches the servo on pin 11 to the servo object
}

void loop() {
  myservo.write(0); // set the servo to postion 0
  delay(1000); // wait one second
  myservo.write(90); // set teh servor to position 90
  delay(1000); // wait one more second
}
