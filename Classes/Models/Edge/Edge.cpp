#include "../../Global.h"
#include "Edge.h"

USING_NS_CC;

GameEdge * GameEdge::_edge = NULL;

GameEdge::GameEdge() {}

GameEdge::~GameEdge() {
    if (_edge != NULL) {
        _edge->removeFromParentAndCleanup(true);
        _edge = NULL;
    }
}

GameEdge * GameEdge::getInstance() {
    if (_edge == NULL) {
        _edge = GameEdge::create();
    }
    return _edge;
}

bool GameEdge::init() {
    if (!Sprite::init()) {
        return false;
    }

    return true;
}

void GameEdge::createGameEdgeSegment(cocos2d::Point start, cocos2d::Point end) {
    auto visibleSize = Global::getVisibleSize();

}

