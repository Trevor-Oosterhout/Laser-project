#ifndef MSGVERIFIER_HPP
#define MSGVERIFIER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "RegistrationControl.hpp"
#include "ScoreControl.hpp"

/// @file

// this file contains doxygen lines


/// \brief
/// rtos task MSGverifier
/// \details
/// This class is an rtos task.
/// This task implements verifying the messages that are received from other players.
/// Verified messages are passed on, messages that do not verify are thrown away.

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
                lastmessage = 0x000f;
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

    /// \brief
    /// writes to the verificationchannel
    /// \details
    /// This function can be called by other tasks to write to the verificationchannel.
    void reportmessage(uint16_t msg){
        verificationchannel.write(msg);
    }


    /// \brief
    /// verifies the message
    /// \details
    /// Verifies the message, checking if the xor bits are correct.
    /// true if all xor bits are correct, otherwise false.
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
