#include "../Global.h"
#include "SingleLevelScene.h"
#include "StartGameScene.h"
#include "../GamePlayScene/GamePlayScene.h"

USING_NS_CC;

cocos2d::Scene * SingleLevel::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SingleLevel::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool SingleLevel::init() {
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	addBackground(); // 添加背景
	addMenu(); // 添加菜单
	addUI(); // 添加UI
	// 每分钟更新一次时间
	this->schedule(schedule_selector(SingleLevel::updateTime), 60.0f, kRepeatForever, 0);
	return true;
}

void SingleLevel::addBackground() {
	// 添加背景
	auto background = Sprite::create("images/startgame/SingleLevel/background.png");
	// 位置设置为屏幕中心
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	// 加入场景
	this->addChild(background, Global::LAYER_BACKGROUND);
}

void SingleLevel::addMenu() {
	// 返回
	auto returnItem = MenuItemImage::create(
		"images/startgame/SingleLevel/return_normal.png",
		"images/startgame/SingleLevel/return_selected.png",
		CC_CALLBACK_1(SingleLevel::menuReturnCallback, this)
		);
	returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2 + 20,
		origin.x + visibleSize.height - returnItem->getContentSize().height / 2 - 20));
	// 章节2, 屏幕中间
	auto Chapter2_Item = MenuItemImage::create(
		"images/startgame/SingleLevel/Chapter2_normal.png",
		"images/startgame/SingleLevel/Chapter2_selected.png",
		CC_CALLBACK_1(SingleLevel::menuChapter2Callback, this)
		);
	Chapter2_Item->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.x + visibleSize.height / 2));
	// 设置两个按钮之间的水平间隔
	const float horizonalSpace = 20 + Chapter2_Item->getContentSize().width;
	// 章节1, 章节2左边
	auto Chapter1_Item = MenuItemImage::create(
		"images/startgame/SingleLevel/Chapter1_normal.png",
		"images/startgame/SingleLevel/Chapter1_selected.png",
		CC_CALLBACK_1(SingleLevel::menuChapter1Callback, this)
		);
	Chapter1_Item->setPosition(Vec2(Chapter2_Item->getPosition().x - horizonalSpace,
		Chapter2_Item->getPosition().y));
	// 章节3, 章节1右边
	auto Chapter3_Item = MenuItemImage::create(
		"images/startgame/SingleLevel/Chapter3_normal.png",
		"images/startgame/SingleLevel/Chapter3_selected.png",
		CC_CALLBACK_1(SingleLevel::menuChapter3Callback, this)
		);
	Chapter3_Item->setPosition(Vec2(Chapter2_Item->getPosition().x + horizonalSpace,
		Chapter2_Item->getPosition().y));

	// create menu, it's an autorelease object
	auto menu = Menu::create(returnItem, Chapter1_Item, Chapter2_Item, Chapter3_Item, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, Global::LAYER_UI);
}

void SingleLevel::addUI() {
	// 时间-设置一个sprite作为背景图, 一个Label显示文字
	auto timeBg = Sprite::create("images/startgame/SingleLevel/time.png");
	timeBg->setPosition(Vec2(origin.x + visibleSize.width - timeBg->getContentSize().width / 2 - 20,
		origin.y + timeBg->getContentSize().height / 2 + 20));
	timeLabel = Label::create(Global::getSystemTime(), "fonts/arial.ttf", 20);
	timeLabel->setPosition(timeBg->getPosition());
	this->addChild(timeBg, Global::LAYER_UI);
	this->addChild(timeLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级
}

void SingleLevel::menuChapter1Callback(cocos2d::Ref * pSender) {
	cocos2d::log("the first chapter"); // test
	this->unscheduleAllSelectors(); // 停止所有调度
    Global::game_level = GAME_INFO::first;
	// 创建场景
	auto GameScene = GamePlay::createScene();
	// 切换场景
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, GameScene));
}

void SingleLevel::menuChapter2Callback(cocos2d::Ref * pSender) {
	cocos2d::log("the second chapter"); // test
}

void SingleLevel::menuChapter3Callback(cocos2d::Ref * pSender) {
	cocos2d::log("the third chapter"); // test
}

void SingleLevel::menuReturnCallback(cocos2d::Ref * pSender) {
	cocos2d::log("return"); // test
	//// 本场景出栈并切换到上个场景
	// Director::getInstance()->popScene();
    Global::game_level = GAME_INFO::none;
	this->unscheduleAllSelectors(); // 停止所有调度
	// 创建场景
	auto StartGameScene = StartGame::createScene();
	// 切换场景
	Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25f, StartGameScene));
}

void SingleLevel::updateTime(float f) {
	// 更新时间
	timeLabel->setString(Global::getSystemTime());
}
