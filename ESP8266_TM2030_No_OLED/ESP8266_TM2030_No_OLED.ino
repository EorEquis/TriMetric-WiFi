#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "";
const char* password = "";
const int led = 13;
String webString="";

ESP8266WebServer server(80);

void handleRoot() {
  digitalWrite(led, 1);
  getTriMetric();
  server.send(200, "text/plain", webString);
  webString = "";
  digitalWrite(led, 0);
}
void getTriMetric() {
  const char introChar = 'T';
  bool introCharRx = false;
  while (!introCharRx) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == introChar) {
        webString += c;
        // Serial.print(c);
        introCharRx = true;
      }
    }
  }
  while (introCharRx) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c != introChar) {
        webString += c;
      }
      else if (c == introChar) {
        introCharRx = false;
      }
    }
  }
}

void setup(){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.begin(9600);     // UART Serial for comm w/ PC

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
  
}   // End Setup

void loop(){

  server.handleClient();

} // End Loop
