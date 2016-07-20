#include "../../Global.h"
#include "Edge.h"

USING_NS_CC;

#define BITMASK_EDGE 0x00000001
#define BITMASK_BULLET 0x00000010
#define BITMASK_BULLET2 0x00000020

GameEdge * GameEdge::_edge = NULL;
Point GameEdge::edgeStartPoint;
Point GameEdge::edgeEndPoint;

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
    if (!Node::init()) {
        return false;
    }
    
    return true;
}

Node * GameEdge::createGameEdgeSegment(cocos2d::Point start, cocos2d::Point end) {
    edgeStartPoint = start;
    edgeEndPoint = end;
    auto aNode = GameEdge::create();
    aNode->setPhysicsBody(PhysicsBody::createEdgeSegment(edgeStartPoint, edgeEndPoint));
    aNode->getPhysicsBody()->setDynamic(false);
    
    aNode->getPhysicsBody()->setCategoryBitmask(BITMASK_EDGE);
    aNode->getPhysicsBody()->setContactTestBitmask(BITMASK_BULLET | BITMASK_BULLET2);
    return aNode;
}

Node * GameEdge::createSoilEdge(cocos2d::Point start, cocos2d::Point end) {
    edgeStartPoint = start;
    edgeEndPoint = end;
    auto aNode = GameEdge::create();
    auto size = Size(abs(end.x - start.x), abs(end.y - start.y));
    aNode->setPhysicsBody(PhysicsBody::createBox(size));
    aNode->getPhysicsBody()->setDynamic(false);
    return aNode;
}

Node * GameEdge::createSteelEdge(cocos2d::Point start, cocos2d::Point end) {
    edgeStartPoint = start;
    edgeEndPoint = end;
    auto aNode = GameEdge::create();
    auto size = Size(abs(end.x - start.x), abs(end.y - start.y));
    aNode->setPhysicsBody(PhysicsBody::createBox(size));
    aNode->getPhysicsBody()->setDynamic(false);
    return aNode;
}

Node * GameEdge::createStockEdge(cocos2d::Point start, cocos2d::Point end) {
    edgeStartPoint = start;
    edgeEndPoint = end;
    auto aNode = GameEdge::create();
    auto size = Size(abs(end.x - start.x), abs(end.y - start.y));
    aNode->setPhysicsBody(PhysicsBody::createBox(size));
    aNode->getPhysicsBody()->setDynamic(false);
    return aNode;
}
