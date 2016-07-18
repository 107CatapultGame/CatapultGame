#ifndef __PLAYER_SCENE_H__
#define __PLAYER_SCENE_H__

#include "cocos2d.h"

class Player : public cocos2d::Sprite {
    public:
    Player();
    ~Player();
    CREATE_FUNC(Player);
    static Player * getInstance();
    virtual bool init();


    void createSprite();

    //属性参数
    void setHealth(int value);
    void setInitialHealth(int value);
    int getHealth();
    int getInitialHealth();

    bool getDirection();

    void killEnemy(int mode);

    void jump();
    void running(bool direction, bool isClimbing);
    void stopping();
    void attacked(int);
    void death();
    private:
    static Player * _player;

    cocos2d::Sprite * body;
    //Sprite *head, *hand, *body;
    ////跑步动画
    //Sprite *runner;
    ////死亡动画
    //Sprite *deathSprite;
    ////Sprite *hand1, *hand2, *hand3;
    ////climb动画
    //Sprite *climbSprite;
    //Sprite *hpBgSprite;
    //cocos2d::ProgressTimer *hpBar;
    //Sprite *hpBar_back;
    //Sprite *hpBar_fore;

    // 血量
    unsigned int hpValue;
    unsigned int initialHpValue;
};

#endif // __PLAYER_SCENE_H__
