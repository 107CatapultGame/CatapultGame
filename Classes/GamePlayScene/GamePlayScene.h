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

    virtual void onEnter();

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	//void update(float dt);

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
    /** 造成伤害 */
    bool hurt(SPRITE_TYPE spriteType, BULLET_TYPE bulletType);
    void gameOver(bool ifWin);
    // scheduler
    void updateTime(float f); // 更新时间
    void updateScore(); // 更新分数
    void updateBoundary(float f); // 更新场景杀
    void attack(float f); // 更新相互攻击
    void playerAttack(cocos2d::Vec2 touchPos);
    void updateTimeForProgressBar(float f);
    void move(float f);
    void restoreDamage(float f); // 取消伤害加成
    void restoreVelocity(float f); // 取消速度加成
    void playAudioEffect();
    // 监听器
    void addListener();
    // selector callback functions
    void gamePauseCallback();
    void menuReturnMainCallback(cocos2d::Ref* pSender);
    bool onConcactBegan(cocos2d::PhysicsContact& contact);
    bool onConcactPreSolve(cocos2d::PhysicsContact& contact);
    bool onConcactPostSolve(cocos2d::PhysicsContact& contact);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event*event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event*event);
    //bool onConcactBegan(cocos2d::PhysicsContact& contact);

    cocos2d::PhysicsWorld* m_world;

    cocos2d::Sprite * player; // 玩家
    cocos2d::Sprite * enemy; // 敌人
    float catapultSize;
    // Vector<PhysicsBody*> enemys; // 敌人
    //cocos2d::Size visibleSize; // 窗体尺寸
    cocos2d::Vec2 origin; // 起点坐标
                          // 文字Label
    cocos2d::Label * scoreLabel; // 分数
    cocos2d::Sprite * gameItem1; // 物品1
    cocos2d::Sprite * gameItem2; // 物品2
    cocos2d::Sprite * gameItem3; // 物品3
    cocos2d::Vector<cocos2d::SpriteFrame*> playerAttackAnimation;
    cocos2d::Vector<cocos2d::SpriteFrame*> enemyAttackAnimation;
    int moveDirection; // 0=no, 1=left, 2=right
    bool flip;
    cocos2d::ProgressTimer * player_hp;
    cocos2d::ProgressTimer * enemy1_hp;
    float playerHp;
    float enemy1Hp;
    const float playerInitHP = 500;
    const float enemy1InitHP = 800;
    int bulletCount; // 子弹计数-满10个自动更换大型子弹
    bool canJump;
	cocos2d::ProgressTimer * attackProgress;
    float totalTimeForProgressBar;
    // 基础伤害和基础速度, 道具加成效果会改变这些值
    float baseBullet1Hurt;
    float baseBullet2Hurt;
    float baseVelocity;
    // 三种道具是否被使用
    bool gameItemUsed[3];
    bool pause;
};

#endif // __GAME_SCENE_H__
