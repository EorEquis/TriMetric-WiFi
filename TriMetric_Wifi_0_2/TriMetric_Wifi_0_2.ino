#include <SPI.h>
#include <WiFi101.h>
//#include <SoftwareSerial.h>
#include "secrets.h" 

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

String currString = "";
String webString = "T%=N/A,W=N/A,DSC=N/A,DSE=N/A,PW=N/A,r%=N/A,pD=N/A,V=N/A,FV=N/A,V2=N/A,A=N/A,FA=N/A,PW=N/A,AH=N/A,";
const char introChar = 'T';
bool introCharRx = false;

unsigned long previousMillis = 0;  

void setup() {
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,3,4);
  //TriMetric.begin(2400);
  Serial.begin(2400);


  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);       // don't continue
  }

    Serial.print("Attempting to connect to Network named: ");
    Serial.print(ssid);                   // print the network name (SSID);

    status = WiFi.begin(ssid, pass);
    
  // attempt to connect to WiFi network:
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();                           // start the web server on port 80
  printWiFiStatus();                        // you're connected now, so print out the status
    Serial.end
}

void loop() {

  unsigned long currentMillis = millis();
  
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println();
    Serial.print("I have a client ");
    Serial.print("sending htmlString = ");
    Serial.println(webString);
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.print(webString);
    //client.print("I be here in the obs");
    client.println();
    client.stop();
    Serial.println("client disonnected");
  }
        
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.begin(2400);
    while (!introCharRx) {
      if (Serial.available() > 0) {
        char c = Serial.read();
        if (c == introChar) {
          curString = String(c);
          introCharRx = true;
        }
      }
    }
    while (introCharRx) {
      if (Serial.available() > 0) {
        char c = Serial.read();
        if (c != introChar) {
          curString = curString + String(c);
        }
        else if (c == introChar) {
          introCharRx = false;
          webString=curString;
          curString="";
        }
      }
    }
    //Serial.println();
    Serial.end();

  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
