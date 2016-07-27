#ifndef __MAP1_SCENE_H__
#define __MAP1_SCENE_H__

#include "cocos2d.h"

class Map1 : public cocos2d::TMXTiledMap {
    public:
    Map1();
    ~Map1();
    CREATE_FUNC(Map1);
    static Map1 * getInstance();
    virtual bool init();

    void createTileMap();
    /** 从给定的group中获取给定name的对象 */
    cocos2d::ValueMap getObject(std::string objGroup, std::string objName);
    /** 从给定的group中获取对象组 */
    cocos2d::ValueVector getObjects(std::string objGroup);
    /** 从给定的位置计算TileMap坐标 */
    cocos2d::Point getTileCoordForPosition(cocos2d::Point pos);
    cocos2d::Size getTileSize();
    cocos2d::Size getMapSize();
    void removeTileAt(cocos2d::Point pos);

    private:
    static Map1 * _map1;

    cocos2d::TMXTiledMap * map;
    cocos2d::TMXLayer * meta;
};

#endif // __MAP1_SCENE_H__
