#include "../Global.h"
#include "StartGameScene.h"
#include "../MainpageScene.h"
#include "SingleLevelScene.h"

USING_NS_CC;

cocos2d::Scene * StartGame::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = StartGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool StartGame::init() {
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
    this->schedule(schedule_selector(StartGame::updateTime), 60.0f, kRepeatForever, 0);
    return true;
}

void StartGame::addBackground() {
    // 添加背景
    auto background = Sprite::create("images/startgame/background.png");
    // 位置设置为屏幕中心
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    // 加入场景
    this->addChild(background, Global::LAYER_BACKGROUND);
}

void StartGame::addMenu() {
    // 返回
    auto returnItem = MenuItemImage::create(
        "images/startgame/return_normal.png",
        "images/startgame/return_selected.png",
        CC_CALLBACK_1(StartGame::menuReturnCallback, this)
        );
    returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2 + 20,
        origin.x + visibleSize.height - returnItem->getContentSize().height / 2 - 20));
    // 合作, 屏幕中间
    auto coopItem = MenuItemImage::create(
        "images/startgame/coop_normal.png",
        "images/startgame/coop_selected.png",
        CC_CALLBACK_1(StartGame::menuCoopCallback, this)
        );
    coopItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.x + visibleSize.height / 2));
    // 设置两个按钮之间的水平间隔
    const float horizonalSpace = 20 + coopItem->getContentSize().width;
    // 单人游戏, 合作左边
    auto singleItem = MenuItemImage::create(
        "images/startgame/single_normal.png",
        "images/startgame/single_selected.png",
        CC_CALLBACK_1(StartGame::menuSingleCallback, this)
        );
    singleItem->setPosition(Vec2(coopItem->getPosition().x - horizonalSpace,
        coopItem->getPosition().y));
    // 对战, 合作右边
    auto battleItem = MenuItemImage::create(
        "images/startgame/battle_normal.png",
        "images/startgame/battle_selected.png",
        CC_CALLBACK_1(StartGame::menuBattleCallback, this)
        );
    battleItem->setPosition(Vec2(coopItem->getPosition().x + horizonalSpace,
        coopItem->getPosition().y));

    // create menu, it's an autorelease object
    auto menu = Menu::create(returnItem, singleItem, coopItem, battleItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, Global::LAYER_UI);
}

void StartGame::addUI() {
    // 时间-设置一个sprite作为背景图, 一个Label显示文字
    auto timeBg = Sprite::create("images/startgame/time.png");
    timeBg->setPosition(Vec2(origin.x + visibleSize.width - timeBg->getContentSize().width / 2 - 20,
        origin.y + timeBg->getContentSize().height / 2 + 20));
    timeLabel = Label::create(Global::getSystemTime(), "fonts/arial.ttf", 20);
    timeLabel->setPosition(timeBg->getPosition());
    this->addChild(timeBg, Global::LAYER_UI);
    this->addChild(timeLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级
}

void StartGame::switchScene() {
	unscheduleAllSelectors(); // 停止调度器
	// CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void StartGame::menuSingleCallback(cocos2d::Ref * pSender) {
    cocos2d::log("single"); // test
	switchScene();
	// 创建新场景
	auto SingleLevelScene = SingleLevel::createScene();
	// 特效切换场景
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, SingleLevelScene));
}

void StartGame::menuCoopCallback(cocos2d::Ref * pSender) {
    cocos2d::log("coop"); // test
}

void StartGame::menuBattleCallback(cocos2d::Ref * pSender) {
    cocos2d::log("battle"); // test
}

void StartGame::menuReturnCallback(cocos2d::Ref * pSender) {
    cocos2d::log("return"); // test
                            //// 本场景出栈并切换到上个场景
                            //Director::getInstance()->popScene();
    this->unscheduleAllSelectors(); // 停止所有调度
                                    // 创建场景
    auto MainScene = Mainpage::createScene();
    // 切换场景
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25f, MainScene));
}

void StartGame::updateTime(float f) {
    // 更新时间
    timeLabel->setString(Global::getSystemTime());
}
