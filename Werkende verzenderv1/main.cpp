#include "hwlib.hpp"
#include "pinout.hpp"
#include "stdint.h"

uint16_t createmessage(uint16_t playernumber, uint16_t data){
    uint16_t msg = 0;
    uint16_t controlebits = 0;
    uint16_t playerbits = 0;
    uint16_t databits = 0;
    controlebits = playernumber ^ data;
    playerbits = playernumber << 10;
    databits = data << 5;
    msg = msg | controlebits;
    msg = msg | playerbits;
    msg = msg | databits;
    return msg;
}

void sentbit(bool bit,hwlib::target::d2_36kHz & irled){
    if(!bit){
        irled.write(1);
        irled.flush();
        hwlib::wait_us(800);
        irled.write(0);
        irled.flush();
        hwlib::wait_us(1600);
    }
    else{
        irled.write(1);
        irled.flush();
        hwlib::wait_us(1600);
        irled.write(0);
        irled.flush();
        hwlib::wait_us(800);
    }
}

void sentmessage(uint16_t bits,hwlib::target::d2_36kHz & irled){ // deze gebruik je niet let dus op je spelling
    for(int i = 15 ; i >= 0; --i) {
		if(bits & (1 << i)){
			sentbit(1,irled);
		}
		else{
		    sentbit(0,irled);
		}
	}
}

void sendmessage(uint16_t message,hwlib::target::d2_36kHz & irled){ // deze gebruik je 
    sentmessage(message,irled);
    hwlib::wait_ms(4); // small break between messages (gets rid of corrupt messages in the receiver)
    sentmessage(message,irled);
}


int main(){
   auto ir = hwlib::target::d2_36kHz();
   auto button = hwlib::target::pin_in(hwlib::target::pins::d22);

   int spelernummer = 10;
   int teversturendata = 15;

   uint16_t data = createmessage(spelernummer,teversturendata);


   for(;;){
       button.refresh();
       if(button.read() == 1){
           sendmessage(data,ir);
       }
       hwlib::wait_ms(300);
   }
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