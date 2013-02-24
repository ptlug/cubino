#include <Arduino.h>
#include <LiquidCrystal.h>
#include "Thermostat.hh"

Thermostat th(13, 6);

void setup() {
    th.setup();
    th.attachControlButtons(2, 3, 4);
    th.attachLcd(new LiquidCrystal(7, 8, 9, 10, 11, 12), 16, 2);
}

void loop() {
    th.processInput();
    th.processTemperature();
    th.displayTemperature();
}
