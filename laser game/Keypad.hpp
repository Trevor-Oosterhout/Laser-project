#ifndef KEYPAD_HPP
#define KEYPAD_HPP


/// @file

// this file contains doxygen lines

/// \brief
/// keypad class
/// \details
/// this class checks if the keypad is pressed and sends the information to its listeners


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

    /// \brief
    /// adds a listener
    /// \details
    /// adds a listener to send data to
    void addlistener(KeypadListener *l){
        listeners[alisteners] = l;
        alisteners++;
    }


};


#endif // KEYPAD_HPP
