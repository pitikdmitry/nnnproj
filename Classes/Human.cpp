//
// Created by nyam on 10.05.17.
//

#include "Human.h"

Human* Human::create()
{
    Human* human = new Human();
    if(human && human->initWithFile ("girl2/Idle (1).png" ))
    {
        human->autorelease();
        return human;
    }

    CC_SAFE_DELETE(human);
    return NULL;
}

void Human::initCharacter()
{
    walking = false;
    attacking = false;
    jumping = false;
    idling = true;
    direction = 1;
    key_A = false;
    key_D = false;
    life = true;
    char str[100] = {0};

    auto PlayerBody = PhysicsBody::createBox(Size(PLAYER_PHYSICS_BODY_WIDTH,PLAYER_PHYSICS_BODY_HEIGHT), PhysicsMaterial(PLAYER_DENSITY, PLAYER_RESTITUTION, PLAYER_FRICTION));
    PlayerBody->PhysicsBody::setMass(0.1f);
    PlayerBody->setDynamic(true);
    PlayerBody->setVelocityLimit(SPEED_PLAYER_LIMIT);
    PlayerBody->setGravityEnable(true);
    PlayerBody->setRotationEnable(false);
    PlayerBody->setContactTestBitmask(true);
    PlayerBody->setCollisionBitmask(HUMAN_BITMASK);
    addComponent(PlayerBody);

    Vector<SpriteFrame*> idleAnimFrames(16);
    for (int i = 1; i <= 16; i++)
    {
        sprintf(str, "girl2/Idle (%i).png", i);
        auto frame = SpriteFrame::create(str, Rect(0, 0, HUMAN_SPRITE_WIDTH, HUMAN_SPRITE_WIDTH));
        frame->setAnchorPoint(Vec2(0.5, 0));
        idleAnimFrames.pushBack(frame);
    }
    auto idleAnimation = Animation::createWithSpriteFrames(idleAnimFrames, 0.1f);
    idleAnimateZombi = Animate::create(idleAnimation);
    idleAnimateZombi->retain();
    this->runAction(RepeatForever::create(idleAnimateZombi));


    Vector<SpriteFrame*> walkAnimFrames(20);
    for(int i = 1; i <= 20; i++)
    {
        sprintf(str, "girl2/Walk (%i).png",i);
        auto frame = SpriteFrame::create(str,Rect(0, 0, HUMAN_SPRITE_WIDTH, HUMAN_SPRITE_WIDTH));
        frame->setAnchorPoint(Vec2(0.5, 0));
        walkAnimFrames.pushBack(frame);
    }
    auto walkAnimation = Animation::createWithSpriteFrames(walkAnimFrames, 0.1f);
    walkAnimateZombi = Animate::create(walkAnimation);
    walkAnimateZombi->retain();
    
}