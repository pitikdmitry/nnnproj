
#ifndef MYGAME_GAMEOVER_H
#define MYGAME_GAMEOVER_H

#include "cocos2d.h"
USING_NS_CC;

class GameOverScene : public cocos2d::Layer {

public:
    virtual bool init();

    static cocos2d::Scene *createScene();
    void Restart(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);

    //  void GameOverScene::startAgainCallback(cocos2d::Ref *pSender);

    CREATE_FUNC(GameOverScene);

};


#endif //MYGAME_GAMEOVER_H