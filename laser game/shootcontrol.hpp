#ifndef SHOOTCONTROL_HPP
#define SHOOTCONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "IRmessageControl.hpp"
#include "PlayerConfig.hpp"

/// @file

// this file contains doxygen lines

/// \brief
/// shootcontrol class
/// \details
/// this class implements the shooting control for the laser game
/// it will send the IR message to other players when the trigger is pulled


class ShootControl : public rtos::task<>{
private:
    rtos::flag BeginFlag;
    rtos::flag GameOverFlag;
    rtos::clock TriggerClock;

    hwlib::pin_in & Trigger;
    IRmessageControl & irmessagecontrol;
    PlayerConfig & playerconfig;

public:

    /// \brief
    /// constructor shootcontrolclass
    /// \details
    /// this constructor initializes all variables
    ShootControl(
        hwlib::pin_in & Trigger,
        IRmessageControl & irmessagecontrol,
        PlayerConfig & playerconfig
    ):
        task( 8, "ShootControl"),
        BeginFlag ( this, "BeginFlag"),
        GameOverFlag ( this, "GameOverFlag"),
        TriggerClock ( this, 50000, "TriggerClock"),
        Trigger ( Trigger ),
        irmessagecontrol ( irmessagecontrol ),
        playerconfig ( playerconfig )
    {}


    /// \brief
    /// sets the GameOverFlag
    /// \details
    /// sets the GameOverFlag to leave the shooting state and go back to the idle state
    void SignalGameOver();


    /// \brief
    /// sets the BeginFlag
    /// \details
    /// sets the BeginFlag to leave the idle state
    void Begin();

    /// \brief
    /// loops through the states indefinitely
    void main();


    /// \brief
    /// Creates the IR message
    /// \details
    /// creates the IR message according to our IR protocol
    uint16_t createmessage(uint16_t playernumber, uint16_t data);
};


#endif // SHOOTCONTROL_HPP
