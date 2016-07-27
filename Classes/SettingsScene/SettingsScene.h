#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Settings : public cocos2d::Layer {
	public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(Settings);

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
	// scheduler
	void updateTime(float f);

	cocos2d::Size visibleSize; // 窗体尺寸
	cocos2d::Vec2 origin; // 起点坐标
	cocos2d::Label * timeLabel;

	void soundSliderEvent(Ref * pSender, cocos2d::ui::Slider::EventType type);
	void bgmSliderEvent(Ref * pSender, cocos2d::ui::Slider::EventType type);
	void setBackgroundMusicVolume(float volume);
	void setEffectsVolume(float volume);
	//cocos2d::ui::Slider*  bgmslider;
};

#endif // __SETTINGS_SCENE_H__
