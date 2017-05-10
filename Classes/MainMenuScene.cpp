
#include "HelloWorldScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

const float ENEMY_SCALE_FACTOR = 1.4f;

Scene* MainMenu::createScene()
{
    auto scene = Scene::create();

    auto layer = MainMenu::create();

    scene->addChild(layer);

    return scene;
}

bool MainMenu::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(MainMenu::menuCloseCallback, this));

    closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                 origin.y + closeItem->getContentSize().height/2));


    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    auto ZombieLabel = Label::createWithTTF("Zombies", "fonts/Marker Felt.ttf", 120);
    ZombieLabel->setTextColor(Color4B::RED);
    ZombieLabel->setPosition(visibleSize.width/2, visibleSize.height/2 + ZombieLabel->getContentSize().height + 100);
    this->addChild(ZombieLabel);
    auto ZombieLabel1 = Label::createWithTTF("One Minute to Midnight", "fonts/Marker Felt.ttf", 50);
    ZombieLabel1->setTextColor(Color4B::BLACK);
    ZombieLabel1->setPosition(visibleSize.width/2, visibleSize.height/2 + ZombieLabel1->getContentSize().height + 60);
    this->addChild(ZombieLabel1);

    Sprite *zombieGameOver = Sprite::create("male/Attack (3).png");
    zombieGameOver->setPosition(Point(650, 100));
    zombieGameOver->setAnchorPoint(Point::ZERO);
    zombieGameOver->setScale(ENEMY_SCALE_FACTOR);
    zombieGameOver->setFlippedX(true);
    zombieGameOver->retain();
    this->addChild(zombieGameOver);

    for (int i = 0; i < 7; ++i) {
        Sprite *girlGameOver = Sprite::create("Idle_1.png");
        girlGameOver->setPosition(Point(100 + i*80, 100));
        girlGameOver->setAnchorPoint(Point::ZERO);
        girlGameOver->setScale(ENEMY_SCALE_FACTOR);
        girlGameOver->setFlippedX(true);
        girlGameOver->retain();
        this->addChild(girlGameOver);
    }
    auto menuitem_1 = MenuItemFont::create("Play", CC_CALLBACK_1(MainMenu::Play, this));
    auto menuitem_3 = MenuItemFont::create("Help", CC_CALLBACK_1(MainMenu::Help, this));

    menuitem_1->setColor(ccc3(160, 32, 240)); menuitem_1->setFontSize(100);
    menuitem_3->setColor(ccc3(0, 0, 255));


    menuitem_1->setPosition(Point(visibleSize.width / 4, (visibleSize.height / 4) * 4));
    menuitem_3->setPosition(Point((visibleSize.width / 4), (visibleSize.height / 4) * 2));

    menu = Menu::create(menuitem_1, menuitem_3,  NULL);
    menu->alignItemsVertically();
    this->addChild(menu);

    //add background for game
    auto bgImage = Sprite::create("BG.png");
    bgImage->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    // add background image at z-position = -1, bottom of all
    // the z-position value is relative to parent
    this->addChild(bgImage, -1);
    // calculate the scaling factor to fill the screen size
    float rX = visibleSize.width / bgImage->getContentSize().width;
    float rY = visibleSize.height / bgImage->getContentSize().height;
    // set the scaling factor to the background image
    bgImage->setScaleX(rX);
    bgImage->setScaleY(rY);
    return true;
}

void MainMenu::Play(Ref *pSender)
{
    CCScene *mScene = HelloWorld::createScene();

    CCDirector::sharedDirector()->replaceScene(mScene);

}


void MainMenu::Help(Ref *pSender)
{   Size visibleSize = Director::getInstance()->getVisibleSize();
    auto startAgainLabel = Label::createWithTTF("On all the issues that arise, \n contact Ekaterina Sukhanova "
                                                        "and Anastasia Tyulkina.", "fonts/Marker Felt.ttf", 30);
    startAgainLabel->setTextColor(Color4B::BLACK);
    startAgainLabel->setPosition(visibleSize.width/2 - 100, visibleSize.height/2 + startAgainLabel->getContentSize().height- 150);
    this->addChild(startAgainLabel);

}


void MainMenu::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}