#ifndef KEYPADLISTENER_HPP
#define KEYPADLISTENER_HPP

/// @file

// this file contains doxygen lines

/// \brief
/// KeypadListener class
/// \details
/// this class implements a virtual function to write in the childs keypadchannel

class KeypadListener{
public:
    virtual void ButtonPressed(char c) = 0;

};


#endif // KEYPADLISTENER_HPP
