#ifndef LEADERCONTROL_HPP
#define LEADERCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Keypadlistener.hpp"


class LeaderControl : public KeypadListener, public rtos::task<>{
private:
    rtos::channel<char, 4> keypadchannel;
    rtos::timer secondtimer;

    GameTime & gametime;
    Display & display;
    IRmessageControl & irmessagecontrol;

public:
    LeaderControl(GameTime & gametime, Display & display, IRmessageControl & irmessagecontrol ):
    task (10, "LeaderControl"),
    keypadchannel(this, "keypadchannel"),
    secondtimer(this, "secondtimer"),
    gametime ( gametime ),
    display ( display ),
    irmessagecontrol ( irmessagecontrol )
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
                            roundtime = roundtime * 10;
                        }
                        roundtime = roundtime + temp;
                        display.cmd(roundtime);
                        dubbel++;
                    }
                    else if(input == '#'){
                        if(roundtime <= 15 && roundtime > 0){
                            cmd = createmessage(0, roundtime);
                            state = sendmessage;
                        }
                        roundtime = 0;
                        dubbel = 0;
                    }
                    break;
                case sendmessage:
                    input = keypadchannel.read();
                    if(input == '*'){
                        irmessagecontrol.sendsignal(cmd);
                        hwlib::cout << "sendmessage" << '\n' << hwlib::flush;
                    }
                    else if(input == '#'){
                        roundtime = 30;
                        cmd = createmessage(0, roundtime);
                        state = sendstart;
                    }
                    break;

                case sendstart:
                secondtimer.set(1000000);
                    auto event = wait(keypadchannel + secondtimer);
                    if(event == secondtimer){
                        roundtime--;
                        cmd = createmessage(0, roundtime);
                        if(roundtime == 0){
                            state = idle;
                        }
                    }
                    else{
                        input = keypadchannel.read();
                        if(input == '*'){
                            irmessagecontrol.sendsignal(cmd);
                        }
                    }

                    break;
                }
            }
        }



};



#endif // LEADERCONTROL_HPP
