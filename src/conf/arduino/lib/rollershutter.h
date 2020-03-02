#ifndef rollershutter_h
#define rollershutter_h
#ifdef fRollershutter
#define EFFECTIVE_AUTO_DOWN "AUTODOWN"
#define EFFECTIVE_AUTO_UP "AUTOUP"
#define EFFECTIVE_DOWN "DOWN"
#define EFFECTIVE_UP "UP"

#define PERCENTAGE_DELTA 0.5

int SERVO_MAX = -1;
int SERVO_MIN = -1;
float SERVO_CURR_POS = 0.0;
boolean prevMoveInProgress = false;
int PWM_CURR_POS = 0;

int SERVO_MAXIMUM_RANGE = SERVO_MAXIMUM_RANGE_VALUE;
int SERVO_UP_MARIGIN = SERVO_UP_MARIGIN_VALUE;

#define SERVO_READ_INTERVAL_MILLIS 60000
int ServoLastReadInMillis = 0;
const int SERVOS = 1;

struct servo {
  String name;
  int pwtPin;
  int manualUpPin;
  int manualDownPin;
  int upLimitPin;
  int downLimitPin;
  String messageTopic;
  String messageValue;
  int currentPWM;
};
servo servos[SERVOS];

int sign(float value) {
  if (value > 0.0) {
    return 1;
  } else if (value < 0.0) {
    return -1;
  } else {
    return 0;
  }
}

void sendPositionMQTTMessage(String msg) {
  String messageTopic = GLOBAL_MQTT_MESSAGE_PREFIX + "/" +
                        GLOBAL_MQTT_MULTISENSOR_NAME + "_Position";
  sendMQTTWithTypeConversion(messageTopic, msg);
}

void initializeServo() {
  Serial.println("Initialize 360Servo...");
  iix = 0;
// name, pwtPin, manualUpPin, manualDownPin, upLimitPin, downLimitPin,
// messageTopic, messageValue, currentPWM
#ifdef FamilyRoomRollershutter
  servos[iix] = {"window", pinD2,         pinD6, pinD7, pinD1,
                 pinD5,    "servoWindow", "",    0};
  iix++;
#endif
#ifdef BedroomRollershutter
  servos[iix] = {"window", pinD2,         pinD7, pinD6, pinD5,
                 pinD1,    "servoWindow", "",    0};
  iix++;
#endif

  for (iix = 0; iix < SERVOS; iix = iix + 1) {
    pinMode(servos[iix].pwtPin, OUTPUT);
    pinMode(servos[iix].manualUpPin, INPUT_PULLUP);
    pinMode(servos[iix].manualDownPin, INPUT_PULLUP);
    pinMode(servos[iix].upLimitPin, INPUT_PULLUP);
    pinMode(servos[iix].downLimitPin, INPUT_PULLUP);
    Serial.println("Servo " + servos[iix].name + " pin " + servos[iix].pwtPin +
                   " set to OUTPUT");
    Serial.println("Servo " + servos[iix].name + " pins " +
                   servos[iix].manualUpPin + ", " + servos[iix].manualDownPin +
                   ", " + servos[iix].upLimitPin + ", " +
                   servos[iix].downLimitPin + " set to INPUT_PULLUP");
  }
}

void processServo() {
  // name, pwtPin, manualUpPin, manualDownPin, upLimitPin, downLimitPin,
  // messageTopic, messageValue, currentPWM

  for (iix = 0; iix < SERVOS; iix = iix + 1) {
    int manualUpValue = digitalRead(servos[iix].manualUpPin);
    int manualDownValue = digitalRead(servos[iix].manualDownPin);
    int upLimitValue = digitalRead(servos[iix].upLimitPin);
    int downLimitValue = digitalRead(servos[iix].downLimitPin);
    // String messageValue = servos[iix].messageValue;
    String messageValue = GLOBAL_MQTT_MSG;

    if (messageValue == "OFF") {
      messageValue = "AUTOUP";
    }

    if (messageValue == "ON") {
      messageValue = "AUTODOWN";
    }

    int upValue = SERVO_UP;
    int downValue = SERVO_DOWN;

    int currentPWM = 0;
    float currentDirection = 0.0;

    int realUpLimitValue = digitalRead(servos[iix].upLimitPin);
    int realDownLimitValue = digitalRead(servos[iix].downLimitPin);

    downLimitValue = HIGH; // hack for broken hardware

    boolean isPercent = (messageValue == "0" || messageValue.toInt() != 0);
    boolean isAuto = (messageValue == "AUTOUP" || messageValue == "AUTODOWN");
    boolean isManual = (manualUpValue == LOW || manualDownValue == LOW);
    boolean isDebug = (messageValue == "DEBUG");
    boolean isUp = (manualUpValue == LOW || messageValue == EFFECTIVE_UP ||
                    messageValue == EFFECTIVE_AUTO_UP);
    boolean isDown =
        (manualDownValue == LOW || messageValue == EFFECTIVE_DOWN ||
         messageValue == EFFECTIVE_AUTO_DOWN);

    float oldPosition = SERVO_CURR_POS;

    if (manualUpValue == LOW || messageValue == EFFECTIVE_UP ||
        messageValue == EFFECTIVE_AUTO_UP) {
      if (upLimitValue != LOW) {
        currentPWM = upValue;
        currentDirection = CURRENT_DIRECTION_UP;
      } else {
        currentPWM = 0;
        currentDirection = 0.0;
        GLOBAL_MQTT_MSG = "STOP";
      }
      if (manualUpValue == LOW) {
        currentPWM = upValue;
        currentDirection = CURRENT_DIRECTION_UP;
      }
    } else if (manualDownValue == LOW || messageValue == EFFECTIVE_DOWN ||
               messageValue == EFFECTIVE_AUTO_DOWN) {
      if (downLimitValue != LOW) {
        currentPWM = downValue;
        currentDirection = CURRENT_DIRECTION_DOWN;
      } else {
        currentPWM = 0;
        GLOBAL_MQTT_MSG = "STOP";
        currentDirection = 0.0;
      }
      if (manualDownValue == LOW) { // override by manual
        currentPWM = downValue;
        currentDirection = CURRENT_DIRECTION_DOWN;
      }
    } else if (isPercent) {
      long percentValue = messageValue.toInt();
      if (abs(SERVO_CURR_POS - messageValue.toInt()) > PERCENTAGE_DELTA) {
        if (sign(SERVO_CURR_POS - messageValue.toInt()) == -1) {
          currentPWM = downValue;
          currentDirection = CURRENT_DIRECTION_DOWN;
        } else {
          currentPWM = upValue;
          currentDirection = CURRENT_DIRECTION_UP;
        }
      } else {
        currentPWM = 0;
        GLOBAL_MQTT_MSG = "STOP";
        currentDirection = 0.0;
      }
    } else {
      currentPWM = 0;
      GLOBAL_MQTT_MSG = "STOP";
      currentDirection = 0.0;
    }

    if (manualDownValue == LOW || manualUpValue == LOW) {
      GLOBAL_MQTT_MSG = "STOP";
    }

    if (downLimitValue == LOW && messageValue == EFFECTIVE_DOWN) {
      GLOBAL_MQTT_MSG = "STOP";
    }

    if (upLimitValue == LOW && messageValue == EFFECTIVE_UP) {
      GLOBAL_MQTT_MSG = "STOP";
    }

    if (upLimitValue == LOW) {
      SERVO_MAX = SERVO_MAXIMUM_RANGE;
      SERVO_MIN = 0;
      SERVO_CURR_POS = SERVO_MIN;
      oldPosition = SERVO_CURR_POS;
      // isDebug = true;
    }

    if (downLimitValue == LOW) {
      SERVO_MAX = SERVO_MAXIMUM_RANGE;
      SERVO_MIN = 0;
      SERVO_CURR_POS = SERVO_MAX;
      oldPosition = SERVO_CURR_POS;
      // isDebug = true;
    }

    if (messageValue == "PRGUP" || messageValue == "PRGDOWN") {
      // Set oldPosition to fake to be sure, that current position will be sent
      // after servo programming.
      oldPosition = 0.1235;

      GLOBAL_MQTT_MSG = "STOP";
      currentPWM = 0;
      currentDirection = 0.0;
      if (messageValue == "PRGDOWN") {
        SERVO_MAX = SERVO_MAXIMUM_RANGE;
        SERVO_MIN = 0;
        SERVO_CURR_POS = SERVO_MAX;
        oldPosition = SERVO_CURR_POS;
      } else {
        SERVO_MAX = SERVO_MAXIMUM_RANGE;
        SERVO_MIN = 0;
        SERVO_CURR_POS = SERVO_MIN;
        oldPosition = SERVO_CURR_POS;
      }
      Serial.print("Up value: ");
      Serial.println(SERVO_MAX);
      Serial.print("Down value: ");
      Serial.println(SERVO_MIN);
      boolean isCalibrated_1 = ((SERVO_MAX != -1) && (SERVO_MIN != -1));
      if (isCalibrated_1) {
        Serial.print("Range set to: ");
        Serial.println(SERVO_MAX - SERVO_MIN);
      }
      isDebug = true;
    }

    boolean isCalibrated = ((SERVO_MAX != -1) && (SERVO_MIN != -1));

    if (isCalibrated && isAuto) {
      if ((SERVO_CURR_POS >= SERVO_MAX + SERVO_UP_MARIGIN &&
           messageValue == EFFECTIVE_AUTO_DOWN) ||
          (SERVO_CURR_POS <= SERVO_MIN && messageValue == EFFECTIVE_AUTO_UP)) {
        currentPWM = 0;
        GLOBAL_MQTT_MSG = "STOP";
        currentDirection = 0.0;
        Serial.println("Cancel auto message - out of range");
      }
    }

    if (!isCalibrated && isAuto) {
      currentPWM = 0;
      GLOBAL_MQTT_MSG = "STOP";
      currentDirection = 0.0;
      Serial.println("Cancel auto message - not calibrated");
    }

    if (currentPWM != 0) {
      delay(1);
      digitalWrite(servos[iix].pwtPin, HIGH);
      delayMicroseconds(currentPWM);
      digitalWrite(servos[iix].pwtPin, LOW);
      SERVO_CURR_POS += currentDirection;
      PWM_CURR_POS += sign(currentDirection);
    }

    boolean currMoveInProgress = ((oldPosition - SERVO_CURR_POS) != 0);
    if (prevMoveInProgress && !currMoveInProgress) {
      // servo was stopped during the cycle

      // correct to <0,100> for OpenHAB dimmer
      if (SERVO_CURR_POS > 100) {
        SERVO_CURR_POS = 100;
      }
      if (SERVO_CURR_POS < 0) {
        SERVO_CURR_POS = 0;
      }
      sendPositionMQTTMessage(String(SERVO_CURR_POS));
      isDebug = true; // send debug messages if move is ended
    } else {
      if (abs(millis() - ServoLastReadInMillis) > SERVO_READ_INTERVAL_MILLIS) {
        isDebug = true;
        ServoLastReadInMillis = millis();
      }
    }

    if (currentDirection != 0.0) {
      delay(LOOP_DELAY_VALUE);
    }

    if (isDebug) {
      Serial.println("Send debug messages");
      sendDebugMQTTMessage("messageValue", String(messageValue));
      sendDebugMQTTMessage("CurrPos", String(SERVO_CURR_POS));
      sendDebugMQTTMessage("PWMCurrPos", String(PWM_CURR_POS));
      sendDebugMQTTMessage("MinLimit", String(SERVO_MIN));
      sendDebugMQTTMessage("MaxLimit", String(SERVO_MAX));
      sendDebugMQTTMessage("UpLimitProbe (LOW active)", String(upLimitValue));
      sendDebugMQTTMessage("DownLimitProbe (LOW active)",
                           String(downLimitValue));
      sendDebugMQTTMessage("RealUpLimitProbe (LOW active)",
                           String(realUpLimitValue));
      sendDebugMQTTMessage("RealDownLimitProbe (LOW active)",
                           String(realDownLimitValue));
      sendDebugMQTTMessage("Millis", String(millis()));
    }

    prevMoveInProgress = currMoveInProgress;
  }
}
#endif
#endif