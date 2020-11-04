#ifndef MSGVERIFIER_HPP
#define MSGVERIFIER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "RegistrationControl.hpp"
#include "ScoreControl.hpp"

class MSGverifier: public rtos::task<>{
private:
    RegistrationControl & registrationcontrol;
    ScoreControl & scorecontrol;
    rtos::channel <uint16_t, 30> verificationchannel;
    rtos::timer doublemessagetimer;

    void main(){
        uint16_t lastmessage = 0x000f,receivedmessage;
        int playerID;
        bool XOR;
        for(;;){
            auto event = wait(verificationchannel + doublemessagetimer);
            if(event == verificationchannel){
                receivedmessage = verificationchannel.read();
                XOR = checkmessage(receivedmessage);
                if(XOR){
                    playerID = (receivedmessage & 0x7C00) >> 10;
                    if(lastmessage != receivedmessage){
                        doublemessagetimer.set(60000);
                        lastmessage = receivedmessage;
                        if(playerID > 0){
                            scorecontrol.ReportHit(receivedmessage);
                        }
                        else{
                            registrationcontrol.ReportCmd(receivedmessage);
                        }
                    }
                }
            }
            else{
                lastmessage = 0x000f; // deze waarde kan niet worden geverifieerd, dus kan de message nooit gelijjk zijn aan deze waarde
            }
        }
    }

public:
    MSGverifier(RegistrationControl & registrationcontrol, ScoreControl & scorecontrol):
    task(3, "MSGverifier"),
    registrationcontrol(registrationcontrol),
    scorecontrol(scorecontrol),
    verificationchannel(this,"verificationchannel"),
    doublemessagetimer(this,"doublemessagetimer")
    {}

    void reportmessage(uint16_t msg){
        verificationchannel.write(msg);
    }

    bool checkmessage(const uint16_t & msg){
        uint16_t check, leftXOR, rightXOR, controlebits;
        controlebits = msg & 0x001F;
        leftXOR = (msg & 0x7C00) >> 10;
        rightXOR = (msg & 0x03E0) >> 5;
        check = leftXOR ^ rightXOR;
        if(check == controlebits){
            return true;
        }
        return false;
    }
};



#endif //MSGVERIFIER_HPP