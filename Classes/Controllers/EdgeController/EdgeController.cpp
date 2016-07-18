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
    return true;
}

void EdgeController::createGameEdgeSegment() {
    auto visibleSize = Global::getVisibleSize();
    auto edges = MapController::getInstance()->getObjects("foothold");
    if (edges.empty()) {
        return;
    }
    // 创建边界
    for (auto edge : edges) {
        auto temp = edge.asValueMap();
        int xStart = temp["x"].asInt();
        int xEnd = temp["x"].asInt() + temp["width"].asInt();
        int y = temp["y"].asInt() + temp["height"].asInt();
        this->addChild(GameEdge::createGameEdgeSegment(Point(xStart, y), Point(xEnd, y)));
    }
}
