#ifndef heartbeat_h
#define heartbeat_h
#define HEARTBEAT_READ_INTERVAL_MILLIS 60000
int HeartbeatLastReadInMillis = 0;
int millisCycles = 0;

void sendHeartbeatMQTTMessage(float milliseconds) {
  String messageTopic = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                        GLOBAL_MQTT_MULTISENSOR_NAME + "_Heartbeat";
  String messageValue = String(milliseconds);
  //  String messageValue = String(timeClient.getFormattedTime());
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

// Tasmota algorithm to relalculate rssi signal strenght
int WifiGetRssiAsQuality(int rssi) {
  int quality = 0;

  if (rssi <= -100) {
    quality = 0;
  } else if (rssi >= -50) {
    quality = 100;
  } else {
    quality = 2 * (rssi + 100);
  }
  return quality;
}

void print_uint64_t(uint64_t num) {

  char rev[128];
  char *p = rev + 1;

  while (num > 0) {
    *p++ = '0' + (num % 10);
    num /= 10;
  }
  p--;
  /*Print the number which is now in reverse*/
  while (p > rev) {
    Serial.print(*p--);
  }
}

void sendHeartbeatInfoMQTTMessage() {
  String messageTopic = "tele/" + GLOBAL_MQTT_MULTISENSOR_NAME + "/STATE";
  int signalStrength = WifiGetRssiAsQuality(WiFi.RSSI());

  uint32_t a_millis = millis();
  uint64_t uptime = millisCycles * 0x100000000 + a_millis;
  float uptime_days = 1.0 * uptime / 86400000.0;
  String messageValue = "{\"Uptime\":\"" + String(uptime_days, 4) +
                        "\",\"FreeMem\":" + String(ESP.getFreeHeap()) +
                        ",\"Wifi\":{\"RSSI\":" + String(signalStrength) +
                        ",\"MAC\":\"" + String(WiFi.macAddress()) + "\"}}";
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void processHeartbeat() {
  if (abs(millis() - HeartbeatLastReadInMillis) >
      HEARTBEAT_READ_INTERVAL_MILLIS) {
    if (millis() < HeartbeatLastReadInMillis) {
      millisCycles++;
    }
    sendHeartbeatMQTTMessage(millis());
    sendHeartbeatInfoMQTTMessage();
    HeartbeatLastReadInMillis = millis();
  }
}
#endif
