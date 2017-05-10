
#ifndef MYGAME_MAINMENUSCENE_H
#define MYGAME_MAINMENUSCENE_H

#include "cocos2d.h"
USING_NS_CC;
class MainMenu : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();


    void Play(cocos2d::Ref* pSender);
    void Help(cocos2d::Ref* pSender);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
};

#endif //MYGAME_MAINMENUSCENE_H