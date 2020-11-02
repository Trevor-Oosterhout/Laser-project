#ifndef REGISTRATIONCONTROL_HPP
#define REGISTRATIONCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Keypadlistener.hpp"
#include "PlayerConfig.hpp"
#include "GameTime.hpp"
#include "TimeControl.hpp"

class RegistrationControl : public KeypadListener, public rtos::task<>{
private:
    rtos::channel<char, 16> keypadchannel;
    rtos::channel<uint16_t, 4> IRreportchannel;

    PlayerConfig & playerconfig;
    GameTime & gametime;
    TimeControl & timecontrol;


public:

    RegistrationControl(PlayerConfig & playerconfig, GameTime & gametime, TimeControl & timecontrol):
    task (9, "RegistrationControl"),
    keypadchannel( this, "keypadchannel"),
    IRreportchannel( this, "IRreportchannel"),
    playerconfig ( playerconfig ),
    gametime ( gametime ),
    timecontrol ( timecontrol )
    {}

    void ButtonPressed(char c) override{
        keypadchannel.write(c);
    }

    void ReportCmd(uint16_t cmd){
        IRreportchannel.write(cmd);
    }

    void main(){
        enum states { idle, playernummer, waitinput, weaponpower, leadercmd, startsignal};
        states state = idle;
        char input;
        int playernumber, weaponchoice;
        uint16_t cmd;
        for(;;){
            switch(state){
                case idle:
                    input = keypadchannel.read();
                    if(input == 'A'){
                        state = playernummer;
                    }
                    break;
                case playernummer:
                    input = keypadchannel.read();
                    if(input >= '1' && input <= '9'){
                        playernumber = input - '0';
                        playerconfig.SetPlayerID(playernumber);
                        state = waitinput;
                    }
                    break;
                case waitinput:
                    input = keypadchannel.read();
                    if(input == 'B'){
                        state = weaponpower;
                    }
                    break;
                case weaponpower:
                    input = keypadchannel.read();
                    if(input >= '1' && input <= '9'){
                        weaponchoice = input - '0';
                        playerconfig.SetPlayerPower(weaponchoice);
                        state = leadercmd;
                    }
                    break;
                case leadercmd:
                    cmd = IRreportchannel.read();
                    playernumber = (cmd & 0x7C00) >> 10;
                    if(playernumber == 0){
                        cmd = (cmd & 0x03E0) >> 5;
                        cmd = cmd * 60;
                        gametime.SetTime(cmd);
                        state = startsignal;
                    }
                    break;
                case startsignal:
                    cmd = IRreportchannel.read();
                    playernumber = (cmd & 0x7C00) >> 10;
                    if(playernumber == 0){
                        cmd = (cmd & 0x03E0) >> 5;
                        if(cmd == 30){
                            gametime.SetWaitTime(30);
                            timecontrol.Begin();
                            state = idle;
                        }
                    }
                    break;
            }
        }
    }

};



#endif // REGISTRATIONCONTROL_HPP
