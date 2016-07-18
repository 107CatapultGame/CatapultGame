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

    auto edgeGroup = map->getObjectGroup("foothold");
    auto edges = edgeGroup->getObjects();
    for (auto edge : edges) {
        auto temp = edge.asValueMap();
        int x = temp["x"].asInt() + temp["width"].asInt() / 2;
        int y = temp["y"].asInt() + temp["height"].asInt() / 2;
    }
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
