#ifndef LEADERCONTROL_HPP
#define LEADERCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadListener.hpp"
// #include "Functie.hpp"


class LeaderControl : public KeypadListener, public rtos::task<>{
private:
    rtos::channel<char, 16> keypadchannel;
    rtos::timer secondtimer;

    GameTime & gametime;
    Display & display;
    IRmessageControl & irmessagecontrol;

public:
    LeaderControl(GameTime & gametime, Display & display, IRmessageControl & irmessagecontrol):
    task (10, "LeaderControl"),
    gametime ( gametime ),
    display ( display ),
    irmessagecontrol ( irmessagecontrol ),
    keypadchannel(this, "keypadchannel"),
    secondtimer(this, "secondtimer")
    {}



    void ButtonPressed(char c) override{
        keypadchannel.write(c);
    }

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


    void main(){
        enum states {idle, maketime, sendstart, sendmessage};
        states state = idle;
        char input;
        uint16_t roundtime = 0;
        uint16_t temp = 0;
        int dubbel = 0;
        uint16_t cmd;
        for(;;){
            switch(state){
                case idle:
                    input = keypadchannel.read();
                    if(input == 'C'){
                        state = maketime;
                    }
                    break;
                case maketime:
                    input = keypadchannel.read();
                    if(input >= '1' && input <= '9'){
                        temp = input - '0';
                        if(dubbel > 0){
                            roundtime * 10;
                        }
                        roundtime = roundtime + temp;
                        display.cmd(roundtime);
                        dubbel++;
                    }
                    else if(input == '#'){
                        cmd = createmessage(0, roundtime);
                        state = sendmessage;
                    }
                    break;
                case sendmessage:
                    input = keypadchannel.read();
                    if(input == '*'){
                        irmessagecontrol.sendsignal(cmd);
                    }
                    else if(input == '#'){
                        roundtime = 30;
                        cmd = createmessage(0, roundtime);
                        state = sendstart;
                        secondtimer.set(1000000);
                    }
                    break;
                case sendstart:
                    auto event = (keypadchannel + secondtimer);
                    if(event == secondtimer){
                        roundtime--;
                        cmd = createmessage(0, roundtime);
                        secondtimer.set(1000000);
                    }
                    else if( event == keypadchannel){
                        input = keypadchannel.read();
                        if(input == '*'){
                            irmessagecontrol.sendsignal(cmd);
                        }
                    }
                    else if(roundtime < 0){
                        state = idle;
                        secondtimer.cancel();
                        }
                    break;
                }
            }
        }



};



#endif // LEADERCONTROL_HPP
