#ifndef __STRENGTH_SCENE_H__
#define __STRENGTH_SCENE_H__

#include "cocos2d.h"

class Strengthen : public cocos2d::Layer {
  public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(Strengthen);
};

#endif // __STRENGTH_SCENE_H__
