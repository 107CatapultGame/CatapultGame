#ifndef __GLOBAL_SCENE_H__
#define __GLOBAL_SCENE_H__
#include <string>

typedef unsigned char uint_8;

class Global {
    public:
    Global();
    ~Global();
    /** 获取当前时间, 格式hh:mm */
    static std::string getSystemTime();

    // 用以指定Sprite的层级
    const static uint_8 LAYER_BACKGROUND; // 背景层
    const static uint_8 LAYER_GAMEPLAY;   // 游戏层
    const static uint_8 LAYER_UI;         // 界面层
};

#endif // __GLOBAL_SCENE_H__
