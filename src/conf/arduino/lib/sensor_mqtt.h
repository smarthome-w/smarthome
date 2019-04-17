#ifndef sensor_mqtt_h
#define sensor_mqtt_h
  String calculateMessageName(int sensorType, int valueType) {
    String messageName = "";
    if (sensorType == SENSOR_DHT22) {
      if (valueType == SENSOR_VALUE_TEMPERATURE) {
        messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + GLOBAL_MQTT_MULTISENSOR_NAME + "_Indoor_Temperature_Sensor";
      } else if (valueType == SENSOR_VALUE_HUMIDITY) {
        messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + GLOBAL_MQTT_MULTISENSOR_NAME + "_Humidity_Sensor";
      } else if (valueType == SENSOR_VALUE_HEATINDEX) {
        messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + GLOBAL_MQTT_MULTISENSOR_NAME + "_HeatIndex";
      }
    } else if (sensorType == SENSOR_DS18B20) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + GLOBAL_MQTT_MULTISENSOR_NAME + "_Outdoor_Temperature_Sensor";
    } else if (sensorType == SENSOR_PIR) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + "PIR_" + GLOBAL_MQTT_MULTISENSOR_NAME;
    } else if (sensorType == SENSOR_CIRCUIT) {
      messageName = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + "CIR_" + GLOBAL_MQTT_MULTISENSOR_NAME;
    }
    return messageName;
  }
#endif