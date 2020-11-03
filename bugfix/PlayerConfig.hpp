#ifndef PLAYERCONFIG_HPP
#define PLAYERCONFIG_HPP

struct PlayerConfig{
private:
    int PlayerID = 0;
    int PlayerPower = 0;
    long long int Cooldown[10] = {0, 100000, 200000, 250000, 300000, 500000, 1000000, 1500000, 10000000};

public:

    void SetPlayerID(char ButtonID){
        PlayerID = ButtonID;
    }

    char GetPlayerID(){
        return PlayerID;
    }

    void SetPlayerPower(char ButtonID){
        PlayerPower = ButtonID;
    }

    char GetPlayerPower(){
        return PlayerPower;
    }
    int GetCooldown(unsigned int i){
        return Cooldown[i];
    }
};

#endif // PLAYERCONFIG_HPP
