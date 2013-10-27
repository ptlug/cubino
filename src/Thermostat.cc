#include "Thermostat.hh"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <a18n.h>

Thermostat::Thermostat(int pinSwitch, int pinTempBus) {
    intTemp = extTemp = 0.0;
    btnDec = btnSave = btnInc = 0;
    display = 0;
    temperature = TEMP_DEFAULT;
    this->pinSwitch = pinSwitch;
    this->pinTempBus = pinTempBus;
    pinAlarm = -1;
    imanager = new a18n(LOCALE, translations);
}

void Thermostat::setup() {
    load();
    tempBus = new OneWire(pinTempBus);
    tempSensors = new DallasTemperature(tempBus);
    pinMode(pinSwitch, OUTPUT);
}

void Thermostat::attachAlarm(int pinAlarm) {
    this->pinAlarm = pinAlarm;
    pinMode(pinAlarm, OUTPUT);
}

void Thermostat::attachLcd(LiquidCrystal *lcd, int columns, int rows) {
    display = lcd;
    lcd->begin(columns, rows);
}

void Thermostat::attachControlButtons(int pinBtnDec, int pinBtnInc, int pinBtnSave) {

    btnDec = new DebouncedButton(pinBtnDec);
    btnInc = new DebouncedButton(pinBtnInc);
    btnSave = new DebouncedButton(pinBtnSave);

    btnDec->setMode(INPUT);
    btnInc->setMode(INPUT);
    btnSave->setMode(INPUT);

}

void Thermostat::setTemperature(float temperature) {
    this->temperature = temperature;
}

float Thermostat::getTemperature() {
    return temperature;
}

float Thermostat::getExtTemperature() {
    return extTemp;
}

float Thermostat::getIntTemperature() {
    return intTemp;
}

void Thermostat::increment() {
    temperature += TEMP_INCREMENT;
}

void Thermostat::decrement() {
    temperature -= TEMP_INCREMENT;
}

void Thermostat::save() {
    int part_1 = (int)(temperature);
    int part_2 = (int)((temperature - part_1) * 10);
    EEPROM.write(ADDR_TEMPERATURE_1, part_1);
    EEPROM.write(ADDR_TEMPERATURE_2, part_2);
}

void Thermostat::load() {
    int part_1 = EEPROM.read(ADDR_TEMPERATURE_1);
    int part_2 = EEPROM.read(ADDR_TEMPERATURE_2);

    if(part_1 == 255 && part_2 == 255) {
        part_1 = 25;
        part_2 = 5;
    }

    temperature = (float)(part_1 + (part_2 * 0.1));
}

void Thermostat::displayTemperature() {

    if(display == 0)
        return;

    char row[16];
    char buf1[10], buf2[10];
    sprintf(row, "In:%s  Ex:%s", formatTemperature(intTemp, buf1), formatTemperature(extTemp, buf2));
    display->setCursor(0, 0);
    display->print(row);
    const char* translation = imanager->getTranslation("limit");
    sprintf(row, "%s: %s", translation, formatTemperature(temperature, buf1));
    display->setCursor(0, 1);
    display->print(row);
}

char* Thermostat::formatTemperature(float num, char* buf) {
    dtostrf(num, 1, 1, buf);
    return buf;
}

void Thermostat::processInput() {

    if (btnInc != 0 && btnInc->digitRead() == HIGH && lock[0] != 1) {    
        increment();
        lock[0] = 1;
    } else if (btnInc != 0 && btnInc->digitRead() == LOW) {
        lock[0] = 0;
    }

    if (btnDec != 0 && btnDec->digitRead() == HIGH && lock[1] != 1) {   
        decrement();
        lock[1] = 1;
    } else if (btnDec != 0 && btnDec->digitRead() == LOW) {
        lock[1] = 0;
    }

    if (btnSave != 0 && btnSave->digitRead() == HIGH && lock[2] != 1) {    
        save();
        lock[2] = 1;
    } else if (btnSave != 0 && btnSave->digitRead() == LOW) {
        lock[2] = 0;
    }

}

void Thermostat::processTemperature() {

    if(lock[0] != 0 || lock[1] != 0)
        return;

    tempSensors->requestTemperatures();
    intTemp = (float)tempSensors->getTempCByIndex(1);
    extTemp = (float)tempSensors->getTempCByIndex(0);

    if(intTemp > temperature + ALARM_DELTA) {
        if(pinAlarm != -1)
            playAlarm();
    } else {
        if(pinAlarm != -1)
            stopAlarm();
    }

    if(intTemp > temperature) {
        digitalWrite(pinSwitch, LOW);
    }
    if(intTemp < temperature - TEMP_DELTA) {
        digitalWrite(pinSwitch, HIGH);
    }

}

void Thermostat::playAlarm() {
    digitalWrite(pinAlarm, alarmStatus);
    if(alarmStatus == HIGH)
        alarmStatus = LOW;
    else
        alarmStatus = HIGH;
}

void Thermostat::stopAlarm() {
    alarmStatus = LOW;
    digitalWrite(pinAlarm, LOW);
}


