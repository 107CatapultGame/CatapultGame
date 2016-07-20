#ifndef __EDGE_SCENE_H__
#define __EDGE_SCENE_H__

#include "cocos2d.h"

class GameEdge : public cocos2d::Node {
    public:
    GameEdge();
    ~GameEdge();
    CREATE_FUNC(GameEdge);
    static GameEdge * getInstance();
    virtual bool init();

    static Node * createGameEdgeSegment(cocos2d::Point start, cocos2d::Point end);
    static Node * createSoilEdge(cocos2d::Point start, cocos2d::Point end);
    static Node * createSteelEdge(cocos2d::Point start, cocos2d::Point end);
    static Node * createStockEdge(cocos2d::Point start, cocos2d::Point end);

    //属性参数

    private:
    static GameEdge * _edge;

    static cocos2d::Point edgeStartPoint;
    static cocos2d::Point edgeEndPoint;
};

#endif // __EDGE_SCENE_H__
