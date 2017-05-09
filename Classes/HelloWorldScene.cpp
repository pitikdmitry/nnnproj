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

    map_path = "map/map_new.tmx";
    mapAmountOfObjets = 4;

    groundObjects = { "object1", "object2", "object3", "object4", "water" };
//    background_path = "map/BG1.png";
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

    if (_map == nullptr)
    {
        std::cout << "ERROR";
        return 0;
    }

    _scale_map_x = _visibleSize.width / _map->getContentSize().width;
    _scale_map_y = _visibleSize.height / _map->getContentSize().height;

//    _map->setScaleX(_scale_map_x);
//    _map->setScaleY(_scale_map_y);
//    _map_centre = Point(_map->getContentSize().width / 2, _map->getContentSize().height / 2);



    return 0;
}

int HelloWorld::settingUpGroundObjects( )
{
    for (auto &NameObject: groundObjects)
    {
        ValueMap object = _objectGroup->getObject(NameObject);

        if (object == ValueMap())
        {
            std::cout << "ERROR";
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
            this->addChild(pol_node);
        }
        catch (std::out_of_range &err)
        {
            std::cout << "ERROR";
            return 0;
        }
    }
    return 0;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Scene::init() ) {
        return false;
    }

    _director = Director::getInstance();
    _visibleSize = _director->getVisibleSize();
    _origin = _director->getVisibleOrigin();
    Size size = Director::getInstance()->getVisibleSize();

    Vec2 vec_center(_visibleSize.width / 2, _visibleSize.height/2);

    int error = setupMap();
    if (error != 0)
    {
        std::cout << "ERROR";
        return false;
    }
    _map = TMXTiledMap::create(map_path);
    addChild(_map, 0);

    std::string nameGroupObject = "objects";
    _objectGroup = _map->getObjectGroup(nameGroupObject);
    if (_objectGroup == nullptr)
    {
        std::cout << "ERROR";
        return false;
    }

    PhysicsBody* edgeBox = PhysicsBody::createEdgeBox(_map->getContentSize());
    Node* edgeNode = Node::create();

    if (edgeBox == nullptr || edgeNode == nullptr)
    {
        std::cout << "ERROR";
        return false;
    }

    edgeNode->setPhysicsBody(edgeBox);
    edgeNode->setPosition(_map_centre);

//    this->addChild(edgeNode);

    error = settingUpGroundObjects( );
    if( error != 0 )
    {
        std::cout << "ERROR";
        return false;
    }

    zombi = Player::create();
    zombi->setPosition(Vec2(_origin.x + _visibleSize.width / 2 , _origin.y + _visibleSize.height / 2));
//    zombi->setPosition(Vec2(673,1212));
    zombi->initPlayer();
    this->addChild(zombi, 5);

    Follow* camera = Follow::create(zombi);
////    camera->setBoundary(-3 * size.width, 0, -size.height * 67 / 40, 0);
    this->runAction( camera );


    auto listener = EventListenerKeyboard::create();
    auto contactListener = EventListenerPhysicsContact::create();

    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::Pressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::Released, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    contactListener->onContactSeparate = CC_CALLBACK_1(HelloWorld::onContactSeparate, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt)
{
    zombi->update();
    for( auto it = zombies.begin( ))
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

bool HelloWorld::onContactBegin( cocos2d::PhysicsContact &contact)
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();

    if ((PLAYER_BITMASK == a->getCollisionBitmask() && GROUND_BITMASK == b->getCollisionBitmask()) ||
        (PLAYER_BITMASK == b->getCollisionBitmask() && GROUND_BITMASK == a->getCollisionBitmask()))
    {
        zombi->is_onGround = true;
    }
    return true;
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
