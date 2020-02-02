#ifndef stepper_h
#define stepper_h
#ifdef fStepperMotor
#define EFFECTIVE_AUTO_DOWN "AUTODOWN"
#define EFFECTIVE_AUTO_UP "AUTOUP"
#define EFFECTIVE_DOWN "DOWN"
#define EFFECTIVE_UP "UP"

#define PERCENTAGE_DELTA 0.5

int STEPPER_MAX = -1;
int STEPPER_MIN = -1;
float STEPPER_CURR_POS = 0.0;
boolean prevMoveInProgress = false;
int STEP_CURR_POS = 0;

int STEPPER_MAXIMUM_RANGE = STEPPER_MAXIMUM_RANGE_VALUE;
int STEPPER_UP_MARIGIN = STEPPER_UP_MARIGIN_VALUE;

const int STEPPERS = 1;

struct stepper {
  String name;
  int stepperPin;
  int dirPin;
  int enablePin;
  int manualUpPin;
  int manualDownPin;
  int upLimitPin;
  int downLimitPin;
  String messageTopic;
  String messageValue;
  int currentStepper;
};
stepper steppers[STEPPERS];

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

void initializeStepperMotor() {
  Serial.println("Initialize StepperMotor...");
  iix = 0;

  // name, stepperPin, dirPin, enablePin, manualUpPin, manualDownPin,
  // upLimitPin, downLimitPin, messageTopic, messageValue, currentStepper
  steppers[iix] = {"window", pinD3, pinD4,           pinD2, pinD6, pinD7,
                   pinD1,    pinD5, "stepperWindow", "",    0};
  iix++;

  for (iix = 0; iix < STEPPERS; iix = iix + 1) {
    pinMode(steppers[iix].stepperPin, OUTPUT);
    pinMode(steppers[iix].dirPin, OUTPUT);
    pinMode(steppers[iix].enablePin, OUTPUT);
    pinMode(steppers[iix].manualUpPin, INPUT_PULLUP);
    pinMode(steppers[iix].manualDownPin, INPUT_PULLUP);
    pinMode(steppers[iix].upLimitPin, INPUT_PULLUP);
    pinMode(steppers[iix].downLimitPin, INPUT_PULLUP);
    Serial.println("Stepper (stepper) " + steppers[iix].name + " pin " +
                   steppers[iix].stepperPin + " set to OUTPUT");
    Serial.println("Stepper (dir) " + steppers[iix].name + " pin " +
                   steppers[iix].dirPin + " set to OUTPUT");
    Serial.println("Stepper (enable) " + steppers[iix].name + " pin " +
                   steppers[iix].enablePin + " set to OUTPUT");
    Serial.println(
        "Stepper " + steppers[iix].name + " pins " + steppers[iix].manualUpPin +
        ", " + steppers[iix].manualDownPin + ", " + steppers[iix].upLimitPin +
        ", " + steppers[iix].downLimitPin + " set to INPUT_PULLUP");
    digitalWrite(steppers[iix].enablePin, HIGH);
  }
}

void processStepperMotor() {
  // name, stepperPin, manualUpPin, manualDownPin, upLimitPin, downLimitPin,
  // messageTopic, messageValue, currentStepper

  for (iix = 0; iix < STEPPERS; iix = iix + 1) {
    int manualUpValue = digitalRead(steppers[iix].manualUpPin);
    int manualDownValue = digitalRead(steppers[iix].manualDownPin);
    int upLimitValue = digitalRead(steppers[iix].upLimitPin);
    int downLimitValue = digitalRead(steppers[iix].downLimitPin);
    // String messageValue = steppers[iix].messageValue;
    String messageValue = GLOBAL_MQTT_MSG;

    if (messageValue == "OFF") {
      messageValue = "AUTOUP";
    }

    if (messageValue == "ON") {
      messageValue = "AUTODOWN";
    }

    int upValue = STEPPER_UP;
    int downValue = STEPPER_DOWN;

    int currentStepper = 0;
    float currentDirection = 0.0;

    int realUpLimitValue = digitalRead(steppers[iix].upLimitPin);
    int realDownLimitValue = digitalRead(steppers[iix].downLimitPin);

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

    float oldPosition = STEPPER_CURR_POS;

    if (manualUpValue == LOW || messageValue == EFFECTIVE_UP ||
        messageValue == EFFECTIVE_AUTO_UP) {
      if (upLimitValue != LOW) {
        currentStepper = upValue;
        currentDirection = CURRENT_DIRECTION_UP;
      } else {
        currentStepper = 0;
        currentDirection = 0.0;
        GLOBAL_MQTT_MSG = "STOP";
      }
      if (manualUpValue == LOW) {
        currentStepper = upValue;
        currentDirection = CURRENT_DIRECTION_UP;
      }
    } else if (manualDownValue == LOW || messageValue == EFFECTIVE_DOWN ||
               messageValue == EFFECTIVE_AUTO_DOWN) {
      if (downLimitValue != LOW) {
        currentStepper = downValue;
        currentDirection = CURRENT_DIRECTION_DOWN;
      } else {
        currentStepper = 0;
        GLOBAL_MQTT_MSG = "STOP";
        currentDirection = 0.0;
      }
      if (manualDownValue == LOW) { // override by manual
        currentStepper = downValue;
        currentDirection = CURRENT_DIRECTION_DOWN;
      }
    } else if (isPercent) {
      long percentValue = messageValue.toInt();
      if (abs(STEPPER_CURR_POS - messageValue.toInt()) > PERCENTAGE_DELTA) {
        if (sign(STEPPER_CURR_POS - messageValue.toInt()) == -1) {
          currentStepper = downValue;
          currentDirection = CURRENT_DIRECTION_DOWN;
        } else {
          currentStepper = upValue;
          currentDirection = CURRENT_DIRECTION_UP;
        }
      } else {
        currentStepper = 0;
        GLOBAL_MQTT_MSG = "STOP";
        currentDirection = 0.0;
      }
    } else {
      currentStepper = 0;
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
      STEPPER_MAX = STEPPER_MAXIMUM_RANGE;
      STEPPER_MIN = 0;
      STEPPER_CURR_POS = STEPPER_MIN;
      oldPosition = STEPPER_CURR_POS;
      // isDebug = true;
    }

    if (downLimitValue == LOW) {
      STEPPER_MAX = STEPPER_MAXIMUM_RANGE;
      STEPPER_MIN = 0;
      STEPPER_CURR_POS = STEPPER_MAX;
      oldPosition = STEPPER_CURR_POS;
      // isDebug = true;
    }

    if (messageValue == "PRGUP" || messageValue == "PRGDOWN") {
      // Set oldPosition to fake to be sure, that current position will be sent
      // after stepper programming.
      oldPosition = 0.1235;

      GLOBAL_MQTT_MSG = "STOP";
      currentStepper = 0;
      currentDirection = 0.0;
      if (messageValue == "PRGDOWN") {
        STEPPER_MAX = STEPPER_MAXIMUM_RANGE;
        STEPPER_MIN = 0;
        STEPPER_CURR_POS = STEPPER_MAX;
        oldPosition = STEPPER_CURR_POS;
      } else {
        STEPPER_MAX = STEPPER_MAXIMUM_RANGE;
        STEPPER_MIN = 0;
        STEPPER_CURR_POS = STEPPER_MIN;
        oldPosition = STEPPER_CURR_POS;
      }
      Serial.print("Up value: ");
      Serial.println(STEPPER_MAX);
      Serial.print("Down value: ");
      Serial.println(STEPPER_MIN);
      boolean isCalibrated_1 = ((STEPPER_MAX != -1) && (STEPPER_MIN != -1));
      if (isCalibrated_1) {
        Serial.print("Range set to: ");
        Serial.println(STEPPER_MAX - STEPPER_MIN);
      }
      isDebug = true;
    }

    boolean isCalibrated = ((STEPPER_MAX != -1) && (STEPPER_MIN != -1));

    if (isCalibrated && isAuto) {
      if ((STEPPER_CURR_POS >= STEPPER_MAX + STEPPER_UP_MARIGIN &&
           messageValue == EFFECTIVE_AUTO_DOWN) ||
          (STEPPER_CURR_POS <= STEPPER_MIN &&
           messageValue == EFFECTIVE_AUTO_UP)) {
        currentStepper = 0;
        GLOBAL_MQTT_MSG = "STOP";
        currentDirection = 0.0;
        Serial.println("Cancel auto message - out of range");
      }
    }

    if (!isCalibrated && isAuto) {
      currentStepper = 0;
      GLOBAL_MQTT_MSG = "STOP";
      currentDirection = 0.0;
      Serial.println("Cancel auto message - not calibrated");
    }

    if (currentStepper != 0) {
      digitalWrite(steppers[iix].enablePin, LOW); // enable driver
      if (sign(currentDirection) > 0) {
        digitalWrite(steppers[iix].dirPin, LOW);
      } else {
        digitalWrite(steppers[iix].dirPin, HIGH);
      }
      digitalWrite(steppers[iix].stepperPin, HIGH);
      // delayMicroseconds(currentStepper);
      delayMicroseconds(1500);
      digitalWrite(steppers[iix].stepperPin, LOW);
      delayMicroseconds(1500);
      STEPPER_CURR_POS += currentDirection;
      STEP_CURR_POS += sign(currentDirection);
      //      Serial.println("step");
    } else {
      digitalWrite(steppers[iix].enablePin, HIGH); // disable driver
    }

    if (isDebug) {
      Serial.println("Send debug messages");
      sendDebugMQTTMessage("messageValue", String(messageValue));
      sendDebugMQTTMessage("CurrPos", String(STEPPER_CURR_POS));
      sendDebugMQTTMessage("StepCurrPos", String(STEP_CURR_POS));
      sendDebugMQTTMessage("MinLimit", String(STEPPER_MIN));
      sendDebugMQTTMessage("MaxLimit", String(STEPPER_MAX));
      sendDebugMQTTMessage("UpLimitProbe (LOW active)", String(upLimitValue));
      sendDebugMQTTMessage("DownLimitProbe (LOW active)",
                           String(downLimitValue));
      sendDebugMQTTMessage("RealUpLimitProbe (LOW active)",
                           String(realUpLimitValue));
      sendDebugMQTTMessage("RealDownLimitProbe (LOW active)",
                           String(realDownLimitValue));
      sendDebugMQTTMessage("Millis", String(millis()));
    }

    boolean currMoveInProgress = ((oldPosition - STEPPER_CURR_POS) != 0);
    if (prevMoveInProgress && !currMoveInProgress) {
      // stepper was stopped during the cycle

      // correct to <0,100> for OpenHAB dimmer
      if (STEPPER_CURR_POS > 100) {
        STEPPER_CURR_POS = 100;
      }
      if (STEPPER_CURR_POS < 0) {
        STEPPER_CURR_POS = 0;
      }
      sendPositionMQTTMessage(String(STEPPER_CURR_POS));
    }

    if (currentDirection != 0.0) {
      delay(LOOP_DELAY_VALUE);
    }
    prevMoveInProgress = currMoveInProgress;
  }
}
#endif
#endif