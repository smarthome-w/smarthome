#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>

#include "wifi.h"

#define SKETCH_VERSION "20190311"

//#define TechnicalRoom1
#define TechnicalRoom2

#ifdef TechnicalRoom1
#define fName "FF_TechnicalRoom1"
#define WIFIIP 54
#define SENSOR_SIZE 4
#endif

#ifdef TechnicalRoom2
#define fName "FF_TechnicalRoom2"
#define WIFIIP 55
#define SENSOR_SIZE 5
#endif


const String MQTT_MESSAGE_PREFIX = "myHome";
const String MQTT_MULTISENSOR_NAME = fName;

const String EMPTY_STRING = "";

const int SENSOR_DHT22 = 1;
const int SENSOR_DS18B20 = 2;
const int SENSOR_PIR = 3;
const int SENSOR_CIRCUIT = 4;

const int SENSOR_VALUE_TEMPERATURE = 1;
const int SENSOR_VALUE_HUMIDITY = 2;
const int SENSOR_VALUE_HEATINDEX = 3;
const int SENSOR_VALUE_CIRCUIT = 4;
const int SENSOR_VALUE_PIR = 5;


#define DHTPIN 4
#define DHTTYPE DHT22
#define DHT22_READ_INTERVAL_MILLIS 60000

const int pinD0 = 16;
const int pinD1 = 5;
const int pinD2 = 4;
const int pinD3 = 0;
const int pinD4 = 2;
const int pinD5 = 14;
const int pinD6 = 12;
const int pinD7 = 13;
const int pinD8 = 15;

const int NORMALLY_CLOSED = 1;
const int NORMALLY_OPEN = 2;

const int NO_VALUE = -1;

const int CIRCUITS = SENSOR_SIZE;

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

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHTPIN, DHTTYPE);

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
#ifdef TechnicalRoom1
  circuits[iix] = {"GF_FamilyRoom", pinD1, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED}; iix++;
  circuits[iix] = {"GF_Kitchen", pinD2, SENSOR_CIRCUIT, SENSOR_VALUE_CIRCUIT, NO_VALUE, NORMALLY_CLOSED}; iix++;
  circuits[iix] = {"FF_Corridor", pinD5, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED}; iix++;
  circuits[iix] = {"FF_Bedroom", pinD6, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED}; iix++;
#endif
#ifdef TechnicalRoom2
  circuits[iix] = {"GF_FamilyRoom", pinD1, SENSOR_CIRCUIT, SENSOR_VALUE_CIRCUIT, NO_VALUE, NORMALLY_CLOSED}; iix++;
  circuits[iix] = {"GF_MainDoor", pinD2, SENSOR_CIRCUIT, SENSOR_VALUE_CIRCUIT, NO_VALUE, NORMALLY_CLOSED}; iix++;
  circuits[iix] = {"GF_Corridor", pinD5, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED}; iix++;
  circuits[iix] = {"GF_MainLock", pinD6, SENSOR_CIRCUIT, SENSOR_VALUE_CIRCUIT, NO_VALUE, NORMALLY_CLOSED}; iix++;
  circuits[iix] = {"GF_Kitchen", pinD7, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED}; iix++;
#endif

  Serial.println("Initialize digital input...");
  for (iix = 0; iix < CIRCUITS; iix = iix  + 1) {
    pinMode(circuits[iix].pin, INPUT_PULLUP);
    //pinMode(circuits[iix].pin, INPUT_PULLUP);
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

void initializeDHT() {
  Serial.println("Initialize DHT...");
}

//------------------------------------------
#define DS18B20_READ_INTERVAL_MILLIS 60000
int DS18B20LastReadInMillis = 0;

void initializeDS18B20() {
  Serial.println("Initialize DS18B20...");
  DS18B20.begin();

  float temperature = -127.0;
  //  do {
  //    DS18B20.requestTemperatures();
  //    temperature = DS18B20.getTempCByIndex(0);
  //    Serial.println("Initializing temperature read...");
  //    delay(100);
  //  } while (temperature == 85.0 || temperature == (-127.0));
  //  Serial.print("Initial temperature ");
  //  Serial.print(temperature);
  //  Serial.println(" C");
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

  String messageTopic = calculateMessageName(SENSOR_DHT22, SENSOR_VALUE_TEMPERATURE, EMPTY_STRING);
  String messageValue = String(temperature);
  sendMQTTWithTypeConversion(messageTopic, messageValue);

  messageTopic = calculateMessageName(SENSOR_DHT22, SENSOR_VALUE_HUMIDITY, EMPTY_STRING);
  messageValue = String(humidity);
  sendMQTTWithTypeConversion(messageTopic, messageValue);

  messageTopic = calculateMessageName(SENSOR_DHT22, SENSOR_VALUE_HEATINDEX, EMPTY_STRING);
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
  String messageTopic = calculateMessageName(circuits[circuitIndex].sensorType, circuits[circuitIndex].sensorValueType, circuits[circuitIndex].name);

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

void processDHT22() {
  if (abs(millis() - DHT22LastReadInMillis) > DHT22_READ_INTERVAL_MILLIS) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      DHT22LastReadInMillis = millis();
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    sendDHT22MQTTMessage(t, h, hic);
    DHT22LastReadInMillis = millis();
  }
}

void sendDS18B20MQTTMessage(float temperature) {
  String messageTopic = calculateMessageName(SENSOR_DS18B20, SENSOR_VALUE_TEMPERATURE, EMPTY_STRING);
  String messageValue = String(temperature);
  sendMQTTWithTypeConversion(messageTopic, messageValue);
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

#define VCC_READ_INTERVAL_MILLIS 60000
int VCCLastReadInMillis = 0;

void sendVCCMQTTMessage(float vcc) {
  String messageTopic = MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_VCC";
  String messageValue = String(vcc);
  sendMQTTWithTypeConversion(messageTopic, messageValue);
}

void processVCC() {
  if (abs(millis() - VCCLastReadInMillis) > VCC_READ_INTERVAL_MILLIS) {
    float vcc;
    vcc = ESP.getVcc();
    sendVCCMQTTMessage(vcc);
    VCCLastReadInMillis = millis();
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

#ifdef TechnicalRoom2

#define STATE_OFF 0
#define STATE_WARN 1
#define STATE_TRIGGER 2

int entryWatchState = STATE_OFF;
boolean messageSent = false;

#define ENTRY_INTERVAL_MILLIS 60000
int lastEntryInMillis = 0;
int lastCorridorMoveInMillis = 0;

void sendEntryWatchMQTTMessage(String msg) {
  String messageTopic = MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_EntryWatch";
  sendMQTTWithTypeConversion(messageTopic, msg);
}

void processEntryWatch(){
  int sPIR = circuits[2].previousValue;
  int sCIR = circuits[1].previousValue;

  if (sPIR == HIGH) {
    lastCorridorMoveInMillis = millis();
  }
  if (entryWatchState == STATE_OFF && \
      sCIR == HIGH && sPIR == LOW && \
      (abs(millis() - lastEntryInMillis) > ENTRY_INTERVAL_MILLIS) && \
      (abs(millis() - lastCorridorMoveInMillis) > ENTRY_INTERVAL_MILLIS)) {
    entryWatchState = STATE_WARN;
    messageSent = false;
    lastEntryInMillis = millis();
  }
  if (entryWatchState == STATE_WARN && sCIR == HIGH && sPIR == HIGH) {
    entryWatchState = STATE_TRIGGER;
  }
  if (entryWatchState == STATE_TRIGGER) {
    if (messageSent == false) {
      sendEntryWatchMQTTMessage("OPEN");
      delay(1000);
      sendEntryWatchMQTTMessage("CLOSED");
      messageSent = true;
    }
    entryWatchState = STATE_OFF;
  }

//  Serial.print("sCIR ");
//  Serial.print(sCIR);
//  Serial.print(" sPIR ");
//  Serial.print(sPIR);
//  Serial.print(" entryWatchState ");
//  Serial.print(entryWatchState);
//  Serial.print(" messageSent ");
//  Serial.print(messageSent);
//  Serial.println("");
}
#endif

void processOTA() {
  ArduinoOTA.handle();
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("");
  Serial.print(SKETCH_VERSION);
  Serial.print("   ");
  Serial.println(fName);
  initializeWatchdog();
  initializeDigitalInput();
  initializeWiFi();
  initializeOTA();
  initializeMQTT();
  //  initializeDHT();
  //  initializeDS18B20();
}

void loop() {
  processWatchdog();
  processOTA();
  processHeartbeat();
  processPinInputs();
#ifdef TechnicalRoom2
  processEntryWatch();
#endif
  //  processDHT22();
  //  processDS18B20();
  //  processVCC();
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

String calculateMessageName(int sensorType, int valueType, String overrideMultisensorName) {
  String multisensorName = MQTT_MULTISENSOR_NAME;
  if (overrideMultisensorName != EMPTY_STRING) {
    multisensorName = overrideMultisensorName;
  }

  String messageName = "";
  if (sensorType == SENSOR_DHT22) {
    if (valueType == SENSOR_VALUE_TEMPERATURE) {
      messageName = MQTT_MESSAGE_PREFIX + "/" + multisensorName + "_Indoor_Temperature_Sensor";
    } else if (valueType == SENSOR_VALUE_HUMIDITY) {
      messageName = MQTT_MESSAGE_PREFIX + "/" + multisensorName + "_Humidity_Sensor";
    } else if (valueType == SENSOR_VALUE_HEATINDEX) {
      messageName = MQTT_MESSAGE_PREFIX + "/" + multisensorName + "_HeatIndex";
    }
  } else if (sensorType == SENSOR_DS18B20) {
    messageName = MQTT_MESSAGE_PREFIX + "/" + multisensorName + "_Outdoor_Temperature_Sensor";
  } else if (sensorType == SENSOR_PIR) {
    messageName = MQTT_MESSAGE_PREFIX + "/" + "PIR_" + multisensorName;
  } else if (sensorType == SENSOR_CIRCUIT) {
    messageName = MQTT_MESSAGE_PREFIX + "/" + "CIR_" + multisensorName;
  }
  return messageName;
}

