#include "SettingsScene.h"
#include "../Global.h"
#include "../MainpageScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

cocos2d::Scene * Settings::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Settings::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Settings::init() {
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
	this->schedule(schedule_selector(Settings::updateTime), 60.0f, kRepeatForever, 0);
	return true;
}

void Settings::addBackground() {
	// 添加背景
	auto background = Sprite::create("images/settings/background.png");
	// 位置设置为屏幕中心
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	// 加入场景
	this->addChild(background, Global::LAYER_BACKGROUND);
}

void Settings::addMenu() {
	// 返回按钮
	auto returnItem = MenuItemImage::create(
		"images/settings/return_normal.png",
		"images/settings/return_selected.png",
		CC_CALLBACK_1(Settings::menuReturnCallback, this)
	);
	returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2 + 20,
		origin.x + visibleSize.height - returnItem->getContentSize().height / 2 - 20));

	// create menu, it's an autorelease object
	auto menu = Menu::create(returnItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, Global::LAYER_UI);
}

void Settings::addUI() {
	// 时间-设置一个sprite作为背景图, 一个Label显示文字
	auto timeBg = Sprite::create("images/strengthen/time.png");
	timeBg->setPosition(Vec2(origin.x + visibleSize.width - timeBg->getContentSize().width / 2 - 20,
		origin.y + timeBg->getContentSize().height / 2 + 20));
	this->addChild(timeBg, Global::LAYER_UI);
	timeLabel = Label::create(Global::getSystemTime(), "fonts/arial.ttf", 20);
	timeLabel->setPosition(timeBg->getPosition());
	this->addChild(timeLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级

	// 设置标题栏
	auto title = Sprite::create("images/settings/title.png");
	title->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + title->getContentSize().height / 2 + 20));
	this->addChild(title, Global::LAYER_UI);

	// 音效
	auto sound = Sprite::create("images/settings/soundEffect.png");
	sound->setPosition(Vec2(origin.x + 300,
		origin.y + 500));
	this->addChild(sound, Global::LAYER_UI);

	// 背景音乐
	auto bgm = Sprite::create("images/settings/bgm.png");
	bgm->setPosition(Vec2(origin.x + sound->getPosition().x,
		origin.y + sound->getPosition().y - 200));
	this->addChild(bgm, Global::LAYER_UI);

	// 音效滑动条
	auto soundSlider = ui::Slider::create();
	soundSlider->loadBarTexture("images/settings/slider.png");
	soundSlider->loadSlidBallTextures("images/settings/pull.png", "images/settings/pull.png", "images/settings/pull.png");
	soundSlider->loadProgressBarTexture("images/settings/slider.png");
    soundSlider->setEnabled(true);
    soundSlider->setPercent(CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume() * 100);

	soundSlider->setPosition(Vec2(origin.x + sound->getPosition().x + soundSlider->getContentSize().width / 2 + 150,
		origin.y + sound->getPosition().y));
	
	soundSlider->addEventListener(CC_CALLBACK_2(Settings::soundSliderEvent, this));
	this->addChild(soundSlider, Global::LAYER_UI);

	// 背景音乐滑动条
	auto bgmslider = ui::Slider::create();
	bgmslider->loadBarTexture("images/settings/slider.png");
	bgmslider->loadSlidBallTextures("images/settings/pull.png", "images/settings/pull.png", "images/settings/pull.png");
	bgmslider->loadProgressBarTexture("images/settings/slider.png");
    bgmslider->setEnabled(true);
    bgmslider->setPercent(CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() * 100);

	bgmslider->setPosition(Vec2(origin.x + soundSlider->getPosition().x,
		origin.y + bgm->getPosition().y));
	
	bgmslider->addEventListener(CC_CALLBACK_2(Settings::bgmSliderEvent, this));
	this->addChild(bgmslider, Global::LAYER_UI);
}

void Settings::soundSliderEvent(Ref * pSender, ui::Slider::EventType type) {
	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
		auto slider = dynamic_cast<ui::Slider *>(pSender);
		float percent = slider->getPercent();
		//cocos2d::log(percent);
		setEffectsVolume(percent);
	}
}

void Settings::bgmSliderEvent(Ref * pSender, ui::Slider::EventType type) {
	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
		ui::Slider * slider = dynamic_cast<ui::Slider *>(pSender);
		float percent = slider->getPercent();
		cocos2d::log("percent:　　%f", percent/100);
		setBackgroundMusicVolume(percent/100);
	}
}

void Settings::setBackgroundMusicVolume(float volume) {
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
}

void Settings::setEffectsVolume(float volume) {
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
}

void Settings::menuReturnCallback(cocos2d::Ref * pSender) {
	cocos2d::log("return"); // test
	this->unscheduleAllSelectors(); // 停止所有调度
	// 创建场景
	auto MainScene = Mainpage::createScene();
	// 切换场景
	Director::getInstance()->popScene();
}

void Settings::switchScene() {
	unscheduleAllSelectors(); // 停止调度
}

void Settings::updateTime(float f) {
	// 更新时间
	timeLabel->setString(Global::getSystemTime());
}
