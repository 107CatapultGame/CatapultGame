#include "../Global.h"
#include "StartGameScene.h"
#include "../MainpageScene.h"

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

    addBackground(); // ��ӱ���
    addMenu(); // ��Ӳ˵�
    addUI(); // ���UI
    // ÿ���Ӹ���һ��ʱ��
    this->schedule(schedule_selector(StartGame::updateTime), 60.0f);
    return true;
}

void StartGame::addBackground() {
    // ��ӱ���
    auto background = Sprite::create("images/startgame/background.png");
    // λ������Ϊ��Ļ����
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    // ���볡��
    this->addChild(background, Global::LAYER_BACKGROUND);
}

void StartGame::addMenu() {
    // ����
    auto returnItem = MenuItemImage::create(
        "images/startgame/return_normal.png",
        "images/startgame/return_selected.png",
        CC_CALLBACK_1(StartGame::menuReturnCallback, this)
        );
    returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2 + 20,
        origin.x + visibleSize.height - returnItem->getContentSize().height / 2 - 20));
    // ����, ��Ļ�м�
    auto coopItem = MenuItemImage::create(
        "images/startgame/coop_normal.png",
        "images/startgame/coop_selected.png",
        CC_CALLBACK_1(StartGame::menuCoopCallback, this)
        );
    coopItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.x + visibleSize.height / 2));
    // ����������ť֮���ˮƽ���
    const float horizonalSpace = 20 + coopItem->getContentSize().width;
    // ������Ϸ, �������
    auto singleItem = MenuItemImage::create(
        "images/startgame/single_normal.png",
        "images/startgame/single_selected.png",
        CC_CALLBACK_1(StartGame::menuSingleCallback, this)
        );
    singleItem->setPosition(Vec2(coopItem->getPosition().x - horizonalSpace,
        coopItem->getPosition().y));
    // ��ս, �����ұ�
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
    // ʱ��-����һ��sprite��Ϊ����ͼ, һ��Label��ʾ����
    auto timeBg = Sprite::create("images/startgame/time.png");
    timeBg->setPosition(Vec2(origin.x + visibleSize.width - timeBg->getContentSize().width / 2 - 20,
        origin.y + timeBg->getContentSize().height / 2 + 20));
    timeLabel = Label::create(Global::getSystemTime(), "fonts/arial.ttf", 20);
    timeLabel->setPosition(timeBg->getPosition());
    this->addChild(timeBg, Global::LAYER_UI);
    this->addChild(timeLabel, Global::LAYER_UI + 1); // ���ֲ�ȱ���ͼ���һ��
}

void StartGame::menuSingleCallback(cocos2d::Ref * pSender) {
    cocos2d::log("single"); // test
}

void StartGame::menuCoopCallback(cocos2d::Ref * pSender) {
    cocos2d::log("coop"); // test
}

void StartGame::menuBattleCallback(cocos2d::Ref * pSender) {
    cocos2d::log("battle"); // test
}

void StartGame::menuReturnCallback(cocos2d::Ref * pSender) {
    cocos2d::log("return"); // test
    //// ��������ջ���л����ϸ�����
    //Director::getInstance()->popScene();
    this->unscheduleAllSelectors(); // ֹͣ���е���
    // ��������
    auto MainScene = Mainpage::createScene();
    // �л�����
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f, MainScene));
}

void StartGame::updateTime(float f) {
    // ����ʱ��
    timeLabel->setString(Global::getSystemTime());
}
