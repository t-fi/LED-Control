#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "PCA9685.h"

IPAddress local_IP(4,20,4,20);
IPAddress gateway(4,20,4,0);
IPAddress subnet(255,255,255,0);  

WiFiUDP Udp;
unsigned int localUdpPort = 4200;
char incomingPacket[4];

PCA9685 pwmController = PCA9685();

void setup() {
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("espenis");

  Udp.begin(localUdpPort);
}

void loop() {
  if (Udp.parsePacket()) {
    Udp.read(incomingPacket, 4);
    for(uint8_t ledIndex = 0; ledIndex < 4; ledIndex++)
      pwmController.setLedValue(ledIndex, incomingPacket[ledIndex] << 4);
  }
}