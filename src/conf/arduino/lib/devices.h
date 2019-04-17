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
//#define fRollershutter
////////////////////////////////////////////////////////////////////////////////////////

#ifndef devices_h
#define devices_h

    #define Kitchen
    //#define AnnRoom
    //#define AliceRoom
    //#define Bedroom
    //#define FamilyRoom
    //#define TechnicalRoom1
    //#define TechnicalRoom2
    //#define FamilyRoomRollershutter
    //#define BedroomRollershutter
    //#define TestRoom

    #ifdef Kitchen
        #define fName "GF_Kitchen_LED_Relay"
        #define WIFIIP 35
        #define CIRCUITS_NO 0
        #define BUTTONS_NO 1
        #define fRelay
        #define fDitigalInput
        #define fMQTTInput
        #define fMQTTOutput
        const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeY";
        const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
        #define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeY/GF_Kitchen_LED_Relay_Command"
    #endif

    #ifdef TestRoom
        #define fName "FF_TestRoom"
        #define WIFIIP 49
        #define fDitigalInput
        //#define fDHT22
        #define fDS18B20
        #define fCIR
        #define fAnalog
        #define fMQTTOutput
        #define CIRCUITS_NO 1
        const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHome";
        const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
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
        #define fCIR
        #define CIRCUITS_NO 4
        const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHome";
        const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
    #endif

    #ifdef TechnicalRoom2
        #define fName "FF_TechnicalRoom2"
        #define WIFIIP 55
        #define fDitigalInput
        #define fMQTTOutput
        #define fCIR
        #define CIRCUITS_NO 5
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
        #define CURRENT_DIRECTION_UP (-100.0/2187.0)
        #define CURRENT_DIRECTION_DOWN (100.0/1985.0)
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
        #define CURRENT_DIRECTION_UP (-100.0/2800.0)
        #define CURRENT_DIRECTION_DOWN (100.0/1667.0)
        #define fRollershutter
        #define fMQTTInput
        #define fMQTTOutput
        const String GLOBAL_MQTT_MESSAGE_PREFIX = "myHomeX";
        const String GLOBAL_MQTT_MULTISENSOR_NAME = fName;
        #define GLOBAL_MQTT_SUBSCRIBE_PREFIX "myHomeX/FF_Bedroom_Rollershutter"
    #endif
#endif
