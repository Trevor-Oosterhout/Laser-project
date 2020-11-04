#ifndef TIMECONTROL_HPP
#define TIMECONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Display.hpp"
#include "Lives.hpp"
#include "GameTime.hpp"
#include "ScoreControl.hpp"
#include "TransferControl.hpp"

/// @file

// this file contains doxygen lines

/// \brief
/// rtos task TimeControl
/// \details
/// This class is a rtos task.
/// This task implements the waiting period before the game and during the game,
/// and shows it on the display


class TimeControl: public rtos::task<>{
    enum states {idle,pregame_countdown,postgame_countdown};
private:
    Display & display;
    GameTime & gametime;
    Lives & lives;
    ScoreControl & scorecontrol;
    ShootControl & shootcontrol;
    rtos::flag beginflag;
    rtos::clock secondclock;
    TransferControl & transfercontrol;


    void main(){
        states state = idle;
        int l;
        for(;;){
            switch (state)
            {
            case idle:
                wait(beginflag);
                state = pregame_countdown;
                break;
            case pregame_countdown:
                display.pregame();
                wait(secondclock);
                gametime.SetWaitTime((gametime.GetWaitTime() -1 ));
                if(gametime.GetWaitTime() < 0){
                    scorecontrol.Begin();
                    shootcontrol.Begin();
                    state = postgame_countdown;
                }
                break;

            case postgame_countdown:
                display.update();
                wait(secondclock);
                gametime.SetTime((gametime.GetTime()- 1));
                l = lives.GetLives();
                if(l > 0 && gametime.GetTime() >=0){
                    state = postgame_countdown;
                }
                else{
                    scorecontrol.SignalGameOver();
                    shootcontrol.SignalGameOver();
                    transfercontrol.Setshowflag();
                    state = idle;
                }
                break;

            }
        }

    }

public:
    TimeControl( Display & display, GameTime & gametime, Lives & lives, ScoreControl & scorecontrol, ShootControl & shootcontrol, TransferControl & transfercontrol):
        task(7 ,"timecontrol"),
        display(display),
        gametime(gametime),
        lives(lives),
        scorecontrol(scorecontrol),
        shootcontrol(shootcontrol),
        beginflag(this,"beginflag"),
        secondclock(this,1000000,"secondclock"),
        transfercontrol (transfercontrol)
    {}
    /// \brief
    /// sets the beginflag
    /// \details
    /// This function can be called by other tasks to set the beginflag.
    void Begin(){
        beginflag.set();
    }

};





#endif // TIMECONTROL_HPP
