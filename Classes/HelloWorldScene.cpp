#include "HelloWorldScene.h"
#include <iostream>
#include "SimpleAudioEngine.h"


HelloWorld::HelloWorld()
{
    _map = nullptr;
    _objectGroup = nullptr;
    _director = nullptr;
    _visibleSize = Size::ZERO;
    _origin = Vec2::ZERO;

    map_path = "map/map.tmx";
    mapAmountOfObjets = 4;

    groundObjects = { "object1", "object2", "object3", "object4", "water" };
    background_path = "map/BG.png";
}

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();

    scene->getPhysicsWorld()->setFixedUpdateRate( 2000 );
    scene->getPhysicsWorld()->setSpeed(0.7f);
    scene->getPhysicsWorld()->setGravity(Vect(0.f, -10000.0f));
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

int HelloWorld::setupMap()
{
    _map = TMXTiledMap::create(map_path);
    if (_map == nullptr)
    {
        std::cout << "ERROR_MAP_SETUP";
        return 1;
    }

    _scale_map_x = _visibleSize.width / _map->getContentSize().width;
    _scale_map_y = _visibleSize.height / _map->getContentSize().height;

//    _map->setScaleX(_scale_map_x);
//    _map->setScaleY(_scale_map_y);
//    _map_centre = Point(_map->getContentSize().width / 2, _map->getContentSize().height / 2);



    addChild(_map, 0);
    return 0;
}

int HelloWorld::settingUpEdgeBox()
{
    PhysicsBody* edgeBox = PhysicsBody::createEdgeBox(_map->getContentSize());
    Node* edgeNode = Node::create();

    if (edgeBox == nullptr || edgeNode == nullptr)
    {
        std::cout << "ERROR_CREAT_EDGE_BOX";
        return 1;
    }

    edgeNode->setPhysicsBody(edgeBox);
    edgeNode->setPosition(_map_centre);

    this->addChild(edgeNode);

    return 0;
}

int HelloWorld::settingUpGroundObjects( )
{
    for (auto &NameObject: groundObjects)
    {
        ValueMap object = _objectGroup->getObject(NameObject);

        if (object == ValueMap())
        {
            std::cout << "ERROR_IN_SETTING_UP_OBEJCTS";
            return 0;
        }

        try
        {
            float x = 0.0f;
            float y = 0.0f;
            float height = 0.0f;
            float width = 0.0f;
            x = object.at("x").asFloat();
            y = object.at("y").asFloat();
            height = object.at("height").asFloat();
            width = object.at("width").asFloat();

            Vec2 rectangle[4];
            rectangle[0] = Vec2(x, y);
            rectangle[1] = Vec2(x, y + height);
            rectangle[2] = Vec2(x + width, y + height);
            rectangle[3] = Vec2(x + width, y);

            PhysicsBody* polygon = PhysicsBody::createPolygon(rectangle, 4, PhysicsMaterial(GROUND_DENSITY, GROUND_RESTITUTION, GROUND_FRICTION));
            polygon->setDynamic(false);
            polygon->setGravityEnable(false);
            polygon->setContactTestBitmask( true );
            polygon->setCollisionBitmask( GROUND_BITMASK );

            auto pol_node = Node::create();
            pol_node->setPhysicsBody(polygon);
            this->addChild(pol_node, 1);
        }
        catch (std::out_of_range &err)
        {
            std::cout << "ERROR_IN_SETTING_UP_OBEJCTS";
            return 0;
        }
    }
    return 0;
}

int HelloWorld::settingUpBackGround( )
{
        Sprite*  background = Sprite::create(background_path);
    if (background == nullptr)
    {

    }

    float scale_map_background_X = _map->getContentSize().width / background->getContentSize().width;
    float scale_map_background_Y = _map->getContentSize().height / background->getContentSize().height;

    if (scale_map_background_X > scale_map_background_Y)
    {
        background->setScale(scale_map_background_X);
    }
    else
    {
        background->setScale(scale_map_background_Y);
    }
    background->setPosition(_map_centre);
    addChild(background, -1);
    return 0;
}

int HelloWorld::settingUpEventListeners( )
{
    //слушает клавиатуру
    auto listener = EventListenerKeyboard::create();
    //слушает столкновения
    auto contactListener = EventListenerPhysicsContact::create();

    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::Pressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::Released, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(HelloWorld::onContactSeparate, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    return 0;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    _director = Director::getInstance();
    _visibleSize = _director->getVisibleSize();
    _origin = _director->getVisibleOrigin();
    Size size = Director::getInstance()->getVisibleSize();

    Vec2 vec_center(_origin.x + _visibleSize.width / 2, _origin.y + _visibleSize.height/2);

    int error = setupMap();
    if (error != 0)
    {
        std::cout << "ERROR_MAP";
        return false;
    }

    //Название группы объектов твердых тел на карте
    std::string nameGroupObject = "objects";
    _objectGroup = _map->getObjectGroup(nameGroupObject);
    if (_objectGroup == nullptr)
    {
        std::cout << "ERROR_GROUP";
        return false;
    }
    //достаем твердые тела из карты и отображаем их на карте
    error = settingUpGroundObjects( );
    if( error != 0 )
    {
        std::cout << "ERROR_OBJECTS";
        return false;
    }

    error = settingUpBackGround( );
    if( error != 0 )
    {
        std::cout << "ERROR_OBJECTS";
        return false;
    }
    //Делаем твердое тело по периметру карты
//    error = settingUpEdgeBox( );
    if( error != 0 )
    {
        std::cout << "ERROR_BIG_BOX";
        return false;
    }

    //Обработчик клавиатуры и столкновений
    error = settingUpEventListeners( );
    if( error != 0 )
    {
        std::cout << "ERROR_LISTENERS";
        return false;
    }

    //Инициализация анимация для персонажа
    error = InitAnimationsForZombie( );
    if( error != 0 )
    {
        std::cout << "ERROR_INIT_ZOBIE_ANIM";
        return false;
    }

    zombi = Zombi::create();
    zombi->setPosition(Vec2(_origin.x + _visibleSize.width / 2 + 100, _origin.y + _visibleSize.height / 2));
//    zombi->setPosition(Vec2(673,1212));
    zombi->initCharacter( idleAnimFrames, walkAnimFrames, attackAnimFrames, jumpAnimFrames, zombiBody);
    this->addChild(zombi, 5);


    human = Human::create();
    human->setPosition(Vec2(300,500));
//    zombi->setPosition(Vec2(673,1212));
    human->initCharacter();
    this->addChild(human, 7);

    human2 = Human::create();
    human2->setPosition(Vec2(_origin.x + _visibleSize.width / 2 + 250, _origin.y + _visibleSize.height / 2));
//    zombi->setPosition(Vec2(673,1212));
    human2->initCharacter();
    this->addChild(human2, 7);



    //Камера, будет следить за игроком
    Follow* camera = Follow::create(zombi);
//    camera->setBoundary(-3 * size.width, 0, -size.height * 67 / 40, 0);
    this->runAction( camera );
    creatingNewPlayer = false;

    //Означает что будет постоянно вызывать метод update данного класса(в дланном случае HelloWorld
    //Но я не уверена
    this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt)
{
    zombi->update();
    int i = 0;

    if( creatingNewPlayer == true ){
        Zombi* newZombi = Zombi::create();
        Vec2 playersPosition = zombi->getPhysicsBody()->getPosition();
        playersPosition.x -= 10;
//    newZombi->setPosition( playersPosition );
//    newZombi->setPosition(Vec2(_origin.x + _visibleSize.width / 2 + 100, _origin.y + _visibleSize.height / 2));
        newZombi->initCharacter( idleAnimFrames, walkAnimFrames, attackAnimFrames, jumpAnimFrames, zombiBody);
        newZombi->setPosition(Vec2(300,500));
        this->addChild(newZombi, 5);
        zombies.push_back( newZombi );
        creatingNewPlayer = false;
    }
    for( auto it = zombies.begin( ); it != zombies.end( ); ++it ){
        (*it)->update( );
    }
}

void HelloWorld::Pressed( EventKeyboard::KeyCode keyCode, Event* event )
{
    switch (keyCode)
    {
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            zombi->walking= true;
            zombi->direction = 0;
            zombi->key_A = true;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            zombi->walking = true;
            zombi->direction = 1;
            zombi->key_D = true;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            zombi->jumping = true;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            break;
        default:
            break;
    }
}

void HelloWorld::Released( EventKeyboard::KeyCode keyCode, Event* event )
{
    switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
            zombi->key_A = false;
            if(zombi->key_D == false)
            {
                zombi->walking= false;
            }
            else
            {
                zombi->direction = 1;
            }
            if(zombi->walking == false && zombi->jumping == false)
            {
                zombi->idling = true;
            }
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            zombi->key_D = false;
            if(zombi->key_A == false)
            {
                zombi->walking= false;
            }
            else
            {
                zombi->direction = 0;
            }
            if(zombi->walking == false && zombi->jumping == false)
            {
                zombi->idling = true;
            }
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            zombi->jumping = false;
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_S:
            break;
        default:
            break;
    }
}

int HelloWorld::createNewZombie( )
{

}

bool HelloWorld::onContactBegin( cocos2d::PhysicsContact &contact)
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();

    if ((PLAYER_BITMASK == a->getCollisionBitmask() && GROUND_BITMASK == b->getCollisionBitmask()) ||
        (PLAYER_BITMASK == b->getCollisionBitmask() && GROUND_BITMASK == a->getCollisionBitmask()))
    {
        zombi->is_onGround = true;
        return true;
    }
    else if ((HUMAN_BITMASK == a->getCollisionBitmask() && GROUND_BITMASK == b->getCollisionBitmask()) ||
        (HUMAN_BITMASK == b->getCollisionBitmask() && GROUND_BITMASK == a->getCollisionBitmask()))
    {
        return true;
    }

    else if (HUMAN_BITMASK == a->getCollisionBitmask() && PLAYER_BITMASK == b->getCollisionBitmask())
    {
//        createNewZombie();
//        creatingNewPlayer = true;
//        a->getNode()->removeFromParent();
        zombi->attacking = true;
        return false;
    }
    else if (HUMAN_BITMASK == b->getCollisionBitmask() && PLAYER_BITMASK == a->getCollisionBitmask())
    {
//        createNewZombie();
        creatingNewPlayer = true;
//        b->getNode()->removeFromParent();
        zombi->attacking = true;
        return false;
    }
}

bool HelloWorld::onContactSeparate( cocos2d::PhysicsContact &contact)
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();

    if ((PLAYER_BITMASK == a->getCollisionBitmask() && GROUND_BITMASK == b->getCollisionBitmask()) ||
        (PLAYER_BITMASK == b->getCollisionBitmask() && GROUND_BITMASK == a->getCollisionBitmask()))
    {
        zombi->is_onGround = false;
    }
    return true;
}


int HelloWorld::InitAnimationsForZombie( )
{
    zombiBody = PhysicsBody::createBox(Size(PLAYER_PHYSICS_BODY_WIDTH,PLAYER_PHYSICS_BODY_HEIGHT), PhysicsMaterial(PLAYER_DENSITY, PLAYER_RESTITUTION, PLAYER_FRICTION));
    zombiBody->PhysicsBody::setMass(0.1f);
    zombiBody->setDynamic(true);
    zombiBody->setVelocityLimit(SPEED_PLAYER_LIMIT);
    zombiBody->setGravityEnable(true);
    zombiBody->setRotationEnable(false);
    zombiBody->setContactTestBitmask(true);
    zombiBody->setCollisionBitmask(PLAYER_BITMASK);

    char str[100] = {0};

//    Vector<SpriteFrame*> idleAnimFrames(15);
    for (int i = 1; i <= 15; i++)
    {
        sprintf(str, "male/Idle (%i).png", i);
        auto frame = SpriteFrame::create(str, Rect(0, 0, 74, 115));
        frame->setAnchorPoint(Vec2(0.5, 0));
        idleAnimFrames.pushBack(frame);
    }
//    idleAnimation = Animation::createWithSpriteFrames(idleAnimFrames, 0.1f);
//    idleAnimate = Animate::create(idleAnimation);
//    idleAnimate->retain();
//    this->runAction(RepeatForever::create(idleAnimate));


//    Vector<SpriteFrame*> walkAnimFrames(10);
    for(int i = 1; i <= 10; i++)
    {
        sprintf(str, "male/Walk (%i).png",i);
        auto frame = SpriteFrame::create(str,Rect(0, 0, 87, 117));
        frame->setAnchorPoint(Vec2(0.5, 0));
        walkAnimFrames.pushBack(frame);
    }
//    walkAnimation = Animation::createWithSpriteFrames(walkAnimFrames, 0.1f);
//    walkAnimate = Animate::create(walkAnimation);
//    walkAnimate->retain();


//    Vector<SpriteFrame*> attackAnimFrames(8);
    for(int i = 1; i <= 8; i++)
    {
        sprintf(str, "male/Attack (%i).png",i);
        auto frame = SpriteFrame::create(str,Rect(0, 0, 97, 115));
        frame->setAnchorPoint(Vec2(0.5, 0));
        attackAnimFrames.pushBack(frame);
    }
//    attackAnimation = Animation::createWithSpriteFrames(attackAnimFrames, 0.1f);
//    attackAnimate = Animate::create(attackAnimation);
//    attackAnimate->retain();

    Vector<SpriteFrame*> jumpAnimFrames(5);
    for(int i = 1; i <= 5; i++)
    {
        sprintf(str, "male/Jump (%i).png",i);
        auto frame = SpriteFrame::create(str,Rect(0, 0, 87, 117));
        frame->setAnchorPoint(Vec2(0.5, 0));
        jumpAnimFrames.pushBack(frame);
    }
//    jumpAnimation = Animation::createWithSpriteFrames(jumpAnimFrames, 0.3f);
//    jumpAnimate = Animate::create(jumpAnimation);
//    jumpAnimate->retain();

    return 0;
}
