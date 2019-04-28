#ifndef digitalio_h
#define digitalio_h
#ifdef fDitigalInput
const int CIRCUITS = CIRCUITS_NO;
#endif

#ifdef fRelay
const int BUTTONS = BUTTONS_NO;
#endif

int iix;

struct circuit
{
  String name;
  int pin;
  int sensorType;
  int sensorValueType;
  int previousValue;
  int type;
};

#ifdef fDitigalInput
circuit circuits[CIRCUITS];
#endif

#ifdef fRelay
circuit buttons[BUTTONS];
#endif

void dumpPinInputs()
{
#ifdef fDitigalInput
  for (iix = 0; iix < CIRCUITS; iix++)
  {
    Serial.print(circuits[iix].name);
    Serial.print(": ");
    Serial.print(circuits[iix].previousValue);
    Serial.print(" ");
  }
  Serial.println("");
#endif
#ifdef fRelay
  for (iix = 0; iix < BUTTONS; iix++)
  {
    Serial.print(buttons[iix].name);
    Serial.print(": ");
    Serial.print(buttons[iix].previousValue);
    Serial.print(" ");
  }
  Serial.println("");
#endif
}

void initializeDigitalInput()
{
#ifdef fDitigalInput
  Serial.println("Initialize definition data...");
  iix = 0;
#ifdef fCIR
  circuits[iix] = {"pinD6", pinD6, SENSOR_CIRCUIT, SENSOR_VALUE_CIRCUIT, NO_VALUE, NORMALLY_CLOSED};
  iix++;
#endif
#ifdef fPIR
  circuits[iix] = {"pinD5", pinD5, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED};
  iix++;
#endif
#ifdef TechnicalRoom1
  circuits[iix] = {"GF_FamilyRoom", pinD1, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED};
  iix++;
  circuits[iix] = {"GF_Kitchen", pinD2, SENSOR_CIRCUIT, SENSOR_VALUE_CIRCUIT, NO_VALUE, NORMALLY_CLOSED};
  iix++;
  circuits[iix] = {"FF_Corridor", pinD5, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED};
  iix++;
  circuits[iix] = {"FF_Bedroom", pinD6, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED};
  iix++;
#endif
#ifdef TechnicalRoom2
  circuits[iix] = {"GF_FamilyRoom", pinD1, SENSOR_CIRCUIT, SENSOR_VALUE_CIRCUIT, NO_VALUE, NORMALLY_CLOSED};
  iix++;
  circuits[iix] = {"GF_MainDoor", pinD2, SENSOR_CIRCUIT, SENSOR_VALUE_CIRCUIT, NO_VALUE, NORMALLY_CLOSED};
  iix++;
  circuits[iix] = {"GF_Corridor", pinD5, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED};
  iix++;
  circuits[iix] = {"GF_MainLock", pinD6, SENSOR_CIRCUIT, SENSOR_VALUE_CIRCUIT, NO_VALUE, NORMALLY_CLOSED};
  iix++;
  circuits[iix] = {"GF_Kitchen", pinD7, SENSOR_PIR, SENSOR_VALUE_PIR, NO_VALUE, NORMALLY_CLOSED};
  iix++;
#endif
  Serial.println("Initialize digital input...");
  for (iix = 0; iix < CIRCUITS; iix = iix + 1)
  {
    pinMode(circuits[iix].pin, INPUT_PULLUP);
    Serial.println("Circuit " + circuits[iix].name + " pin " + circuits[iix].pin + " set to INPUT_PULLUP");
  }
#endif
}

void initializeDigitalButton()
{
#ifdef fRelay
  Serial.println("Initialize button data...");
  iix = 0;
#ifdef fRelay
  buttons[iix] = {"pinD6-relay", pinD6, SENSOR_RELAY, SENSOR_VALUE_RELAY, NO_VALUE, NORMALLY_CLOSED};
  iix++;
  buttons[iix] = {"pinD3", pinD3, SENSOR_BUTTON, SENSOR_VALUE_BUTTON_GPIO0, NO_VALUE, NORMALLY_CLOSED};
  iix++;
#ifdef fRelayAuto
  buttons[iix] = {"pinRx", pinRx, SENSOR_BUTTON, SENSOR_VALUE_BUTTON_GPIORX, NO_VALUE, NORMALLY_OPEN};
  iix++;
#endif
  //        buttons[iix] = {"pinD7", pinD7, SENSOR_BUTTON, SENSOR_VALUE_BUTTON_GPIO13, NO_VALUE, NORMALLY_OPEN}; iix++;
#endif
  Serial.println("Initialize button input...");
  for (iix = 0; iix < BUTTONS; iix = iix + 1)
  {
    pinMode(buttons[iix].pin, INPUT_PULLUP);
    Serial.println("Button " + buttons[iix].name + " pin " + buttons[iix].pin + " set to INPUT_PULLUP");
  }
#endif
}

void sendCircuitMQTTMessage(int circuitIndex, int value)
{
#ifdef fDitigalInput
  boolean retained = true;
  String msg;

  if (!client.connected())
  {
    reconnectMQTT();
  }

  int currentType = circuits[circuitIndex].type;
  String messageTopic = calculateMessageName(circuits[circuitIndex].sensorType, circuits[circuitIndex].sensorValueType);

  if (currentType == NORMALLY_CLOSED)
  {
    if (value == LOW)
    {
      msg = "CLOSED";
    }
    else
    {
      msg = "OPEN";
    }
  }
  else
  {
    if (value == LOW)
    {
      msg = "OPEN";
    }
    else
    {
      msg = "CLOSED";
    }
  }

  sendMQTTWithTypeConversion(messageTopic, msg);
#endif
}

void sendButtonMQTTMessage(int buttonIndex, int value)
{
#ifdef fRelay
  boolean retained = true;
  String msg;

  if (!client.connected())
  {
    reconnectMQTT();
  }

  int currentType = buttons[buttonIndex].type;
  String messageTopic = calculateMessageName(buttons[buttonIndex].sensorType, buttons[buttonIndex].sensorValueType);

  if (currentType == NORMALLY_CLOSED)
  {
    if (value == LOW)
    {
      msg = "CLOSED";
    }
    else
    {
      msg = "OPEN";
    }
  }
  else
  {
    if (value == LOW)
    {
      msg = "OPEN";
    }
    else
    {
      msg = "CLOSED";
    }
  }

  sendMQTTWithTypeConversion(messageTopic, msg);
#endif
}

void processPinInputs()
{
  int value;

#ifdef fDitigalInput
  for (iix = 0; iix < CIRCUITS; iix++)
  {
    value = digitalRead(circuits[iix].pin);
    if (value != circuits[iix].previousValue)
    {
      circuits[iix].previousValue = value;
      sendCircuitMQTTMessage(iix, value);
    }
  }
#endif

#ifdef fRelay
  for (iix = 0; iix < BUTTONS; iix++)
  {
    value = digitalRead(buttons[iix].pin);
    if (value != buttons[iix].previousValue)
    {
      buttons[iix].previousValue = value;
      sendButtonMQTTMessage(iix, value);
    }
  }
#endif
}
#endif
