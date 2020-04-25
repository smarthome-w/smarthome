#define SKETCH_VERSION "20200425"
#include "lib/devices.h"

#include "lib/wifi.h"
#include "lib/wifi_connect.h"
#include "lib/mqtt.h"
#include "lib/watchdog.h"
#include "lib/ntp.h"
#include "lib/sensors.h"
#include "lib/sensor_mqtt.h"
#include "lib/ota.h"
#include "lib/dht.h"
#include "lib/heartbeat.h"
#include "lib/DS18B20.h"
#include "lib/BMP280.h"
#include "lib/BME280.h"

#include "lib/digitalio.h"
#include "lib/entrywatch.h"
#include "lib/analog.h"
#include "lib/relay.h"
#include "lib/rollershutter.h"
#include "lib/stepper.h"

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

  #ifdef watchdog_h
    initializeWatchdog();
  #endif
  #ifdef fStepperMotor
    initializeStepperMotor();
  #endif
  #ifdef fDitigalInput
    initializeDigitalInput();
  #endif
  #ifdef fRelay
    initializeDigitalButton();
  #endif
  #ifdef wifi_connect_h
    initializeWiFi();
  #endif
  #ifdef ntp_h
//    initializeNTP();
  #endif

  #ifdef ota_h
    initializeOTA();
  #endif
  #ifdef mqtt_h
    initializeMQTT();
  #endif
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
  #ifdef fBME280
    initializeBME280();
  #endif
  #ifdef fRelay
    initializeRelay();
  #endif
  #ifdef fRollershutter
    initializeServo();
  #endif
  Serial.println("Sensors initialized");
  #ifdef fMQTTOutput
    sendDebugMQTTMessage("VersionOnStart", SKETCH_VERSION);
    sendDebugMQTTMessage("MAC", WiFi.macAddress());
  #endif
  Serial.println("Setup finished");
}

void loop() {
  #ifdef watchdog_h
    processWatchdog();
  #endif
  #ifdef ota_h
    processOTA();
  #endif
    processHeartbeat();
  #ifdef ntp_h
 //   processNTP();
  #endif
  // relay MUST be before digital input
  // raw previous value is required
  #ifdef fRelay
    processRelay();
    processRelayState();
  #endif
  #ifdef fDitigalInput
    processPinInputs();
  #endif
  #ifdef TechnicalRoom2
    processEntryWatch();
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
  #ifdef fBME280
    processBME280();
  #endif
    //  dumpPinInputs();
  #ifdef fRollershutter
    processServo();
  #endif
  #ifdef fStepperMotor
    processStepperMotor();
  #endif
  #ifdef fMQTTInput
    processMQTTInput();
  #endif
  // loop for rollershutter is shorter due to proper PWM modulation
  #ifndef fRollershutter
  #ifndef fStepperMotor
  delay(100);
  #endif
  #else
  delay(5);
  #endif
//  makeStep();
}
