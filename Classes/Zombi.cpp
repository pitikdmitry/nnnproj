#include "Zombi.h"

Zombi* Zombi::create()
{
    Zombi* zombi = new Zombi();
    if(zombi && zombi->initWithFile ("male/Idle (1).png" ))
    {
        zombi->autorelease();
        return zombi;
    }

    CC_SAFE_DELETE(zombi);
    return NULL;
}

void Zombi::initCharacter( Vector<SpriteFrame*> idleAnimFrames, Vector<SpriteFrame*> walkAnimFrames, Vector<SpriteFrame*> attackAnimFrames,
                           Vector<SpriteFrame*> jumpAnimFrames, PhysicsBody* zombiBody2 )
{
    walking = false;
    attacking = false;
    jumping = false;
    idling = true;
    direction = 1;
    key_A = false;
    key_D = false;
    life = true;

    attackTimer = 0;

    auto zombiBody = PhysicsBody::createBox(Size(PLAYER_PHYSICS_BODY_WIDTH,PLAYER_PHYSICS_BODY_HEIGHT), PhysicsMaterial(PLAYER_DENSITY, PLAYER_RESTITUTION, PLAYER_FRICTION));
    zombiBody->PhysicsBody::setMass(0.1f);
    zombiBody->setDynamic(true);
    zombiBody->setVelocityLimit(SPEED_PLAYER_LIMIT);
    zombiBody->setGravityEnable(true);
    zombiBody->setRotationEnable(false);
    zombiBody->setContactTestBitmask(true);
    zombiBody->setCollisionBitmask(PLAYER_BITMASK);

    addComponent(zombiBody);

    auto idleAnimation = Animation::createWithSpriteFrames(idleAnimFrames, 0.1f);
    idleAnimateZombi = Animate::create(idleAnimation);
    idleAnimateZombi->retain();
    auto walkAnimation = Animation::createWithSpriteFrames(walkAnimFrames, 0.1f);
    walkAnimateZombi = Animate::create(walkAnimation);
    walkAnimateZombi->retain();
    auto attackAnimation = Animation::createWithSpriteFrames(attackAnimFrames, 0.1f);
    attackAnimateZombi = Animate::create(attackAnimation);
    attackAnimateZombi->retain();
    auto jumpAnimation = Animation::createWithSpriteFrames(jumpAnimFrames, 0.3f);
    jumpAnimateZombi = Animate::create(jumpAnimation);
    jumpAnimateZombi->retain();

//    idleAnimateZombi = idleAnimate; //объект для анимации
//    walkAnimateZombi = walkAnimate;
//    attackAnimateZombi = attackAnimate;
//    jumpAnimateZombi = jumpAnimate;
}


void Zombi::update()
{
    if ( false == jumping && false == walking && false == attacking ) {

        if ( current_anim != idling_anim && true == is_onGround) {
            idle();
        }
    }

    if( attacking ){
        if( current_anim != attacking_anim ){
            attack( );
        }
        ++attackTimer;
        if( attackTimer > 50 ){
            attacking = false;
            attackTimer = 0;
        }
    }

    if (walking) {
        float speed = 0;

        if (true == is_onGround && attacking == false ) {
            speed = SPEED_OF_THE_PLAYER;
        }
        else if( false == is_onGround && attacking == false ){
            speed = SPEED_OF_THE_PLAYER_IN_THE_AIR;
        }
        else{
            speed = SPEED_WHILE_ATTACKING;
        }
        if (direction == 0) {
            this->setScaleX(-1);
            Vec2 body_velocity = this->getPhysicsBody()->getVelocity();
            body_velocity.x = -speed;
            this->getPhysicsBody()->setVelocity(body_velocity);
        }
        else if (direction == 1) {
            this->setScaleX(1);
            Vec2 body_velocity = this->getPhysicsBody()->getVelocity();
            body_velocity.x = speed;
            this->getPhysicsBody()->setVelocity(body_velocity);
        }

        if (true == is_onGround && false == jumping && false == attacking && current_anim != walking_anim) {
            walk();
        }
    }

    if (jumping ) {
        if (current_anim != jumping_anim) {
            jump();
        }
        if (true == is_onGround) {
            this->getPhysicsBody()->applyImpulse(Vec2(0, PLAYER_HEIGHT_OF_THE_JUMP));
        }
    }

}


void Zombi::walk()
{
    current_anim = walking_anim;
    this->stopAllActions();
    this->runAction(RepeatForever::create(walkAnimateZombi));
}

void Zombi::attack()
{
    current_anim = attacking_anim;
    this->stopAllActions( );
    this->runAction(RepeatForever::create(attackAnimateZombi));
}

void Zombi::idle()
{
    current_anim = idling_anim;
    this->stopAllActions();
    this->runAction(RepeatForever::create(idleAnimateZombi));
}

void Zombi::jump()
{
    current_anim = jumping_anim;
    this->stopAllActions();
    this->runAction(RepeatForever::create(jumpAnimateZombi));
}



















