#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <U8g2lib.h>


const char* ssid = "";
const char* password = "";
const int led = 13;
String webString="";
String draw="";
int x = 0;
int y = 8;
char charBuf[64];

ESP8266WebServer server(80);
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);

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
  u8g2.begin();
  u8g2.setFont(u8g2_font_profont10_tf); // set the target font to calculate the pixel width
  u8g2.setFontMode(0);    // enable transparent mode, which is faster
  u8g2.clear();
  
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.begin(9600);     // UART Serial for comm w/ PC

  u8g2.drawStr(0,8,"Connecting");
  u8g2.sendBuffer();
  x = u8g2.getUTF8Width("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    x += u8g2.getUTF8Width(".");
    u8g2.drawStr(x,8,".");
    u8g2.sendBuffer();
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  u8g2.clear();
 
  u8g2.drawStr(0,8,"Connected to :");

  draw = String(ssid);
  draw.toCharArray(charBuf,draw.length()+1);
  u8g2.drawStr(0,16,charBuf);
  u8g2.drawStr(0,26,"IP : ");
  draw = WiFi.localIP().toString();
  draw.toCharArray(charBuf,draw.length()+1);
  x = u8g2.getUTF8Width("IP : ");  
  u8g2.drawStr(x,26,charBuf);
  u8g2.sendBuffer();
  
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
