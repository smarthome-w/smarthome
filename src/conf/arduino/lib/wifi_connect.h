#ifndef wifi_connect_h
#define wifi_connect_h
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiClient espClient;

void initializeWiFi() {
  Serial.println("Initialize WIFI...");

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.softAPdisconnect(true);
  WiFi.hostname(fName);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  IPAddress ip(IP_O1, IP_O2, IP_O3, WIFIIP);
  IPAddress gateway(IP_G1, IP_G2, IP_G3, IP_G4);
  IPAddress subnet(IP_SN1, IP_SN2, IP_SN3, IP_SN4);
  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}
#endif
