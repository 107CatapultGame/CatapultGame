#include "../../Global.h"
#include "Edge.h"

USING_NS_CC;

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
    this->setPhysicsBody(PhysicsBody::createEdgeSegment(edgeStartPoint, edgeEndPoint));
    this->getPhysicsBody()->setDynamic(false);
    return true;
}

Node * GameEdge::createGameEdgeSegment(cocos2d::Point start, cocos2d::Point end) {
    edgeStartPoint = start;
    edgeEndPoint = end;
    auto aNode = GameEdge::create();

    //bitmask aBitmask = Constant::getEdge1Bitmask();
    //auto physicsBody = aNode->getPhysicsBody();
    //physicsBody->setCategoryBitmask(aBitmask.categoryBitmask);
    //physicsBody->setCollisionBitmask(aBitmask.collisionBitmask);
    //physicsBody->setContactTestBitmask(aBitmask.contactTestBitmask);
    //physicsBody->setGroup(aBitmask.group);
    return aNode;
}

