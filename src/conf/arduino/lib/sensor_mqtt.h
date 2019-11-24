#ifndef sensor_mqtt_h
#define sensor_mqtt_h

#define EMPTY_STRING ""

String calculateMessageName(int sensorType, int valueType,
                            String overrideMultisensorName) {
  String multisensorName = GLOBAL_MQTT_MULTISENSOR_NAME;
  if (overrideMultisensorName != EMPTY_STRING) {
    multisensorName = overrideMultisensorName;
  }

  String messageName = "";
  if (sensorType == SENSOR_DHT22) {
    if (valueType == SENSOR_VALUE_TEMPERATURE) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                    GLOBAL_MQTT_MULTISENSOR_NAME + "_Indoor_Temperature_Sensor";
    } else if (valueType == SENSOR_VALUE_HUMIDITY) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                    GLOBAL_MQTT_MULTISENSOR_NAME + "_Humidity_Sensor";
    } else if (valueType == SENSOR_VALUE_HEATINDEX) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                    GLOBAL_MQTT_MULTISENSOR_NAME + "_HeatIndex";
    }
  } else if (sensorType == SENSOR_BME280) {
    if (valueType == SENSOR_VALUE_TEMPERATURE) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                    GLOBAL_MQTT_MULTISENSOR_NAME +
                    "_Indoor_Temperature_Sensor1";
    } else if (valueType == SENSOR_VALUE_HUMIDITY) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                    GLOBAL_MQTT_MULTISENSOR_NAME + "_Humidity_Sensor1";
    } else if (valueType == SENSOR_VALUE_PRESSURE) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                    GLOBAL_MQTT_MULTISENSOR_NAME + "_Pressure1";
    }
  } else if (sensorType == SENSOR_DS18B20) {
    messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                  GLOBAL_MQTT_MULTISENSOR_NAME + "_Outdoor_Temperature_Sensor";
  } else if (sensorType == SENSOR_PIR) {
    messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + "PIR_" + multisensorName;
  } else if (sensorType == SENSOR_CIRCUIT) {
    messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + "CIR_" + multisensorName;
  } else if (sensorType == SENSOR_BUTTON) {
    if (valueType == SENSOR_VALUE_BUTTON_GPIO0) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                    GLOBAL_MQTT_MULTISENSOR_NAME + "_GPIO0";
    } else if (valueType == SENSOR_VALUE_BUTTON_GPIO2) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                    GLOBAL_MQTT_MULTISENSOR_NAME + "_GPIO2";
    } else if (valueType == SENSOR_VALUE_BUTTON_GPIO13) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                    GLOBAL_MQTT_MULTISENSOR_NAME + "_GPIO13";
    }
  } else if (sensorType == SENSOR_RELAY) {
    messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + "RELAY_" +
                  GLOBAL_MQTT_MULTISENSOR_NAME;
  }
  return messageName;
}
#endif
