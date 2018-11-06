#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "PCA9685.h"


// access point properties
IPAddress local_IP(4,20,4,20);
IPAddress gateway(4,20,4,0);
IPAddress subnet(255,255,255,0);  

// udp messaging
WiFiUDP Udp;
unsigned int localUdpPort = 4200;
char incomingPacket[4];

// PCA
PCA9685 pwmController;

void setup() {
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("espenis");

  Udp.begin(localUdpPort);

  Wire.begin(0, 2);
  Wire.setClock(400000);              // Supported baud rates are 100kHz, 400kHz, and 1000kHz

  pwmController = PCA9685();      // Software resets all PCA9685 devices on Wire line
}

void parseColors(){
  for(uint8_t ledIndex = 0; ledIndex < 4; ledIndex++)
    pwmController.setLedValue(ledIndex, incomingPacket[ledIndex] << 4);
}

void loop() {
  if (Udp.parsePacket()) {
    Udp.read(incomingPacket, 4);
    parseColors();
  }
}