#ifndef KEYPAD_HPP
#define KEYPAD_HPP


class Keypad : public rtos::task<> {
private:
    rtos::clock keypadclock;

    KeypadListener* listeners[2];
    unsigned int alisteners = 0;

    hwlib::keypad< 16 > & hwlibkeypad;



public:
    Keypad(hwlib::keypad< 16 > & hwlibkeypad):
    task(6, "Keypad"),
    keypadclock(this, 200000, "keypadclock"),
    hwlibkeypad (hwlibkeypad)
    {}

    void main(){
        char c;
        for(;;){
            wait(keypadclock);
            c = hwlibkeypad.pressed();
            if(c != '\0'){
                for(unsigned int i = 0; i < alisteners; i++){
                    listeners[i]->ButtonPressed(c);
                }
            }
        }
    }

    void addlistener(KeypadListener *l){
        listeners[alisteners] = l;
        alisteners++;
    }


};


#endif // KEYPAD_HPP
