#ifndef __EDGEController_SCENE_H__
#define __EDGEController_SCENE_H__

#include "cocos2d.h"

class EdgeController : public cocos2d::Sprite {
    public:
    EdgeController();
    ~EdgeController();
    CREATE_FUNC(EdgeController);
    static EdgeController * getInstance();
    virtual bool init();


    void createGameEdgeSegment();
    void createSoilEdge();
    void createSteelEdge();
    void createStockEdge();

    //属性参数

    private:
    static EdgeController * _edgeController;
};

#endif // __EDGEController_SCENE_H__
