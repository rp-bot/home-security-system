#include "WiFiS3.h"

// #include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "TIE_fighter00";        // your network SSID (name)
char pass[] = "";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);      // initialize serial communication
  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    // delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    String requestData = "";                // String to hold the data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out to the serial monitor
        requestData += c;                   // append the read character to requestData
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            if (requestData.indexOf("GET /?inputText=") >= 0) {
              int start = requestData.indexOf('=') + 1;
              int end = requestData.indexOf(' ', start);
              String inputText = requestData.substring(start, end);
              inputText.replace("+", " ");
              Serial.print("Input text: ");
              Serial.println(inputText);
            }

            // Send a standard HTTP response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<!DOCTYPE html><html>");
            client.println("<head><title>Arduino Input Form</title></head>");
            client.println("<body>");
            client.println("<h1>Enter text and submit</h1>");
            client.println("<form action=\"/\" method=\"GET\">");
            client.println("Text: <input type=\"text\" name=\"inputText\">");
            client.println("<input type=\"submit\" value=\"Submit\">");
            client.println("</form>");
            client.println("</body></html>");

            client.println();
            break; // Exit the loop
          }
          currentLine = "";
        } else if (c != '\r') {
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
