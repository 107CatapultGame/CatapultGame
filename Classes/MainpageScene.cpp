#include "Global.h"
#include "MainpageScene.h"
#include "StartGameScene/StartGameScene.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

cocos2d::Scene * Mainpage::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = Mainpage::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Mainpage::init() {
    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    preloadBGM(); // 预载入BGM
    addBackground(); // 添加背景
    addMenu(); // 添加菜单
    addUI(); // 添加UI
    // 播放bgm
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("musics/main_bgm.mp3", true);

    // 每分钟更新一次时间
    this->schedule(schedule_selector(Mainpage::updateTime), 60.0f);
    return true;
}

void Mainpage::addBackground() {
    // 添加背景
    auto background = Sprite::create("images/mainpage/background.png");
    // 位置设置为屏幕中心
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    // 加入场景
    this->addChild(background, Global::LAYER_BACKGROUND);
}

void Mainpage::addMenu() {
    // 设置两个按钮之间的垂直间隔
    const float verticalSpace = 20;

    // 开始游戏
    auto startItem = MenuItemImage::create();
    startItem->setNormalImage(Sprite::create("images/mainpage/start.png"));
    startItem->setPosition(Vec2(startItem->getContentSize().width / 2 + origin.x + 20,
        visibleSize.height + origin.y - startItem->getContentSize().height / 2 - 100));
    startItem->setCallback(CC_CALLBACK_1(Mainpage::menuStartCallback, this));
    // 物品栏
    auto inventoryItem = MenuItemImage::create();
    inventoryItem->setNormalImage(Sprite::create("images/mainpage/inventory.png"));
    inventoryItem->setPosition(Vec2(startItem->getPosition().x,
        startItem->getPosition().y - inventoryItem->getContentSize().height - verticalSpace));
    inventoryItem->setCallback(CC_CALLBACK_1(Mainpage::menuInventoryCallback, this));
    // 强化系统
    auto strengthenItem = MenuItemImage::create();
    strengthenItem->setNormalImage(Sprite::create("images/mainpage/strengthen.png"));
    strengthenItem->setPosition(Vec2(inventoryItem->getPosition().x,
        inventoryItem->getPosition().y - strengthenItem->getContentSize().height - verticalSpace));
    strengthenItem->setCallback(CC_CALLBACK_1(Mainpage::menuStrengthenCallback, this));
    // 商店
    auto storeItem = MenuItemImage::create();
    storeItem->setNormalImage(Sprite::create("images/mainpage/store.png"));
    storeItem->setPosition(Vec2(strengthenItem->getPosition().x,
        strengthenItem->getPosition().y - storeItem->getContentSize().height - verticalSpace));
    storeItem->setCallback(CC_CALLBACK_1(Mainpage::menuStoreCallback, this));
    // 设置
    auto settingsItem = MenuItemImage::create(
        "images/mainpage/settings_normal.png",
        "images/mainpage/settings_selected.png",
        CC_CALLBACK_1(Mainpage::menuSettingsCallback, this)
        );
    settingsItem->setPosition(Vec2(storeItem->getPosition().x,
        storeItem->getPosition().y - settingsItem->getContentSize().height - verticalSpace * 4));

    // create menu, it's an autorelease object
    auto menu = Menu::create(startItem, inventoryItem, strengthenItem, storeItem, settingsItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, Global::LAYER_UI);
}

void Mainpage::addUI() {
    // 设置两个按钮之间的垂直间隔
    const float verticalSpace = 20;

    // 积分-设置一个sprite作为背景图, 一个Label显示文字
    auto scoreBg = Sprite::create("images/mainpage/score.png");
    scoreBg->setPosition(Vec2(origin.x + visibleSize.width - scoreBg->getContentSize().width / 2 - 20,
        visibleSize.height + origin.y - scoreBg->getContentSize().height / 2 - 120));
    scoreLabel = Label::create("score", "fonts/arial.ttf", 20);
    scoreLabel->setPosition(scoreBg->getPosition());
    this->addChild(scoreBg, Global::LAYER_UI);
    this->addChild(scoreLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

    // 金币-设置一个sprite作为背景图, 一个Label显示文字
    auto goldBg = Sprite::create("images/mainpage/gold.png");
    goldBg->setPosition(Vec2(scoreBg->getPosition().x,
        scoreBg->getPosition().y - goldBg->getContentSize().height - verticalSpace));
    goldLabel = Label::create("gold", "fonts/arial.ttf", 20);
    goldLabel->setPosition(goldBg->getPosition());
    this->addChild(goldBg, Global::LAYER_UI);
    this->addChild(goldLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

    // 时间-设置一个sprite作为背景图, 一个Label显示文字
    auto timeBg = Sprite::create("images/mainpage/time.png");
    timeBg->setPosition(Vec2(origin.x + visibleSize.width - timeBg->getContentSize().width / 2 - 20,
        origin.y + timeBg->getContentSize().height / 2 + verticalSpace));
    timeLabel = Label::create(Global::getSystemTime(), "fonts/arial.ttf", 20);
    timeLabel->setPosition(timeBg->getPosition());
    this->addChild(timeBg, Global::LAYER_UI);
    this->addChild(timeLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

    // 用户名-设置一个sprite作为背景图, 一个Label显示文字
    auto usernameBg = Sprite::create("images/mainpage/username.png");
    usernameBg->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + usernameBg->getContentSize().height / 2 + verticalSpace));
    usernameLabel = Label::create("username", "fonts/arial.ttf", 20);
    usernameLabel->setPosition(usernameBg->getPosition());
    this->addChild(usernameBg, Global::LAYER_UI);
    this->addChild(usernameLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级
}

void Mainpage::preloadBGM() {
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("musics/main_bgm.mp3");
}

void Mainpage::playBGM() {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    if (!audio->isBackgroundMusicPlaying()) {
        audio->playBackgroundMusic(("musics/main_bgm.mp3"));
    }
}

void Mainpage::switchScene() {
    unscheduleAllSelectors(); // 停止调度器
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void Mainpage::updateTime(float f) {
    // 更新时间
    timeLabel->setString(Global::getSystemTime());
}

void Mainpage::menuStartCallback(cocos2d::Ref* pSender) {
    cocos2d::log("start"); // test
    switchScene();
    // 创建新场景
    auto StartScene = StartGame::createScene();
    // 特效切换场景
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.5f, StartScene));
}

void Mainpage::menuInventoryCallback(cocos2d::Ref* pSender) {
    cocos2d::log("inventory"); // test
}

void Mainpage::menuStrengthenCallback(cocos2d::Ref* pSender) {
    cocos2d::log("strengthen"); // test
}

void Mainpage::menuStoreCallback(cocos2d::Ref* pSender) {
    cocos2d::log("store"); // test
}

void Mainpage::menuSettingsCallback(cocos2d::Ref* pSender) {
    cocos2d::log("setting"); // test
}
