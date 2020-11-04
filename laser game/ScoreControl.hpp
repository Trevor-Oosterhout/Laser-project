#ifndef SCORECONTROL_HPP
#define SCORECONTROL_HPP


#include "hwlib.hpp"
#include "rtos.hpp"
#include "Display.hpp"
#include "shootcontrol.hpp"
#include "PlayerConfig.hpp"
#include "stats.hpp"
#include "GameTime.hpp"
#include "TransferControl.hpp"


/// @file

// this file contains doxygen lines

/// \brief
/// rtos task ScoreControl
/// \details
/// This task is an rtos task.
/// This task implements receiving hits from other players.


class ScoreControl : public rtos::task<>{
private:
    rtos::flag BeginFlag;
    rtos::flag GameOverFlag;
    rtos::channel<uint16_t, 10> hitchannel;


    Display & terminal;
    Lives & lives;
    ShootControl & shootcontrol;
    PlayerConfig & playerconfig;
    Stats & stats;
    GameTime & gametime;
    TransferControl & transfercontrol;


public:
    ScoreControl(
        Display & terminal,
        Lives & lives,
        ShootControl & shootcontrol,
        PlayerConfig & playerconfig,
        Stats & stats,
        GameTime & gametime,
        TransferControl & transfercontrol
    ):
        task(4, "ScoreControl"),
        BeginFlag ( this, "BeginFlag"),
        GameOverFlag ( this, "GameOverFlag"),
        hitchannel ( this, "hitchannel"),
        terminal ( terminal ),
        lives ( lives ),
        shootcontrol ( shootcontrol ),
        playerconfig(playerconfig),
        stats (stats),
        gametime (gametime),
        transfercontrol ( transfercontrol)
        {}



    /// \brief
    /// sets the GameOverFlag
    /// \details
    /// This function can be called by other tasks to set the GameOverFlag.
    void SignalGameOver(){
        GameOverFlag.set();
    }

    /// \brief
    /// sets the beginflag
    /// \details
    /// This function can be called by other tasks to set the beginflag.
    void Begin(){
        BeginFlag.set();
    }

    /// \brief
    /// wrties to the hitchannel
    /// \details
    /// This function can be called by other tasks to write to the hitchannel.
    void ReportHit( uint16_t hit){
        hitchannel.write(hit);
    }

    void main(){
        enum states { idle, waitingforhit };
        states state = idle;
        uint16_t hit;
        int playerID, currentlives, weaponpower;
        for(;;){
            switch (state){
                case idle:
                    wait(BeginFlag);
                    state = waitingforhit;
                    hitchannel.clear();
                    currentlives = lives.GetLives();
                    break;

                case waitingforhit:
                    auto event = wait(GameOverFlag + hitchannel);
                    if(event == GameOverFlag){
                        state = idle;
                    }
                    else{
                        hit = hitchannel.read();
                        weaponpower = (hit & 0x03E0) >> 5;
                        playerID = (hit & 0x7C00) >> 10;
                        if((playerID != 0) && (playerID != playerconfig.GetPlayerID())){
                            currentlives = lives.GetLives() - weaponpower;
                            lives.SetLives(currentlives);
                            stats.SetStat(playerID, gametime.GetTime());
                        }
                        if(currentlives > 0){
                            state = waitingforhit;
                        }
                        else{
                            state = idle;
                            shootcontrol.SignalGameOver();
                            lives.SetLives(0);
                            transfercontrol.Setshowflag();
                        }
                        terminal.update();
                    }
                    break;
            }
        }

    }


};



#endif // SCORECONTROL_HPP
