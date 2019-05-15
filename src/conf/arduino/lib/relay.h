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
  boolean hardwareON = false;
  boolean hardwareOFF = false;
  boolean hardwareON_ON = false;
#ifndef fRelayPIR
  hardwareON = (digitalRead(buttons[2].pin) == LOW);
  hardwareOFF = (digitalRead(buttons[2].pin) == HIGH && buttons[2].previousValue == LOW);
  hardwareON_ON = (digitalRead(buttons[2].pin) == LOW && buttons[2].previousValue == HIGH);
#endif
  boolean toggleON = (digitalRead(buttons[1].pin) == LOW && buttons[1].previousValue == HIGH);
  boolean mqttON = (GLOBAL_MQTT_MSG == "ON");
  boolean mqttOFF = (GLOBAL_MQTT_MSG == "OFF");

  if (toggleON)
  {
    sendDebugMQTTMessage("toggleON", String(toggleON));
  }
  if (hardwareON_ON)
  {
    sendDebugMQTTMessage("hardwareON_ON", String(hardwareON_ON));
  }
  if (hardwareOFF)
  {
    sendDebugMQTTMessage("hardwareOFF", String(hardwareOFF));
  }

  int currentRelay = digitalRead(RELAY_PIN);

  if (GLOBAL_MQTT_MSG == "DEBUG")
  {
#ifndef fRelayPIR
    String stateStr = "relay:" + String(currentRelay) + "pin:" + String(digitalRead(buttons[2].pin));
    sendDebugMQTTMessage("State", stateStr);
#else
    String stateStr = "relay:" + String(currentRelay);
    sendDebugMQTTMessage("State", stateStr);
    for (iix = 0; iix < CIRCUITS; iix++)
    {
      stateStr = circuits[iix].name + ":" + String(circuits[iix].previousValue);
      sendDebugMQTTMessage("State", stateStr);
    }
#endif
  }

  if (GLOBAL_MQTT_MSG == "RESTART")
  {
    ESP.restart();
  }

  int nextRelay = currentRelay;

  if (currentRelay == HIGH)
  {
    if (!hardwareON)
    {
      if (toggleON || mqttOFF)
      {
        nextRelay = LOW;
      }
    }
    if (hardwareOFF)
    {
      nextRelay = LOW;
    }
  }
  else
  {
    if (hardwareON)
    {
      nextRelay = HIGH;
    }
    else if (toggleON || mqttON)
    {
      nextRelay = HIGH;
    }
  }
  if (currentRelay != nextRelay)
  {
    digitalWrite(RELAY_PIN, nextRelay);
    sendRelayMsg();
  }
  GLOBAL_MQTT_MSG = "";
}
#endif
#endif