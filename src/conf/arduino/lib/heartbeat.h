#ifndef heartbeat_h
#define heartbeat_h
#define HEARTBEAT_READ_INTERVAL_MILLIS 60000
int HeartbeatLastReadInMillis = 0;

void sendHeartbeatMQTTMessage(float milliseconds)
{
  String messageTopic = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + GLOBAL_MQTT_MULTISENSOR_NAME + "_Heartbeat";
  String messageValue = String(milliseconds);
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void processHeartbeat()
{
  if (abs(millis() - HeartbeatLastReadInMillis) > HEARTBEAT_READ_INTERVAL_MILLIS)
  {
    sendHeartbeatMQTTMessage(millis());
    HeartbeatLastReadInMillis = millis();
  }
}
#endif
