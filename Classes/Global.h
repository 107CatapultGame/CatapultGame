#ifndef __GLOBAL_SCENE_H__
#define __GLOBAL_SCENE_H__
#include "cocos2d.h"

#include <string>
#include <map>

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
    foothold, soil, steel, stock, flag, meta
};

static enum SPRITE_TYPE {
    player, enemy1
};

static enum BULLET_TYPE {
    bullet1, bullet2
};

/** Global全局类, 放置一些全局层面的变量与函数 */
class Global {
    public:
    Global();
    ~Global();
    /** 获取当前时间, 格式hh:mm */
    static std::string getSystemTime();
    /** 获取当前分数, 格式Score: 100 */
    static std::string getScore();
    /** 获取窗口大小 */
    static cocos2d::Size getVisibleSize();
    /** 获取地图元素掩码 */
    static bitmask getMapElemBitmask(MAP_ELEM_TYPE type, SPRITE_TYPE sType = player);
    /** 获取玩家/敌人掩码 */
    static bitmask getSpriteBitmask(SPRITE_TYPE type);
    /** 获取石头掩码 */
    static bitmask getBullet1Bitmask(SPRITE_TYPE type);
    static bitmask getBullet2Bitmask(SPRITE_TYPE type);
    /** 获取地图元素标签 */
    static int getMapElemTag(MAP_ELEM_TYPE type, SPRITE_TYPE sType = player);
    /** 获取玩家/敌人标签 */
    static int getSpriteTag(SPRITE_TYPE type);
    static int getBullet1Tag(SPRITE_TYPE type);
    static int getBullet2Tag(SPRITE_TYPE type);
    /** 获取子弹伤害数值 */
    static float getBulletHurt(BULLET_TYPE type);

    // 用以指定Sprite的层级
    const static uint_8 LAYER_BACKGROUND; // 背景层
    const static uint_8 LAYER_GAMEPLAY;   // 游戏层
    const static uint_8 LAYER_UI;         // 界面层
    // 游戏模式和关卡
    static GAME_INFO game_mode;
    static GAME_INFO game_level;
    // 分数和游戏胜利
    static int score;
    const static int winScore;
    static bool ifWin;
    // 掩码
    const static int BITMASK_COLLISON;
    const static int BITMASK_EDGE;
    const static int BITMASK_FLAG;
    const static int BITMASK_BODY;
    const static int BITMASK_PLAYER;
    const static int BITMASK_ENEMY1;
    const static int BITMASK_BULLET1;
    const static int BITMASK_BULLET2;
    // 标签
    const static int TAG_FOOTHOLD; // 1-10
    const static int TAG_SOIL;
    const static int TAG_STOCK;
    const static int TAG_META;
    const static int TAG_PLAYER_FLAG;
    const static int TAG_ENEMY1_FLAG;
    const static int TAG_PLAYER_BODY; // 11-20
    const static int TAG_ENEMY1_BODY;
    const static int TAG_PLAYER_BULLET1; // 21-30
    const static int TAG_ENEMY1_BULLET1;
    const static int TAG_PLAYER_BULLET2; // 31-40
    const static int TAG_ENEMY1_BULLET2;

    private:
    static cocos2d::Size visibleSize;
    //static cocos2d::Vec2 origin;
    // 子弹伤害
    static std::map<BULLET_TYPE, float> bulletHurt;

};

#endif // __GLOBAL_SCENE_H__
