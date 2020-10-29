#include "hwlib.hpp"
#include "IRmessageControl.hpp"
#include "pinout.hpp"
#include "SignalDetector.hpp"
#include "MSGdecoder.hpp"





int main(){
   auto ir = hwlib::d2_38kHz();

   IRmessageControl messagecontrol(ir);
   
   
   /*
   auto receivervoeding = hwlib::target::pin_out(hwlib::target::pins::d5);
   auto receiverinput = hwlib::target::pin_in(hwlib::target::pins::d9);
   receivervoeding.write(1);
   receivervoeding.flush();

    for(;;){
        receiverinput.refresh();
        if(receiverinput.read() ==1){
            hwlib::cout << "ik zie signaal" << "\n";

        }
        else{
            hwlib::cout << "ik zie geen signaal" << "\n";
        }
        hwlib::wait_ms(1000);
    }

*/

}