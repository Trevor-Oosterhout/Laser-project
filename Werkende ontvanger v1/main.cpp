#include "hwlib.hpp"
#include "rtos.hpp"
#include "SignalDetector.hpp"
#include "MSGdecoder.hpp"
#include "MSGverifier.hpp"



int main(){
    hwlib::wait_ms(2000);
    hwlib::cout << "Het programma is gestart" << "\n";
    hwlib::wait_ms(1000);
    auto receiver = hwlib::target::pin_in(hwlib::target::pins::d31);
    auto verifier = MSGverifier();
    auto decoder = MSGdecoder(verifier);
    auto detector = SignalDetector(decoder,receiver);
    rtos::run();
}