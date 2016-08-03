#include "StrengthenScene.h"
#include "../Global.h"
#include "../MainpageScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

cocos2d::Scene * Strengthen::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Strengthen::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Strengthen::init() {
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
	this->schedule(schedule_selector(Strengthen::updateTime), 60.0f, kRepeatForever, 0);
	return true;
}

void Strengthen::addBackground() {
	// 添加背景
	auto background = Sprite::create("images/strengthen/background.png");
	// 位置设置为屏幕中心
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	// 加入场景
	this->addChild(background, Global::LAYER_BACKGROUND);
}

void Strengthen::addMenu() {
	// 返回按钮
	auto returnItem = MenuItemImage::create(
		"images/strengthen/return_normal.png",
		"images/strengthen/return_selected.png",
		CC_CALLBACK_1(Strengthen::menuReturnCallback, this)
	);
	returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2 + 20,
		origin.y + visibleSize.height - returnItem->getContentSize().height / 2 - 20));

	// 强化按钮
	auto strengthenItem = MenuItemImage::create(
		"images/strengthen/strengthen_normal.png",
		"images/strengthen/strengthen_selected.png",
		CC_CALLBACK_1(Strengthen::menuStrengthenCallback, this)
	);
	strengthenItem->setPosition(Vec2(origin.x + visibleSize.width - 300,
		origin.y + 300));

	// create menu, it's an autorelease object
	auto menu = Menu::create(returnItem, strengthenItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, Global::LAYER_UI);
}

void Strengthen::addUI() {
	// 设置两个sprite之间的水平距离
	const float horizontalSpace = 20;
	// 设置两个sprite之间的垂直距离
	const float verticalSpace = 60;
	// 设置缩放后的像素值
	const float pixel = 220;

	// 时间-设置一个sprite作为背景图, 一个Label显示文字
	auto timeBg = Sprite::create("images/strengthen/time.png");
	timeBg->setPosition(Vec2(origin.x + visibleSize.width - timeBg->getContentSize().width / 2 - 20,
		origin.y + timeBg->getContentSize().height / 2 + 20));
	this->addChild(timeBg, Global::LAYER_UI);
	timeLabel = Label::create(Global::getSystemTime(), "fonts/arial.ttf", 20);
	timeLabel->setPosition(timeBg->getPosition());
	this->addChild(timeLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

	// 等级显示
	auto rankBg = Sprite::create("images/strengthen/rank.png");
	rankBg->setScale(pixel / rankBg->getContentSize().width);

	rankBg->setPosition(Vec2(origin.x + 260, origin.y + 150));
	this->addChild(rankBg, Global::LAYER_UI);
	rankLabel = Label::create("silver", "fonts/arial.ttf", 20);
	rankLabel->setPosition(rankBg->getPosition());
	this->addChild(rankLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

	// 血量显示
	auto hpBg = Sprite::create("images/strengthen/hp.png");
	hpBg->setScale(pixel / hpBg->getContentSize().width);

	hpBg->setPosition(Vec2(origin.x + rankBg->getPosition().x + hpBg->getContentSize().width + horizontalSpace,
		origin.y + rankBg->getPosition().y));
	this->addChild(hpBg, Global::LAYER_UI);
	hpLabel = Label::create("xxx", "fonts/arial.ttf", 20);
	hpLabel->setPosition(hpBg->getPosition());
	this->addChild(hpLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

	// 移动距离显示
	auto moveBg = Sprite::create("images/strengthen/move.png");
	moveBg->setScale(pixel / moveBg->getContentSize().width);

	moveBg->setPosition(Vec2(origin.x + rankBg->getPosition().x,
		origin.y + rankBg->getPosition().y - rankBg->getContentSize().height - verticalSpace));
	this->addChild(moveBg, Global::LAYER_UI);
	moveLabel = Label::create("      xxx", "fonts/arial.ttf", 20);
	moveLabel->setPosition(moveBg->getPosition());
	this->addChild(moveLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

	// 投石距离
	auto launchBg = Sprite::create("images/strengthen/launch.png");
	launchBg->setScale(pixel / launchBg->getContentSize().width);

	launchBg->setPosition(Vec2(origin.x + moveBg->getPosition().x + moveBg->getContentSize().width + horizontalSpace,
		origin.y + moveBg->getPosition().y));
	this->addChild(launchBg, Global::LAYER_UI);
	launchLabel = Label::create("      xxx", "fonts/arial.ttf", 20);
	launchLabel->setPosition(launchBg->getPosition());
	this->addChild(launchLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

	// 消耗金币
	auto goldConsumedBg = Sprite::create("images/strengthen/goldConsumed.png");
	goldConsumedBg->setScale(pixel / goldConsumedBg->getContentSize().width);
	// location
	goldConsumedBg->setPosition(Vec2(origin.x + visibleSize.width - 300,
		origin.y + 400));
	this->addChild(goldConsumedBg, Global::LAYER_UI);
	goldConsumedLabel = Label::create("        xxx", "fonts/arial.ttf", 20);
	goldConsumedLabel->setPosition(goldConsumedBg->getPosition());
	this->addChild(goldConsumedLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级
	
	// 消耗材料
	auto materialConsumedBg = Sprite::create("images/strengthen/materialConsumed.png");
	materialConsumedBg->setScale(pixel / materialConsumedBg->getContentSize().width);
	// location
	materialConsumedBg->setPosition(Vec2(origin.x + visibleSize.width - 300,
		origin.y + 500));
	this->addChild(materialConsumedBg, Global::LAYER_UI);
	materialConsumedLabel = Label::create("        xxx", "fonts/arial.ttf", 20);
	materialConsumedLabel->setPosition(materialConsumedBg->getPosition());
	this->addChild(materialConsumedLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

	// 金币
	auto goldBg = Sprite::create("images/strengthen/gold.png");
	goldBg->setPosition(Vec2(origin.x + visibleSize.width - goldBg->getContentSize().width / 2 - 20,
		origin.y + visibleSize.height - goldBg->getContentSize().height / 2 - 20));
	auto goldLabel = Label::create("", "fonts/arial.ttf", 20);
	goldLabel->setPosition(goldBg->getPosition());
	this->addChild(goldBg, Global::LAYER_UI);
	this->addChild(goldLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级
}

void Strengthen::menuReturnCallback(cocos2d::Ref * pSender) {
	cocos2d::log("return"); // test
	Global::game_mode = GAME_INFO::none;
	this->unscheduleAllSelectors(); // 停止所有调度
	// 创建场景
	auto MainScene = Mainpage::createScene();
	// 切换场景
	Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25f, MainScene));
}

void Strengthen::menuStrengthenCallback(cocos2d::Ref * pSender) {
	cocos2d::log("strengthen"); // test
}

void Strengthen::switchScene() {
	unscheduleAllSelectors(); // 停止调度
}

void Strengthen::updateTime(float f) {
	// 更新时间
	timeLabel->setString(Global::getSystemTime());
}
