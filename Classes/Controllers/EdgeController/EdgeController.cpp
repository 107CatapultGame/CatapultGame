#include "../../Global.h"
#include "EdgeController.h"
#include "../MapController/MapController.h"

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

    return true;
}

void EdgeController::createGameEdgeSegment() {
    auto visibleSize = Global::getVisibleSize();
    auto edges = MapController::getInstance()->getObjects("foothold");
    if (edges.empty()) {
        return;
    }
    for (auto edge : edges) {
        auto temp = edge.asValueMap();
        int x = temp["x"].asInt() + temp["width"].asInt() / 2;
        int y = temp["y"].asInt() + temp["height"].asInt() / 2;
    }
}

