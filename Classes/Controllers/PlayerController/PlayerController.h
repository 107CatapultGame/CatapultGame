#ifndef __PLAYERCONTROLLER_SCENE_H__
#define __PLAYERCONTROLLER_SCENE_H__

#include "cocos2d.h"
#include "progressTime.h"
#include "../../Models/Player/Player.h"

USING_NS_CC;

class PlayerController : public cocos2d::Layer {
    public:
    PlayerController();
    ~PlayerController();
	CREATE_FUNC(PlayerController);
	static PlayerController * getInstance();
	virtual bool init();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event*event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event*event);
	void updateTimeForProgressBar(float dt);
	void updateTimeForAddExp(float dt);

	static void touchBegan(Vec2);
	static void touchEnded(Vec2);

	private:
	static PlayerController * playerController_;
	static float totalTimeForProgressBar;
	static Player* player_;
	static progressTime * progressTime_;
};

#endif // __PLAYERCONTROLLER_SCENE_H__
