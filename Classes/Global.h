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
    bitmask(int_32 cate = 0, int_32 con = 0, int_32 col = 0, int_32 gp = 0) {
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

/** Global全局类, 放置一些全局层面的变量与函数 */
class Global {
    public:
    Global();
    ~Global();
    /** 获取当前时间, 格式hh:mm */
    static std::string getSystemTime();
    /** 获取窗口大小 */
    static cocos2d::Size getVisibleSize();
    /** 获取玩家掩码 */
    static bitmask getPlayerBitmask();
    static bitmask getPlayerBulletBitmask();
    /** 获取敌人掩码 */
    static bitmask getEnemy1Bitmask();
    static bitmask getEnemy1BulletBitmask();
    static int getPlayerTag();
    static int getPlayerBulletTag();
    static int getEnemy1Tag();
    static int getEnemyBulletTag();

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
