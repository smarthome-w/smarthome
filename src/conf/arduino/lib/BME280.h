#ifndef BME280_h
#define BME280_h
#ifdef fBME280
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

Adafruit_BME280 bme;
#define BME280_READ_INTERVAL_MILLIS 60000
int BME280LastReadInMillis = 0;

void sendBME280MQTTMessage(float temperature, float humidity, float pressure) {
  if (!client.connected()) {
    reconnectMQTT();
  }

  String messageTopic;
  String messageValue;

  if (!isnan(temperature)) {
    messageTopic =
        calculateMessageName(SENSOR_BME280, SENSOR_VALUE_TEMPERATURE, "");
    messageValue = String(temperature);
    sendMQTTWithTypeConversion(messageTopic, messageValue);
  }

  if (!isnan(humidity)) {
    messageTopic =
        calculateMessageName(SENSOR_BME280, SENSOR_VALUE_HUMIDITY, "");
    messageValue = String(humidity);
    sendMQTTWithTypeConversion(messageTopic, messageValue);
  }

  if (!isnan(pressure)) {
    messageTopic =
        calculateMessageName(SENSOR_BME280, SENSOR_VALUE_PRESSURE, "");
    messageValue = String(pressure);
    sendMQTTWithTypeConversion(messageTopic, messageValue);
  }
}

void initializeBME280() {
  Serial.println("Initialize BME280...");
  if (!bme.begin(0x76, &Wire)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
}

void processBME280() {
  if (abs(millis() - BME280LastReadInMillis) > BME280_READ_INTERVAL_MILLIS) {
    float pressure = bme.readPressure() / 100.0;
    float temperature = bme.readTemperature();
    float humidity = bme.readHumidity();
    if (pressure != 0.0) {
      sendBME280MQTTMessage(temperature, humidity, pressure);
      BME280LastReadInMillis = millis();
    }
  }
}
#endif
#endif
