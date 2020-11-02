#ifndef IRMESSAGECONTROL_HPP
#define IRMESSAGECONTROL_HPP
#include "hwlib.hpp"
#include "rtos.hpp"
#include <stdint.h>
#include "pinout.hpp"

class IRmessageControl : public rtos::task<>{
private:
    hwlib::pin_out & irled; // zorg wel dat deze een d2_38khz/d2_36khz is
    rtos::channel< uint16_t, 10> IRmessageChannel;

    void sentbit(bool bit){
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

    void sentmessage(uint16_t bits){ // deze gebruik je niet let dus op je spelling
	    for(int i = 15 ; i >= 0; --i) {
		    if(bits & (1 << i)){
			    sentbit(1);
		    }
		    else{
			    sentbit(0);
		    }
	    }
    }

    void sendmessage(uint16_t message){ // deze gebruik je
        sentmessage(message);
        hwlib::wait_ms(4); // small break between messages (gets rid of corrupt messages in the receiver)
        sentmessage(message);
    }

    void main(){
        for(;;){
            uint16_t message = IRmessageChannel.read();
            sendmessage(message);
        }
    }

public:
    IRmessageControl(hwlib::pin_out & irled): // geef dus de constructor een d2_38khz mee
        task(5,"IRmessageControl.hpp"),
        irled(irled),
        IRmessageChannel(this,"message buffer")
    {}

    void sendsignal(uint16_t msg){
        IRmessageChannel.write(msg);
    }
};
#endif // IRMESSAGECONTROL_HPP
