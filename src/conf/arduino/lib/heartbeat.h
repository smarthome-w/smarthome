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

/*
void sendHeartbeatInfoMQTTMessage() {
  String messageTopic = "tele/" + GLOBAL_MQTT_MULTISENSOR_NAME + "/STATE";
  // String messageValue = "{\"Time\":\"123\"}";

  String messageValue = "{\"Time\":\"" + String(timeClient.getFormattedTime()) +
                        "\",\"FreeMem\":" + String(ESP.getFreeHeap()) +
                        ",\"Wifi\":{\"RSSI\":" + String(WiFi.RSSI()) +
                        ",\"MAC\":\"" + String(WiFi.macAddress()) + "\"}}";
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}
*/

void processHeartbeat() {
  if (abs(millis() - HeartbeatLastReadInMillis) >
      HEARTBEAT_READ_INTERVAL_MILLIS) {
    sendHeartbeatMQTTMessage(millis());
    //    sendHeartbeatInfoMQTTMessage();
    HeartbeatLastReadInMillis = millis();
  }
}
#endif
