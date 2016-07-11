#ifndef __MAINPAGE_SCENE_H__
#define __MAINPAGE_SCENE_H__

#include "cocos2d.h"

class Mainpage : public cocos2d::Layer {
    public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(Mainpage);

    private:
    // ��������
    /** Ϊ������ӱ��� */
    void addBackground();
    /** Ϊ������Ӳ˵� */
    void addMenu();
    /** Ϊ�������UIԪ�� */
    void addUI();
    /** Ԥ��BGM */
    void preloadBGM();
    // selector callback functions
    void menuStartCallback(cocos2d::Ref* pSender);
    void menuInventoryCallback(cocos2d::Ref* pSender);
    void menuStrengthenCallback(cocos2d::Ref* pSender);
    void menuStoreCallback(cocos2d::Ref* pSender);
    void menuSettingsCallback(cocos2d::Ref* pSender);
    // scheduler
    void updateTime(float f);

    cocos2d::Size visibleSize; // ����ߴ�
    cocos2d::Vec2 origin; // �������
    // ��������Label
    cocos2d::Label * scoreLabel; // ����
    cocos2d::Label * goldLabel; // ���
    cocos2d::Label * timeLabel; // ʱ��
    cocos2d::Label * usernameLabel; // �û���
};

#endif // __MAINPAGE_SCENE_H__
