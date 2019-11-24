#ifndef BPM280_h
#define BPM280_h
#ifdef fBMP280
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>

Adafruit_BMP280 bme;
#define BMP280_READ_INTERVAL_MILLIS 60000
int BMP280LastReadInMillis = 0;

void sendBMP280MQTTMessage(float value) {
  String messageTopic =
      "myHome/GF_FamilyRoom_Pressure_Sensor"; // MQTT_MESSAGE_PREFIX + "/" +
                                              // MQTT_MULTISENSOR_NAME +
                                              // "_Pressure";
  String messageValue = String(value);
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void initializeBMP280() {
  Serial.println("Initialize BMP280...");
  if (!bme.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  }
}

void processBMP280() {
  if (abs(millis() - BMP280LastReadInMillis) > BMP280_READ_INTERVAL_MILLIS) {
    float pressure = bme.readPressure() / 100.0;
    if (pressure != 0.0) {
      sendBMP280MQTTMessage(pressure);
      BMP280LastReadInMillis = millis();
    }
  }
}
#endif
#endif
