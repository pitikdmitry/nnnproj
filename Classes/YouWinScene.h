

#ifndef MYGAME_YOUWINSCENE_H
#define MYGAME_YOUWINSCENE_H

#include "cocos2d.h"
USING_NS_CC;

class YouWinScene : public cocos2d::Layer {

public:
    virtual bool init();

    static cocos2d::Scene *createScene();
    void Restart(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(YouWinScene);

};

#endif //MYGAME_YOUWINSCENE_H