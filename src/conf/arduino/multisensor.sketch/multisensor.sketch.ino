#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#include "wifi.h"
#include "sensors.h"

////////////////////////////////////////////////////////////////////////////////////////
// Pins available
////////////////////////////////////////////////////////////////////////////////////////
// D0 - unused
// D1 - BMP280 (SCL)
// D2 - BMP280 (SDA)
// D3 - unused
// D4 - DS18B20 (OneWire)
// D5 - PIRu (LOW active)
// D6 - CIR (LOW active)
// D7 - DHT22
// D8 - unused
// A0 - Photoresistor
////////////////////////////////////////////////////////////////////////////////////////
// Features avialable for specific multisensor
////////////////////////////////////////////////////////////////////////////////////////
//#define fBMP280
//#define fDHT22
//#define fDS18B20
//#define fDitigalInput
//#define fCIR
//#define fPIR
//#define fAnalog
////////////////////////////////////////////////////////////////////////////////////////

#define SKETCH_VERSION "20190402"

//#define AnnRoom
#define AliceRoom
//#define Bedroom
//#define FamilyRoom
//#define TestRoom

#ifdef TestRoom
#define fName "FF_TestRoom"
#define CIRCUITS_NO 0
#define WIFIIP 49
#endif

#ifdef Bedroom
#define fName "FF_Bedroom"
#define fDitigalInput
#define fDHT22
#define fCIR
#define fAnalog
#define CIRCUITS_NO 1
#define WIFIIP 50
#endif

#ifdef AliceRoom
#define fName "FF_AliceRoom"
#define fDitigalInput
#define fDHT22
#define fCIR
#define fPIR
#define fAnalog
#define CIRCUITS_NO 2
#define WIFIIP 51
#endif

#ifdef AnnRoom
#define fName "FF_AnnRoom"
#define fDitigalInput
#define fDHT22
#define fCIR
#define fPIR
#define fDS18B20
#define CIRCUITS_NO 2
#define WIFIIP 52
#endif

#ifdef FamilyRoom
#define fName "GF_FamilyRoom"
#define fDHT22
#define fDS18B20
#define CIRCUITS_NO 0
#define WIFIIP 53
#define fAnalog
#define fBMP280
#endif

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

const String MQTT_MESSAGE_PREFIX = "myHome";
const String MQTT_MULTISENSOR_NAME = fName;

#define DHTPIN 13 // D7
#define DHTTYPE DHT22
#define DHT22_READ_INTERVAL_MILLIS 60000

const int CIRCUITS = CIRCUITS_NO;

int iix;
int status = WL_IDLE_STATUS;

struct circuit {
  String name;
  int pin;
  int sensorType;
  int sensorValueType;
  int previousValue;
  int type;
};

circuit circuits[CIRCUITS];

Ticker secondTick;
volatile int watchdogCount = 0;
const int WATCHDOG_TRIGGER = 30;

void ISRwatchdog() {
  watchdogCount++;
  if (watchdogCount >= WATCHDOG_TRIGGER) {
    ESP.restart();
  }
}

void sendDebugMQTTMessage(String prefix, String msg) {
  String messageTopic = MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_Debug_" + prefix;
  sendMQTTWithTypeConversion(messageTopic, msg);
}

const int WATCHDOG_SENSOR_TRIGGER = 10;

int processWatchdogSensor(long lastSuccessfulRead, long readInterval) {
  Serial.print(abs(millis() - lastSuccessfulRead));
  Serial.print(":");
  Serial.println(readInterval * WATCHDOG_SENSOR_TRIGGER);

  sendDebugMQTTMessage("DHT22_1", String(abs(millis() - lastSuccessfulRead)));
  sendDebugMQTTMessage("DHT22_2", String(readInterval * WATCHDOG_SENSOR_TRIGGER));

  if (abs(millis() - lastSuccessfulRead) >= (readInterval * WATCHDOG_SENSOR_TRIGGER)) {
    sendDebugMQTTMessage("DHT22_3", "Restart");
    delay(100);
    Serial.println("Restart device due to unresponsible sensor");
    ESP.restart();
  }
}

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHTPIN, DHTTYPE, 15);

char topic_buff[100];
char value_buff[100];

//------------------------------------------
//DS18B20
#define ONE_WIRE_BUS D4 //Pin to which is attached a temperature sensor

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

void callback(char* topic, byte* payload, unsigned int length) {
}

void reconnectMQTT() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void initializeDigitalInput() {
  Serial.println("Initialize definition data...");
  iix = 0;
#ifdef fCIR
  circuits[iix] = {"pinD6", pinD6, SENSOR_CIRCUIT, SENSOR_VALUE_CIRCUIT, NO_VALUE, NORMALLY_CLOSED}; iix++;
#endif
#ifdef fPIR
  circuits[iix] = {"pinD5", pinD5, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED}; iix++;
#endif
  Serial.println("Initialize digital input...");
  for (iix = 0; iix < CIRCUITS; iix = iix  + 1) {
    pinMode(circuits[iix].pin, INPUT_PULLUP);
    Serial.println("Circuit " + circuits[iix].name + " pin " + circuits[iix].pin + " set to INPUT_PULLUP");
  }
}

void initializeOTA() {
  Serial.println("Initialize OTA...");
  ArduinoOTA.setHostname(fName);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}

void initializeWiFi() {
  Serial.println("Initialize WIFI...");

  WiFi.persistent(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  WiFi.mode(WIFI_STA);

  IPAddress ip(192, 168, 0, WIFIIP);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 0, 0);
  WiFi.config(ip, gateway, subnet);


  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());
}

void initializeMQTT() {
  Serial.println("Initialize MQTT...");
  client.setServer(MQTT_SERVER, 1883);
  //client.setCallback(callback);
  reconnectMQTT();
}

void sendMQTTWithTypeConversion(String messageTopic, String messageValue) {
  Serial.println("Publish message:" + messageTopic + " " + messageValue);
  boolean retained = true;

  if (!client.connected()) {
    reconnectMQTT();
  }

  messageTopic.toCharArray(topic_buff, messageTopic.length() + 1);
  messageValue.toCharArray(value_buff, messageValue.length() + 1);

  client.publish(topic_buff, value_buff);
}

void sendDHT22MQTTMessage(float temperature, float humidity, float heatIndex) {
  if (!client.connected()) {
    reconnectMQTT();
  }

  String messageTopic = calculateMessageName(SENSOR_DHT22, SENSOR_VALUE_TEMPERATURE);
  String messageValue = String(temperature);
  sendMQTTWithTypeConversion(messageTopic, messageValue);

  messageTopic = calculateMessageName(SENSOR_DHT22, SENSOR_VALUE_HUMIDITY);
  messageValue = String(humidity);
  sendMQTTWithTypeConversion(messageTopic, messageValue);

  messageTopic = calculateMessageName(SENSOR_DHT22, SENSOR_VALUE_HEATINDEX);
  messageValue = String(heatIndex);
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void sendCircuitMQTTMessage(int circuitIndex, int value) {
  boolean retained = true;
  String msg;

  if (!client.connected()) {
    reconnectMQTT();
  }

  int currentType = circuits[circuitIndex].type;
  String messageTopic = calculateMessageName(circuits[circuitIndex].sensorType, circuits[circuitIndex].sensorValueType);

  if (currentType == NORMALLY_CLOSED) {
    if (value == LOW) {
      msg = "CLOSED";
    } else {
      msg = "OPEN";
    }
  } else {
    if (value == LOW) {
      msg = "OPEN";
    } else {
      msg = "CLOSED";
    }
  }

  sendMQTTWithTypeConversion(messageTopic, msg);
}

void processPinInputs() {
  int value;
  for (iix = 0; iix < CIRCUITS; iix++) {
    value = digitalRead(circuits[iix].pin);
    if (value != circuits[iix].previousValue) {
      circuits[iix].previousValue = value;
      sendCircuitMQTTMessage(iix, value);
    }
  }
}

int DHT22LastReadInMillis = 0;
int DHT22LastSuccessfulReadInMillis = 0;

void initializeDHT22() {
  Serial.println("Initialize DHT22...");
  dht.begin();
  DHT22LastSuccessfulReadInMillis = millis();
}

void processDHT22() {
  if (abs(millis() - DHT22LastReadInMillis) > DHT22_READ_INTERVAL_MILLIS) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      Serial.println(h);
      Serial.println(t);
      processWatchdogSensor (DHT22LastSuccessfulReadInMillis, DHT22_READ_INTERVAL_MILLIS);
      DHT22LastReadInMillis = millis();
      sendDebugMQTTMessage("DHT22_4", String(t));
      sendDebugMQTTMessage("DHT22_5", String(h));
      return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    sendDHT22MQTTMessage(t, h, hic);
    DHT22LastReadInMillis = millis();
    DHT22LastSuccessfulReadInMillis = DHT22LastReadInMillis;
  }
}

#define DS18B20_READ_INTERVAL_MILLIS 60000
int DS18B20LastReadInMillis = 0;

void sendDS18B20MQTTMessage(float temperature) {
  String messageTopic = calculateMessageName(SENSOR_DS18B20, SENSOR_VALUE_TEMPERATURE);
  String messageValue = String(temperature);
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void initializeDS18B20() {
  Serial.println("Initialize DS18B20...");
  DS18B20.begin();

  float temperature = -127.0;
}

void processDS18B20() {
  if (abs(millis() - DS18B20LastReadInMillis) > DS18B20_READ_INTERVAL_MILLIS) {
    float temperature;
    DS18B20.requestTemperatures();
    temperature = DS18B20.getTempCByIndex(0);
    if (temperature != 85.0 && temperature != (-127.0)) {
      sendDS18B20MQTTMessage(temperature);
    }
    DS18B20LastReadInMillis = millis();
  }
}

#define HEARTBEAT_READ_INTERVAL_MILLIS 60000
int HeartbeatLastReadInMillis = 0;

void sendHeartbeatMQTTMessage(float milliseconds) {
  String messageTopic = MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_Heartbeat";
  String messageValue = String(milliseconds);
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void processHeartbeat() {
  if (abs(millis() - HeartbeatLastReadInMillis) > HEARTBEAT_READ_INTERVAL_MILLIS) {
    sendHeartbeatMQTTMessage(millis());
    HeartbeatLastReadInMillis = millis();
  }
}

void initializeWatchdog() {
  Serial.println("Initialize Watchdog...");
  secondTick.attach(1, ISRwatchdog);
}

void processWatchdog() {
  watchdogCount = 0;
}

#define ANALOG_READ_INTERVAL_MILLIS 60000
int AnalogLastReadInMillis = 0;

void initializeAnalog() {
  Serial.println("Initialize analog...");
}

void sendAnalogMQTTMessage(int value) {
  String messageTopic = MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_Analog";
  String messageValue = String(value);
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void processAnalog() {
  if (abs(millis() - AnalogLastReadInMillis) > ANALOG_READ_INTERVAL_MILLIS) {
    int analogValue = analogRead(A0);   // read the input on analog pin 0
    sendAnalogMQTTMessage(analogValue);
    AnalogLastReadInMillis = millis();
  }
};

Adafruit_BMP280 bme;
#define BMP280_READ_INTERVAL_MILLIS 60000
int BMP280LastReadInMillis = 0;

void sendBMP280MQTTMessage(float value) {
  String messageTopic = "myHome/GF_FamilyRoom_Pressure_Sensor"; //MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_Pressure";
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

void processOTA() {
  ArduinoOTA.handle();
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("");
  Serial.println("");
  Serial.print(SKETCH_VERSION);
  Serial.print("   ");
  Serial.println(fName);

  initializeWatchdog();
#ifdef fDitigalInput
  initializeDigitalInput();
#endif
  initializeWiFi();
  initializeOTA();
  initializeMQTT();
#ifdef fDHT22
  initializeDHT22();
#endif
#ifdef fAnalog
  initializeAnalog();
#endif
#ifdef fDS18B20
  initializeDS18B20();
#endif
#ifdef fBMP280
  initializeBMP280();
#endif
  sendDebugMQTTMessage("VersionOnStart", SKETCH_VERSION);
  sendDebugMQTTMessage("MAC", WiFi.macAddress());
}

void loop() {
  processWatchdog();
  processOTA();
  processHeartbeat();
#ifdef fDitigalInput
  processPinInputs();
#endif
#ifdef fDHT22
  processDHT22();
#endif
#ifdef fAnalog
  processAnalog();
#endif
#ifdef fDS18B20
  processDS18B20();
#endif
#ifdef fBMP280
  processBMP280();
#endif
  //  dumpPinInputs();
  delay(100);
}

void dumpPinInputs() {
  for (iix = 0; iix < CIRCUITS; iix++) {
    Serial.print(circuits[iix].name);
    Serial.print(": ");
    Serial.print(circuits[iix].previousValue);
    Serial.print(" ");
  }
  Serial.println("");
}

String calculateMessageName(int sensorType, int valueType) {
  String messageName = "";
  if (sensorType == SENSOR_DHT22) {
    if (valueType == SENSOR_VALUE_TEMPERATURE) {
      messageName = MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_Indoor_Temperature_Sensor";
    } else if (valueType == SENSOR_VALUE_HUMIDITY) {
      messageName = MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_Humidity_Sensor";
    } else if (valueType == SENSOR_VALUE_HEATINDEX) {
      messageName = MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_HeatIndex";
    }
  } else if (sensorType == SENSOR_DS18B20) {
    messageName = MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_Outdoor_Temperature_Sensor";
  } else if (sensorType == SENSOR_PIR) {
    messageName = MQTT_MESSAGE_PREFIX + "/" + "PIR_" + MQTT_MULTISENSOR_NAME;
  } else if (sensorType == SENSOR_CIRCUIT) {
    messageName = MQTT_MESSAGE_PREFIX + "/" + "CIR_" + MQTT_MULTISENSOR_NAME;
  }
  return messageName;
}

