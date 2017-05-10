#ifndef MYGAME_PLAYER_H
#define MYGAME_PLAYER_H

#include "cocos2d.h"
USING_NS_CC;

#define SPEED_OF_THE_PLAYER 950.0f
#define SPEED_OF_THE_PLAYER_IN_THE_AIR 800.0f
#define PLAYER_HEIGHT_OF_THE_JUMP 700

#define SPEED_WHILE_ATTACKING 500.0f

#define PLAYER_BITMASK 0x000001
#define PLAYER_DENSITY 500.0f
#define PLAYER_RESTITUTION 0.0000001f

#define PLAYER_FRICTION 1.0f
#define PLAYER_PHYSICS_BODY_WIDTH 87.0f
#define PLAYER_PHYSICS_BODY_HEIGHT 117.0f

#define SPEED_PLAYER_LIMIT 2700.0f

class Zombi: public cocos2d::Sprite
{
public:
    Zombi( ): walking( false ), attacking( false ), idling( false ), jumping( false ), life( false ) {}
    static Zombi* create();
//    void initCharacter( Animation* idleAnimation, Animation* walkAnimation, Animation* attackAnimation,
//                        Animation* jumpAnimation, PhysicsBody* zombiBody );
    void initCharacter( Vector<SpriteFrame*> idleAnimFrames, Vector<SpriteFrame*> walkAnimFrames, Vector<SpriteFrame*> attackAnimFrames,
                               Vector<SpriteFrame*> jumpAnimFrames, PhysicsBody* zombiBody2 );

    virtual void update();

    void walk();
    void attack();
    void idle();
    void jump();

    bool walking;
    bool attacking;
    bool idling;
    bool jumping;

    bool life;

    bool key_A;
    bool key_D;

    bool is_onGround;

    Animate* idleAnimateZombi; //объект для анимации
    Animate* walkAnimateZombi;
    Animate* attackAnimateZombi;
    Animate* jumpAnimateZombi;

    int direction;
    enum{ idling_anim, walking_anim, attacking_anim, jumping_anim } current_anim;

protected:

private:
    unsigned int attackTimer;

};


#endif //MYGAME_PLAYER_H
