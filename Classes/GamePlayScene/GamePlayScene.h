#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GamePlay : public cocos2d::Layer {
    public:
    void setPhysicsWorld(cocos2d::PhysicsWorld * world);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(cocos2d::PhysicsWorld* world);

    // implement the "static create()" method manually
    static GamePlay* create(cocos2d::PhysicsWorld* world);
    private:
    // 辅助函数
    /** 为场景添加背景 */
    void addBackground();
    /** 为场景添加菜单 */
    void addMenu();
    /** 为场景添加UI元素 */
    void addUI();
    /** 为场景添加游戏元素 */
    void addGameItem();
    // scheduler
    void updateTime(float f); // 更新时间
    void attack(float f); // 更新相互攻击
    void attackAction(float f);
    // 监听器
    void addContactListener();
    // selector callback functions
    void menuReturnCallback(cocos2d::Ref* pSender);
    bool onConcactBegan(cocos2d::PhysicsContact& contact);
    bool onConcactPreSolve(cocos2d::PhysicsContact& contact);
    bool onConcactPostSolve(cocos2d::PhysicsContact& contact);
    //bool onConcactBegan(cocos2d::PhysicsContact& contact);

    cocos2d::PhysicsWorld* m_world;

    cocos2d::Sprite * player; // 玩家
    cocos2d::Sprite * enemy; // 敌人
    // Vector<PhysicsBody*> enemys; // 敌人
    //cocos2d::Size visibleSize; // 窗体尺寸
    cocos2d::Vec2 origin; // 起点坐标
                          // 文字Label
    cocos2d::Label * timeLabel; // 时间
    cocos2d::Vector<cocos2d::SpriteFrame*> playerAttack;
    cocos2d::Vector<cocos2d::SpriteFrame*> enemyAttack;
    //cocos2d::TMXTiledMap * tileMap;
    //cocos2d::TMXLayer * background;
};

#endif // __GAME_SCENE_H__
