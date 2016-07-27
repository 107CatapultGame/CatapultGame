#include "../../Global.h"
#include "MapController.h"

USING_NS_CC;

MapController * MapController::_mapController = NULL;
Map1 * MapController::map1 = NULL;

MapController::MapController() {}

MapController::~MapController() {
    if (_mapController != NULL) {
        _mapController->removeFromParentAndCleanup(true);
        _mapController = NULL;
    }
}

MapController * MapController::getInstance() {
    if (_mapController == NULL) {
        _mapController = MapController::create();
    }
    return _mapController;
}

bool MapController::init() {
    if (!Layer::init()) {
        return false;
    }

    switch (Global::game_level) {
        case GAME_INFO::first:
            map1 = Map1::getInstance();
            this->addChild(map1);
            break;
        default:
            break;
    }

    return true;
}

cocos2d::ValueMap MapController::getObject(std::string objGroup, std::string objName) {
    if (!objGroup.empty() && !objName.empty()) {
        switch (Global::game_level) {
            case GAME_INFO::first:
                return map1->getObject(objGroup, objName);
                break;
            default:
                break;
        }
    }
}

cocos2d::ValueVector MapController::getObjects(std::string objGroup) {
    if (!objGroup.empty()) {
        switch (Global::game_level) {
            case GAME_INFO::first:
                return map1->getObjects(objGroup);
                break;
            default:
                break;
        }
    }
}

cocos2d::Point MapController::getTileCoordForPosition(cocos2d::Point pos) {
    switch (Global::game_level) {
        case GAME_INFO::first:
            return map1->getTileCoordForPosition(pos);
            break;
        default:
            break;
    }
}

Size MapController::getTileSize() {
    switch (Global::game_level) {
        case GAME_INFO::first:
            return map1->getTileSize();
            break;
        default:
            break;
    }
}

cocos2d::Size MapController::getMapSize() {
    switch (Global::game_level) {
        case GAME_INFO::first:
            return map1->getMapSize();
            break;
        default:
            break;
    }
}

void MapController::removeTileAt(cocos2d::Point pos) {
    switch (Global::game_level) {
        case GAME_INFO::first:
            map1->removeTileAt(getTileCoordForPosition(pos));
            break;
        default:
            break;
    }
}
