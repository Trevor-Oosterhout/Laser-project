#ifndef DISPLAY_HPP
#define DISPLAY_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "Lives.hpp"
#include "GameTime.hpp"


class Display : public rtos::task<>{
private:
    rtos::flag updateflag;
    rtos::flag pregameflag;
    rtos::channel< int, 5 > buffer;

    hwlib::terminal_from & terminal;
    GameTime & gametime;
    Lives & lives;

public:
    Display(hwlib::terminal_from & terminal, GameTime & gametime, Lives & lives):
    task(11, "display"),
    updateflag (this, "updateflag"),
    pregameflag (this, "pregameflag"),
    buffer (this, "buffer"),
    terminal ( terminal ),
    gametime ( gametime ),
    lives ( lives )
    {}

    void main(){
        enum states {idle, pregame, postgame, leader};
        states state = idle;
        char c;
        int t;
        for(;;){
            switch (state) {
                case idle:{
                    auto event = (updateflag + buffer + pregameflag);
                    if(event == buffer){
                        state = leader;
                    }
                    else if (event == pregameflag){
                        state = pregame;
                    }
                    else{
                        state = postgame;
                    }
                    break;
                }
                case pregame:{
                    auto event = (pregameflag + updateflag);
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
                    if(t <= 0){
                        state = idle;
                        break;
                    }
                    terminal << "round ends in: \n" << t << " seconds \n";
                    terminal << lives.GetLives() << " lives remaining" << hwlib::flush;
                    break;
                }
                case leader:{
                    c = buffer.read();
                    terminal << "\f";
                    terminal << "press # to confirm \n press * to give \n command to other \n players \n";
                    terminal << "time set to"  << c << hwlib::flush;
                    wait(buffer);
                    break;
                }
            }
        }
    }



    void update(){
        updateflag.set();
    }

    void cmd(int c){
        buffer.write(c);
    }

    void pregame(){
        pregameflag.set();
    }


};





#endif // DISPLAY_HPP
