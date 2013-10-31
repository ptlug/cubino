#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Logging.h>
#include "Thermostat.hh"

#define LOGLEVEL LOG_LEVEL_DEBUG

Thermostat th(13, 6);

void setup() {
    th.setup();
    th.attachControlButtons(2, 3, 4);
    th.attachAlarm(5);
    th.attachLcd(new LiquidCrystal(7, 8, 9, 10, 11, 12), 16, 2);
    Log.Init(LOGLEVEL, 9600L);
}

void loop() {
    Log.Info("Start cycle"CR);
    th.processInput();
    th.processTemperature();
    th.displayTemperature();
    Log.Info("End cycle"CR);
}
