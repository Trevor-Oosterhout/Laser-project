#include "shootcontrol.hpp"


void ShootControl::SignalGameOver(){
    GameOverFlag.set();
}


void ShootControl::Begin(){
    BeginFlag.set();
}


void ShootControl::main{
    enum states = {Idle, Shoot, Cooldown};
    states state = Idle;
    uint16_t IRmessage;
    long long int Cooldown_us;
    for(;;){
        switch(state){
            case Idle:
                wait( BeginFlag );
                IRmessage = GenerateMessage(playerconfig.GetPlayerID(), playerconfig.GetPlayerPower());
                Cooldown_us = playerconfig.GetCooldown(playerconfig.GetPlayerPower());
                state = Shoot;

                break;

            case Shoot:
                auto event = wait( TriggerClock + GameOverFlag );
                if( event == TriggerClock ){
                    if( Trigger.read() ){
                        irmessagecontrol.SendMessage(IRmessage);
                        state = Cooldown;
                    }
                }
                else if( event == GameOverFlag ){
                    state = Idle;
                }
                else{
                    state = Shoot;
                }

                break;

            case Cooldown:
                hwlib::wait_us(Cooldown_us);
                state = Shoot;

                break;
        }
    }
}
