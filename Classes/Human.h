//
// Created by nyam on 10.05.17.
//

#ifndef MYGAME_ZOMBI_H
#define MYGAME_ZOMBI_H
#include "Zombi.h"

#define HUMAN_SPRITE_WIDTH 82
#define HUMAN_SPRITE_WIDTH 90

#define HUMAN_BITMASK 0x000002

class Human : public Zombi {
public:
    void initCharacter();
    static Human* create();

private:
};


#endif //MYGAME_ZOMBI_H