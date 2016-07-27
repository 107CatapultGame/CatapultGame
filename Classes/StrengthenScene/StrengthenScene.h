#ifndef __STRENGTH_SCENE_H__
#define __STRENGTH_SCENE_H__

#include "cocos2d.h"

class Strengthen : public cocos2d::Layer {
  public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(Strengthen);

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
	void menuReturnCallback(cocos2d::Ref* pSender);
	// 确认强化按钮
	void menuStrengthenCallback(cocos2d::Ref* pSender);
	// scheduler
	void updateTime(float f);

	cocos2d::Size visibleSize; // 窗体尺寸
	cocos2d::Vec2 origin; // 起点坐标
	cocos2d::Label * timeLabel; // 文字Label
	cocos2d::Label * rankLabel; // 等级Label
	cocos2d::Label * hpLabel; // 血量Label
	cocos2d::Label * moveLabel; // 移动距离Label
	cocos2d::Label * launchLabel; // 投石距离Label
	cocos2d::Label * goldConsumedLabel; // 消耗金币Label
	cocos2d::Label * materialConsumedLabel; // 消耗材料Label
};

#endif // __STRENGTH_SCENE_H__
