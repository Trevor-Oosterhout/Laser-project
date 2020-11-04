#ifndef TRANSFERCONTROL_HPP
#define TRANSFERCONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "stats.hpp"


/// @file

// this file contains doxygen lines

/// \brief
/// transfercontrol class
/// \details
/// this class shows the stats of the player on the computer screen
/// it displays when and by whom the player was hit.


class TransferControl : public rtos::task<>{
private:
    rtos::flag showflag;
    Stats & stats;

public:
    TransferControl(Stats & stats):
    task(12, "TransferControl"),
    showflag(this, "showflag"),
    stats ( stats )
    {}

    /// \brief
    /// sets the showflag
    /// \details
    /// sets the showflag to print the stats to the computer
    void Setshowflag(){
        showflag.set();
    }


    /// \brief
    /// loops through the states indefinitely
    void main(){
        unsigned int j;
        for(;;){
            wait(showflag);
            j = stats.Getnhits();
            if(j == 0){
                hwlib::cout << "you were not hit" << '\n';
            }
            else{
                for(unsigned int i = 0; i < j; i++){
                    hwlib::cout << "player: " << stats.GetStatPlayer(i) << "hit you "
                    << "at: " << stats.GetStatHittime(i) << '\n';
                }
            }
        }
    }



};






#endif // TRANSFERCONTROL_HPP
