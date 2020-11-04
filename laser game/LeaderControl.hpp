#ifndef LEADERCONTROL_HPP
#define LEADERCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Keypadlistener.hpp"


/// @file

// this file contains doxygen lines

/// \brief
/// leadercontrol class
/// \details
/// this class implements the leader control for the laser game
/// it will send the time and start signal in the form of an IR message
/// to other players when the '*' is pressed


class LeaderControl : public KeypadListener, public rtos::task<>{
private:
    rtos::channel<char, 4> keypadchannel;
    rtos::timer secondtimer;

    GameTime & gametime;
    IRmessageControl & irmessagecontrol;
    hwlib::terminal_from & terminal;

public:
    LeaderControl(GameTime & gametime, IRmessageControl & irmessagecontrol, hwlib::terminal_from & terminal ):
    task (10, "LeaderControl"),
    keypadchannel(this, "keypadchannel"),
    secondtimer(this, "secondtimer"),
    gametime ( gametime ),
    irmessagecontrol ( irmessagecontrol ),
    terminal (terminal)
    {
        terminal << '\f' << "press A \n for player \n press C \n for leader" << hwlib::flush;
    }


    /// \brief
    /// writes in the keypadchannel
    /// \details
    /// the keypad writes which character was pressed in the keypad channel
    void ButtonPressed(char c) override{
        keypadchannel.write(c);
    }

    /// \brief
    /// Creates the IR message
    /// \details
    /// creates the IR message according to our IR protocol
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
        int roundtime = 0;
        uint16_t temp = 0;
        int dubbel = 0;
        uint16_t cmd;
        for(;;){
            switch(state){
                case idle:
                    input = keypadchannel.read();
                    if(input == 'C'){
                        state = maketime;
                        terminal << '\f' << "time set to: \n";
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
                        terminal << '\f' << "time set to: \n";
                        terminal << roundtime << hwlib::flush;
                        dubbel++;
                    }
                    else if(input == '#'){
                        if(roundtime <= 15 && roundtime > 0){
                            cmd = createmessage(0, roundtime);
                            state = sendmessage;
                        }
                        else{
                        terminal << "invalid" << hwlib::flush;
                        roundtime = 0;
                        dubbel = 0;
                        }
                    }
                    break;
                case sendmessage:
                    terminal << '\f' << "press * to \n command others \n # when done" << hwlib::flush;
                    input = keypadchannel.read();
                    if(input == '*'){
                        irmessagecontrol.sendsignal(cmd);
                    }
                    else if(input == '#'){
                        roundtime = 30;
                        cmd = createmessage(0, roundtime);
                        state = sendstart;
                        terminal << '\f' << "press * to \n start others" << hwlib::flush;
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
