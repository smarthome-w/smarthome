#ifndef relay_h
#define relay_h
  const int RELAY_PIN = D1;

  #define RELAY_STATE_READ_INTERVAL_MILLIS 30000

  int RelayLastReadInMillis = 0;

  void sendRelayMQTTMessage(String state) {
    String messageTopic = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + GLOBAL_MQTT_MULTISENSOR_NAME;
    sendMQTTWithTypeConversion(messageTopic, state);
  }

  void processRelayState() {
    if (abs(millis() - RelayLastReadInMillis) > RELAY_STATE_READ_INTERVAL_MILLIS) {
      String state_msg = "OFF";
      
      if (digitalRead(RELAY_PIN) == HIGH) {
        state_msg = "ON";
      }
      
      sendRelayMQTTMessage(state_msg);
      RelayLastReadInMillis = millis();
    }
  }

  void initializeRelay() {
    Serial.println("Initialize relay...");
    pinMode(RELAY_PIN, OUTPUT);
  }

  void processRelay() {
    if (GLOBAL_MQTT_MSG != "") {
      Serial.print("GLOBAL_MQTT_MSG: ");
      Serial.println(GLOBAL_MQTT_MSG);
    }
    if (GLOBAL_MQTT_MSG == "ON"){
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("Relay pin to high");
    }
    if (GLOBAL_MQTT_MSG == "OFF"){
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("Relay pin to low");
    }
    GLOBAL_MQTT_MSG = "";
  }
#endif