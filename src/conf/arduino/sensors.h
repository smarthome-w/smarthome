#ifndef sensors_h
#define sensors_h

const int SENSOR_DHT22 = 1;
const int SENSOR_DS18B20 = 2;
const int SENSOR_PIR = 3;
const int SENSOR_CIRCUIT = 4;

const int SENSOR_VALUE_TEMPERATURE = 1;
const int SENSOR_VALUE_HUMIDITY = 2;
const int SENSOR_VALUE_HEATINDEX = 3;
const int SENSOR_VALUE_CIRCUIT = 4;
const int SENSOR_VALUE_PIR = 5;

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