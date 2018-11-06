#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// access point properties
IPAddress local_IP(4,20,4,20);
IPAddress gateway(4,20,4,0);
IPAddress subnet(255,255,255,0);  

// udp messaging
WiFiUDP Udp;
unsigned int localUdpPort = 4200;
char incomingPacket[255];
char replyPacket[] = "Hi there! Got the message :-)";

void setup() {
  Serial.begin(9600);
  Serial.print("HI!");

  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("espenis");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  Udp.begin(localUdpPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print(Udp.read(incomingPacket, 255));

    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();
  }
}