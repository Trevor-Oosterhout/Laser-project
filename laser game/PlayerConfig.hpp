#ifndef PLAYERCONFIG_HPP
#define PLAYERCONFIG_HPP


/// @file

// this file contains doxygen lines

/// \brief
/// playerconfig struct
/// \details
/// this struct stores the values the player has chosen during the games preparation

struct PlayerConfig{
private:
    int PlayerID = 0;
    int PlayerPower = 0;
    long long int Cooldown[10] = {0, 5000, 100000, 200000, 250000, 300000, 500000, 1000000, 1500000, 10000000};

public:
    /// \brief
    /// sets the playerID
    void SetPlayerID(int ButtonID){
        PlayerID = ButtonID;
    }
    /// \brief
    /// returns the value PlayerID
    int GetPlayerID(){
        return PlayerID;
    }

    /// \brief
    /// sets PlayerPower
    void SetPlayerPower(int ButtonID){
        PlayerPower = ButtonID;
    }

    /// \brief
    /// returns the value PlayerPower
    int GetPlayerPower(){
        return PlayerPower;
    }

    /// \brief
    /// returns Cooldown of the weapon
    long long int GetCooldown(unsigned int i){
        return Cooldown[i];
    }
};

#endif // PLAYERCONFIG_HPP
