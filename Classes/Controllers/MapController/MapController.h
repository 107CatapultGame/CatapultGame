#ifndef __MAPCONTROLLER_SCENE_H__
#define __MAPCONTROLLER_SCENE_H__

#include "../../Models/Maps/Map1.h"

#include "cocos2d.h"

#include <string>

class MapController : public cocos2d::Layer {
    public:
    MapController();
    ~MapController();
    CREATE_FUNC(MapController);
    static MapController * getInstance();
    virtual bool init();

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
    static MapController * _mapController;

    static Map1 * map1;
};

#endif // __MAPCONTROLLER_SCENE_H__
