#ifndef SCORECONTROL_HPP
#define SCORECONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "Display.hpp"
#include "shootcontrol.hpp"

class ScoreControl : public rtos::task<>{
private:
    rtos::flag BeginFlag;
    rtos::flag GameOverFlag;
    rtos::channel<uint16_t, 10> hitchannel;


    Display & terminal;
    hwlib::pin_out & Speaker;
    Lives & lives;
    ShootControl & shootcontrol;


public:
    ScoreControl(
        Display & terminal,
        hwlib::pin_out & Speaker,
        Lives & lives,
        ShootControl & shootcontrol
    ):
        task(4, "ScoreControl"),
        BeginFlag ( this, "BeginFlag"),
        GameOverFlag ( this, "GameOverFlag"),
        hitchannel ( this, "hitchannel"),
        terminal ( terminal ),
        Speaker (Speaker),
        lives ( lives ),
        shootcontrol ( shootcontrol )
        {}


    void SignalGameOver(){
        GameOverFlag.set();
    }


    void Begin(){
        BeginFlag.set();
    }

    void ReportHit( uint16_t hit){
        hitchannel.write(hit);
    }

    void main(){
        enum states { idle, waitingforhit };
        states state = idle;
        uint16_t hit;
        int playerID, currentlives, weaponpower;
        for(;;){
            switch (state){
                case idle:
                wait(BeginFlag);
                state = waitingforhit;
                break;

                case waitingforhit:
                auto event = (GameOverFlag + hitchannel);
                if(event == GameOverFlag){
                    state = idle;
                }
                else{
                    hit = hitchannel.read();
                    weaponpower = ((hit & 0x03E0) >> 5);
                    playerID = ((hit & 0x7C00) >> 10);
                    currentlives = lives.GetLives() - weaponpower;
                    lives.SetLives(currentlives);
                    terminal.update();
                    // Speaker.MakeSound();
                    if(currentlives > 0){
                        state = waitingforhit;
                    }
                    else{
                        state = idle;
                        shootcontrol.SignalGameOver();
                    }

                }

            }
        }

    }


};



#endif // SCORECONTROL_HPP
