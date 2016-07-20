#include "PlayerController.h"
#include "../../Global.h"

PlayerController * PlayerController::playerController_ = NULL;
Player * PlayerController::player_ = NULL;
progressTime * PlayerController::progressTime_ = NULL;

PlayerController::PlayerController() {}

PlayerController::~PlayerController() {
	if (playerController_ != NULL) {
		playerController_->removeFromParentAndCleanup(true);
		playerController_ = NULL;
	}
}

PlayerController * PlayerController::getInstance()
{
	if (playerController_ == NULL) {
		playerController_ = PlayerController::create();
	}
	return playerController_;
}

bool PlayerController::init() {
	player_ = Player::getInstance();
	progressTime_ = progressTime::getInstance();
	this->addChild(progressTime_);

	//keyboard listener
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerController::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerController::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	this->schedule(schedule_selector(PlayerController::updateTimeForAddExp), 1.0f);

	return true;
}

float PlayerController::totalTimeForProgressBar;

void PlayerController::touchBegan(Vec2 touchPoint) {
	// Vec2 cameraLocation = CameraController::getInstance()->getCamera()->getPosition();
	Size visibleSize = Global::getVisibleSize();

	//Vec2 touchEndedPosition = Vec2(touchPoint.x + (cameraLocation.x - visibleSize.width / 2), touchPoint.y + (cameraLocation.y - visibleSize.height / 2));
	//archer_->aimAt(touchEndedPosition);

	totalTimeForProgressBar = 0;
	progressTime::getInstance()->setProgressPercent(0);
	player_->schedule(schedule_selector(PlayerController::updateTimeForProgressBar), 0.1f);
}

void PlayerController::touchEnded(Vec2 touchPoint) {
	Size visibleSize = Global::getVisibleSize();
	// Vec2 cameraLocation = CameraController::getInstance()->getCamera()->getPosition();

	// Vec2 touchEndedPosition = Vec2(touchPoint.x + (cameraLocation.x - visibleSize.width / 2), touchPoint.y + (cameraLocation.y - visibleSize.height / 2));
	//archer_->aimAt(touchEndedPosition);

	player_->unschedule(schedule_selector(PlayerController::updateTimeForProgressBar));
	// Vec2 beginPosition = archer_->getPosition();
}

void PlayerController::updateTimeForProgressBar(float dt) {
	totalTimeForProgressBar += dt;
	if (totalTimeForProgressBar >= 10.0 / 10){
		progressTime::getInstance()->setProgressPercent(0);
		totalTimeForProgressBar = 0;
		return;
	}
	else {
		progressTime::getInstance()->setProgressPercent(totalTimeForProgressBar * 10.0 / 10.0 * 100);
	}
}

void PlayerController::updateTimeForAddExp(float dt)
{
}

void PlayerController::onKeyPressed(EventKeyboard::KeyCode keycode, Event*event) {
	cocos2d::log("press");
}

void PlayerController::onKeyReleased(EventKeyboard::KeyCode keycode, Event*event) {
	cocos2d::log("release");
}

