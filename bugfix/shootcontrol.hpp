#ifndef SHOOTCONTROL_HPP
#define SHOOTCONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "IRmessageControl.hpp"
#include "PlayerConfig.hpp"


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
        task( 8, "ShootControl"),
        BeginFlag ( this, "BeginFlag"),
        GameOverFlag ( this, "GameOverFlag"),
        TriggerClock ( this, 50000, "TriggerClock"),
        Trigger ( Trigger ),
        Speaker ( Speaker ),
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

    uint16_t createmessage(uint16_t playernumber, uint16_t data);
};


#endif // SHOOTCONTROL_HPP
