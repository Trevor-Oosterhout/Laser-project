#ifndef LIVES_HPP
#define LIVES_HPP


/// @file

// this file contains doxygen lines

/// \brief
/// lives struct
/// \details
/// this struct stores the amount of lives the player has left


struct Lives{
private:
    int lives;

public:

    /// \brief
    /// sets number of lives
    void SetLives(int l){
        lives = l;
    }

    /// \brief
    /// returns number of lives left
    int GetLives(){
        return lives;
    }
    
};



#endif // LIVES_HPP
