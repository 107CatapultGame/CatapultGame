#include "../Global.h"
#include "../Controllers/MapController/MapController.h"
#include "../Controllers/EdgeController/EdgeController.h"
#include "GamePlayScene.h"
#include "../StartGameScene/SingleLevelScene.h"

USING_NS_CC;

#define BITMASK_CONTACT 0xFFFFFFFF
#define BITMASK_EDGE 0x00000001
#define BITMASK_PLAYER 0x00000002
#define BITMASK_ENEMY 0x00000004
#define BITMASK_BODY 0x00000008
#define BITMASK_BULLET 0x00000010
#define BITMASK_BULLET2 0x00000020

void GamePlay::setPhysicsWorld(PhysicsWorld * world) {
    m_world = world;
}

cocos2d::Scene * GamePlay::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    // 红色边框
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    scene->getPhysicsWorld()->setGravity(Point(0, -300)); // 设置重力

    // 'layer' is an autorelease object
    auto layer = GamePlay::create(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GamePlay::init(PhysicsWorld* world) {
    // 1. super init first
    if (!Layer::init()) {
        return false;
    }

    this->setPhysicsWorld(world); // 设置物理世界

    origin = Director::getInstance()->getVisibleOrigin();

    addBackground(); // 添加背景
    //this->addChild(background/* Global::LAYER_BACKGROUND*/);
    addMenu(); // 添加菜单
    //addUI(); // 添加UI
    addGameItem(); // 添加游戏元素

    addListener(); // 添加监听器
    moveDirection = 0;
    flip = false;
    // 每分钟更新一次时间
    //this->schedule(schedule_selector(GamePlay::updateTime), 60.0f, kRepeatForever, 0);
    // 每秒相互攻击
    //this->scheduleOnce(schedule_selector(GamePlay::attack), 2.0f);
    this->schedule(schedule_selector(GamePlay::attack), 1.0f, kRepeatForever, 2.0f);
    return true;
}

GamePlay * GamePlay::create(PhysicsWorld * world) {
    GamePlay* pRet = new(std::nothrow) GamePlay();
    if (pRet && pRet->init(world)) {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    pRet = NULL;
    return NULL;
}

void GamePlay::addBackground() {
    this->addChild(MapController::getInstance(), Global::LAYER_BACKGROUND);
}

void GamePlay::addMenu() {
    // 返回
    auto returnItem = MenuItemImage::create(
        "images/startgame/return_normal.png",
        "images/startgame/return_selected.png",
        CC_CALLBACK_1(GamePlay::menuReturnCallback, this)
        );
    returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2 + 20,
        origin.x + Global::getVisibleSize().height - returnItem->getContentSize().height / 2 - 20));

    // create menu, it's an autorelease object
    auto menu = Menu::create(returnItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, Global::LAYER_UI);
}

void GamePlay::addUI() {
    // 时间-设置一个sprite作为背景图, 一个Label显示文字
    auto timeBg = Sprite::create("images/startgame/time.png");
    timeBg->setPosition(Vec2(origin.x + Global::getVisibleSize().width - timeBg->getContentSize().width / 2 - 20,
        origin.y + timeBg->getContentSize().height / 2 + 20));
    timeLabel = Label::create(Global::getSystemTime(), "fonts/arial.ttf", 20);
    timeLabel->setPosition(timeBg->getPosition());
    this->addChild(timeBg, Global::LAYER_UI);
    this->addChild(timeLabel, Global::LAYER_UI + 1); // 文字层比背景图层高一级
}

void GamePlay::addGameItem() {
    this->addChild(EdgeController::getInstance(), Global::LAYER_GAMEPLAY);
    //// 物理世界边界
    //auto edgeSp = Sprite::create();
    //auto boundBody = PhysicsBody::createEdgeBox(Global::getVisibleSize());
    //boundBody->setDynamic(false);
    ////boundBody->setTag(TAG_EDGE);
    //edgeSp->setPosition(Point(Global::getVisibleSize().width / 2, Global::getVisibleSize().height / 2));
    //edgeSp->setPhysicsBody(boundBody);
    //this->addChild(edgeSp, Global::LAYER_GAMEPLAY);

    const float catapultSize = 80;

    auto spawnPoint = MapController::getInstance()->getObject("born", "spawnpoint");
    int x = spawnPoint["x"].asInt() + spawnPoint["width"].asInt() / 2;
    int y = spawnPoint["y"].asInt() + spawnPoint["height"].asInt() / 2;
    //创建一张贴图
    auto texture0 = Director::getInstance()->getTextureCache()->addImage("images/gameplay/$catapult_wood_attack_toright.png");
    //从贴图中以像素单位切割，创建关键帧
    auto frame0 = SpriteFrame::createWithTexture(texture0, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 210, 210)));

    // 添加玩家
    player = Sprite::createWithSpriteFrame(frame0);
    cocos2d::log("%d, %d", x, y);
    player->setScale(catapultSize / player->getContentSize().height);
    cocos2d::log("%d, %d", x, y);
    player->setPosition(x, y);
    // 玩家的物理body
    auto playerBody = PhysicsBody::createBox(Size(player->getContentSize().height, player->getContentSize().width));
    playerBody->setCategoryBitmask(BITMASK_PLAYER | BITMASK_BODY); // 设置类别掩码
    playerBody->setContactTestBitmask(BITMASK_ENEMY); // 设置接触测试掩码
    playerBody->setRotationEnable(false);
    playerBody->setMass(40.0f);
    player->setPhysicsBody(playerBody);
    this->addChild(player);
    player->setAnchorPoint(Vec2(0.5, 0.5));
    //setViewPointCenter(_player->getPosition());

    // 添加敌人
    auto enemySpawnPoint = MapController::getInstance()->getObject("born", "enemyspawnpoint");
    x = enemySpawnPoint["x"].asInt() + enemySpawnPoint["width"].asInt() / 2;
    y = enemySpawnPoint["y"].asInt() + enemySpawnPoint["height"].asInt() / 2;
    //创建一张贴图
    auto textureE = Director::getInstance()->getTextureCache()->addImage("images/gameplay/$catapult_wood_attack_toleft.png");
    //从贴图中以像素单位切割，创建关键帧
    auto frameE = SpriteFrame::createWithTexture(textureE, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 210, 210)));

    enemy = Sprite::createWithSpriteFrame(frameE);
    enemy->setScale(catapultSize / enemy->getContentSize().height);
    cocos2d::log("%d, %d", x, y);
    enemy->setPosition(x, y);
    // 敌人的物理body
    auto enemyBody = PhysicsBody::createBox(Size(enemy->getContentSize().height, enemy->getContentSize().width));
    enemyBody->setCategoryBitmask(BITMASK_ENEMY | BITMASK_BODY); // 设置类别掩码
    enemyBody->setContactTestBitmask(BITMASK_PLAYER); // 设置接触测试掩码
    enemyBody->setRotationEnable(false);
    enemyBody->setMass(40.0f);
    enemy->setPhysicsBody(enemyBody);
    this->addChild(enemy);
    //攻击动画
    playerAttack.reserve(5);
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(texture0, CC_RECT_PIXELS_TO_POINTS(Rect(210 * i, 0, 210, 210)));
        playerAttack.pushBack(frame);
    }
    playerAttack.pushBack(frame0);
    enemyAttack.reserve(5);
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(textureE, CC_RECT_PIXELS_TO_POINTS(Rect(210 * i, 0, 210, 210)));
        enemyAttack.pushBack(frame);
    }
    enemyAttack.pushBack(frameE);

    // 玩家血条
    playerHp = 100.0;
    auto hpBar = Sprite::create("images/gameplay/hp_content.png", CC_RECT_PIXELS_TO_POINTS(Rect(5, 0, 10, 14)));
    auto hpBack = Sprite::create("images/gameplay/hp_bar.png");
    player_hp = ProgressTimer::create(hpBar);
    player_hp->setScaleX(player->getContentSize().width / player_hp->getContentSize().width);
    //pT->setAnchorPoint(Vec2::ZERO);
    player_hp->setType(ProgressTimerType::BAR);
    player_hp->setBarChangeRate(Point(1, 0));
    //player_hp->setMidpoint(Point(0, 1));
    player_hp->setPercentage(playerHp);
    player_hp->setPosition(player->getContentSize().width / 2, player->getContentSize().height + 7.5);
    player->addChild(player_hp, Global::LAYER_GAMEPLAY + 1);
    hpBack->setScaleX(player->getContentSize().width / hpBack->getContentSize().width);
    hpBack->setPosition(player_hp->getPosition());
    player->addChild(hpBack, Global::LAYER_GAMEPLAY);

    // 怪物血条
    enemy1Hp = 100.0;
    auto e1HpBar = Sprite::create("images/gameplay/hp_content.png", CC_RECT_PIXELS_TO_POINTS(Rect(5, 0, 10, 14)));
    auto e1HpBack = Sprite::create("images/gameplay/hp_bar.png");
    enemy1_hp = ProgressTimer::create(e1HpBar);
    enemy1_hp->setScaleX(enemy->getContentSize().width / enemy1_hp->getContentSize().width);
    enemy1_hp->setType(ProgressTimerType::BAR);
    enemy1_hp->setBarChangeRate(Point(1, 0));
    enemy1_hp->setPercentage(enemy1Hp);
    enemy1_hp->setPosition(enemy->getContentSize().width / 2, enemy->getContentSize().height + 7.5);
    enemy->addChild(enemy1_hp, Global::LAYER_GAMEPLAY + 1);
    e1HpBack->setScaleX(enemy->getContentSize().width / e1HpBack->getContentSize().width);
    e1HpBack->setPosition(enemy1_hp->getPosition());
    enemy->addChild(e1HpBack, Global::LAYER_GAMEPLAY);
}

void GamePlay::hurt(SPRITE_TYPE spriteType, int bulletType) {
    const float bullet1 = 5.0f;
    const float bullet2 = 25.0f;
    switch (spriteType) {
        case SPRITE_TYPE::player:
            if (bulletType == BITMASK_BULLET) {
                playerHp -= bullet1;
            } else if (bulletType == BITMASK_BULLET2) {
                playerHp -= bullet2;
            }
            if (playerHp < 0) {
                playerHp = 0;
            }
            /* 动画 */
            player_hp->runAction(ProgressTo::create(0.2f, playerHp));
            break;
        case SPRITE_TYPE::enemy1:
            if (bulletType == BITMASK_BULLET) {
                enemy1Hp -= bullet1;
            } else if (bulletType == BITMASK_BULLET2) {
                enemy1Hp -= bullet2;
            }
            if (enemy1Hp < 0) {
                enemy1Hp = 0;
            }
            /* 动画 */
            enemy1_hp->runAction(ProgressTo::create(0.2f, enemy1Hp));
            break;
        default:
            break;
    }
}

void GamePlay::updateTime(float f) {
    // 更新时间
    timeLabel->setString(Global::getSystemTime());
}

void GamePlay::attack(float f) {
    /* 动作 */
    auto plAkAnimation = Animation::createWithSpriteFrames(playerAttack, 0.1f);
    auto enAkAnimation = Animation::createWithSpriteFrames(enemyAttack, 0.1f);

    auto playerAttackAnimate = Animate::create(plAkAnimation);
    auto enemyAttackAnimate = Animate::create(enAkAnimation);

    player->runAction(playerAttackAnimate);
    enemy->runAction(enemyAttackAnimate);

    const float velocity = 200;
    
    // 玩家的投石
    auto playerBullet = Sprite::create("images/gameplay/bullet.png");
    playerBullet->setPosition(player->getPosition()
        + Vec2(player->getContentSize().width * player->getScaleX() / 2
        + playerBullet->getContentSize().width / 2, 0));
    // 子弹的物理body
    auto pbBody = PhysicsBody::createCircle(playerBullet->getContentSize().height / 2);
    pbBody->setCategoryBitmask(BITMASK_PLAYER | BITMASK_BULLET); // 设置类别掩码
    //pbBody->setCollisionBitmask(0);
    pbBody->setContactTestBitmask(BITMASK_ENEMY | BITMASK_EDGE); // 设置接触测试掩码
    pbBody->setVelocity(Vec2(velocity, 0));
    playerBullet->setPhysicsBody(pbBody);
    this->addChild(playerBullet, Global::LAYER_GAMEPLAY);


    // 敌人的投石
    auto enemyBullet = Sprite::create("images/gameplay/bullet.png");
    enemyBullet->setPosition(enemy->getPosition()
        - Vec2(enemy->getContentSize().width * enemy->getScaleX() / 2
        + enemyBullet->getContentSize().width / 2, 0));
    // 子弹的物理body
    auto ebBody = PhysicsBody::createCircle(enemyBullet->getContentSize().height / 2);
    ebBody->setCategoryBitmask(BITMASK_ENEMY | BITMASK_BULLET); // 设置类别掩码
    ebBody->setContactTestBitmask(BITMASK_PLAYER | BITMASK_EDGE); // 设置接触测试掩码
    ebBody->setVelocity(Vec2(-velocity, 0));
    enemyBullet->setPhysicsBody(ebBody);
    this->addChild(enemyBullet, Global::LAYER_GAMEPLAY);
}

void GamePlay::move(float f) {
    cocos2d::log("%f,%f", player->getPositionX(), player->getPositionY());
    const float velocity = 200;
    auto curVelocity = player->getPhysicsBody()->getVelocity();
    switch (moveDirection) {
        case 2:
            if (player->getPositionX() < origin.x + Global::getVisibleSize().width - 40 && curVelocity.x > -20) {
                player->getPhysicsBody()->setVelocity(Vec2(velocity, curVelocity.y));
            } else {
                moveDirection = 0;
                auto temp = player->getPhysicsBody();
                player->getPhysicsBody()->setVelocity(Vec2(0, curVelocity.y));
            }
            break;
        case 1:
            if (player->getPositionX() > origin.x + 40) {
                player->getPhysicsBody()->setVelocity(Vec2(-velocity, curVelocity.y));
            } else {
                moveDirection = 0;
                player->getPhysicsBody()->setVelocity(Vec2(0, curVelocity.y));
            }
            break;
        case 0:
            player->getPhysicsBody()->setVelocity(Vec2(0, curVelocity.y));
            break;
        default:
            break;
    }
}

void GamePlay::addListener() {
    // 碰撞
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GamePlay::onConcactBegan, this);
    contactListener->onContactPreSolve = CC_CALLBACK_1(GamePlay::onConcactPreSolve, this);
    contactListener->onContactPostSolve = CC_CALLBACK_1(GamePlay::onConcactPostSolve, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(contactListener, 1);
    // 键盘
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GamePlay::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(GamePlay::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(keyboardListener, 1);
}

void GamePlay::menuReturnCallback(cocos2d::Ref * pSender) {
    cocos2d::log("return"); // test
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners(); 
    this->unscheduleAllSelectors(); // 停止所有调度
    // 创建场景
    auto SingleLevelScene = SingleLevel::createScene();
    // 切换场景
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25f, SingleLevelScene));
}

bool GamePlay::onConcactBegan(cocos2d::PhysicsContact & contact) {
    cocos2d::log("began"); // test
    auto body1 = contact.getShapeA()->getBody();
    auto body2 = contact.getShapeB()->getBody();
    auto sprite1 = (Sprite *)contact.getShapeA()->getBody()->getNode();
    auto sprite2 = (Sprite *)contact.getShapeB()->getBody()->getNode();

    if ((body1->getCategoryBitmask() & body2->getContactTestBitmask()) ||
        (body2->getCategoryBitmask() & body1->getContactTestBitmask())) {
        // 子弹和地板碰撞要消除子弹
        if (body1->getCategoryBitmask() & (BITMASK_BULLET | BITMASK_BULLET2) &&
            body2->getCategoryBitmask() & BITMASK_EDGE) {
            body1->removeFromWorld();
            sprite1->removeFromParentAndCleanup(true);
            return false;
        }else if (body2->getCategoryBitmask() & (BITMASK_BULLET | BITMASK_BULLET2) &&
            body1->getCategoryBitmask() & BITMASK_EDGE) {
            body2->removeFromWorld();
            sprite2->removeFromParentAndCleanup(true);
            return false;
        }
        // 条件成立， 执行相交通知
        return true;
    } else { // 没有相交通知
        return false;
    }
    return false;
}

bool GamePlay::onConcactPreSolve(cocos2d::PhysicsContact & contact) {
    cocos2d::log("pre"); // test
    auto body1 = contact.getShapeA()->getBody();
    auto body2 = contact.getShapeB()->getBody();
    auto sprite1 = (Sprite *)contact.getShapeA()->getBody()->getNode();
    auto sprite2 = (Sprite *)contact.getShapeB()->getBody()->getNode();

    if (sprite1 && sprite2) {
        // 爆炸粒子
        ParticleExplosion* explosion = ParticleExplosion::create();
        explosion->setScale(0.5);
        explosion->setPosition(sprite1->getPosition());
        // 添加粒子
        addChild(explosion);
        // 若二者都是子弹则都移除
        if (body1->getCategoryBitmask() & (BITMASK_BULLET | BITMASK_BULLET2) &&
            body2->getCategoryBitmask() & (BITMASK_BULLET | BITMASK_BULLET2)) {
            body1->removeFromWorld();
            body2->removeFromWorld();
            sprite1->removeFromParentAndCleanup(true);
            sprite2->removeFromParentAndCleanup(true);
        } else if (body1->getCategoryBitmask() & BITMASK_BODY && body2->getCategoryBitmask() & (BITMASK_BULLET | BITMASK_BULLET2)) {
            // 其中一个不是子弹则只移除子弹
            body1->setVelocity(Vec2::ZERO);
            body2->removeFromWorld();
            sprite2->removeFromParentAndCleanup(true);
            return false;
        } else if (body2->getCategoryBitmask() & BITMASK_BODY && body1->getCategoryBitmask() & (BITMASK_BULLET | BITMASK_BULLET2)) {
            body2->setVelocity(Vec2::ZERO);
            body1->removeFromWorld();
            sprite1->removeFromParentAndCleanup(true);
            return false;
        }
    }
    return true;
}

bool GamePlay::onConcactPostSolve(cocos2d::PhysicsContact & contact) {
    cocos2d::log("post"); // test
    //auto body1 = contact.getShapeA()->getBody();
    //auto body2 = contact.getShapeB()->getBody();
    //auto sprite1 = (Sprite *)contact.getShapeA()->getBody()->getNode();
    //auto sprite2 = (Sprite *)contact.getShapeB()->getBody()->getNode();

    //// 确认两个精灵有效
    //if (sprite1 && sprite2) {
    //    // 若二者都是子弹则都移除
    //    if (body1->getCategoryBitmask() & BITMASK_BULLET &&
    //        body2->getCategoryBitmask() & BITMASK_BULLET) {
    //        body1->removeFromWorld();
    //        body2->removeFromWorld();
    //        sprite1->removeFromParentAndCleanup(true);
    //        sprite2->removeFromParentAndCleanup(true);
    //    } else if (body1->getCategoryBitmask() & BITMASK_BODY) {
    //        // 其中一个不是子弹则只移除子弹
    //        body1->setVelocity(Vec2::ZERO);
    //        body2->removeFromWorld();
    //        sprite2->removeFromParentAndCleanup(true);
    //        return false;
    //    } else if (body2->getCategoryBitmask() & BITMASK_BODY) {
    //        body2->setVelocity(Vec2::ZERO);
    //        body1->removeFromWorld();
    //        sprite1->removeFromParentAndCleanup(true);
    //        return false;
    //    }
    //}
    return true;
}

void GamePlay::onKeyPressed(EventKeyboard::KeyCode keycode, Event * event) {
    switch (keycode) {
        case EventKeyboard::KeyCode::KEY_A:
            if (!flip) {
                player->setFlippedX(true);
                flip = !flip;
            }
            moveDirection = 1;
            this->schedule(schedule_selector(GamePlay::move), 0.1f, kRepeatForever, 0);
            break;
        case EventKeyboard::KeyCode::KEY_D:
            if (flip) {
                player->setFlippedX(false);
                flip = !flip;
            }
            moveDirection = 2;
            this->schedule(schedule_selector(GamePlay::move), 0.1f, kRepeatForever, 0);
            break;
        case EventKeyboard::KeyCode::KEY_SPACE:
            cocos2d::log("%f", player->getPhysicsBody()->getVelocity().y);
            if (player->getPosition().y < 40 + 35 + 5) {
                player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() + Vec2(0, 200));
            }
            break;
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            break;
        default:
            break;
    }
}

void GamePlay::onKeyReleased(EventKeyboard::KeyCode keycode, Event * event) {
    switch (keycode) {
        case EventKeyboard::KeyCode::KEY_A:
            moveDirection = 0;
            player->getPhysicsBody()->setVelocity(Vec2(0, player->getPhysicsBody()->getVelocity().y));
            this->unschedule(schedule_selector(GamePlay::move));
            break;
        case EventKeyboard::KeyCode::KEY_D:
            moveDirection = 0;
            player->getPhysicsBody()->setVelocity(Vec2(0, player->getPhysicsBody()->getVelocity().y));
            this->unschedule(schedule_selector(GamePlay::move));
            break;
        case EventKeyboard::KeyCode::KEY_SPACE:
            break;
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            break;
        default:
            break;
    }
}
