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
    // rtos::channel< int, 5 > buffer;

    hwlib::terminal_from & terminal;
    GameTime & gametime;
    Lives & lives;

public:
    Display(hwlib::terminal_from & terminal, GameTime & gametime, Lives & lives):
    task(11, "display"),
    updateflag (this, "updateflag"),
    pregameflag (this, "pregameflag"),
    // buffer (this, "buffer"),
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
                    terminal << l << " lives remaining" << hwlib::flush;
                    break;
                }
            }
        }
    }



    void update(){
        updateflag.set();
    }
    //
    // void cmd(int c){
    //     buffer.write(c);
    // }

    void pregame(){
        pregameflag.set();
    }

    // void message(std::array<char, 60> c){
    //     terminal <<'\f';
    //     for(unsigned i = 0; i < 96; i++){
    //         terminal << c[i];
    //     }
    //
    //
    //     terminal << hwlib::flush;
    // }
    //
    // void numericvalue(int x){
    //     terminal << x << hwlib::flush;
    // }


};





#endif // DISPLAY_HPP
