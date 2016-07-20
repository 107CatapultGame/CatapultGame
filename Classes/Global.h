#ifndef __GLOBAL_SCENE_H__
#define __GLOBAL_SCENE_H__
#include "cocos2d.h"

#include <string>

typedef unsigned char uint_8;
typedef long int_32;

/** 掩码结构体 */
struct bitmask {
    int_32 categoryBitmask;    // 类别
    int_32 contactTestBitmask; // 接触
    int_32 collisionBitmask;   // 碰撞
    int_32 group;              // 组别
    bitmask(int_32 cate = 0xffffffff, int_32 con = 0, int_32 col = 0xffffffff, int_32 gp = 0) {
        categoryBitmask = cate;
        contactTestBitmask = con;
        collisionBitmask = col;
        group = gp;
    }
};

/** 游戏类型枚举 */
static enum GAME_INFO {
    none, first, second, third
};

static enum MAP_ELEM_TYPE {
    foothold, soil, steel, stock, playerFlag, enemyFlag
};

static enum SPRITE_TYPE {
    player, enemy1
};

/** Global全局类, 放置一些全局层面的变量与函数 */
class Global {
    public:
    Global();
    ~Global();
    /** 获取当前时间, 格式hh:mm */
    static std::string getSystemTime();
    /** 获取窗口大小 */
    static cocos2d::Size getVisibleSize();
    /** 获取地图元素掩码 */
    static bitmask getMapElemBitmask(MAP_ELEM_TYPE type);
    /** 获取玩家/敌人掩码 */
    static bitmask getSpriteBitmask(SPRITE_TYPE type);
    /** 获取石头掩码 */
    static bitmask getBullet1Bitmask(SPRITE_TYPE type);
    static bitmask getBullet2Bitmask(SPRITE_TYPE type);
    /** 获取地图元素标签 */
    static bitmask getMapElemTag(MAP_ELEM_TYPE type);
    /** 获取玩家/敌人标签 */
    static int getSpriteTag(SPRITE_TYPE type);
    static int getBullet1Tag(SPRITE_TYPE type);
    static int getBullet2Tag(SPRITE_TYPE type);

    // 用以指定Sprite的层级
    const static uint_8 LAYER_BACKGROUND; // 背景层
    const static uint_8 LAYER_GAMEPLAY;   // 游戏层
    const static uint_8 LAYER_UI;         // 界面层
    // 游戏模式和关卡
    static GAME_INFO game_mode;
    static GAME_INFO game_level;

    private:
    static cocos2d::Size visibleSize;
    //static cocos2d::Vec2 origin;
    // 碰撞掩码
    static bitmask bitmask_player;
    static bitmask bitmask_enemy1;
};

#endif // __GLOBAL_SCENE_H__
