#ifndef DS18B20_h
#define DS18B20_h
#ifdef fDS18B20
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <SPI.h>

#define ONE_WIRE_BUS D4 //Pin to which is attached a temperature sensor

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

#define DS18B20_READ_INTERVAL_MILLIS 60000
int DS18B20LastReadInMillis = 0;

void sendDS18B20MQTTMessage(float temperature)
{
  String messageTopic = calculateMessageName(SENSOR_DS18B20, SENSOR_VALUE_TEMPERATURE);
  String messageValue = String(temperature);
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void initializeDS18B20()
{
  Serial.println("Initialize DS18B20...");
  DS18B20.begin();

  float temperature = -127.0;
}

void processDS18B20()
{
  if (abs(millis() - DS18B20LastReadInMillis) > DS18B20_READ_INTERVAL_MILLIS)
  {
    float temperature;
    DS18B20.requestTemperatures();
    temperature = DS18B20.getTempCByIndex(0);
    if (temperature != 85.0 && temperature != (-127.0))
    {
      sendDS18B20MQTTMessage(temperature);
    }
    DS18B20LastReadInMillis = millis();
  }
}
#endif
#endif
