#ifndef ntp_h
#define ntp_h
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
/*
const long utcOffsetInSeconds = 3600;

boolean isNTPInitialized = false;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "193.25.222.240", utcOffsetInSeconds);

void initializeNTP() {
  Serial.println("Initialize NTP...");
  if (!isNTPInitialized) {
    timeClient.begin();
    isNTPInitialized = true;
  }
}

void processNTP() {
  if (!isNTPInitialized) {
    initializeNTP();
  }
  timeClient.update();
}
*/
#endif