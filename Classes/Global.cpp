#include "Global.h"

#include <ctime>

#define BITMASK_CONTACT  0xFFFFFFFF
#define BITMASK_FOOTHOLD 0x00000001
#define BITMASK_SHAPE    0x00000002
#define BITMASK_FLAG     0x00000004
#define BITMASK_PLAYER   0x00000008
#define BITMASK_BULLET1  0x00000010
#define BITMASK_BULLET2  0x00000020
#define BITMASK_ENEMY1   0x00000040

const uint_8 Global::LAYER_BACKGROUND = 0;
const uint_8 Global::LAYER_GAMEPLAY = 10;
const uint_8 Global::LAYER_UI = 20;

GAME_INFO Global::game_mode = GAME_INFO::none;
GAME_INFO Global::game_level = GAME_INFO::none;

cocos2d::Size Global::visibleSize = cocos2d::Size::ZERO;

bitmask Global::bitmask_player = bitmask();

Global::Global() {}

Global::~Global() {}

std::string Global::getSystemTime() {
    std::string result = "";
    char temp[20];
    time_t now = time(NULL); // 获取时间
    struct tm * local = localtime(&now); // 获取系统时间
    strftime(temp, 20, "%H:%M", local); // 设置时间格式为hh:mm
    result = temp;
    return result; // 返回值
}

cocos2d::Size Global::getVisibleSize() {
    if (visibleSize.equals(cocos2d::Size::ZERO)) {
        visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    }
    return visibleSize;
}

bitmask Global::getMapElemBitmask(MAP_ELEM_TYPE type) {
    bitmask result;
    switch (type) {
        case foothold:
            result.categoryBitmask = BITMASK_FOOTHOLD;
            result.collisionBitmask = BITMASK_PLAYER;
            //result.contactTestBitmask = 
            result.group = -1;
            break;
        case soil:
            result.categoryBitmask = BITMASK_SHAPE;
            result.collisionBitmask = BITMASK_BULLET1 | BITMASK_BULLET2;
            result.contactTestBitmask = BITMASK_BULLET1 | BITMASK_BULLET2;
            result.group = -2;
            break;
        case steel:
            result.categoryBitmask = BITMASK_SHAPE;
            //result.collisionBitmask = BITMASK_BULLET1 | BITMASK_BULLET2;
            //result.contactTestBitmask = BITMASK_BULLET1 | BITMASK_BULLET2;
            result.group = -3;
            break;
        case stock:
            result.categoryBitmask = BITMASK_SHAPE;
            result.collisionBitmask = BITMASK_BULLET1 | BITMASK_BULLET2;
            result.contactTestBitmask = BITMASK_BULLET1 | BITMASK_BULLET2;
            result.group = -4;
            break;
        case playerFlag:
            result.categoryBitmask = BITMASK_FLAG;
            result.collisionBitmask = BITMASK_PLAYER;
            //result.contactTestBitmask = 
            result.group = -5;
            break;
        case enemyFlag:
            result.categoryBitmask = BITMASK_FLAG;
            result.collisionBitmask = BITMASK_PLAYER;
            //result.contactTestBitmask = 
            result.group = -5;
            break;
        default:
            break;
    }
    return result;
}

bitmask Global::getSpriteBitmask(SPRITE_TYPE type) {
    bitmask result;
    switch (type) {
        case player:
            result.categoryBitmask = BITMASK_PLAYER;
            //result.collisionBitmask = BITMASK_;
            //result.contactTestBitmask = 
            result.group = -6;
            break;
        case enemy1:
            result.categoryBitmask = BITMASK_ENEMY1;
            //result.collisionBitmask = BITMASK_PLAYER;
            //result.contactTestBitmask = 
            result.group = -7;
            break;
        default:
            break;
    }
    return result;
}

bitmask Global::getBullet1Bitmask(SPRITE_TYPE type) {
    bitmask result;
    switch (type) {
        case player:
            result.categoryBitmask = BITMASK_BULLET1;
            //result.collisionBitmask = BITMASK_CONTACT ^ BITMASK_PLAYER;
            result.contactTestBitmask = BITMASK_CONTACT ^ BITMASK_PLAYER;
            result.group = 1;
            break;
        case enemy1:
            result.categoryBitmask = BITMASK_BULLET1;
            //result.collisionBitmask = BITMASK_CONTACT ^ BITMASK_ENEMY1;
            result.contactTestBitmask = BITMASK_CONTACT ^ BITMASK_PLAYER;
            result.group = 1;
            break;
        default:
            break;
    }
    return result;
}

bitmask Global::getBullet2Bitmask(SPRITE_TYPE type) {
    return bitmask();
}

bitmask Global::getMapElemTag(MAP_ELEM_TYPE type) {
    return bitmask();
}

int Global::getSpriteTag(SPRITE_TYPE type) {
    return 0;
}

int Global::getBullet1Tag(SPRITE_TYPE type) {
    return 0;
}

int Global::getBullet2Tag(SPRITE_TYPE type) {
    return 0;
}
