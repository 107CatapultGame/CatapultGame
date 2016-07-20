#ifndef __PROGRESS_TIME_H__
#define __PROGRESS_TIME_H__
#include "cocos2d.h"

USING_NS_CC;

class progressTime : public cocos2d::Layer{
public:
	static progressTime* getInstance();
	void initial();
	void setProgressPercent(float);
	float getProgressPercent();

private:
	progressTime();
	~progressTime();
	static progressTime* progressTimeInstance;

	Sprite* progressSprite1;
	Sprite* progressSprite2;
	ProgressTimer* progressTimer;
	Label* progressTimerLabel;
};

#endif