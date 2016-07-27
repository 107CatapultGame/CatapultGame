#include "../../Global.h"
#include "Map1.h"

USING_NS_CC;

Map1 * Map1::_map1 = NULL;

Map1::Map1() {}

Map1::~Map1() {
    if (_map1 != NULL) {
        _map1->removeFromParentAndCleanup(true);
        _map1 = NULL;
    }
}

Map1 * Map1::getInstance() {
    if (_map1 == NULL) {
        _map1 = Map1::create();
    }
    return _map1;
}

bool Map1::init() {
    if (!TMXTiledMap::init()) {
        return false;
    }
    createTileMap();

    meta = map->getLayer("meta");
    meta->setVisible(false);

    //auto edges = map->getObjectGroup("foothold")->getObjects();
    //if (!edges.empty()) {
    //    // 创建边界
    //    for (auto edge : edges) {
    //        auto temp = edge.asValueMap();
    //        int xStart = temp["x"].asInt();
    //        int xEnd = temp["x"].asInt() + temp["width"].asInt();
    //        int y = temp["y"].asInt() + temp["height"].asInt();
    //        this->addChild(GameEdge::createGameEdgeSegment(Point(xStart, y), Point(xEnd, y)));
    //    }
    //}

    return true;
}

void Map1::createTileMap() {
    map = TMXTiledMap::create("images/gameplay/map/map1.tmx");
    addChild(map);
}

cocos2d::ValueMap Map1::getObject(std::string objGroup, std::string objName) {
    auto group = map->getObjectGroup(objGroup);
    CCASSERT(NULL != group, "'%s' object group not found", objGroup);
    auto object = group->getObject(objName);
    CCASSERT(NULL != group, "'%s' object not found", objName);
    return object;
}

cocos2d::ValueVector Map1::getObjects(std::string objGroup) {
    auto group = map->getObjectGroup(objGroup);
    CCASSERT(NULL != group, "'%s' object group not found", objGroup);
    auto objects = group->getObjects();
    return objects;
}

cocos2d::Point Map1::getTileCoordForPosition(cocos2d::Point pos) {
    int x = pos.x / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - pos.y) / map->getTileSize().height;
    return Point(x, y);
}

cocos2d::Size Map1::getTileSize() {
    return map->getTileSize();
}

cocos2d::Size Map1::getMapSize() {
    return map->getMapSize();
}

void Map1::removeTileAt(cocos2d::Point pos) {
    map->getLayer("barrier")->removeTileAt(pos);
    map->getLayer("background")->setTileGID(map->getLayer("background")->getTileGIDAt(Vec2(0, 0)), pos);
}
