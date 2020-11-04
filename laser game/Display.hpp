#ifndef DISPLAY_HPP
#define DISPLAY_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "Lives.hpp"
#include "GameTime.hpp"

/// @file

// this file contains doxygen lines

/// \brief
/// rtos task Display
/// \details
/// This class is a rtos task.
/// This task implements showing messages on the display.

class Display : public rtos::task<>{
private:
    rtos::flag updateflag;
    rtos::flag pregameflag;

    hwlib::terminal_from & terminal;
    GameTime & gametime;
    Lives & lives;

public:
    Display(hwlib::terminal_from & terminal, GameTime & gametime, Lives & lives):
    task(11, "display"),
    updateflag (this, "updateflag"),
    pregameflag (this, "pregameflag"),
    terminal ( terminal ),
    gametime ( gametime ),
    lives ( lives )
    {}

    void main(){
        enum states {idle, pregame, postgame};
        states state = idle;
        int t, l;
        for(;;){
            switch (state) {
                case idle:{
                    auto event = wait(updateflag + pregameflag);
                    if (event == pregameflag){
                        state = pregame;
                    }
                    else{
                        state = postgame;
                    }
                    break;
                }
                case pregame:{
                    auto event = wait(pregameflag + updateflag);
                    if(event == updateflag){
                        state = postgame;
                        break;
                    }
                    else{
                        terminal << "\f";
                        t = gametime.GetWaitTime();
                        terminal << "game begins in: \n" << t << " seconds" << hwlib::flush;
                    }
                    break;
                }
                case postgame:{
                    wait(updateflag);
                    terminal << "\f";
                    t = gametime.GetTime();
                    l = lives.GetLives();
                    if(t <= 0){
                        state = idle;
                        break;
                    }
                    terminal << "round ends in: \n" << t << " seconds \n";
                    hwlib::wait_us(100);
                    terminal << l << " lives remaining" << hwlib::flush;
                    break;
                }
            }
        }
    }


    /// \brief
    /// sets the updateflag
    /// \details
    /// This function can be called by other tasks to set the updateflag,
    /// which results in displaying the gametime and the player's remaining lives.
    void update(){
        updateflag.set();
    }


    /// \brief
    /// sets the pregameflag
    /// \details
    /// This function can be called by other tasks to set the pregameflag,
    /// which results in displaying the preparation time.
    void pregame(){
        pregameflag.set();
    }



};





#endif // DISPLAY_HPP
