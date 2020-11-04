#ifndef STATS_HPP
#define STATS_HPP


/// @file

// this file contains doxygen lines

/// \brief
/// stats struct
/// \details
/// this struct holds the values when and by whom the player was hit


struct Stats{
private:
    int player[10];
    int hittime[10];
    unsigned int nhits = 0;

public:

    /// \brief
    /// sets the player and the time the player was hit
    void SetStat(int p, int t ){
        player[nhits] = p;
        hittime[nhits] = t;
        nhits++;
    }


    /// \brief
    /// returns amount of values stored
    unsigned int Getnhits(){
        return nhits;
    }

    /// \brief
    /// returns the enemies player number
    int GetStatPlayer( unsigned int i){
        return player[i];
    }

    /// \brief
    /// returns the time the player got hit
    int GetStatHittime( unsigned int i){
        return hittime[i];
    }





};



#endif // STATS_HPP
