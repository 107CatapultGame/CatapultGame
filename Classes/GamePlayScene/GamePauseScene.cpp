#include "../Global.h"
#include "GamePauseScene.h"
#include "../MainpageScene.h"
#include "../SettingsScene/SettingsScene.h"

USING_NS_CC;

cocos2d::Scene * GamePause::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GamePause::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GamePause::init() {
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}
    cocos2d::log("on init");
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	addBackground(); // 添加背景
	addMenu(); // 添加菜单
	return true;
}

void GamePause::addBackground() {
	// 添加背景
	auto background = Sprite::create("images/gamepause/background.png");
	// 位置设置为屏幕中心
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	// 加入场景
	this->addChild(background, Global::LAYER_BACKGROUND);
}

void GamePause::addMenu() {
	// 返回主界面
	auto returnItem = MenuItemImage::create(
		"images/gamepause/return.png",
		"images/gamepause/return.png",
		CC_CALLBACK_1(GamePause::menuReturnCallback, this)
		);
	returnItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.x + visibleSize.height / 2));

    // 设置两个按钮之间的垂直间隔
    const float verticalSpace = 20 + returnItem->getContentSize().height;

	// 继续游戏
	auto continueItem = MenuItemImage::create(
		"images/gamepause/continue.png",
		"images/gamepause/continue.png",
		CC_CALLBACK_1(GamePause::menuContinueCallback, this)
		);
	continueItem->setPosition(returnItem->getPosition() + Vec2(0, verticalSpace));
	// 设置
	auto settingsItem = MenuItemImage::create(
		"images/gamepause/settings.png",
		"images/gamepause/settings.png",
		CC_CALLBACK_1(GamePause::menuSettingsCallback, this)
		);
	settingsItem->setPosition(returnItem->getPosition() - Vec2(0, verticalSpace));

	// create menu, it's an autorelease object
	auto menu = Menu::create(returnItem, settingsItem, continueItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, Global::LAYER_UI);
}

void GamePause::menuSettingsCallback(cocos2d::Ref * pSender) {
    this->unscheduleAllSelectors(); // 停止所有调度
                                    // 创建场景
    auto SettingsScene = Settings::createScene();
    // 切换场景
    Director::getInstance()->pushScene(TransitionSlideInT::create(0.25f, SettingsScene));
}

void GamePause::menuContinueCallback(cocos2d::Ref * pSender) {
	//this->unscheduleAllSelectors(); // 停止所有调度
	// 切换场景
    Director::getInstance()->popScene();
}

void GamePause::menuReturnCallback(cocos2d::Ref * pSender) {
    Global::game_level = GAME_INFO::none;
    Global::score = 0;
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    Director::getInstance()->popScene();
    // 创建场景
    auto MainpageScene = Mainpage::createScene();
    // 切换场景
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25f, MainpageScene));
}
