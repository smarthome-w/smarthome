#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <Wire.h>
#include <SPI.h>

#include "wifi.h"

#define VirtualCross

#ifdef VirtualCross
#define fName "GF_CorridorVirtualCross"
#define fDitigalInput
#define fCIR
#define WIFIIP 80
#define CIRCUITS_NO 1
#endif

const String MQTT_MESSAGE_PREFIX = "myHome";
const String MQTT_MULTISENSOR_NAME = fName;

const int SENSOR_DHT22 = 1;
const int SENSOR_DS18B20 = 2;
const int SENSOR_PIR = 3;
const int SENSOR_CIRCUIT = 4;

const int SENSOR_VALUE_CIRCUIT = 4;
const int SENSOR_VALUE_PIR = 5;

const int pinGPIO0 = 0;
const int pinGPIO2 = 2;

const int NORMALLY_CLOSED = 1;
const int NORMALLY_OPEN = 2;

const int NO_VALUE = -1;

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

WiFiClient espClient;
PubSubClient client(espClient);

char topic_buff[100];
char value_buff[100];

//------------------------------------------
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
  circuits[iix] = {"pinGPIO2", pinGPIO2, SENSOR_CIRCUIT, SENSOR_VALUE_CIRCUIT, NO_VALUE, NORMALLY_CLOSED}; iix++;
#endif
#ifdef fPIR
  circuits[iix] = {"pinGPIO2", pinGPIO2, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED}; iix++;
#endif
  Serial.println("Initialize digital input...");
  for (iix = 0; iix < CIRCUITS; iix = iix  + 1) {
    pinMode(circuits[iix].pin, INPUT_PULLUP);
    Serial.println("Circuit " + circuits[iix].name + " pin " + circuits[iix].pin + " set to INPUT_PULLUP");
  }
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

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("");

  initializeWatchdog();
#ifdef fDitigalInput
  initializeDigitalInput();
#endif
  initializeWiFi();
  initializeMQTT();
}

void loop() {
  processWatchdog();
  processHeartbeat();
#ifdef fDitigalInput
  processPinInputs();
#endif
  delay(100);
}

String calculateMessageName(int sensorType, int valueType) {
  String messageName = "";
  if (sensorType == SENSOR_PIR) {
    messageName = MQTT_MESSAGE_PREFIX + "/" + "PIR_" + MQTT_MULTISENSOR_NAME;
  } else if (sensorType == SENSOR_CIRCUIT) {
    messageName = MQTT_MESSAGE_PREFIX + "/" + "CIR_" + MQTT_MULTISENSOR_NAME;
  }
  return messageName;
}

