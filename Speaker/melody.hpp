#ifndef _MELODY_HPP
#define _MELODY_HPP

#include "note_player.hpp"

// an abstract melody

class melody {
public:
   void play( note_player & p ){
   	p.play( note{ 700, 350000 } );
}

};

#endif
