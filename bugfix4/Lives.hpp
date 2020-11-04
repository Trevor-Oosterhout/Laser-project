#ifndef LIVES_HPP
#define LIVES_HPP


struct Lives{
private:
    int lives;

public:

    void SetLives(int l){
        lives = l;
    }

    int GetLives(){
        return lives;
    }
};



#endif // LIVES_HPP
