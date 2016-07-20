#include "../../Global.h"
#include "EdgeController.h"
#include "../MapController/MapController.h"
#include "../../Models/Edge/Edge.h"

USING_NS_CC;

EdgeController * EdgeController::_edgeController = NULL;

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
    createGameEdgeSegment();
    //createSoilEdge();
    //createSteelEdge();
    //createStockEdge();
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
        if (temp["name"].asString() != "foothold_1") {
            continue;
        }
        int xStart = temp["x"].asInt();
        int xEnd = temp["x"].asInt() + temp["width"].asInt();
        int y = temp["y"].asInt() + temp["height"].asInt() / 2;
        this->addChild(GameEdge::createGameEdgeSegment(Point(xStart, y), Point(xEnd, y)));
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
        auto shape = GameEdge::createSoilEdge(Point(xStart, yStart), Point(xEnd, yEnd));
        shape->setPosition(Vec2(temp["x"].asInt() + temp["width"].asInt() / 2,
            temp["y"].asInt() + temp["height"].asInt() / 2));
        this->addChild(shape);
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
        auto shape = GameEdge::createSteelEdge(Point(xStart, yStart), Point(xEnd, yEnd));
        shape->setPosition(Vec2(temp["x"].asInt() + temp["width"].asInt() / 2,
            temp["y"].asInt() + temp["height"].asInt() / 2));
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
        auto shape = GameEdge::createStockEdge(Point(xStart, yStart), Point(xEnd, yEnd));
        shape->setPosition(Vec2(temp["x"].asInt() + temp["width"].asInt() / 2,
            temp["y"].asInt() + temp["height"].asInt() / 2));
        this->addChild(shape);
    }
}
