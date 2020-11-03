#include "shootcontrol.hpp"


void ShootControl::SignalGameOver(){
    GameOverFlag.set();
}


void ShootControl::Begin(){
    BeginFlag.set();
}

uint16_t createmessage(uint16_t playernumber, uint16_t data){
    uint16_t msg = 0;
    uint16_t controlebits = 0;
    uint16_t playerbits = 0;
    uint16_t databits = 0;
    controlebits = playernumber ^ data;
    playerbits = playernumber << 10;
    databits = data << 5;
    msg = msg | controlebits;
    msg = msg | playerbits;
    msg = msg | databits;
    return msg;
}


void ShootControl::main(){
    enum states {Idle, Shoot, Cooldown};
    states state = Idle;
    uint16_t IRmessage;
    long long int Cooldown_us;
    for(;;){
        switch(state){
            case Idle:
                wait( BeginFlag );
                IRmessage = createmessage(playerconfig.GetPlayerID(), playerconfig.GetPlayerPower());
                Cooldown_us = playerconfig.GetCooldown(playerconfig.GetPlayerPower());
                state = Shoot;

                break;

            case Shoot:{
                auto event = wait( TriggerClock + GameOverFlag );
                if( event == TriggerClock ){
                    Trigger.refresh();
                    if( Trigger.read() == 1){
                        irmessagecontrol.sendsignal(IRmessage);
                        state = Cooldown;
                    }
                }
                else if( event == GameOverFlag ){
                    state = Idle;
                }

                break;
            }
            case Cooldown:
                hwlib::wait_us(Cooldown_us);
                state = Shoot;

                break;
        }
    }
}
