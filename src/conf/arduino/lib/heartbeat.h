#ifndef heartbeat_h
#define heartbeat_h
#define HEARTBEAT_READ_INTERVAL_MILLIS 60000
int HeartbeatLastReadInMillis = 0;

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

void sendHeartbeatInfoMQTTMessage() {
  String messageTopic = "tele/" + GLOBAL_MQTT_MULTISENSOR_NAME + "/STATE";
  int signalStrength = WifiGetRssiAsQuality(WiFi.RSSI());

  String messageValue = "{\"Time\":\"" + String(millis()) +
                        "\",\"FreeMem\":" + String(ESP.getFreeHeap()) +
                        ",\"Wifi\":{\"RSSI\":" + String(signalStrength) +
                        ",\"MAC\":\"" + String(WiFi.macAddress()) + "\"}}";
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void processHeartbeat() {
  if (abs(millis() - HeartbeatLastReadInMillis) >
      HEARTBEAT_READ_INTERVAL_MILLIS) {
    sendHeartbeatMQTTMessage(millis());
    sendHeartbeatInfoMQTTMessage();
    HeartbeatLastReadInMillis = millis();
  }
}
#endif
