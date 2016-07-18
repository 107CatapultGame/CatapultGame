#ifndef __EDGE_SCENE_H__
#define __EDGE_SCENE_H__

#include "cocos2d.h"

class GameEdge : public cocos2d::Sprite {
    public:
    GameEdge();
    ~GameEdge();
    CREATE_FUNC(GameEdge);
    static GameEdge * getInstance();
    virtual bool init();


    void createGameEdgeSegment(cocos2d::Point start, cocos2d::Point end);

    //属性参数

    private:
    static GameEdge * _edge;

    cocos2d::Sprite * body;

};

#endif // __EDGE_SCENE_H__
