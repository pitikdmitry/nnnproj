#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "Zombi.h"
#include "Human.h"
#include "cocos2d.h"
USING_NS_CC;

#define GROUND_BITMASK 0x0005
#define WATER_BITMASK 0x0004

#define GROUND_DENSITY 1000.0f
#define GROUND_RESTITUTION 0.0000001f
#define GROUND_FRICTION 5.0f


class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    HelloWorld();
//    ~HelloWorld();

    virtual bool init();
    Sprite* player;
    bool creatingPlayer;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    //карта
    TMXTiledMap *_map;
    //центр карты
    Point _map_centre;
    //служебный режиссер кокоса
    cocos2d::Director *_director;
    //Размер окна
    cocos2d::Size _visibleSize;
    //смещение точки (0,0) нашего окна в пространственных координатах, но я не уверена
    Vec2 _origin;

    std::list<Zombi*> zombies;
    std::list<Human*> humans;

//    Animation* idleAnimation; //объект для анимации
//    Animation* walkAnimation; //объект для анимации
//    Animation* attackAnimation; //объект для анимации
//    Animation* jumpAnimation; //объект для анимации
    Vector<SpriteFrame*> idleAnimFrames;
    Vector<SpriteFrame*> walkAnimFrames;
    Vector<SpriteFrame*> attackAnimFrames;
    Vector<SpriteFrame*> jumpAnimFrames;
    PhysicsBody* zombiBody;
    //группа объекто твердых тел из карты
    TMXObjectGroup *_objectGroup;
    //массив с названиями объектов
    std::vector<std::string> groundObjects;
    //камера
    Follow* camera;
    Zombi* cameraFollowing;

    int mapAmountOfObjets;
    std::string map_path;

    //карту устанавливаем
    std::string background_path;
    int setupMap();
    int settingUpGroundObjects( );
    int settingUpEdgeBox( );
    int settingUpBackGround( );
    int settingUpEventListeners( );

    int CreatingHumans( Vec2 position );
    int InitAnimationsForZombie( );
    int createNewZombie( );

    void update(float dt);

    bool onContactBegin( cocos2d::PhysicsContact &contact);
    bool onContactSeparate( cocos2d::PhysicsContact &contact);

    void Pressed( EventKeyboard::KeyCode keyCode, Event* event );
    void Released( EventKeyboard::KeyCode keyCode, Event* event );

    float _scale_map_x;
    float _scale_map_y;

    int count;
};

#endif // __HELLOWORLD_SCENE_H__