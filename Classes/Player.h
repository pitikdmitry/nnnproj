#ifndef MYGAME_PLAYER_H
#define MYGAME_PLAYER_H

#include "cocos2d.h"
USING_NS_CC;

#define SPEED_OF_THE_PLAYER 950.0f
#define SPEED_OF_THE_PLAYER_IN_THE_AIR 800.0f
#define PLAYER_BITMASK 0x000001
#define PLAYER_HEIGHT_OF_THE_JUMP 700

#define PLAYER_DENSITY 500.0f
#define PLAYER_RESTITUTION 0.0000001f
#define PLAYER_FRICTION 1.0f

#define PLAYER_PHYSICS_BODY_WIDTH 149.0f
#define PLAYER_PHYSICS_BODY_HEIGHT 230.0f
#define SPEED_PLAYER_LIMIT 2700.0f

class Player: public cocos2d::Sprite
{
public:
    static Player* create();
    virtual void initPlayer();
    Animate* idleAnimate; //объект для анимации
    Animate* walkAnimate;
    //Animate* attackAnimate;
    Animate* jumpAnimate;

    void update();

    void walk();
   // void attack();
    void idle();
    void jump();

    bool walking;
    bool attacking;
    bool idling;
    bool jumping;

    bool key_A;
    bool key_D;

    bool is_onGround;


    int direction;
    enum{ idling_anim, walking_anim, attacking_anim, jumping_anim } current_anim;



protected:



};


#endif //MYGAME_PLAYER_H
