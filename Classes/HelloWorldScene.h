#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include <vector>
USING_NS_CC;

#define GROUND_BITMASK 0x000002
#define GROUND_DENSITY 1000.0f
#define GROUND_RESTITUTION 0.0000001f
#define GROUND_FRICTION 5.0f

class HelloWorld : public cocos2d::Scene
{
public:
    HelloWorld();
//    ~HelloWorld();
    static cocos2d::Scene* createScene();

    void update(float dt);
    bool onContactBegin( cocos2d::PhysicsContact &contact);
    bool onContactSeparate( cocos2d::PhysicsContact &contact);

    virtual bool init();

    void Pressed( EventKeyboard::KeyCode keyCode, Event* event );
    void Released( EventKeyboard::KeyCode keyCode, Event* event );

    CREATE_FUNC(HelloWorld);
private:

    TMXTiledMap *_map;
    Point _map_centre;
    TMXObjectGroup *_objectGroup;
    std::vector<Player*> zombies;
    Player* zombi;
    cocos2d::Director *_director;
    cocos2d::Size _visibleSize;
    Vec2 _origin;
    std::vector<std::string> groundObjects;
//    Follow* camera;

    int mapAmountOfObjets;
    std::string map_path;

    std::string background_path;
    int setupMap();
    int settingUpGroundObjects( );
    int createNewZombie( );

    float _scale_map_x;
    float _scale_map_y;


};

#endif // __HELLOWORLD_SCENE_H__


