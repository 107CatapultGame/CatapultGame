#include "Global.h"

#include <ctime>

#define BITMASK_CONTACT 0xFFFFFFFF
#define BITMASK_PLAYER 0x00000001
#define BITMASK_ENEMY 0x00000002
#define BITMASK_BODY 0x00000004
#define BITMASK_BULLET 0x00000008

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

bitmask Global::getPlayerBitmask() {
    bitmask result = bitmask(
        BITMASK_PLAYER | BITMASK_BODY,
        BITMASK_ENEMY,
        BITMASK_ENEMY | BITMASK_BODY,
        -1
        );
    return result;
}

bitmask Global::getEnemy1Bitmask() {
    bitmask result = bitmask(
        BITMASK_ENEMY | BITMASK_BODY,
        BITMASK_PLAYER,
        BITMASK_PLAYER | BITMASK_BODY,
        -2
        );
    return result;
}
