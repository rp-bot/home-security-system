//Set the pin numbers for the motion sensor
const int trigPin = 9;
const int echoPin = 10;
//Declare the necessary variables for implementing the distance sensor 
float duration, distance;

void setup() {
  // put your setup code here, to run once:
  //Set the pin modes for the distance sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop() {
  //Send alternating pulse to the distance sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //Measure the length of time between pulses of the distance sensor
  duration = pulseIn(echoPin, HIGH);
  //Convert the time to distance, .0343 = speed of sound, divison by 2 because the sound travels then bounces back then travels 
  distance = (duration * .0343) / 2;
 
  //Minimal delay for the sensor, 1 second 
  delayMicroseconds(100000);

}
