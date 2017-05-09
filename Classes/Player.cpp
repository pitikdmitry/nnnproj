#include "Player.h"

Player* Player::create()
{
    Player* zombi = new Player();
    if(zombi && zombi->initWithFile ("male/Idle (1).png" ))
    {
        zombi->autorelease();
        return zombi;
    }

    CC_SAFE_DELETE(zombi);
    return NULL;
}

void Player::walk()
{
    current_anim = walking_anim;
    this->stopAllActions();
    this->runAction(RepeatForever::create(walkAnimate));
}
//void Player::attack()
//{
//    current_anim = attacking_anim;
//    this->stopAllActions( );
//    this->runAction(RepeatForever::create( attackAnimate ));
//}
void Player::idle()
{
    current_anim = idling_anim;
    this->stopAllActions();
    this->runAction(RepeatForever::create(idleAnimate));
}
void Player::jump()
{
    current_anim = jumping_anim;
    this->stopAllActions();
    this->runAction(RepeatForever::create(jumpAnimate));
}

void Player::initPlayer()
{
    walking = false;
    attacking = false;
    jumping = false;
    idling = true;
    direction = 1;
    key_A = false;
    key_D = false;
    char str[100] = {0};

    Vector<SpriteFrame*> idleAnimFrames(15);
    for (int i = 1; i <= 15; i++)
    {
        sprintf(str, "male/Idle (%i).png", i);
        auto frame = SpriteFrame::create(str, Rect(0, 0, 149, 230));
        frame->setAnchorPoint(Vec2(0.5, 0));
        idleAnimFrames.pushBack(frame);
    }
    auto idleAnimation = Animation::createWithSpriteFrames(idleAnimFrames, 0.1f);
    idleAnimate = Animate::create(idleAnimation);
    idleAnimate->retain();
    this->runAction(RepeatForever::create(idleAnimate));


    Vector<SpriteFrame*> walkAnimFrames(10);
    for(int i = 1; i <= 10; i++)
    {
        sprintf(str, "male/Walk (%i).png",i);
        auto frame = SpriteFrame::create(str,Rect(0, 0, 149, 230));
        frame->setAnchorPoint(Vec2(0.5, 0));
        walkAnimFrames.pushBack(frame);
    }
    auto walkAnimation = Animation::createWithSpriteFrames(walkAnimFrames, 0.1f);
    walkAnimate = Animate::create(walkAnimation);
    walkAnimate->retain();


//    Vector<SpriteFrame*> attackAnimFrames(8);
//    for(int i = 1; i <= 8; i++)
//    {
//        sprintf(str, "male/Attack (%i).png",i);
//        auto frame = SpriteFrame::create(str,Rect(0, 0, 430, 519));
//        frame->setAnchorPoint(Vec2(0.5, 0));
//        attackAnimFrames.pushBack(frame);
//    }
//    auto attackAnimation = Animation::createWithSpriteFrames(attackAnimFrames, 0.1f);
    //attackAnimate = Animate::create(attackAnimation);
    //attackAnimate->retain();

    Vector<SpriteFrame*> jumpAnimFrames(5);
    for(int i = 2; i <= 6; i++)
    {
        sprintf(str, "male/Walk (%i).png",i);
        auto frame = SpriteFrame::create(str,Rect(0, 0, 149, 230));
        frame->setAnchorPoint(Vec2(0.5, 0));
        jumpAnimFrames.pushBack(frame);
    }
    auto jumpAnimation = Animation::createWithSpriteFrames(jumpAnimFrames, 0.3f);
    jumpAnimate = Animate::create(jumpAnimation);
    jumpAnimate->retain();

    auto PlayerBody = PhysicsBody::createBox(Size(PLAYER_PHYSICS_BODY_WIDTH,PLAYER_PHYSICS_BODY_HEIGHT), PhysicsMaterial(PLAYER_DENSITY, PLAYER_RESTITUTION, PLAYER_FRICTION));
    PlayerBody->PhysicsBody::setMass(0.1f);
    PlayerBody->setDynamic(true);
    PlayerBody->setVelocityLimit(SPEED_PLAYER_LIMIT);
    PlayerBody->setGravityEnable(true);
    PlayerBody->setRotationEnable(false);
    PlayerBody->setContactTestBitmask(true);
    PlayerBody->setCollisionBitmask(PLAYER_BITMASK);
    addComponent(PlayerBody);

}


void Player::update()
{
    if ( false == jumping && false == walking ) {

        if ( current_anim != idling_anim && true == is_onGround) {
            idle();
        }
    }

    if (walking) {
        float speed = 0;

        if (true == is_onGround) {
            speed = SPEED_OF_THE_PLAYER;
        } else {
            speed = SPEED_OF_THE_PLAYER_IN_THE_AIR;
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

        if (true == is_onGround && false == jumping && current_anim != walking_anim) {
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






















