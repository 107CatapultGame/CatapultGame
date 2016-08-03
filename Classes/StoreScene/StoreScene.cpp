#include "StoreScene.h"
#include "../Global.h"
#include "../MainpageScene.h"

USING_NS_CC;

cocos2d::Scene * Store::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Store::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Store::init() {
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
	this->schedule(schedule_selector(Store::updateTime), 60.0f, kRepeatForever, 0);
	return true;
}

void Store::addBackground() {
	// 添加背景
	auto background = Sprite::create("images/store/background.png");
	// 位置设置为屏幕中心
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	// 加入场景
	this->addChild(background, Global::LAYER_BACKGROUND);
}

void Store::addMenu() {
	const float space = 190;
	// 返回按钮
	auto returnItem = MenuItemImage::create(
		"images/store/return_normal.png",
		"images/store/return_selected.png",
		CC_CALLBACK_1(Store::menuReturnCallback, this)
	);
	returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2 + 20,
		origin.y + visibleSize.height - returnItem->getContentSize().height / 2 - 20));

	// 购买按钮
	auto BuyItem1 = MenuItemImage::create(
		"images/store/buy_normal.png",
		"images/store/buy_selected.png",
		CC_CALLBACK_1(Store::menuBuy1Callback, this)
	);
	BuyItem1->setPosition(Vec2(origin.x + 330,
		origin.y + 220));

	auto BuyItem2 = MenuItemImage::create(
		"images/store/buy_normal.png",
		"images/store/buy_selected.png",
		CC_CALLBACK_1(Store::menuBuy2Callback, this)
	);
	BuyItem2->setPosition(Vec2(origin.x + BuyItem1->getPosition().x + BuyItem1->getContentSize().width + space,
		origin.y + BuyItem1->getPosition().y));

	auto BuyItem3 = MenuItemImage::create(
		"images/store/buy_normal.png",
		"images/store/buy_selected.png",
		CC_CALLBACK_1(Store::menuBuy3Callback, this)
	);
	BuyItem3->setPosition(Vec2(origin.x + BuyItem2->getPosition().x + BuyItem2->getContentSize().width + space,
		origin.y + BuyItem1->getPosition().y));

	// create menu, it's an autorelease object
	auto menu = Menu::create(returnItem, BuyItem1, BuyItem2, BuyItem3, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, Global::LAYER_UI);
}

void Store::addUI() {
	// 时间-设置一个sprite作为背景图, 一个Label显示文字
	auto timeBg = Sprite::create("images/store/time.png");
	timeBg->setPosition(Vec2(origin.x + visibleSize.width - timeBg->getContentSize().width / 2 - 20,
		origin.y + timeBg->getContentSize().height / 2 + 20));
	this->addChild(timeBg, Global::LAYER_UI);
	timeLabel = Label::create(Global::getSystemTime(), "fonts/arial.ttf", 20);
	timeLabel->setPosition(timeBg->getPosition());
	this->addChild(timeLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

	// 金币
	auto goldBg = Sprite::create("images/store/gold.png");
	goldBg->setPosition(Vec2(origin.x + visibleSize.width - goldBg->getContentSize().width / 2 - 20,
		origin.y + visibleSize.height - goldBg->getContentSize().height / 2 - 20));
	auto goldLabel = Label::create("", "fonts/arial.ttf", 20);
	goldLabel->setPosition(goldBg->getPosition());
	this->addChild(goldBg, Global::LAYER_UI);
	this->addChild(goldLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

    // 设置标题栏
	auto title = Sprite::create("images/store/store.png");
	title->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + title->getContentSize().height / 2 + 20));
	this->addChild(title, Global::LAYER_UI);
}

void Store::menuReturnCallback(cocos2d::Ref * pSender) {
	cocos2d::log("return"); // test
	Global::game_mode = GAME_INFO::none;
	this->unscheduleAllSelectors(); // 停止所有调度
	// 创建场景
	auto MainScene = Mainpage::createScene();
	// 切换场景
	Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25f, MainScene));
}

void Store::menuBuy1Callback(cocos2d::Ref* pSender) {
	cocos2d::log("buy 1");
}

void Store::menuBuy2Callback(cocos2d::Ref* pSender) {
	cocos2d::log("buy 2");
}

void Store::menuBuy3Callback(cocos2d::Ref* pSender) {
	cocos2d::log("buy 3");
}

void Store::switchScene() {
	unscheduleAllSelectors(); // 停止调度
}

void Store::updateTime(float f) {
	// 更新时间
	timeLabel->setString(Global::getSystemTime());
}
