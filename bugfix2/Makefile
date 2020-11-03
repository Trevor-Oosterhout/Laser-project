#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
#
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES := shootcontrol.cpp

# header files in this project
HEADERS := SignalDetector.hpp MSGverifier.hpp MSGdecoder.hpp TimeControl.hpp shootcontrol.hpp ScoreControl.hpp RegistrationControl.hpp PlayerConfig.hpp pinout.hpp Lives.hpp LeaderControl.hpp Keypadlistener.hpp Keypad.hpp IRmessageControl.hpp GameTime.hpp Display.hpp

# other places to look for files for this project
SEARCH  :=

# set REATIVE to the next higher directory
# and defer to the Makefile.due there
RELATIVE := $(RELATIVE)../
include $(RELATIVE)Makefile.due
