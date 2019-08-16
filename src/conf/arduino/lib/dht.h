#ifndef dht_h
#define dht_h
#ifdef fDHT22
#include <DHT.h>

#define DHTPIN 13 // D7
#define DHTTYPE DHT22
#define DHT22_READ_INTERVAL_MILLIS 60000

DHT dht(DHTPIN, DHTTYPE, 15);

int DHT22LastReadInMillis = 0;
int DHT22LastSuccessfulReadInMillis = 0;

void sendDHT22MQTTMessage(float temperature, float humidity, float heatIndex)
{
  if (!client.connected())
  {
    reconnectMQTT();
  }

  String messageTopic = calculateMessageName(SENSOR_DHT22, SENSOR_VALUE_TEMPERATURE, "");
  String messageValue = String(temperature);
  sendMQTTWithTypeConversion(messageTopic, messageValue);

  messageTopic = calculateMessageName(SENSOR_DHT22, SENSOR_VALUE_HUMIDITY, "");
  messageValue = String(humidity);
  sendMQTTWithTypeConversion(messageTopic, messageValue);

  messageTopic = calculateMessageName(SENSOR_DHT22, SENSOR_VALUE_HEATINDEX, "");
  messageValue = String(heatIndex);
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void initializeDHT22()
{
  Serial.println("Initialize DHT22...");
  dht.begin();
  DHT22LastSuccessfulReadInMillis = millis();
}

const int WATCHDOG_SENSOR_TRIGGER = 10;

int processWatchdogSensor(long lastSuccessfulRead, long readInterval)
{
  Serial.print(abs(millis() - lastSuccessfulRead));
  Serial.print(":");
  Serial.println(readInterval * WATCHDOG_SENSOR_TRIGGER);

  //sendDebugMQTTMessage("DHT22_1", String(abs(millis() - lastSuccessfulRead)));
  //sendDebugMQTTMessage("DHT22_2", String(readInterval * WATCHDOG_SENSOR_TRIGGER));

  if (abs(millis() - lastSuccessfulRead) >= (readInterval * WATCHDOG_SENSOR_TRIGGER))
  {
    sendDebugMQTTMessage("DHT22_3", "Restart");
    delay(100);
    Serial.println("Restart device due to unresponsible sensor");
    ESP.restart();
  }
}

void processDHT22()
{
  if (abs(millis() - DHT22LastReadInMillis) > DHT22_READ_INTERVAL_MILLIS)
  {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t))
    {
      Serial.println("Failed to read from DHT sensor!");
      processWatchdogSensor(DHT22LastSuccessfulReadInMillis, DHT22_READ_INTERVAL_MILLIS);
      DHT22LastReadInMillis = millis();
      //sendDebugMQTTMessage("DHT22_4", String(t));
      //sendDebugMQTTMessage("DHT22_5", String(h));
      return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    sendDHT22MQTTMessage(t, h, hic);
    DHT22LastReadInMillis = millis();
    DHT22LastSuccessfulReadInMillis = DHT22LastReadInMillis;
  }
}
#endif
#endif
