#ifndef relay_h
#define relay_h
#ifdef fRelay
const int RELAY_PIN = pinD6;

#define RELAY_STATE_READ_INTERVAL_MILLIS 30000

int RelayLastReadInMillis = 0;

void sendRelayMQTTMessage(String state)
{
  String messageTopic = GLOBAL_MQTT_MESSAGE_PREFIX + "/" + GLOBAL_MQTT_MULTISENSOR_NAME;
  sendMQTTWithTypeConversion(messageTopic, state);
}

void processRelayState()
{
  if (abs(millis() - RelayLastReadInMillis) > RELAY_STATE_READ_INTERVAL_MILLIS)
  {
    String state_msg = "OFF";

    if (digitalRead(RELAY_PIN) == HIGH)
    {
      state_msg = "ON";
    }

    sendRelayMQTTMessage(state_msg);
    RelayLastReadInMillis = millis();
  }
}

void sendRelayMsg()
{
  String state_msg = "OFF";
  if (digitalRead(RELAY_PIN) == HIGH)
  {
    state_msg = "ON";
  }
  sendRelayMQTTMessage(state_msg);
}

void initializeRelay()
{
  Serial.println("Initialize relay...");
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void processRelay()
{
  //boolean hardwareON = (digitalRead(buttons[3].pin) == HIGH);
  //boolean hardwareOFF = (digitalRead(buttons[3].pin) == LOW && buttons[3].previousValue == HIGH);
  boolean hardwareON = false;
  boolean hardwareOFF = false;
  boolean toggleON = (digitalRead(buttons[1].pin) == LOW && buttons[1].previousValue == HIGH);
  boolean mqttON = (GLOBAL_MQTT_MSG == "ON");
  boolean mqttOFF = (GLOBAL_MQTT_MSG == "OFF");

  int currentRelay = digitalRead(RELAY_PIN);
  /*
    Serial.print(currentRelay);
    Serial.print(":");
    Serial.print(hardwareON);
    Serial.print(":");
    Serial.print(hardwareOFF);
    Serial.print(":");
    Serial.print(toggleON);
    Serial.print(":");
    Serial.print(mqttON);
    Serial.print(":");
    Serial.print(mqttOFF);
    Serial.println(":");
*/
  if (currentRelay == HIGH && hardwareOFF)
  {
    digitalWrite(RELAY_PIN, LOW);
  }
  else if (currentRelay == LOW && (hardwareON || mqttON))
  {
    digitalWrite(RELAY_PIN, HIGH);
  }
  else if (currentRelay == LOW && toggleON)
  {
    digitalWrite(RELAY_PIN, HIGH);
  }
  else if (currentRelay == HIGH && toggleON)
  {
    digitalWrite(RELAY_PIN, LOW);
  }
  else if (currentRelay == HIGH && hardwareOFF)
  {
    digitalWrite(RELAY_PIN, LOW);
  }
  else if (currentRelay == HIGH && mqttOFF)
  {
    digitalWrite(RELAY_PIN, LOW);
  }

  if (currentRelay != digitalRead(RELAY_PIN))
  {
    sendRelayMsg();
  }
  GLOBAL_MQTT_MSG = "";
}
#endif
#endif