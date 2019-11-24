#ifndef analog_h
#define analog_h
#ifdef fAnalog
#define ANALOG_READ_INTERVAL_MILLIS 60000
int AnalogLastReadInMillis = 0;

void initializeAnalog() { Serial.println("Initialize analog..."); }

void sendAnalogMQTTMessage(int value) {
  String messageTopic = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                        GLOBAL_MQTT_MULTISENSOR_NAME + "_Analog";
  String messageValue = String(value);
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void processAnalog() {
  if (abs(millis() - AnalogLastReadInMillis) > ANALOG_READ_INTERVAL_MILLIS) {
    int analogValue = analogRead(A0); // read the input on analog pin 0
    sendAnalogMQTTMessage(analogValue);
    AnalogLastReadInMillis = millis();
  }
};
#endif
#endif