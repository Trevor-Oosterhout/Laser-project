#ifndef PLAYERCONFIG_HPP
#define PLAYERCONFIG_HPP

struct PlayerConfig{
private:
    char PlayerID = 0;
    char PlayerPower = 0;
    long long int Cooldown[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

public:

    PlayerConfig(){}

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
