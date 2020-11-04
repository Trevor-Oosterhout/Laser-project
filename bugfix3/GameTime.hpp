#ifndef GAMETIME_HPP
#define GAMETIME_HPP

struct GameTime{
private:
    int starttime, gametime;

public:
    void SetWaitTime(int s){
        starttime = s;
    }


    int GetWaitTime(){
        return starttime;
    }

    void SetTime(int s){
        gametime = s;
    }

    int GetTime(){
        return gametime;
    }



};


#endif // GAMETIME_HPP
