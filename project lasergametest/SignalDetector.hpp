#ifndef SIGNALDETECTOR_HPP
#define SIGNALDETECTOR_HPP


#include "hwlib.hpp"
#include "MSGdecoder.hpp"
#include "rtos.hpp"


class SignalDetector : public rtos::task<>{
private:
    hwlib::pin_in & IRreceiver;
    MSGdecoder & msgdecoder;
    enum states {idle, count};
    rtos::clock PollClock;
    
    void main(){
        bool signal;
        int n;
        states state = idle;
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600);
        msgdecoder.SignalReceived(1600); 
        hwlib::wait_us(1000);
        for(;;){
            switch (state){
                case idle:
                    wait(PollClock);
                    IRreceiver.refresh();
                    signal = IRreceiver.read();
                    if(signal){
                        state = idle;
                    }
                    else{
                        n = 0;
                        state = count;
                    }
                    break;
                case count:
                    wait(PollClock);
                    IRreceiver.refresh();
                    signal = IRreceiver.read();
                    if(signal){
                        msgdecoder.SignalReceived(n);
                        state = idle;
                    }
                    else{
                        n += 100;
                        state = count;
                    }
                    break;
            }
        }
    }  

public:
    SignalDetector(MSGdecoder & msgdecoder, hwlib::pin_in & IRreceiver):
    task(1, "SignalDetector"),
    IRreceiver(IRreceiver),
    msgdecoder(msgdecoder),
    PollClock(this,100,"PollClock")
    {}
};

#endif // SIGNALDETECTOR_HPP