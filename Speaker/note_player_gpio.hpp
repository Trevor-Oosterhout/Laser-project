#ifndef _NOTE_PLAYER_GPIO_HPP
#define _NOTE_PLAYER_GPIO_HPP

#include "note_player.hpp"

// ===========================================================================
//
// simple note player for a speaker connected to a GPIO pin
//
// ===========================================================================

class note_player_gpio : public note_player {
private:   
   hwlib::pin_out & lsp;
public: 
   note_player_gpio( hwlib::pin_out & lsp ):
      lsp( lsp )
   {}
   
   void play( const note & n ){
   if( n.frequency == 0 ){
      hwlib::wait_us( n.duration );

   } else {
      auto half_period = 1'000'000 / ( 2 * n.frequency );    
      auto end = hwlib::now_us() + n.duration;
      do { 
         lsp.write( 1 );
         lsp.flush();
         hwlib::wait_us( half_period );
         lsp.write( 0 );
         lsp.flush();
         hwlib::wait_us( half_period );
      } while ( end > hwlib::now_us() );
   }     
}

};

#endif
