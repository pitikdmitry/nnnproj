
#include "cocos2d.h"
#include <ui/UIDeprecated.h>
#include "GameOver.h"
#include "HelloWorldScene.h"
const float ENEMY_SCALE_FACTOR = 1.4f;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
USING_NS_CC;

Scene *GameOverScene::createScene() {

    auto scene = Scene::create();


    GameOverScene* layer = GameOverScene::create();


    scene->addChild(layer);

    return scene;
}

/*void GameOverScene::startAgainCallback(cocos2d::Ref *pSender) {
    Director::getInstance()->replaceScene(
            TransitionFade::create(0.5, GameScene::createScene(), Color3B(255, 255, 255)));
}*/

bool GameOverScene::init() {
//////////////////////////////
// 1. super init first
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(GameOverScene::menuCloseCallback, this));

    closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                 origin.y + closeItem->getContentSize().height/2));
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    auto menuitem1= MenuItemFont::create("YES!", CC_CALLBACK_1(GameOverScene::Restart, this));
    menuitem1->setColor(ccc3(200, 32, 240));
    menuitem1->setFontSize(40);
    menuitem1->setPosition(Point((visibleSize.width / 4) - 400, (visibleSize.height / 4) - 300));

    auto menuitem2= MenuItemFont::create("NO, EXIT!", CC_CALLBACK_1(GameOverScene::menuCloseCallback, this));
    menuitem2->setColor(ccc3(100, 5, 240));
    menuitem2->setFontSize(40);
    menuitem2->setPosition(Point((visibleSize.width / 4) - 150, (visibleSize.height / 4) - 300));

    menu = Menu::create(menuitem1, menuitem2, NULL);
    //menu->alignItemsVertically();
    this->addChild(menu);

    auto bgImage = Sprite::create("BG.png");
    bgImage->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(bgImage, -1);
    // calculate the scaling factor to fill the screen size
    float rX = visibleSize.width / bgImage->getContentSize().width;
    float rY = visibleSize.height / bgImage->getContentSize().height;
    // set the scaling factor to the background image
    bgImage->setScaleX(rX);
    bgImage->setScaleY(rY);

    Sprite *zombieGameOver = Sprite::create("male/Attack (3).png");
    zombieGameOver->setPosition(Point(650, 100));
    zombieGameOver->setAnchorPoint(Point::ZERO);
    zombieGameOver->setScale(ENEMY_SCALE_FACTOR);
    zombieGameOver->setFlippedX(true);
    zombieGameOver->retain();
    this->addChild(zombieGameOver);

    Sprite *girlGameOver = Sprite::create("Idle_1.png");
    girlGameOver->setPosition(Point(200, 100));
    girlGameOver->setAnchorPoint(Point::ZERO);
    girlGameOver->setScale(ENEMY_SCALE_FACTOR);
    girlGameOver->setFlippedX(true);
    girlGameOver->retain();
    this->addChild(girlGameOver);

    auto gameOVerLabel = Label::createWithTTF("Game over!", "fonts/Marker Felt.ttf", 100);
    gameOVerLabel->setTextColor(Color4B::BLUE);


    auto startAgainLabel = Label::createWithTTF("Play again?", "fonts/Marker Felt.ttf", 70);
    startAgainLabel->setTextColor(Color4B::ORANGE);
    startAgainLabel->setPosition(visibleSize.width/2, visibleSize.height/2 + startAgainLabel->getContentSize().height- 100);
    this->addChild(startAgainLabel);

    // auto playAgainItem = MenuItemLabel::create(startAgainLabel,
    //    CC_CALLBACK_1(GameOverScene::Restart, this));
    //auto menu1 = Menu::create(playAgainItem, NULL);
    gameOVerLabel->setPosition(visibleSize.width/2, visibleSize.height/2 + gameOVerLabel->getContentSize().height * 2);
    // menu1->setPosition(visibleSize.width/2, gameOVerLabel->getPosition().y — gameOVerLabel->getContentSize().height);

    this->addChild(gameOVerLabel);
    //  this->addChild(menu);

}
void GameOverScene::Restart(Ref *pSender)
{
    CCScene *mScene = HelloWorld::createScene();

    CCDirector::sharedDirector()->replaceScene(mScene);

}

void GameOverScene::menuCloseCallback(Ref* pSender)
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

