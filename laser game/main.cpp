#include "hwlib.hpp"
#include "rtos.hpp"
#include "SignalDetector.hpp"
#include "MSGdecoder.hpp"
#include "MSGverifier.hpp"
#include "Keypad.hpp"
#include "PlayerConfig.hpp"
#include "GameTime.hpp"
#include "RegistrationControl.hpp"
#include "LeaderControl.hpp"
#include "Lives.hpp"
#include "shootcontrol.hpp"
#include "ScoreControl.hpp"
#include "TimeControl.hpp"
#include "Display.hpp"
#include "IRmessageControl.hpp"
#include "pinout.hpp"
#include "TransferControl.hpp"
#include "stats.hpp"




int main(){
    hwlib::wait_ms(2000);
    auto scl = hwlib::target::pin_oc( hwlib::target::pins::scl );
    auto sda = hwlib::target::pin_oc( hwlib::target::pins::sda );

    auto i2c = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );

    auto keypadrow_0 = hwlib::target::pin_oc(hwlib::target::pins::d40);
    auto keypadrow_1 = hwlib::target::pin_oc(hwlib::target::pins::d38);
    auto keypadrow_2 = hwlib::target::pin_oc(hwlib::target::pins::d36);
    auto keypadrow_3 = hwlib::target::pin_oc(hwlib::target::pins::d34);

    auto keypadrcol_0 = hwlib::target::pin_in(hwlib::target::pins::d32);
    auto keypadrcol_1 = hwlib::target::pin_in(hwlib::target::pins::d30);
    auto keypadrcol_2 = hwlib::target::pin_in(hwlib::target::pins::d28);
    auto keypadrcol_3 = hwlib::target::pin_in(hwlib::target::pins::d26);

    auto keyrows =  hwlib::port_oc_from_pins_t(keypadrow_0, keypadrow_1, keypadrow_2, keypadrow_3);
    auto keycolums =  hwlib::port_in_from_pins_t(keypadrcol_0, keypadrcol_1, keypadrcol_2, keypadrcol_3);

    auto keypad_matrix = hwlib::matrix_of_switches(keyrows, keycolums);

    auto keypad = hwlib::keypad< 16 >(keypad_matrix, "147*2580369#ABCD");

    Keypad fkeypad(keypad);

    auto display = hwlib::glcd_oled(i2c, 0x3C);
    auto font = hwlib::font_default_8x8();

    auto terminal = hwlib::terminal_from(display, font);

    auto trigger = hwlib::target::pin_in(hwlib::target::pins::d31);
    auto IRreceiver = hwlib::target::pin_in(hwlib::target::pins::d51);

    auto IRLED = hwlib::d2_38kHz();

    IRmessageControl irmessagecontrol(IRLED);

    PlayerConfig playerconfig;
    GameTime gametime;
    Lives lives;
    Stats stats;
    lives.SetLives(9);

    TransferControl transfercontrol(stats);

    Display OLEDterminal(terminal, gametime, lives);

    ShootControl shootcontrol(trigger, irmessagecontrol, playerconfig);

    ScoreControl scorecontrol(OLEDterminal, lives, shootcontrol, playerconfig, stats, gametime, transfercontrol);

    TimeControl timecontrol(OLEDterminal, gametime, lives, scorecontrol, shootcontrol, transfercontrol);

    RegistrationControl registrationcontrol(playerconfig, gametime, timecontrol, terminal);
    fkeypad.addlistener(&registrationcontrol);

    MSGverifier msgverifier(registrationcontrol, scorecontrol);

    MSGdecoder msgdecoder(msgverifier);

    SignalDetector signaldetector(msgdecoder, IRreceiver);

    LeaderControl leadercontrol(gametime, irmessagecontrol, terminal);
    fkeypad.addlistener(&leadercontrol);


    rtos::run();


}

// "D#0*C987B654A321"
