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
    // ��������
    /** Ϊ������ӱ��� */
    void addBackground();
    /** Ϊ������Ӳ˵� */
    void addMenu();
    /** Ϊ�������UIԪ�� */
    void addUI();
    // selector callback functions
    void menuSingleCallback(cocos2d::Ref* pSender);
    void menuCoopCallback(cocos2d::Ref* pSender);
    void menuBattleCallback(cocos2d::Ref* pSender);
    void menuReturnCallback(cocos2d::Ref* pSender);
    // scheduler
    void updateTime(float f);

    cocos2d::Size visibleSize; // ����ߴ�
    cocos2d::Vec2 origin; // �������
    // ����Label
    cocos2d::Label * timeLabel; // ʱ��
};

#endif // __STARTGAME_SCENE_H__
