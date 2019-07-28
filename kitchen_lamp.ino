#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <credentials.hpp>
#include <ArduinoOTA.h>
#include <NeoPixelBrightnessBus.h>

WiFiUDP udp;
uint16_t localPort = 23;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char replyBuffer[] = "acknowledged\r\n";       // a string to send back

const uint16_t PixelCount = 66; // this example assumes 3 pixels, making it smaller will cause a failure
NeoPixelBrightnessBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount, 0);
RgbColor currentColor(0,0,0);

void updateLeds();

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial attach

  strip.Begin();
  strip.Show();

  wifi_station_set_hostname("kitchenLamp");
    
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  udp.begin(localPort);

  currentColor = RgbColor(0,255,0);
  updateLeds();
}

void updateLeds() {
  for (int i = 0; i < PixelCount; ++i) {
    strip.SetPixelColor(i, currentColor);
  }
  strip.Show();
}

void parsePacket(const String& msg) { 
  if (msg == "/LAMPS_GATHER_AND_ANSWER") {
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write(replyBuffer);
    udp.endPacket();
  } else {
    String r = msg.substring(0,3);
    String g = msg.substring(3,6);
    String b = msg.substring(6,9);
  
    currentColor = RgbColor(g.toInt(), r.toInt(), b.toInt());
  }
}

void loop() {
  ArduinoOTA.handle();

  int packetSize = udp.parsePacket();
  if (packetSize) {
    int n = udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    parsePacket(packetBuffer);
  }
  
  updateLeds();
}
