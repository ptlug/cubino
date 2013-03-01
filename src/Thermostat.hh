#ifndef THERMOSTAT_HH
#define THERMOSTAT_HH

#include "DebouncedButton.hh"

#define ADDR_TEMPERATURE_1 0x06
#define ADDR_TEMPERATURE_2 0x07
#define TEMP_INCREMENT 0.5
#define TEMP_DEFAULT 20.0
#define TEMP_DELTA 2.0
#define ALARM_DELTA 1.5

class LiquidCrystal;
class OneWire;
class DallasTemperature;

class Thermostat {

    private:
        float temperature;
        float intTemp;
        float extTemp;
        int lock[3] = {0, 0, 0};
        int pinSwitch, pinTempBus;
        int pinAlarm;
        int alarmStatus;
        DebouncedButton* btnInc;
        DebouncedButton* btnDec;
        DebouncedButton* btnSave;
        LiquidCrystal* display;
        OneWire* tempBus;
        DallasTemperature* tempSensors;

        char* formatTemperature(float num, char *buf);

    public:
        Thermostat(int pinSwitch, int pinTempBus);
        void setup();
        void setTemperature(float temperature);
        float getTemperature();
        float getExtTemperature();
        float getIntTemperature();
        void increment();
        void decrement();
        void save();
        void load();
        void playAlarm();
        void stopAlarm();
        void processInput();
        void processTemperature();
        void displayTemperature();
        void attachAlarm(int pinAlarm);
        void attachLcd(LiquidCrystal *lcd, int columns, int rows);
        void attachControlButtons(int pinBtnDec, int pinBtnInc, int pinBtnSave);

};

#endif
