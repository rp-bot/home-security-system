#include "WiFiS3.h"
// #include <SPI.h>
// #include <MFRC522.h>

// #define SS_PIN 10
// #define RST_PIN 5
// MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
#include <Servo.h>

Servo myservo;
struct Distances {
  int door;
  int mainWindow;
  int bedroomWindow;
};

const int trigPinDistance1 = 0;  // Pin connected to the trigger pin of the sensor
const int echoPinDistance1 = 1;  // Pin connected to the echo pin of the sensor
const int trigPinDistance2 = 6;
const int echoPinDistance2 = 7;
const int trigPinDistance3 = 2;
const int echoPinDistance3 = 4;
const int buzzerPin = 5;  // Pin connected to the buzzer
const int pirPin = 8;     // Pin connected to the PIR motion sensor

float duration1, duration2, duration3;
const int baseDoorDistance = 12;  // Base distance set to 5 cm
const int baseMainWindowDistance = 15;
const int baseBedroomWindowDistance = 13;
const int buzzerFrequency = 1000;  // Frequency of the buzzer tone
int pirState = LOW;                // Value for the current state of PIR sensor (Motion or no motion)
int pirVal = 0;                    // Value to hold the last output V of PIR sensor

char ssid[] = "TIE_fighter00";
char pass[] = "wane ono usayhobur antr";
int keyIndex = 0;

int led = LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

bool previousDoorState = false;  // False means door closed, true means door opened

bool auth = false;
int pos = 0;
void setup() {
  // SPI.begin();         // Initiate  SPI bus
  // mfrc522.PCD_Init();  // Initiate MFRC522

  pinMode(trigPinDistance1, OUTPUT);
  pinMode(echoPinDistance1, INPUT);

  pinMode(trigPinDistance2, OUTPUT);
  pinMode(echoPinDistance2, INPUT);

  pinMode(trigPinDistance3, OUTPUT);
  pinMode(echoPinDistance3, INPUT);

  pinMode(buzzerPin, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
  server.begin();
  printWifiStatus();
}

void loop() {
  // Look for new cards
  // if ( ! mfrc522.PICC_IsNewCardPresent())
  // {
  //   return;
  // }
  // Select one of the cards
  // if ( ! mfrc522.PICC_ReadCardSerial())
  // {
  //   return;
  // }
  //   Serial.print("UID tag :");
  //   String content = "";
  //   byte letter;

  //   for (byte i = 0; i < mfrc522.uid.size; i++) {
  //     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
  //     Serial.print(mfrc522.uid.uidByte[i], HEX);
  //     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
  //     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  //   }

  //   Serial.println();
  //   Serial.print("Message : ");
  //   content.toUpperCase();
  //   if (content.substring(1) == "B4 3E 73 B9" ) //change here the UID of the card/cards that you want to give access
  //   {
  //     Serial.println("Authorized access");
  //     Serial.println();
  //     // delay(3000);
  //     auth=true;
  //   }

  //  else   {
  //     Serial.println(" Access denied");
  //     // delay(3000);
  //   }
  WiFiClient client = server.available();
  Distances distances = measureDistance();
  // delayMicroseconds(100000);
  bool doorState = (distances.door != baseDoorDistance);
  bool mainWindowState = (distances.mainWindow != baseMainWindowDistance);
  bool bedroomWindowState = (distances.bedroomWindow != baseBedroomWindowDistance);

  bool anyOpen = doorState || mainWindowState || bedroomWindowState;  //
  // Serial.println(distances.door);
  if (anyOpen) {
    analogWrite(buzzerPin, 250);  // Turn buzzer on if any window or door is open
  } else {
    analogWrite(buzzerPin, 0);  // Turn buzzer off only if all are closed
  }
  // checkMotion();
  if (client) {
    WiFiClient client = server.available();
    if (client) {
      // Serial.println("new client");
      String currentLine = "";
      // an HTTP request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          // Serial.write(c);
          // if you've gotten to the end of the line (received a newline
          // character) and the line is blank, the HTTP request has ended,
          // so you can send a reply
          if (c == '\n' && currentLineIsBlank) {
            // send a standard HTTP response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println("Refresh: 2");         // refresh the page automatically every 5 sec
            // Serial.println("Page refreshed");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            // output the value of each analog input pin
            // for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            //   int sensorReading = analogRead(analogChannel);
            //   client.print("analog input ");
            //   client.print(analogChannel);
            //   client.print(" is ");
            //   client.print(sensorReading);
            //   client.println("<br />");
            // }
            // if (!auth){
            //   client.println("<h1>SYSTEM LOCKED!!</h1>");
            // }else{
            //   client.println("<h1>SYSTEM UNLOCKED!!</h1>");
            // }

            // Serial.print("door ");
            // Serial.println(distances.door);
            // Serial.print("main window ");
            // Serial.println(distances.mainWindow);
            // Serial.print("bedroom window ");
            // Serial.println(distances.bedroomWindow);
            if (doorState) {

              client.println("<p style=\"font-size:40px; color: red;\">Door is open!</p><br />");
            } else {
              client.println("<p style=\"font-size:40px; color: green;\">Door is closed.</p><br />");
            }
            if (mainWindowState) {
              client.println("<p style=\"font-size:40px; color: red;\">Main window is open!</p><br />");
            } else {
              client.println("<p style=\"font-size:40px; color: green;\">Main window is closed.</p><br />");
            }
            if (bedroomWindowState) {
              client.println("<p style=\"font-size:40px; color: red;\">Bedroom window is open!</p><br />");
            } else {
              client.println("<p style=\"font-size:40px; color: green;\">Bedroom window is closed.</p><br />");
            }
            client.println("<br/>");
            checkMotion();
            if (pirVal == 1) {
              client.print("<p style=\"font-size:40px; color: red;\">MOTION DETECTED!!!. </p><br />");
            } else {
              client.print("<p style=\"font-size:40px; color: green;\">NO MOTION. </p><br />");
            }
            // client.print("<p style=\"font-size:7vw;\">Click <a href=\"/O\">here</a> to toggle lock<br></p>");
            client.print("<br />");
            client.print("<button style=\"font-size:40px;\"><a href=\"/H\">PANIC!!</a></button><br/>");
            // client.print("<button style=\"font-size:40px;\"><a href=\"/S\">STOP</a></button><br/>");
            client.print("<p style=\"font-size:40px;\">Click <a href=\"http://192.168.1.150/\" target=\"_blank\" rel=\"noopener noreferrer\" >Living Room Cam</a><br></p>");
            client.print("<p style=\"font-size:40px;\">Click <a href=\"http://192.168.1.152/\" target=\"_blank\" rel=\"noopener noreferrer\" >Bedroom Cam</a><br></p>");
            //
            client.println("</html>");
            break;
          }
          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
            currentLine = "";
          } else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
            currentLine += c;  // add it to the end of the currentLine
          }
          // Check to see if the client request was "GET /H" or "GET /L":
          if (currentLine.endsWith("GET /H")) {
            warnAboutCops();
          }
         
          // if (currentLine.endsWith("GET /L")) {
          //   digitalWrite(LED_BUILTIN, LOW);  // GET /L turns the LED off
          // }
          // if (currentLine.endsWith("GET /O")){
          //   auth = !auth;
          // }
        }
      }
      // give the web browser time to receive the data
      delay(1);

      // close the connection:
      client.stop();
      // Serial.println("client disconnected");
    }
  }
}


Distances measureDistance() {
  Distances distances;

  // Measure distance for the door
  digitalWrite(trigPinDistance1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDistance1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDistance1, LOW);
  float duration1 = pulseIn(echoPinDistance1, HIGH);
  distances.door = round((duration1 * 0.0343) / 2);

  // Measure distance for the main window
  digitalWrite(trigPinDistance2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDistance2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDistance2, LOW);
  float duration2 = pulseIn(echoPinDistance2, HIGH);
  distances.mainWindow = round((duration2 * 0.0343) / 2);

  // Measure distance for the bedroom window
  digitalWrite(trigPinDistance3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinDistance3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinDistance3, LOW);
  float duration3 = pulseIn(echoPinDistance3, HIGH);
  distances.bedroomWindow = round((duration3 * 0.0343) / 2);

  return distances;
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void warnAboutCops() {
  myservo.attach(11);
  for (pos = 0; pos <= 100; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);           // waits 15 ms for the servo to reach the position
  }
  for (pos = 100; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15 ms for the servo to reach the position
  }
}

void checkMotion() {
  pirVal = digitalRead(pirPin);
  // Serial.println(pirVal);

  if (pirVal == HIGH) {
    if (pirState == LOW) {
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH) {
      pirState = LOW;
    }
  }
}
