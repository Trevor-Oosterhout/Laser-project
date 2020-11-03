#include "hwlib.hpp"
#include "rtos.hpp"
#include "SignalDetector.hpp"
#include "MSGdecoder.hpp"
#include "MSGverifier.hpp"



int main(){
    auto keypadrow_0 = hwlib::target::pin_oc(hwlib::target::pins::d40);
    auto keypadrow_1 = hwlib::target::pin_oc(hwlib::target::pins::d38);
    auto keypadrow_2 = hwlib::target::pin_oc(hwlib::target::pins::d36);
    auto keypadrow_3 = hwlib::target::pin_oc(hwlib::target::pins::d34);

    auto keypadrcol_0 = hwlib::target::pin_in(hwlib::target::pins::d32);
    auto keypadrcol_1 = hwlib::target::pin_in(hwlib::target::pins::d30);
    auto keypadrcol_2 = hwlib::target::pin_in(hwlib::target::pins::d28);
    auto keypadrcol_3 = hwlib::target::pin_in(hwlib::target::pins::d26);

    auto keyrows =  hwlib::port_oc_from_pins_t(keypadrow_0, keypadrow_1, keypadrow_2, keypadrow_3);
    auto keycolums =  hwlib::port_in_from_pins_t(keypadrcol_0, keypadrcol_1, keypadrcol_2, keypadrcol_3);

    auto keypad_matrix = hwlib::matrix_of_switches(keyrows, keycolums);

    auto keypad = hwlib::keypad< 16 >(keypad_matrix, "D#0*C987B654A321");

    char c;
    for(;;){
        c = keypad.pressed();
        if(c != '\0'){
            hwlib::cout << c;
        }
    }


}
