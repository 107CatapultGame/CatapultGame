#include "../../Global.h"
#include "Player.h"

USING_NS_CC;

Player * Player::_player = NULL;

Player::Player() {}

Player::~Player() {
    if (_player != NULL) {
        _player->removeFromParentAndCleanup(true);
        _player = NULL;
    }
}

Player * Player::getInstance() {
    if (_player == NULL) {
        _player = Player::create();
    }
    return _player;
}

bool Player::init() {
    if (!Sprite::init()) {
        return false;
    }

    return true;
}

void Player::createSprite() {
    auto visibleSize = Global::getVisibleSize();

}

