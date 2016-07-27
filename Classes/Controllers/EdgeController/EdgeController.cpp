#include "../../Global.h"
#include "EdgeController.h"
#include "../MapController/MapController.h"
#include "../../Models/Edge/Edge.h"

USING_NS_CC;

#define BITMASK_EDGE 0x00000001
#define BITMASK_BULLET 0x00000010
#define BITMASK_BULLET2 0x00000020

EdgeController * EdgeController::_edgeController = NULL;
std::map<MAP_ELEM_TYPE, float> EdgeController::edgeInitHealth;
std::map<MAP_ELEM_TYPE, int> EdgeController::edgeScore;

EdgeController::EdgeController() {}

EdgeController::~EdgeController() {
    if (_edgeController != NULL) {
        _edgeController->removeFromParentAndCleanup(true);
        _edgeController = NULL;
    }
}

EdgeController * EdgeController::getInstance() {
    if (_edgeController == NULL) {
        _edgeController = EdgeController::create();
    }
    return _edgeController;
}

bool EdgeController::init() {
    if (!Sprite::init()) {
        return false;
    }
    if (edgeInitHealth.empty()) {
        edgeInitHealth.insert(std::pair<MAP_ELEM_TYPE, float>(MAP_ELEM_TYPE::soil, 300.0f));
        edgeInitHealth.insert(std::pair<MAP_ELEM_TYPE, float>(MAP_ELEM_TYPE::stock, 200.0f));
        edgeInitHealth.insert(std::pair<MAP_ELEM_TYPE, float>(MAP_ELEM_TYPE::flag, 300.0f));
        edgeInitHealth.insert(std::pair<MAP_ELEM_TYPE, float>(MAP_ELEM_TYPE::meta, 40.0f));
    }
    if (edgeScore.empty()) {
        edgeScore.insert(std::pair<MAP_ELEM_TYPE, int>(MAP_ELEM_TYPE::soil, 60));
        edgeScore.insert(std::pair<MAP_ELEM_TYPE, int>(MAP_ELEM_TYPE::stock, 30));
        edgeScore.insert(std::pair<MAP_ELEM_TYPE, int>(MAP_ELEM_TYPE::flag, 800));
        edgeScore.insert(std::pair<MAP_ELEM_TYPE, int>(MAP_ELEM_TYPE::meta, 5));
    }
    // 清空以重新创建地图元素
    _soil.clear();
    _stock.clear();
    _flag.clear();
    _meta.clear();
    _soilHealth.clear();
    _stockHealth.clear();
    _flagHealth.clear();
    _metaHealth.clear();
    _soilSize.clear();
    _stockSize.clear();
    _flagSize.clear();
    _metaSize.clear();
    // 创建地图元素
    createGameEdgeSegment();
    createSoilEdge();
    createSteelEdge();
    createStockEdge();
    createMetaEdge();
    createFlagEdge();
    return true;
}

void EdgeController::createGameEdgeSegment() {
    auto edges = MapController::getInstance()->getObjects("foothold");
    if (edges.empty()) {
        return;
    }
    // 创建边界
    for (auto edge : edges) {
        auto temp = edge.asValueMap();
        int xStart = temp["x"].asInt();
        int xEnd = temp["x"].asInt() + temp["width"].asInt();
        int y = temp["y"].asInt() + temp["height"].asInt() / 2;
        auto shape = GameEdge::createGameEdgeSegment(Point(xStart, y), Point(xEnd, y));
        // 设置掩码
        auto bitmask = Global::getMapElemBitmask(MAP_ELEM_TYPE::foothold);
        shape->getPhysicsBody()->setCategoryBitmask(bitmask.categoryBitmask);
        shape->getPhysicsBody()->setCollisionBitmask(bitmask.collisionBitmask);
        shape->getPhysicsBody()->setContactTestBitmask(bitmask.contactTestBitmask);

        shape->getPhysicsBody()->setTag(Global::getMapElemTag(foothold));
        this->addChild(shape);
    }
}

void EdgeController::createSoilEdge() {
    auto edges = MapController::getInstance()->getObjects("soil");
    if (edges.empty()) {
        return;
    }
    // 创建边界
    for (auto edge : edges) {
        auto temp = edge.asValueMap();
        int xStart = temp["x"].asInt();
        int xEnd = temp["x"].asInt() + temp["width"].asInt();
        int yStart = temp["y"].asInt();
        int yEnd = temp["y"].asInt() + temp["height"].asInt();
        auto shape = GameEdge::createShape(Point(xStart, yStart), Point(xEnd, yEnd));
        shape->setPosition(Vec2(temp["x"].asInt() + temp["width"].asInt() / 2,
            temp["y"].asInt() + temp["height"].asInt() / 2));

        // 设置掩码
        auto bitmask = Global::getMapElemBitmask(MAP_ELEM_TYPE::soil);
        shape->getPhysicsBody()->setCategoryBitmask(bitmask.categoryBitmask);
        shape->getPhysicsBody()->setCollisionBitmask(bitmask.collisionBitmask);
        shape->getPhysicsBody()->setContactTestBitmask(bitmask.contactTestBitmask);
        shape->getPhysicsBody()->setTag(Global::getMapElemTag(soil));

        _soil.pushBack(shape);
        _soilHealth.push_back(edgeInitHealth[soil]);
        _soilSize.push_back(Size(temp["width"].asInt(), temp["height"].asInt()));
        this->addChild(_soil.back());

    }
}

void EdgeController::createSteelEdge() {
    auto edges = MapController::getInstance()->getObjects("steel");
    if (edges.empty()) {
        return;
    }
    // 创建边界
    for (auto edge : edges) {
        auto temp = edge.asValueMap();
        int xStart = temp["x"].asInt();
        int xEnd = temp["x"].asInt() + temp["width"].asInt();
        int yStart = temp["y"].asInt();
        int yEnd = temp["y"].asInt() + temp["height"].asInt();
        auto shape = GameEdge::createShape(Point(xStart, yStart), Point(xEnd, yEnd));
        shape->setPosition(Vec2(temp["x"].asInt() + temp["width"].asInt() / 2,
            temp["y"].asInt() + temp["height"].asInt() / 2));

        // 设置掩码
        auto bitmask = Global::getMapElemBitmask(MAP_ELEM_TYPE::steel);
        shape->getPhysicsBody()->setCategoryBitmask(bitmask.categoryBitmask);
        shape->getPhysicsBody()->setCollisionBitmask(bitmask.collisionBitmask);
        shape->getPhysicsBody()->setContactTestBitmask(bitmask.contactTestBitmask);
        shape->getPhysicsBody()->setTag(Global::getMapElemTag(steel));

        this->addChild(shape);
    }
}

void EdgeController::createStockEdge() {
    auto edges = MapController::getInstance()->getObjects("stock");
    if (edges.empty()) {
        return;
    }
    // 创建边界
    for (auto edge : edges) {
        auto temp = edge.asValueMap();
        int xStart = temp["x"].asInt();
        int xEnd = temp["x"].asInt() + temp["width"].asInt();
        int yStart = temp["y"].asInt();
        int yEnd = temp["y"].asInt() + temp["height"].asInt();
        auto shape = GameEdge::createShape(Point(xStart, yStart), Point(xEnd, yEnd));
        shape->setPosition(Vec2(temp["x"].asInt() + temp["width"].asInt() / 2,
            temp["y"].asInt() + temp["height"].asInt() / 2));

        // 设置掩码
        auto bitmask = Global::getMapElemBitmask(MAP_ELEM_TYPE::stock);
        shape->getPhysicsBody()->setCategoryBitmask(bitmask.categoryBitmask);
        shape->getPhysicsBody()->setCollisionBitmask(bitmask.collisionBitmask);
        shape->getPhysicsBody()->setContactTestBitmask(bitmask.contactTestBitmask);
        shape->getPhysicsBody()->setTag(Global::getMapElemTag(stock));

        _stock.pushBack(shape);
        _stockHealth.push_back(edgeInitHealth[stock]);
        _stockSize.push_back(Size(temp["width"].asInt(), temp["height"].asInt()));
        this->addChild(_stock.back());
    }
}

void EdgeController::createMetaEdge() {
    auto edges = MapController::getInstance()->getObjects("meta");
    if (edges.empty()) {
        return;
    }
    // 创建边界
    for (auto edge : edges) {
        auto temp = edge.asValueMap();
        int xStart = temp["x"].asInt();
        int xEnd = temp["x"].asInt() + temp["width"].asInt();
        int yStart = temp["y"].asInt();
        int yEnd = temp["y"].asInt() + temp["height"].asInt();
        auto shape = GameEdge::createShape(Point(xStart, yStart), Point(xEnd, yEnd));
        shape->setPosition(Vec2(temp["x"].asInt() + temp["width"].asInt() / 2,
            temp["y"].asInt() + temp["height"].asInt() / 2 + MapController::getInstance()->getTileSize().height));

        // 设置掩码
        auto bitmask = Global::getMapElemBitmask(MAP_ELEM_TYPE::meta);
        shape->getPhysicsBody()->setCategoryBitmask(bitmask.categoryBitmask);
        shape->getPhysicsBody()->setCollisionBitmask(bitmask.collisionBitmask);
        shape->getPhysicsBody()->setContactTestBitmask(bitmask.contactTestBitmask);
        shape->getPhysicsBody()->setTag(Global::getMapElemTag(meta));

        _meta.pushBack(shape);
        _metaHealth.push_back(edgeInitHealth[meta]);
        _metaSize.push_back(Size(temp["width"].asInt(), temp["height"].asInt()));
        this->addChild(_meta.back());
    }
}

void EdgeController::createFlagEdge() {
    auto edges = MapController::getInstance()->getObjects("flag");
    if (edges.empty()) {
        return;
    }
    // 创建边界
    for (auto edge : edges) {
        auto temp = edge.asValueMap();
        int xStart = temp["x"].asInt();
        int xEnd = temp["x"].asInt() + temp["width"].asInt();
        int yStart = temp["y"].asInt();
        int yEnd = temp["y"].asInt() + temp["height"].asInt();
        auto shape = GameEdge::createShape(Point(xStart, yStart), Point(xEnd, yEnd));
        shape->setPosition(Vec2(temp["x"].asInt() + temp["width"].asInt() / 2,
            temp["y"].asInt() + temp["height"].asInt() / 2));

        // 设置掩码
        if (temp["name"].asString() == "player_flag") {
            auto bitmask = Global::getMapElemBitmask(MAP_ELEM_TYPE::flag);
            shape->getPhysicsBody()->setCategoryBitmask(bitmask.categoryBitmask);
            shape->getPhysicsBody()->setCollisionBitmask(bitmask.collisionBitmask);
            shape->getPhysicsBody()->setContactTestBitmask(bitmask.contactTestBitmask);
            shape->getPhysicsBody()->setTag(Global::getMapElemTag(flag));
        } else if (temp["name"].asString() == "enemy_flag") {
            auto bitmask = Global::getMapElemBitmask(MAP_ELEM_TYPE::flag, SPRITE_TYPE::enemy1);
            shape->getPhysicsBody()->setCategoryBitmask(bitmask.categoryBitmask);
            shape->getPhysicsBody()->setCollisionBitmask(bitmask.collisionBitmask);
            shape->getPhysicsBody()->setContactTestBitmask(bitmask.contactTestBitmask);
            shape->getPhysicsBody()->setTag(Global::getMapElemTag(flag, SPRITE_TYPE::enemy1));
        }

        _flag.pushBack(shape);
        _flagHealth.push_back(edgeInitHealth[flag]);
        _flagSize.push_back(Size(temp["width"].asInt(), temp["height"].asInt()));
        this->addChild(_flag.back());
    }
}

bool EdgeController::hurtSoil(cocos2d::Sprite * sp, BULLET_TYPE type) {
    int i = 0;
    for (auto soil : _soil) {
        if (sp == soil) {
            _soilHealth[i] -= Global::getBulletHurt(type);
            if (_soilHealth[i] <= 0) {
                _soilHealth[i] = 0;
                auto size = _soilSize[i];
                for (float i = 0; i < size.width; i+= MapController::getInstance()->getTileSize().width) {
                    for (float j = 0; j < size.height; j += MapController::getInstance()->getTileSize().height) {
                        MapController::getInstance()->removeTileAt(soil->getPosition() + Size(i - MapController::getInstance()->getTileSize().width, j));
                    }
                }
                soil->removeFromParentAndCleanup(true);
                return true;
            }
            return false;
        }
        i++;
    }
    return false;
}

bool EdgeController::hurtStock(cocos2d::Sprite * sp, BULLET_TYPE type) {
    int i = 0;
    for (auto stock : _stock) {
        if (sp == stock) {
            _stockHealth[i] -= Global::getBulletHurt(type);
            if (_stockHealth[i] <= 0) {
                _stockHealth[i] = 0;
                auto size = _stockSize[i];
                for (float i = 0; i < size.width; i += MapController::getInstance()->getTileSize().width) {
                    for (float j = 0; j < size.height; j += MapController::getInstance()->getTileSize().height) {
                        MapController::getInstance()->removeTileAt(stock->getPosition() + Size(i - MapController::getInstance()->getTileSize().width, j));
                    }
                }
                stock->removeFromParentAndCleanup(true);
                return true;
            }
            return false;
        }
        i++;
    }
    return false;
}

bool EdgeController::hurtMeta(cocos2d::Sprite * sp, BULLET_TYPE type) {
    int i = 0;
    for (auto meta : _meta) {
        if (sp == meta) {
            _metaHealth[i] -= Global::getBulletHurt(type);
            if (_metaHealth[i] <= 0) {
                _metaHealth[i] = 0;
                auto size = _metaSize[i];
                for (float i = 0; i < size.width; i += MapController::getInstance()->getTileSize().width) {
                    for (float j = 0; j < size.height; j += MapController::getInstance()->getTileSize().height) {
                        MapController::getInstance()->removeTileAt(meta->getPosition() + Size(i, j));
                    }
                }
                meta->removeFromParentAndCleanup(true);
                return true;
            }
            return false;
        }
        i++;
    }
    return false;
}

bool EdgeController::hurtFlag(cocos2d::Sprite * sp, BULLET_TYPE type) {
    int i = 0;
    for (auto flag : _flag) {
        if (sp == flag) {
            _flagHealth[i] -= Global::getBulletHurt(type);
            if (_flagHealth[i] <= 0) {
                _flagHealth[i] = 0;
                auto size = _flagSize[i];
                for (float i = 0; i <= size.width; i += MapController::getInstance()->getTileSize().width) {
                    for (float j = 0; j <= size.height; j += MapController::getInstance()->getTileSize().height) {
                        MapController::getInstance()->removeTileAt(flag->getPosition() + Size(i - size.width / 2, j));
                    }
                }
                if (flag->getPhysicsBody()->getTag() == Global::getMapElemTag(MAP_ELEM_TYPE::flag, enemy1)) {
                    Global::score += edgeScore[MAP_ELEM_TYPE::flag];
                }
                flag->removeFromParentAndCleanup(true);
                return true;
            }
            return false;
        }
        i++;
    }
    return false;
}
