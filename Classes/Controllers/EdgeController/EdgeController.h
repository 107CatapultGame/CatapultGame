#ifndef __EDGEController_SCENE_H__
#define __EDGEController_SCENE_H__

#include "cocos2d.h"

#include <map>
#include <vector>

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
    void createMetaEdge();
    void createFlagEdge();
    // 伤害-若障碍物被摧毁则返回true
    bool hurtSoil(cocos2d::Sprite * sp, BULLET_TYPE type);
    bool hurtStock(cocos2d::Sprite * sp, BULLET_TYPE type);
    bool hurtMeta(cocos2d::Sprite * sp, BULLET_TYPE type);
    bool hurtFlag(cocos2d::Sprite * sp, BULLET_TYPE type);

    // 每种障碍物的分数
    static std::map<MAP_ELEM_TYPE, int> edgeScore;

    private:
    static EdgeController * _edgeController;
    // 每种障碍物的初始血量
    static std::map<MAP_ELEM_TYPE, float> edgeInitHealth;

    cocos2d::Vector<cocos2d::Node*> _soil;
    cocos2d::Vector<cocos2d::Node*> _stock;
    cocos2d::Vector<cocos2d::Node*> _flag;
    cocos2d::Vector<cocos2d::Node*> _meta;
    // 每种障碍物的尺寸
    std::vector<cocos2d::Size> _soilSize;
    std::vector<cocos2d::Size> _stockSize;
    std::vector<cocos2d::Size> _flagSize;
    std::vector<cocos2d::Size> _metaSize;
    // 每种障碍物当前血量
    std::vector<float> _soilHealth;
    std::vector<float> _stockHealth;
    std::vector<float> _flagHealth;
    std::vector<float> _metaHealth;

};

#endif // __EDGEController_SCENE_H__
