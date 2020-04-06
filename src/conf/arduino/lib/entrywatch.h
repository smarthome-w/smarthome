#ifndef entrywatch_h
#define entrywatch_h
#ifdef TechnicalRoom2
#define STATE_OFF 0
#define STATE_WARN 1
#define STATE_TRIGGER 2

int entryWatchState = STATE_OFF;
boolean messageSent = false;

#define ENTRY_INTERVAL_MILLIS 60000
int lastEntryInMillis = 0;
int lastCorridorMoveInMillis = 0;

void sendEntryWatchMQTTMessage(String msg) {
  String messageTopic = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                        GLOBAL_MQTT_MULTISENSOR_NAME + "_EntryWatch";
  sendMQTTWithTypeConversion(messageTopic, msg);
}

void processEntryWatch() {
  int sPIR = circuits[3].previousValue;
  int sCIR = circuits[2].previousValue;

  if (sPIR == HIGH) {
    lastCorridorMoveInMillis = millis();
  }
  if (entryWatchState == STATE_OFF && sCIR == HIGH && sPIR == LOW &&
      (abs(millis() - lastEntryInMillis) > ENTRY_INTERVAL_MILLIS) &&
      (abs(millis() - lastCorridorMoveInMillis) > ENTRY_INTERVAL_MILLIS)) {
    entryWatchState = STATE_WARN;
    messageSent = false;
    lastEntryInMillis = millis();
  }
  if (entryWatchState == STATE_WARN && sCIR == HIGH && sPIR == HIGH) {
    entryWatchState = STATE_TRIGGER;
  }
  if (entryWatchState == STATE_TRIGGER) {
    if (messageSent == false) {
      sendEntryWatchMQTTMessage("OPEN");
      delay(1000);
      sendEntryWatchMQTTMessage("CLOSED");
      messageSent = true;
    }
    entryWatchState = STATE_OFF;
  }
}
#endif
#endif
