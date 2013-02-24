#include "DebouncedButton.hh"
#include <Arduino.h>

DebouncedButton::DebouncedButton(int pin) {
    this->pin = pin;
    this->debounceDelay = 50;
    this->lastButtonState = LOW;
    this->lastDebounceTime = 0;
    this->buttonState = LOW;
}

void DebouncedButton::setMode(int mode) {
    pinMode(pin, mode);
}

int DebouncedButton::digitRead() {

    int reading = digitalRead(pin);

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        buttonState = reading;
    }

    lastButtonState = reading;

    return buttonState;

}

void DebouncedButton::digitWrite(int data) {

    digitalWrite(pin, data);
}
