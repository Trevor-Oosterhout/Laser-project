#ifndef MSGVERIFIER_HPP
#define MSGVERIFIER_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include <stdint.h>

class MSGverifier: public rtos::task<>{
private:
    rtos::channel <uint16_t, 10> verificationchannel;
    rtos::timer doublemessagetimer;

    void printint(const uint16_t & bits){
	    for(int i = 15 ; i > -1; --i) {
		    if(bits & (1 << i)){
                hwlib::cout << 1;
            }
		    else{
		    	hwlib::cout << 0;
		    }
    	}
    }    

    void main(){
        uint16_t lastmessage, receivedmessage;
        int playerID;
        bool XOR;
        for(;;){
            auto event = wait(verificationchannel + doublemessagetimer);
            if(event == verificationchannel){
                receivedmessage = verificationchannel.read();
                XOR = checkbit(receivedmessage);
                if(XOR){
                    playerID = (receivedmessage & 0x7c00);
                    doublemessagetimer.set(42000);
                    lastmessage = receivedmessage;
                    if(lastmessage != receivedmessage){
                        if(playerID > 0){
                            hwlib::cout << "Dit bericht moet naar de Shootcontrol:" << "\n";
                            printint(receivedmessage);
                            hwlib::cout << "\n\n";
                        }
                        else{
                            hwlib::cout << "Dit bericht moet naar de registrationcontrol:" << "\n";
                            printint(receivedmessage);
                            hwlib::cout << "\n\n";

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
    MSGverifier():
    task(3, "MSGverifier"),
    verificationchannel(this,"verificationchannel"),
    doublemessagetimer(this,"doublemessagetimer")
    {}

    void reportmessage(uint16_t msg){
        verificationchannel.write(msg);
    }

    bool checkbit(uint16_t & msg){
        uint16_t check, leftXOR, rightXOR;
        bool XOR;
        for(unsigned int i = 0; i < 5; i++){
            check = msg >> (15-11+i) & 1;
            leftXOR = msg >> (15-1+i) & 1;
            rightXOR = msg >> (15-6+i) & 1;
            XOR = (check == (leftXOR ^ rightXOR));
            if(!XOR){
                return false;
            } 
        }
        return true;
    }
};



#endif //MSGVERIFIER_HPP