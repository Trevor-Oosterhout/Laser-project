#ifndef REGISTRATIONCONTROL_HPP
#define REGISTRATIONCONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Keypadlistener.hpp"
#include "PlayerConfig.hpp"
#include "GameTime.hpp"
#include "TimeControl.hpp"


/// @file

// this file contains doxygen lines

/// \brief
/// rtos task RegistrationControl
/// \details
/// This task implements registration for players,
/// allowing them to set their playernumber and weapon power.
/// This information is stored in playerconfig after selection.


class RegistrationControl : public KeypadListener, public rtos::task<>{
private:
    rtos::channel<char, 16> keypadchannel;
    rtos::channel<uint16_t, 4> IRreportchannel;

    PlayerConfig & playerconfig;
    GameTime & gametime;
    TimeControl & timecontrol;
    hwlib::terminal_from & terminal;



public:
    RegistrationControl(PlayerConfig & playerconfig, GameTime & gametime, TimeControl & timecontrol, hwlib::terminal_from & terminal):
    task (9, "RegistrationControl"),
    keypadchannel( this, "keypadchannel"),
    IRreportchannel( this, "IRreportchannel"),
    playerconfig ( playerconfig ),
    gametime ( gametime ),
    timecontrol ( timecontrol ),
    terminal (terminal)

    {}

    /// \brief
    /// writes to the keypadchannel
    /// \details
    /// Allows the keypad to write a char to the keypadchannel.
    void ButtonPressed(char c) override{
        keypadchannel.write(c);
    }


    /// \brief
    /// writes to the IRreportchannel.
    /// \details
    /// allows other tasks to write a command to the IRreportchannel.
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
                    terminal << '\f' << "playernumber 1-9 \n" << hwlib::flush;
                    input = keypadchannel.read();
                    if(input >= '1' && input <= '9'){
                        playernumber = input - '0';
                        playerconfig.SetPlayerID(playernumber);
                        state = waitinput;
                    }
                    break;
                case waitinput:
                    terminal << '\f' << "press b \n" << hwlib::flush;
                    input = keypadchannel.read();
                    if(input == 'B'){
                        state = weaponpower;
                    }
                    break;
                case weaponpower:
                    terminal << '\f' << "weaponpower 1-9 \n" << hwlib::flush;
                    input = keypadchannel.read();
                    if(input >= '1' && input <= '9'){
                        weaponchoice = input - '0';
                        playerconfig.SetPlayerPower(weaponchoice);
                        state = leadercmd;
                    }
                    break;
                case leadercmd:
                    terminal << '\f' << "wait for \n gametime" << hwlib::flush;
                    cmd = IRreportchannel.read();
                    playernumber = (cmd & 0x7C00) >> 10;
                    if(playernumber == 0){
                        cmd = (cmd & 0x03E0) >> 6;
                        cmd = cmd * 60;
                        gametime.SetTime(cmd);
                        state = startsignal;
                    }
                    break;
                case startsignal:
                    IRreportchannel.clear();
                    terminal << '\f' << "wait for \n startsignal" << hwlib::flush;
                    cmd = IRreportchannel.read();
                    playernumber = (cmd & 0x7C00) >> 10;
                    if(playernumber == 0){
                        cmd = (cmd & 0x03E0) >> 5;
                        gametime.SetWaitTime(cmd);
                        timecontrol.Begin();
                        state = idle;
                    }
                    break;
            }
        }
    }

};



#endif // REGISTRATIONCONTROL_HPP
