#include "InventoryScene.h"
#include "../Global.h"
#include "../MainpageScene.h"

USING_NS_CC;

cocos2d::Scene * Inventory::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Inventory::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Inventory::init() {
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
	this->schedule(schedule_selector(Inventory::updateTime), 60.0f, kRepeatForever, 0);
	return true;
}

void Inventory::addBackground() {
	// 添加背景
	auto background = Sprite::create("images/inventory/background.png");
	// 位置设置为屏幕中心
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	// 加入场景
	this->addChild(background, Global::LAYER_BACKGROUND);
}

void Inventory::addMenu() {
	// 返回按钮
	auto returnItem = MenuItemImage::create(
		"images/inventory/return_normal.png",
		"images/inventory/return_selected.png",
		CC_CALLBACK_1(Inventory::menuReturnCallback, this)
	);
	returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2 + 20,
		origin.x + visibleSize.height - returnItem->getContentSize().height / 2 - 20));

	// create menu, it's an autorelease object
	auto menu = Menu::create(returnItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, Global::LAYER_UI);
}

void Inventory::addUI() {
	// 时间-设置一个sprite作为背景图, 一个Label显示文字
	auto timeBg = Sprite::create("images/inventory/time.png");
	timeBg->setPosition(Vec2(origin.x + visibleSize.width - timeBg->getContentSize().width / 2 - 20,
		origin.y + timeBg->getContentSize().height / 2 + 20));
	this->addChild(timeBg, Global::LAYER_UI);
	timeLabel = Label::create(Global::getSystemTime(), "fonts/arial.ttf", 20);
	timeLabel->setPosition(timeBg->getPosition());
	this->addChild(timeLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

	// 拥有的物品-设置在屏幕中心
	auto ownInventory = Sprite::create("images/inventory/ownInventory.png");
	ownInventory->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(ownInventory, Global::LAYER_UI);

	// "物品栏"-设置在拥有的物品下方
	auto inventory = Sprite::create("images/inventory/inventory.png");
	inventory->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + inventory->getContentSize().height / 2 + 20));
	this->addChild(inventory, Global::LAYER_UI);

	// 金币-设置一个sprite作为背景图, 一个Label显示文字
	auto goldBg = Sprite::create("images/inventory/gold.png");
	goldBg->setPosition(Vec2(origin.x + visibleSize.width - goldBg->getContentSize().width / 2 - 20,
		origin.y + visibleSize.height - goldBg->getContentSize().height / 2 - 20));
	auto goldLabel = Label::create("gold", "fonts/arial.ttf", 20);
	goldLabel->setPosition(goldBg->getPosition());
	this->addChild(goldBg, Global::LAYER_UI);
	this->addChild(goldLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级
}

void Inventory::menuReturnCallback(cocos2d::Ref * pSender) {
	cocos2d::log("return"); // test
	Global::game_mode = GAME_INFO::none;
	this->unscheduleAllSelectors(); // 停止所有调度
									// 创建场景
	auto MainScene = Mainpage::createScene();
	// 切换场景
	Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25f, MainScene));
}

void Inventory::switchScene() {
	unscheduleAllSelectors(); // 停止调度
}

void Inventory::updateTime(float f) {
	// 更新时间
	timeLabel->setString(Global::getSystemTime());
}
