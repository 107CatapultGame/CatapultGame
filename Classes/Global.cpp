#include "Global.h"

#include <ctime>
#include <sstream>

const int Global::BITMASK_COLLISON = 0xFFFFFFFF;
const int Global::BITMASK_EDGE = 0x00000001;
const int Global::BITMASK_FLAG = 0x00000002;
const int Global::BITMASK_BODY = 0x00000004;
const int Global::BITMASK_PLAYER = 0x00000010;
const int Global::BITMASK_ENEMY1 = 0x00000040;
const int Global::BITMASK_BULLET1 = 0x00000100;
const int Global::BITMASK_BULLET2 = 0x00000200;

const int Global::TAG_FOOTHOLD = 1;
const int Global::TAG_SOIL = 2;
const int Global::TAG_STOCK = 3;
const int Global::TAG_META = 4;
const int Global::TAG_PLAYER_FLAG = 5;
const int Global::TAG_ENEMY1_FLAG = 6;
const int Global::TAG_PLAYER_BODY = 11;
const int Global::TAG_ENEMY1_BODY = 12;
const int Global::TAG_PLAYER_BULLET1 = 21;
const int Global::TAG_ENEMY1_BULLET1 = 22;
const int Global::TAG_PLAYER_BULLET2 = 31;
const int Global::TAG_ENEMY1_BULLET2 = 32;

const uint_8 Global::LAYER_BACKGROUND = 0;
const uint_8 Global::LAYER_GAMEPLAY = 10;
const uint_8 Global::LAYER_UI = 20;

GAME_INFO Global::game_mode = GAME_INFO::none;
GAME_INFO Global::game_level = GAME_INFO::none;

cocos2d::Size Global::visibleSize = cocos2d::Size::ZERO;

std::map<BULLET_TYPE, float> Global::bulletHurt;

int Global::score = 0;
const int Global::winScore = 1200;
bool Global::ifWin = false;

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

std::string Global::getScore() {
    std::string result = "Score: ";
    std::string temp;
    std::ostringstream out;
    out << score;
    std::istringstream in(out.str());
    in >> temp;
    result += temp;
    return result; // 返回值
}

cocos2d::Size Global::getVisibleSize() {
    if (visibleSize.equals(cocos2d::Size::ZERO)) {
        visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    }
    return visibleSize;
}

bitmask Global::getMapElemBitmask(MAP_ELEM_TYPE type, SPRITE_TYPE sType) {
    bitmask result;
    switch (type) {
        case foothold: // 踏板-可以跳上去
            result.categoryBitmask = BITMASK_EDGE;
            result.contactTestBitmask = BITMASK_BODY | BITMASK_BULLET1 | BITMASK_BULLET2;
            result.group = -1;
            break;
        case soil:
            result.categoryBitmask = BITMASK_EDGE;
            result.contactTestBitmask = BITMASK_BODY | BITMASK_BULLET1 | BITMASK_BULLET2;
            result.group = -2;
            break;
        case steel: // 不能破坏的铁块
            result.categoryBitmask = BITMASK_EDGE;
            result.contactTestBitmask = BITMASK_BODY;
            result.group = -3;
            break;
        case stock:
            result.categoryBitmask = BITMASK_EDGE;
            result.contactTestBitmask = BITMASK_BODY | BITMASK_BULLET1 | BITMASK_BULLET2;
            result.group = -4;
            break;
        case flag:
            if (sType == player) {
                result.categoryBitmask = BITMASK_FLAG | BITMASK_PLAYER;
                result.contactTestBitmask = BITMASK_BULLET1 | BITMASK_BULLET2;
            } else if (sType == enemy1) {
                result.categoryBitmask = BITMASK_FLAG | BITMASK_ENEMY1;
                result.contactTestBitmask = BITMASK_BULLET1 | BITMASK_BULLET2;
            }
            result.group = -5;
            break;
        case meta:
            result.categoryBitmask = BITMASK_EDGE;
            result.contactTestBitmask = BITMASK_BODY | BITMASK_BULLET1 | BITMASK_BULLET2;
            result.group = -6;
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
            result.categoryBitmask = BITMASK_PLAYER | BITMASK_BODY;
            result.contactTestBitmask = BITMASK_EDGE | BITMASK_ENEMY1;
            result.group = -6;
            break;
        case enemy1:
            result.categoryBitmask = BITMASK_ENEMY1 | BITMASK_BODY;
            result.contactTestBitmask = BITMASK_EDGE | BITMASK_PLAYER;
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
            result.categoryBitmask = BITMASK_BULLET1 | BITMASK_PLAYER;
            result.collisionBitmask = ~(BITMASK_PLAYER);
            result.contactTestBitmask = BITMASK_EDGE | BITMASK_ENEMY1;
            result.group = 1;
            break;
        case enemy1:
            result.categoryBitmask = BITMASK_BULLET1 | BITMASK_ENEMY1;
            result.collisionBitmask = ~(BITMASK_ENEMY1);
            result.contactTestBitmask = BITMASK_EDGE | BITMASK_PLAYER;
            result.group = 1;
            break;
        default:
            break;
    }
    return result;
}

bitmask Global::getBullet2Bitmask(SPRITE_TYPE type) {
    bitmask result;
    switch (type) {
        case player:
            result.categoryBitmask = BITMASK_BULLET2 | BITMASK_PLAYER;
            result.collisionBitmask = ~(BITMASK_BODY | BITMASK_PLAYER);
            result.contactTestBitmask = BITMASK_EDGE | BITMASK_ENEMY1;
            result.group = 1;
            break;
        case enemy1:
            result.categoryBitmask = BITMASK_BULLET2 | BITMASK_ENEMY1;
            result.collisionBitmask = ~(BITMASK_BODY | BITMASK_ENEMY1);
            result.contactTestBitmask = BITMASK_EDGE | BITMASK_PLAYER;
            result.group = 1;
            break;
        default:
            break;
    }
    return result;
}

int Global::getMapElemTag(MAP_ELEM_TYPE type, SPRITE_TYPE sType) {
    switch (type) {
        case foothold:
            return TAG_FOOTHOLD;
            break;
        case soil:
            return TAG_SOIL;
            break;
        case stock:
            return TAG_STOCK;
            break;
        case flag:
            if (sType == player) {
                return TAG_PLAYER_FLAG;
            } else if (sType == enemy1) {
                return TAG_ENEMY1_FLAG;
            }
            break;
        case meta:
            return TAG_META;
            break;
        case steel:
        default:
            return 0;
            break;
    }
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

float Global::getBulletHurt(BULLET_TYPE type) {
    if (bulletHurt.empty()) {
        bulletHurt.insert(std::pair<BULLET_TYPE, float>(bullet1, 20.0f));
        bulletHurt.insert(std::pair<BULLET_TYPE, float>(bullet2, 100.0f));
    }
    switch (type) {
        case bullet1:
            return bulletHurt[bullet1];
            break;
        case bullet2:
            return bulletHurt[bullet2];
            break;
        default:
            break;
    }
}
