#ifndef SHOOTCONTROL_HPP
#define SHOOTCONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"

class ShootControl : public rtos::task<>{
private:
    rtos::flag BeginFlag;
    rtos::flag GameOverFlag;
    rtos::clock TriggerClock;

    hwlib::pin_in & Trigger;
    hwlib::pin_out & Speaker;
    IRmessageControl & irmessagecontrol;
    PlayerConfig & playerconfig;

public:

    ShootControl(
        hwlib::pin_in & Trigger,
        hwlib::pin_out & Speaker,
        IRmessageControl & irmessagecontrol,
        PlayerConfig & playerconfig
    ):
        task( 3, "ShootControl"),
        Trigger ( Trigger ),
        Speaker ( Speaker ),
        irmessagecontrol ( irmessagecontrol ),
        playerconfig ( playerconfig ),
        Trigger ( hwlib::target::pin_in( hwlib::target::pins::d7 ) )
        TriggerClock ( this, 50000, "TriggerClock"),
        BeginFlag ( this, "BeginFlag"),
        GameOverFlag ( this, "GameOverFlag")

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


};


#endif // SHOOTCONTROL_HPP
