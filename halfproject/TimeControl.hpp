#ifndef TIMECONTROL_HPP
#define TIMECONTROL_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

class TimeContol: public rtos::task<>{
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
            case pregame_countdowntate = postgame_countdown;
                display.pregame();
                wait(secondclock);
                GameTime.SetWaitTime((GameTime.GetWaitTime() -1 ));
                if(Gametime.GetWaitTime() < 0){
                    scorecontrol.Begin();
                    shootcontrol.Begin();
                    state = postgame_countdown;
                }
                break;

            case postgame_countdown:
                display.update();
                wait(secondclock);
                GameTime.SetTime((GameTime.GetTime()- 1));
                if(Lives.getlives() > 0 && GameTime.GetTime() >=0){
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
    TimeContol( Display & display, GameTime & gametime, Lives & lives, ScoreControl & scorecontrol, ShootControl & shootcontrol):
        task(6,"timecontrol"), // wijzig dit later indien te laage prioriteit
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