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
    // ��������
    /** Ϊ������ӱ��� */
    void addBackground();
    /** Ϊ������Ӳ˵� */
    void addMenu();
    /** Ϊ�������UIԪ�� */
    void addUI();
    // selector callback functions
    void menuStartCallback(cocos2d::Ref* pSender);
    void menuInventoryCallback(cocos2d::Ref* pSender);
    void menuStrengthenCallback(cocos2d::Ref* pSender);
    void menuStoreCallback(cocos2d::Ref* pSender);
    void menuSettingsCallback(cocos2d::Ref* pSender);

    Size visibleSize; // ����ߴ�
    Vec2 origin; // �������
    // ��������Label
    Label * scoreLabel; // ����
    Label * goldLabel; // ���
    Label * timeLabel; // ʱ��
    Label * usernameLabel; // �û���
};

#endif // __MAINPAGE_SCENE_H__
