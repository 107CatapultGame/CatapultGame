#ifndef __STARTGAME_SCENE_H__
#define __STARTGAME_SCENE_H__

#include "cocos2d.h"

class StartGame : public cocos2d::Layer {
  public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(StartGame);
};

#endif // __STARTGAME_SCENE_H__
