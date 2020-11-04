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
    rtos::channel<int, 94> signalchannel;
    rtos::timer BitTimer;

    void main(){
        uint32_t signaltime; // kan niet negatief zijn dus uint
        uint16_t msg;
        int i;
        uint16_t value;
        states state = idle;
        for(;;){
            switch (state){
                case idle:
                    signalchannel.clear();
                    wait(signalchannel);
                    signaltime = signalchannel.read();
                    msg = 0;
                    i = 0;
                    state = BitReceived;
                    break;
                case BitReceived:
                    if(signaltime < 1200 && signaltime > 200){ // de kleinder dan heeft geen nut, als er ene signaal binnen komt dat niet klopt dan wordt deze toch al meegenomen
                        value = 0;
                    }
                    else if(signaltime > 1200 && signaltime < 2000){ // moet de definitieve else zijn, anders dan weet je niet wat value is 
                        value = 1;
                    }
                    else{
                        state = idle;
                        BitTimer.cancel();
                        break;
                    }
                    msg = msg | value;
                    i++;
                    if(i == 16){
                        msgverifier.reportmessage(msg);
                        state = idle;
                        break;
                    }
                    BitTimer.set(3700);
                    auto event = (signalchannel+ BitTimer);
                    if(event == BitTimer){
                        state = idle;
                    }
                    else{
                        BitTimer.cancel();
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