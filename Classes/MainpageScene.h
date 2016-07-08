#ifndef __MAINPAGE_SCENE_H__
#define __MAINPAGE_SCENE_H__

#include "Global.h"

#include "cocos2d.h"

USING_NS_CC;

class Mainpage : public cocos2d::Layer {
    public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(Mainpage);

    private:
    // 辅助函数
    /** 为场景添加背景 */
    void addBackground();
    /** 为场景添加菜单 */
    void addMenu();
    /** 为场景添加UI元素 */
    void addUI();
    // selector callback functions
    void menuStartCallback(cocos2d::Ref* pSender);
    void menuInventoryCallback(cocos2d::Ref* pSender);
    void menuStrengthenCallback(cocos2d::Ref* pSender);
    void menuStoreCallback(cocos2d::Ref* pSender);
    void menuSettingsCallback(cocos2d::Ref* pSender);

    Size visibleSize; // 窗体尺寸
    Vec2 origin; // 起点坐标
    // 几个文字Label
    Label * scoreLabel; // 积分
    Label * goldLabel; // 金币
    Label * timeLabel; // 时间
    Label * usernameLabel; // 用户名
};

#endif // __MAINPAGE_SCENE_H__
