#ifndef watchdog_h
#define watchdog_h
#include <Ticker.h>

Ticker secondTick;
volatile int watchdogCount = 0;
const int WATCHDOG_TRIGGER = 120;

void ISRwatchdog()
{
  watchdogCount++;
  if (watchdogCount >= WATCHDOG_TRIGGER)
  {
    ESP.restart();
  }
}

void initializeWatchdog()
{
  Serial.println("Initialize Watchdog...");
  secondTick.attach(1, ISRwatchdog);
}

void processWatchdog()
{
  watchdogCount = 0;
}
#endif
