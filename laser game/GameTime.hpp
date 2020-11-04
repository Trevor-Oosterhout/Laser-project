#ifndef GAMETIME_HPP
#define GAMETIME_HPP


/// @file

// this file contains doxygen lines

/// \brief
/// GameTime struct
/// \details
/// this struct stores the time that is left and the time until the round starts

struct GameTime{
private:
    int starttime, gametime;

public:
    /// \brief
    /// sets the time until the round starts
    void SetWaitTime(int s){
        starttime = s;
    }

    /// \brief
    /// returns the time until the round starts
    int GetWaitTime(){
        return starttime;
    }

    /// \brief
    /// sets the time that is left in the round
    void SetTime(int s){
        gametime = s;
    }

    /// \brief
    /// returns the time that is left in the round
    int GetTime(){
        return gametime;
    }



};


#endif // GAMETIME_HPP
