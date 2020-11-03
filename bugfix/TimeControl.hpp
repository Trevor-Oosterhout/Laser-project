#ifndef TIMECONTROL_HPP
#define TIMECONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "Display.hpp"
#include "Lives.hpp"
#include "GameTime.hpp"
#include "ScoreControl.hpp"


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

    void main(){
        states state = idle;
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
                if(lives.GetLives() > 0 && gametime.GetTime() >=0){
                    state = postgame_countdown;
                }
                else{
                    scorecontrol.SignalGameOver();
                    shootcontrol.SignalGameOver();
                    state = idle;
                }
                break;

            }
        }

    }

public:
    TimeControl( Display & display, GameTime & gametime, Lives & lives, ScoreControl & scorecontrol, ShootControl & shootcontrol):
        task(7 ,"timecontrol"),
        display(display),
        gametime(gametime),
        lives(lives),
        scorecontrol(scorecontrol),
        shootcontrol(shootcontrol),
        beginflag(this,"beginflag"),
        secondclock(this,1000000,"secondclock")
    {}

    void Begin(){
        beginflag.set();
    }

};





#endif // TIMECONTROL_HPP
