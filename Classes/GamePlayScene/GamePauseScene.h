#ifndef __GAMEPAUSE_SCENE_H__
#define __GAMEPAUSE_SCENE_H__

#include "cocos2d.h"

class GamePause : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GamePause);

private:
	// 辅助函数
	/** 为场景添加背景 */
	void addBackground();
	/** 为场景添加菜单 */
	void addMenu();
	// selector callback functions
	void menuSettingsCallback(cocos2d::Ref* pSender);
	void menuContinueCallback(cocos2d::Ref* pSender);
	void menuReturnCallback(cocos2d::Ref* pSender);

	cocos2d::Size visibleSize; // 窗体尺寸
	cocos2d::Vec2 origin; // 起点坐标
};

#endif // __GAMEPAUSE_SCENE_H__
