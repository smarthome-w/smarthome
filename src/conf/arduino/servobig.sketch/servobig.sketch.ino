#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>

#include "wifi.h"
#include "sensors.h"

#define SKETCH_VERSION "20190404"

//#define FamilyRoomRollershutter
#define BedroomRollershutter

#ifdef FamilyRoomRollershutter
#define fName "GF_FamilyRoom_Rollershutter"
#define WIFIIP 60
#define messagePrefix "myHomeX"
#define messageSubscribePrefix "myHomeX/GF_FamilyRoom_Rollershutter"
#define SERVO_UP_MARIGIN_VALUE 0
#define SERVO_MAXIMUM_RANGE_VALUE 100
#define LOOP_DELAY_VALUE 0
#define SERVO_UP 2500
#define SERVO_DOWN 500
#define CURRENT_DIRECTION_UP (-100.0/2187.0)
#define CURRENT_DIRECTION_DOWN (100.0/1985.0)
#endif

#ifdef BedroomRollershutter
#define fName "FF_Bedroom_Rollershutter"
#define WIFIIP 61
#define messagePrefix "myHomeX"
#define messageSubscribePrefix "myHomeX/FF_Bedroom_Rollershutter"
#define SERVO_UP_MARIGIN_VALUE 0
#define SERVO_MAXIMUM_RANGE_VALUE 100
#define LOOP_DELAY_VALUE 0
#define SERVO_UP 500
#define SERVO_DOWN 2500
#define CURRENT_DIRECTION_UP (-100.0/2800.0)
#define CURRENT_DIRECTION_DOWN (100.0/1667.0)
#endif

#define EFFECTIVE_AUTO_DOWN "AUTODOWN"
#define EFFECTIVE_AUTO_UP "AUTOUP"
#define EFFECTIVE_DOWN "DOWN"
#define EFFECTIVE_UP "UP"

#define PERCENTAGE_DELTA 0.5

const String MQTT_MESSAGE_PREFIX = messagePrefix;
const char* MQTT_SUBSCRIBE_PREFIX = messageSubscribePrefix;
const String MQTT_MULTISENSOR_NAME = fName;

const String EMPTY_STRING = "";

String SERVO_MSG = "";
int SERVO_MAX = -1;
int SERVO_MIN = -1;
float SERVO_CURR_POS = 0.0;
boolean prevMoveInProgress = false;
int PWM_CURR_POS = 0;

int SERVO_MAXIMUM_RANGE = SERVO_MAXIMUM_RANGE_VALUE;
int SERVO_UP_MARIGIN = SERVO_UP_MARIGIN_VALUE;

const int SERVOS = 1;

boolean ignoreFistMQTTMessage = true;

int iix;
int status = WL_IDLE_STATUS;

struct servo {
  String name;
  int pwtPin;
  int manualUpPin;
  int manualDownPin;
  int upLimitPin;
  int downLimitPin;
  String messageTopic;
  String messageValue;
  int currentPWM;
};


servo servos[SERVOS];

Ticker secondTick;
volatile int watchdogCount = 0;
const int WATCHDOG_TRIGGER = 300;

void ISRwatchdog() {
  watchdogCount++;
  if (watchdogCount >= WATCHDOG_TRIGGER) {
    ESP.restart();
  }
}

WiFiClient espClient;
PubSubClient client(espClient);

char topic_buff[100];
char value_buff[100];

int sign(float value) {
  if (value > 0.0) {
    return 1;
  } else if (value < 0.0) {
    return -1;
  } else {
    return 0;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (!ignoreFistMQTTMessage) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    payload[length] = '\0';
    SERVO_MSG = String((char*)payload);
  }
  ignoreFistMQTTMessage = false;
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
      client.subscribe(MQTT_SUBSCRIBE_PREFIX);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//Servo servo360;

void initializeServo() {
  Serial.println("Initialize 360Servo...");
  iix = 0;
  // name, pwtPin, manualUpPin, manualDownPin, upLimitPin, downLimitPin, messageTopic, messageValue, currentPWM
#ifdef FamilyRoomRollershutter
  servos[iix] = {"window", pinD2, pinD6, pinD7, pinD1, pinD5, "servoWindow", "", 0 }; iix++;
#endif
#ifdef BedroomRollershutter
  servos[iix] = {"window", pinD2, pinD7, pinD6, pinD5, pinD1, "servoWindow", "", 0 }; iix++;
#endif

  for (iix = 0; iix < SERVOS; iix = iix  + 1) {
    pinMode(servos[iix].pwtPin, OUTPUT);
    pinMode(servos[iix].manualUpPin, INPUT_PULLUP);
    pinMode(servos[iix].manualDownPin, INPUT_PULLUP);
    pinMode(servos[iix].upLimitPin, INPUT_PULLUP);
    pinMode(servos[iix].downLimitPin, INPUT_PULLUP);
    Serial.println("Servo " + servos[iix].name + " pin " + servos[iix].pwtPin + " set to OUTPUT");
    Serial.println("Servo " + servos[iix].name + " pins " + servos[iix].manualUpPin + \
                   ", " + servos[iix].manualDownPin + \
                   ", " + servos[iix].upLimitPin + \
                   ", " + servos[iix].downLimitPin + \
                   " set to INPUT_PULLUP");
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
  client.setCallback(callback);
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

#define HEARTBEAT_READ_INTERVAL_MILLIS 60000
int HeartbeatLastReadInMillis = 0;

void sendDebugMQTTMessage(String prefix, String msg) {
  String messageTopic = MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_Debug_" + prefix;
  sendMQTTWithTypeConversion(messageTopic, msg);
}

void sendPositionMQTTMessage(String msg) {
  String messageTopic = MQTT_MESSAGE_PREFIX + "/" + MQTT_MULTISENSOR_NAME + "_Position";
  sendMQTTWithTypeConversion(messageTopic, msg);
}

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

void processServo()
{
  // name, pwtPin, manualUpPin, manualDownPin, upLimitPin, downLimitPin, messageTopic, messageValue, currentPWM

  for (iix = 0; iix < SERVOS; iix = iix  + 1) {
    int manualUpValue = digitalRead(servos[iix].manualUpPin);
    int manualDownValue = digitalRead(servos[iix].manualDownPin);
    int upLimitValue = digitalRead(servos[iix].upLimitPin);
    int downLimitValue = digitalRead(servos[iix].downLimitPin);
    //String messageValue = servos[iix].messageValue;
    String messageValue = SERVO_MSG;

    if (messageValue == "OFF") {
      messageValue = "AUTOUP";
    }

    if (messageValue == "ON") {
      messageValue = "AUTODOWN";
    }

    int upValue = SERVO_UP;
    int downValue = SERVO_DOWN;

    int currentPWM = 0;
    float currentDirection = 0.0;

    int realUpLimitValue = digitalRead(servos[iix].upLimitPin);
    int realDownLimitValue = digitalRead(servos[iix].downLimitPin);

    downLimitValue = HIGH; // hack for broken hardware

    boolean isPercent = (messageValue == "0" || messageValue.toInt() != 0);
    boolean isAuto = (messageValue == "AUTOUP" || messageValue == "AUTODOWN");
    boolean isManual = (manualUpValue == LOW || manualDownValue == LOW);
    boolean isDebug = (messageValue == "DEBUG");
    boolean isUp = (manualUpValue == LOW || messageValue == EFFECTIVE_UP || messageValue == EFFECTIVE_AUTO_UP );
    boolean isDown = (manualDownValue == LOW || messageValue == EFFECTIVE_DOWN || messageValue == EFFECTIVE_AUTO_DOWN );

    float oldPosition = SERVO_CURR_POS;

    if (manualUpValue == LOW || messageValue == EFFECTIVE_UP || messageValue == EFFECTIVE_AUTO_UP) {
      if (upLimitValue != LOW) {
        currentPWM = upValue;
        currentDirection = CURRENT_DIRECTION_UP;
      } else {
        currentPWM = 0;
        currentDirection = 0.0;
        SERVO_MSG = "STOP";
      }
      if (manualUpValue == LOW) {
        currentPWM = upValue;
        currentDirection = CURRENT_DIRECTION_UP;
      }
    } else if (manualDownValue == LOW || messageValue == EFFECTIVE_DOWN || messageValue == EFFECTIVE_AUTO_DOWN) {
      if (downLimitValue != LOW) {
        currentPWM = downValue;
        currentDirection = CURRENT_DIRECTION_DOWN;
      } else {
        currentPWM = 0;
        SERVO_MSG = "STOP";
        currentDirection = 0.0;
      }
      if (manualDownValue == LOW) { // override by manual
        currentPWM = downValue;
        currentDirection = CURRENT_DIRECTION_DOWN;
      }
    } else if (isPercent) {
      long percentValue = messageValue.toInt();
      if (abs(SERVO_CURR_POS - messageValue.toInt()) > PERCENTAGE_DELTA) {
        if (sign(SERVO_CURR_POS - messageValue.toInt()) == -1) {
          currentPWM = downValue;
          currentDirection = CURRENT_DIRECTION_DOWN;
        } else {
          currentPWM = upValue;
          currentDirection = CURRENT_DIRECTION_UP;
        }
      } else {
        currentPWM = 0;
        SERVO_MSG = "STOP";
        currentDirection = 0.0;
      }
    } else {
      currentPWM = 0;
      SERVO_MSG = "STOP";
      currentDirection = 0.0;
    }

    if (manualDownValue == LOW || manualUpValue == LOW) {
      SERVO_MSG = "STOP";
    }

    if (downLimitValue == LOW && messageValue == EFFECTIVE_DOWN) {
      SERVO_MSG = "STOP";
    }

    if (upLimitValue == LOW && messageValue == EFFECTIVE_UP) {
      SERVO_MSG = "STOP";
    }

    if (upLimitValue == LOW) {
      SERVO_MAX = SERVO_MAXIMUM_RANGE;
      SERVO_MIN = 0;
      SERVO_CURR_POS = SERVO_MIN;
      oldPosition = SERVO_CURR_POS;
      //isDebug = true;
    }

    if (downLimitValue == LOW) {
      SERVO_MAX = SERVO_MAXIMUM_RANGE;
      SERVO_MIN = 0;
      SERVO_CURR_POS = SERVO_MAX;
      oldPosition = SERVO_CURR_POS;
      //isDebug = true;
    }

    if (messageValue == "PRGUP" || messageValue == "PRGDOWN") {
      // Set oldPosition to fake to be sure, that current position will be sent after servo programming.
      oldPosition = 0.1235;
      
      SERVO_MSG = "STOP";
      currentPWM = 0;
      currentDirection = 0.0;
      if (messageValue == "PRGDOWN") {
        SERVO_MAX = SERVO_MAXIMUM_RANGE;
        SERVO_MIN = 0;
        SERVO_CURR_POS = SERVO_MAX;
        oldPosition = SERVO_CURR_POS;
      } else {
        SERVO_MAX = SERVO_MAXIMUM_RANGE;
        SERVO_MIN = 0;
        SERVO_CURR_POS = SERVO_MIN;
        oldPosition = SERVO_CURR_POS;
      }
      Serial.print("Up value: ");
      Serial.println(SERVO_MAX);
      Serial.print("Down value: ");
      Serial.println(SERVO_MIN);
      boolean isCalibrated_1 = ((SERVO_MAX != -1) && (SERVO_MIN != -1));
      if (isCalibrated_1) {
        Serial.print("Range set to: ");
        Serial.println(SERVO_MAX - SERVO_MIN);
      }
      isDebug = true;
    }

    boolean isCalibrated = ((SERVO_MAX != -1) && (SERVO_MIN != -1));

    if (isCalibrated && isAuto) {
      if ((SERVO_CURR_POS >= SERVO_MAX + SERVO_UP_MARIGIN && messageValue == EFFECTIVE_AUTO_DOWN) || (SERVO_CURR_POS <= SERVO_MIN && messageValue == EFFECTIVE_AUTO_UP)) {
        currentPWM = 0;
        SERVO_MSG = "STOP";
        currentDirection = 0.0;
        Serial.println("Cancel auto message - out of range");
      }
    }

    if (!isCalibrated && isAuto) {
      currentPWM = 0;
      SERVO_MSG = "STOP";
      currentDirection = 0.0;
      Serial.println("Cancel auto message - not calibrated");
    }

    if (currentPWM != 0) {
      delay(1);
      digitalWrite(servos[iix].pwtPin, HIGH);
      delayMicroseconds(currentPWM);
      digitalWrite(servos[iix].pwtPin, LOW);
      SERVO_CURR_POS += currentDirection;
      PWM_CURR_POS += sign(currentDirection);
    }

    if (isDebug) {
      sendDebugMQTTMessage ("messageValue", String(messageValue));
      sendDebugMQTTMessage ("CurrPos", String(SERVO_CURR_POS));
      sendDebugMQTTMessage ("PWMCurrPos", String(PWM_CURR_POS));
      sendDebugMQTTMessage ("MinLimit", String(SERVO_MIN));
      sendDebugMQTTMessage ("MaxLimit", String(SERVO_MAX));
      sendDebugMQTTMessage ("UpLimitProbe (LOW active)", String(upLimitValue));
      sendDebugMQTTMessage ("DownLimitProbe (LOW active)", String(downLimitValue));
      sendDebugMQTTMessage ("RealUpLimitProbe (LOW active)", String(realUpLimitValue));
      sendDebugMQTTMessage ("RealDownLimitProbe (LOW active)", String(realDownLimitValue));
      sendDebugMQTTMessage ("Millis", String(millis()));
    }

    boolean currMoveInProgress = ((oldPosition - SERVO_CURR_POS) != 0);
    if (prevMoveInProgress && !currMoveInProgress) { 
      // servo was stopped during the cycle

      // correct to <0,100> for OpenHAB dimmer
      if (SERVO_CURR_POS > 100) {
        SERVO_CURR_POS = 100;
      }
      if (SERVO_CURR_POS < 0) {
        SERVO_CURR_POS = 0;
      }
      sendPositionMQTTMessage(String(SERVO_CURR_POS)); 
    }
   
    if (currentDirection != 0.0) {
      delay(LOOP_DELAY_VALUE);
    }
    prevMoveInProgress = currMoveInProgress;
  }
}

void processMQTTInput() {
  client.loop();
};

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
  Serial.println("");
  initializeWiFi();
  initializeOTA();
  initializeMQTT();
  initializeServo();
  sendDebugMQTTMessage("VersionOnStart", SKETCH_VERSION);
  sendDebugMQTTMessage("MAC", WiFi.macAddress());
}

void loop() {
  processOTA();
  processWatchdog();
  reconnectMQTT();
  processHeartbeat();
  processServo();
  processMQTTInput();
  delay(5);
}
