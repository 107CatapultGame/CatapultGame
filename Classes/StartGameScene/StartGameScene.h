#ifndef __STARTGAME_SCENE_H__
#define __STARTGAME_SCENE_H__

#include "cocos2d.h"

class StartGame : public cocos2d::Layer {
    public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(StartGame);

    private:
    // 辅助函数
    /** 为场景添加背景 */
    void addBackground();
    /** 为场景添加菜单 */
    void addMenu();
    /** 为场景添加UI元素 */
    void addUI();
    // selector callback functions

	void switchScene();

    void menuSingleCallback(cocos2d::Ref* pSender);
    void menuCoopCallback(cocos2d::Ref* pSender);
    void menuBattleCallback(cocos2d::Ref* pSender);
    void menuReturnCallback(cocos2d::Ref* pSender);
    // scheduler
    void updateTime(float f);

    cocos2d::Size visibleSize; // 窗体尺寸
    cocos2d::Vec2 origin; // 起点坐标
                          // 文字Label
    cocos2d::Label * timeLabel; // 时间
};

#endif // __STARTGAME_SCENE_H__
