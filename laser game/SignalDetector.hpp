#ifndef SIGNALDETECTOR_HPP
#define SIGNALDETECTOR_HPP


#include "hwlib.hpp"
#include "MSGdecoder.hpp"
#include "rtos.hpp"

/// @file

// this file contains doxygen lines


/// \brief
/// rtos task SignalDetector
/// \details
/// This class is a rtos task.
/// This task implements receiving the IR signal,
/// and sends valid signals to msgdecoder.


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
