#ifndef __INVENTORY_SCENE_H__
#define __INVENTORY_SCENE_H__

#include "cocos2d.h"

class Inventory : public cocos2d::Layer {
    public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(Inventory);
};

#endif // __INVENTORY_SCENE_H__
