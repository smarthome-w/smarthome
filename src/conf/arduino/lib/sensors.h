#ifndef sensors_h
#define sensors_h
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

    const int SENSOR_DHT22 = 1;
    const int SENSOR_DS18B20 = 2;
    const int SENSOR_PIR = 3;
    const int SENSOR_CIRCUIT = 4;
    const int SENSOR_BUTTON = 5;
    const int SENSOR_RELAY = 6;

    const int SENSOR_VALUE_TEMPERATURE = 1;
    const int SENSOR_VALUE_HUMIDITY = 2;
    const int SENSOR_VALUE_HEATINDEX = 3;
    const int SENSOR_VALUE_CIRCUIT = 4;
    const int SENSOR_VALUE_PIR = 5;
    const int SENSOR_VALUE_RELAY = 6;
    const int SENSOR_VALUE_BUTTON_GPIO0 = 80;
    const int SENSOR_VALUE_BUTTON_GPIO2 = 81;
    const int SENSOR_VALUE_BUTTON_GPIO13 = 82;

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
#endif