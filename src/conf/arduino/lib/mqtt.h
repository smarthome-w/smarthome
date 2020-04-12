#ifndef mqtt_h
#define mqtt_h

#include <PubSubClient.h>

#undef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 200

boolean ignoreFistMQTTMessage = true;
PubSubClient client(espClient);
String GLOBAL_MQTT_MSG = "";

char topic_buff[100];
char value_buff[1000];

void callback(char *topic, byte *payload, unsigned int length) {
  if (!ignoreFistMQTTMessage) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    payload[length] = '\0';
    GLOBAL_MQTT_MSG = String((char *)payload);
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
#ifdef fMQTTInput
      client.subscribe(GLOBAL_MQTT_SUBSCRIBE_PREFIX);
#endif
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void initializeMQTT() {
  Serial.println("Initialize MQTT...");
  client.setServer(MQTT_SERVER, 1883);
#ifdef fMQTTInput
  client.setCallback(callback);
#endif
  reconnectMQTT();
}

void processMQTTInput() { client.loop(); };

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

void sendDebugMQTTMessage(String prefix, String msg) {
  String messageTopic = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                        GLOBAL_MQTT_MULTISENSOR_NAME + "_Debug_" + prefix;
  sendMQTTWithTypeConversion(messageTopic, msg);
}
#endif
