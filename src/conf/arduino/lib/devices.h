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
//#define fRelay
//#define fRelayAuto
//#define fRelayPIR
//#define fRollershutter

//#define fDisableSerial
////////////////////////////////////////////////////////////////////////////////////////

#ifndef devices_h
#define devices_h

//#define FamilyRoomLED
//#define FamilyRoomLight
//#define CorridorStorage

//#define Kitchen
//#define BedroomLights
#define BedroomWardobe
//#define CorridorLights
//#define AnnRoom
//#define AliceRoom
//#define Bedroom // change CIR to D5 instead D6 (hardware issue)
//#define FamilyRoom
//#define TechnicalRoom1
//#define TechnicalRoom2
//#define FamilyRoomRollershutter
//#define BedroomRollershutter
//#define TestRoom
//#define TestRelay

#ifdef Kitchen
#define fName "GF_Kitchen_LED_Relay"
#define WIFIIP 35
#define CIRCUITS_NO 0
#define BUTTONS_NO 3
#define fRelay
#define fRelayAuto
#define fDitigalInput
#define fMQTTInput
#define fMQTTOutput
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeY";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeY/GF_Kitchen_LED_Relay_Command"
#define fDisableSerial
#endif

#ifdef BedroomLights
#define fName "FF_Bedroom_LED_Relay"
#define WIFIIP 36
#define CIRCUITS_NO 0
#define BUTTONS_NO 3
#define fRelay
#define fDitigalInput
#define fMQTTInput
#define fMQTTOutput
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeY";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeY/FF_Bedroom_LED_Relay_Command"
#endif

#ifdef CorridorLights
#define fName "GF_Corridor_LED_Relay"
#define WIFIIP 37
#define CIRCUITS_NO 0
#define BUTTONS_NO 2
#define fRelay
#define fDitigalInput
#define fMQTTInput
#define fMQTTOutput
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeY";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeY/GF_Corridor_LED_Relay_Command"
#endif

#ifdef BedroomWardobe
#define fName "FF_BedroomWardobe_LED_Relay"
#define WIFIIP 38
#define CIRCUITS_NO 1
#define BUTTONS_NO 2
#define fRelay
#define fRelayPIR
#define fDitigalInput
#define fMQTTInput
#define fMQTTOutput
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeY";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeY/FF_BedroomWardobe_LED_Relay_Command"
#endif

#ifdef FamilyRoomLED
#define fName "GF_FamilyRoom_LED_Relay"
#define WIFIIP 39
#define CIRCUITS_NO 0
#define BUTTONS_NO 3
#define fRelay
#define fDitigalInput
#define fMQTTInput
#define fMQTTOutput
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeY";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeY/GF_FamilyRoom_LED_Relay_Command"
#endif

#ifdef FamilyRoomLight
#define fName "GF_FamilyRoom_Light_Relay"
#define WIFIIP 40
#define CIRCUITS_NO 0
#define BUTTONS_NO 3
#define fRelay
#define fDitigalInput
#define fMQTTInput
#define fMQTTOutput
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeY";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeY/GF_FamilyRoom_Light_Relay_Command"
#endif

#ifdef CorridorStorage
#define fName "GF_Corridor_Storage_Relay"
#define WIFIIP 41
#define CIRCUITS_NO 0
#define BUTTONS_NO 3
#define fRelay
#define fDitigalInput
#define fMQTTInput
#define fMQTTOutput
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeY";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeY/GF_Corridor_Storage_Relay_Command"
#endif

#ifdef TestRelay
#define fName "Test_LED_Relay"
#define WIFIIP 48
#define CIRCUITS_NO 0
#define BUTTONS_NO 3
#define fRelay
#define fRelayAuto
#define fDitigalInput
#define fMQTTInput
#define fMQTTOutput
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeY";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeY/GTEST_LED_Relay_Command"
#define fDisableSerial
#endif

#ifdef TestRoom
#define fName "FF_TestRoom"
#define WIFIIP 49
#define CIRCUITS_NO 0
#define BUTTONS_NO 3
#define fRelay
#define fDitigalInput
#define fMQTTInput
#define fMQTTOutput
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeY";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeY/FF_Bedroom_LED_Relay_Command"
#endif

#ifdef Bedroom
#define fName "FF_Bedroom"
#define WIFIIP 50
#define fDitigalInput
#define fDHT22
#define fCIR
#define fAnalog
#define fMQTTOutput
#define CIRCUITS_NO 1
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHome";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#endif

#ifdef AliceRoom
#define fName "FF_AliceRoom"
#define WIFIIP 51
#define fDitigalInput
#define fDHT22
#define fCIR
#define fPIR
#define fAnalog
#define fDS18B20
#define fBME280
#define fMQTTOutput
#define CIRCUITS_NO 2
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHome";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#endif

#ifdef AnnRoom
#define fName "FF_AnnRoom"
#define WIFIIP 52
#define fDitigalInput
#define fDHT22
#define fCIR
#define fPIR
#define fDS18B20
#define fMQTTOutput
#define CIRCUITS_NO 2
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHome";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#endif

#ifdef FamilyRoom
#define fName "GF_FamilyRoom"
#define WIFIIP 53
#define fDHT22
#define fDS18B20
#define CIRCUITS_NO 0
#define fAnalog
#define fMQTTOutput
#define fBMP280
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHome";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#endif

#ifdef TechnicalRoom1
#define fName "FF_TechnicalRoom1"
#define WIFIIP 54
#define fDitigalInput
#define fMQTTOutput
//#define fCIR
#define CIRCUITS_NO 5
#define BUTTONS_NO 0
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHome";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#endif

#ifdef TechnicalRoom2
#define fName "FF_TechnicalRoom2"
#define WIFIIP 55
#define fDitigalInput
#define fMQTTOutput
//#define fCIR
#define CIRCUITS_NO 6
#define BUTTONS_NO 0
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHome";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#endif

#ifdef FamilyRoomRollershutter
#define fName "GF_FamilyRoom_Rollershutter"
#define WIFIIP 60
#define messagePrefix "myHomeX"
#define SERVO_UP_MARIGIN_VALUE 0
#define SERVO_MAXIMUM_RANGE_VALUE 100
#define LOOP_DELAY_VALUE 0
#define SERVO_UP 2500
#define SERVO_DOWN 500
#define CURRENT_DIRECTION_UP (-100.0 / 2187.0)
#define CURRENT_DIRECTION_DOWN (100.0 / 1985.0)
#define fRollershutter
#define fMQTTInput
#define fMQTTOutput
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeX";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeX/GF_FamilyRoom_Rollershutter"
#endif

#ifdef BedroomRollershutter
#define fName "FF_Bedroom_Rollershutter"
#define WIFIIP 61
#define messagePrefix "myHomeX"
#define SERVO_UP_MARIGIN_VALUE 0
#define SERVO_MAXIMUM_RANGE_VALUE 100
#define LOOP_DELAY_VALUE 0
#define SERVO_UP 500
#define SERVO_DOWN 2500
#define CURRENT_DIRECTION_UP (-100.0 / 2800.0)
#define CURRENT_DIRECTION_DOWN (100.0 / 1667.0)
#define fRollershutter
#define fMQTTInput
#define fMQTTOutput
const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeX";
const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
#define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeX/FF_Bedroom_Rollershutter"
#endif
#endif
