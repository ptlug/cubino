#ifndef DEBOUNCEDBTN_HH
#define DEBOUNCEDBTN_HH

class DebouncedButton {

    private:
        int buttonState;
        long lastDebounceTime;
        int lastButtonState;
        long debounceDelay;
        int pin;

    public:
        DebouncedButton(int pin);
        void setMode(int mode);
        int digitRead();
        void digitWrite(int data);

};

#endif
