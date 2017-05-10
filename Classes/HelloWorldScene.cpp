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

    //путь до карты
    map_path = "map/map.tmx";
    mapAmountOfObjets = 23;

    //названия объектов на карте
    char str[100] = {0};

    for( int i = 1; i <= 23; ++i ){
        sprintf(str, "object%i", i);
        groundObjects.push_back(str);
    }
    groundObjects.push_back("water");
//    groundObjects = { "object1", "object2", "object3", "object4", "object5",
//                      "water" };
    background_path = "map/BG.png";
}

Scene* HelloWorld::createScene()
{
    // создаем сцену с физикой
    auto scene = Scene::createWithPhysics();
    //как часто обновляется сцена
    scene->getPhysicsWorld()->setFixedUpdateRate( 2000 );
    //скорость мира
    scene->getPhysicsWorld()->setSpeed(0.7f);
    //граивтация
    scene->getPhysicsWorld()->setGravity(Vect(0.f, -10000.0f));
    //обводка твердых тел
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

int HelloWorld::setupMap()
{
    //создаем карту
    _map = TMXTiledMap::create(map_path);
    if (_map == nullptr)
    {
        std::cout << "ERROR_MAP_SETUP";
        return 1;
    }

    //растянуть карту чтобы, возможно не используем
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
            //получаем координату икс объекта
            x = object.at("x").asFloat();
            y = object.at("y").asFloat();
            //получаем высоту и ширину
            height = object.at("height").asFloat();
            width = object.at("width").asFloat();

            //массив четырех точек(координаты объекту
            Vec2 rectangle[4];
            rectangle[0] = Vec2(x, y);
            rectangle[1] = Vec2(x, y + height);
            rectangle[2] = Vec2(x + width, y + height);
            rectangle[3] = Vec2(x + width, y);

            //создаем твердое тело для нашей платформы
            PhysicsBody* polygon = PhysicsBody::createPolygon(rectangle, 4, PhysicsMaterial(GROUND_DENSITY, GROUND_RESTITUTION, GROUND_FRICTION));
            polygon->setDynamic(false);
            polygon->setGravityEnable(false);
            polygon->setContactTestBitmask( true );
            if( NameObject == "water"){
                polygon->setCollisionBitmask( WATER_BITMASK );
            }
            else{
                polygon->setCollisionBitmask( GROUND_BITMASK );
            }

            //Создаем объект в который кладем твердое тело
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
    //центр видимого окна
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

    //установка фона возможно не будет
//    error = settingUpBackGround( );
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

    //Расставляем человеков на карте
    Vec2 human1(600, 930);
    Vec2 human2(1100, 700);
    Vec2 human3(2330, 1470);
    Vec2 human4(3100, 1050);
    Vec2 human5(4430, 1500);
    Vec2 human6(5000, 630);
    Vec2 human7(6000, 1150);

    error = CreatingHumans( human1 );
    error = CreatingHumans( human2 );
    error = CreatingHumans( human3 );
    error = CreatingHumans( human4 );
    error = CreatingHumans( human5 );
    error = CreatingHumans( human6 );
    error = CreatingHumans( human7 );
    if( error != 0 )
    {
        std::cout << "ERROR_INIT_ZOBIE_ANIM";
        return false;
    }

    auto zombi = Zombi::create();
    zombi->initCharacter(idleAnimFrames, walkAnimFrames, attackAnimFrames, jumpAnimFrames, zombiBody);
    zombi->setPosition(Vec2(1500, 1500));
    zombies.push_back(zombi);
    addChild(zombi);

    //Камера, будет следить за игроком
    camera = Follow::create(zombi);
//    camera->setBoundary(-3 * size.width, 0, -size.height * 67 / 40, 0);
    this->runAction( camera );
    cameraFollowing = zombi;

    creatingPlayer = false;

    //setting up music
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("ZHU – One Minute to Midnight.mp3", true);

    //Означает что будет постоянно вызывать метод update данного класса(в дланном случае HelloWorld
    //Но я не уверена
    this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt)
{
//    zombi->update();
    if( creatingPlayer == true){
        createNewZombie( );
        creatingPlayer = false;
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
            for( auto &zomb: zombies ){
                zomb->walking= true;
                zomb->direction = 0;
                zomb->key_A = true;
            }
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            for( auto &zomb: zombies ) {
                zomb->walking = true;
                zomb->direction = 1;
                zomb->key_D = true;
            }
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            for( auto &zomb: zombies ) {
                zomb->jumping = true;
            }
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
            for( auto &zomb: zombies ) {
                zomb->key_A = false;
                if (zomb->key_D == false) {
                    zomb->walking = false;
                } else {
                    zomb->direction = 1;
                }
                if (zomb->walking == false && zomb->jumping == false) {
                    zomb->idling = true;
                }
            }
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_D:
            for( auto &zomb: zombies ) {
                zomb->key_D = false;
                if (zomb->key_A == false) {
                    zomb->walking = false;
                } else {
                    zomb->direction = 0;
                }
                if (zomb->walking == false && zomb->jumping == false) {
                    zomb->idling = true;
                }
            }

            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case cocos2d::EventKeyboard::KeyCode::KEY_W:
            for( auto &zomb: zombies ) {
                zomb->jumping = false;
            }
//            zombi2->jumping = false;
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
    auto zombi = Zombi::create();
    zombi->initCharacter(idleAnimFrames, walkAnimFrames, attackAnimFrames, jumpAnimFrames, zombiBody);
    auto it = zombies.begin( );
    Vec2 point2 = zombi->convertToWorldSpace((*it)->getPosition());
    point2.y +=50;
    zombi->setPosition(point2);
    zombies.push_back(zombi);
    addChild(zombi);

    return 0;
}

bool HelloWorld::onContactBegin( cocos2d::PhysicsContact &contact)
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();

    if (HUMAN_BITMASK == a->getCollisionBitmask() && PLAYER_BITMASK == b->getCollisionBitmask())
    {
        if( !creatingPlayer ){
            creatingPlayer = true;
        }
        a->getNode()->removeFromParent();
        for( auto &zomb: zombies ) {
            zomb->attacking = true;
        }
        return true;
    }
    else if (HUMAN_BITMASK == b->getCollisionBitmask() && PLAYER_BITMASK == a->getCollisionBitmask())
    {
        if( !creatingPlayer ){
            creatingPlayer = true;
        }
        b->getNode()->removeFromParent();
        for( auto &zomb: zombies ) {
            zomb->attacking = true;
        }
        return false;
    }
    else if ((PLAYER_BITMASK == a->getCollisionBitmask() && GROUND_BITMASK == b->getCollisionBitmask()) ||
        (PLAYER_BITMASK == b->getCollisionBitmask() && GROUND_BITMASK == a->getCollisionBitmask()))
    {
        for( auto &zomb: zombies ) {
            if( zomb->getPhysicsBody() == a || zomb->getPhysicsBody() == b ){
                zomb->is_onGround = true;
            }
        }
//        zombi2->is_onGround = true;
        return true;
    }
    else if ((HUMAN_BITMASK == a->getCollisionBitmask() && GROUND_BITMASK == b->getCollisionBitmask()) ||
        (HUMAN_BITMASK == b->getCollisionBitmask() && GROUND_BITMASK == a->getCollisionBitmask()))
    {
        return true;
    }
    else if ((PLAYER_BITMASK == a->getCollisionBitmask() && PLAYER_BITMASK == b->getCollisionBitmask()) ||
             (PLAYER_BITMASK == b->getCollisionBitmask() && PLAYER_BITMASK == a->getCollisionBitmask()))
    {

//        zombi2->is_onGround = true;
        return false;
    }
    else if ((PLAYER_BITMASK == a->getCollisionBitmask() && WATER_BITMASK == b->getCollisionBitmask()) ||
        (PLAYER_BITMASK == b->getCollisionBitmask() && WATER_BITMASK == a->getCollisionBitmask()))
    {
        auto it = zombies.begin( );
        if( (*it)->getPhysicsBody() == a || (*it)->getPhysicsBody() == b ){
            //GAME OVVEVERRERE
            //GAME OVVEVERRERE
            //GAME OVVEVERRERE
            //GAME OVVEVERRERE

        }
        else{
            ++it;
            for( it; it != zombies.end( ); ++it ){
                if(  (*it)->getPhysicsBody() == a || (*it)->getPhysicsBody() == b ){
                    (*it)->removeFromParent();
                    zombies.erase(it);
                    return true;
                }
            }
        }

        return true;
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
        for( auto &zomb: zombies ) {
            if( zomb->getPhysicsBody() == a || zomb->getPhysicsBody() == b ){
                zomb->is_onGround = false;
            }
        }
//        zombi2->is_onGround = false;
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

int HelloWorld::CreatingHumans( Vec2 position )
{
    auto human = Human::create();
    human->setPosition(position);
//    zombi->setPosition(Vec2(673,1212));
    human->initCharacter();
    this->addChild(human, 7);
    return 0;
}
