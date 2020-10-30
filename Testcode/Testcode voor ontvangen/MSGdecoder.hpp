#ifndef MSGDECODER_HPP
#define MSGDECODER_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "MSGdecoder.hpp"
#include "MSGverifier.hpp"
#include <stdint.h>


class MSGdecoder : public rtos::task<>{
private:
    enum states {idle, BitReceived};
    MSGverifier & msgverifier;
    rtos::channel<int, 32> signalchannel;
    rtos::timer BitTimer;

    void main(){
        int signaltime;
        uint16_t msg;
        int i;
        uint16_t value;
        states state = idle;
        for(;;){
            switch (state){
                case idle:
                    signaltime = signalchannel.read();
                    hwlib::cout << signaltime << "\n";
                    msg = 0;
                    i = 0;
                    state = BitReceived;
                    break;
                case BitReceived:
                    if(signaltime < 900 && signaltime < 700){
                        value = 0;
                    }
                    else if(signaltime > 1500){
                        value = 1;
                    }
                    msg = (msg | value);
                    i++;
                    if(i == 15){
                        msgverifier.reportmessage(msg);
                        state = idle;
                        break;
                    }
                    BitTimer.set(3000);
                    auto event = (signalchannel+ BitTimer);
                    if(event == BitTimer){
                        state = idle;
                    }
                    else{
                        signaltime = signalchannel.read();
                        msg = (msg << 1);
                        state = BitReceived;
                    }
                    break;
            }
        }

    }

public:
    MSGdecoder(MSGverifier & msgverifier):
    task(2,"MSGdecoder"),
    msgverifier(msgverifier),
    signalchannel(this, "signalchannel"),
    BitTimer(this,"BitTimer")
    {}

    void SignalReceived(int n){
        signalchannel.write(n);
    }
};


#endif