#ifndef IRMESSAGECONTROL_HPP
#define IRMESSAGECONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include <stdint.h>
#include "pinout.hpp"


/// @file

// this file contains doxygen lines

/// \brief
/// IRmessageControl class
/// \details
/// this class turns the IRLED on and off for the correct amount of time to sendsignal
/// an IR message to an other player with our protocol

class IRmessageControl : public rtos::task<>{
private:
    hwlib::pin_out & irled;
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

    void sentmessage(uint16_t bits){
        for(int i = 15 ; i >= 0; --i) {
		    if(bits & (1 << i)){
			    sentbit(1);
		    }
		    else{
			    sentbit(0);
		    }
	    }
    }

    void sendmessage(uint16_t message){
        sentmessage(message);
        hwlib::wait_ms(4);
        sentmessage(message);
    }

    void main(){
        for(;;){
            uint16_t message = IRmessageChannel.read();
            sendmessage(message);
        }
    }

public:
    IRmessageControl(hwlib::pin_out & irled):
        task(5,"IRmessageControl.hpp"),
        irled(irled),
        IRmessageChannel(this,"message buffer")
    {}

    /// \brief
    /// writes in the IRmessageChannel
    /// \details
    /// writes 2 bytes to the IRmessageChannel to send to other players
    void sendsignal(uint16_t msg){
        IRmessageChannel.write(msg);
    }

};

#endif // IRMESSAGECONTROL_HPP
