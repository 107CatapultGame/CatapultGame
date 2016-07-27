#include "../Global.h"
#include "../Controllers/MapController/MapController.h"
#include "../Controllers/EdgeController/EdgeController.h"
#include "GamePlayScene.h"
#include "../MainpageScene.h"
#include "../StartGameScene/SingleLevelScene.h"
#include "../Controllers/PlayerController/progressTime.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

#define ATTACK_COUNT 11 // 每11发子弹有一发是大型子弹

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

    moveDirection = 0;
    flip = false;
    canJump = true;
    playerHp = playerInitHP;
    enemy1Hp = enemy1InitHP;

    addBackground(); // 添加背景
    //this->addChild(background/* Global::LAYER_BACKGROUND*/);
    //addMenu(); // 添加菜单
    addUI(); // 添加UI
    addGameItem(); // 添加游戏元素

    addListener(); // 添加监听器
    // 每分钟更新一次时间
    //this->schedule(schedule_selector(GamePlay::updateTime), 60.0f, kRepeatForever, 0);
    // 每秒相互攻击
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->preloadEffect("musics/paodan.wav");
    this->schedule(schedule_selector(GamePlay::attack), 0.5f, kRepeatForever, 2.0f);
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
    // 分数显示
    scoreLabel = Label::create("Score: 0", "fonts/arial.ttf", 20);
    scoreLabel->setPosition(Global::getVisibleSize() - scoreLabel->getContentSize() / 2 - Size(20, 20));
    this->addChild(scoreLabel, Global::LAYER_UI);
}

void GamePlay::addGameItem() {
    // 物理世界边界
    this->addChild(EdgeController::getInstance(), Global::LAYER_GAMEPLAY);

    catapultSize = 80;

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
    // 掩码
    auto playerBitmask = Global::getSpriteBitmask(SPRITE_TYPE::player);
    playerBody->setCategoryBitmask(playerBitmask.categoryBitmask); // 设置类别掩码
    playerBody->setContactTestBitmask(playerBitmask.contactTestBitmask); // 设置接触测试掩码
    playerBody->setRotationEnable(false);
    playerBody->setMass(40.0f);
    playerBody->setTag(Global::getSpriteTag(SPRITE_TYPE::player));
    player->setPhysicsBody(playerBody);
    this->addChild(player);
    player->setAnchorPoint(Vec2(0.5, 0.5));

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
    // 掩码
    auto enemy1Bitmask = Global::getSpriteBitmask(SPRITE_TYPE::enemy1);
    enemyBody->setCategoryBitmask(enemy1Bitmask.categoryBitmask); // 设置类别掩码
    enemyBody->setContactTestBitmask(enemy1Bitmask.contactTestBitmask); // 设置接触测试掩码
    enemyBody->setRotationEnable(false);
    enemyBody->setMass(40.0f);
    enemyBody->setTag(Global::getSpriteTag(SPRITE_TYPE::enemy1));
    enemy->setPhysicsBody(enemyBody);
    this->addChild(enemy);
    //攻击动画
    playerAttackAnimation.reserve(5);
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(texture0, CC_RECT_PIXELS_TO_POINTS(Rect(210 * i, 0, 210, 210)));
        playerAttackAnimation.pushBack(frame);
    }
    playerAttackAnimation.pushBack(frame0);
    enemyAttackAnimation.reserve(5);
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(textureE, CC_RECT_PIXELS_TO_POINTS(Rect(210 * i, 0, 210, 210)));
        enemyAttackAnimation.pushBack(frame);
    }
    enemyAttackAnimation.pushBack(frameE);

    // 玩家血条
    auto hpBar = Sprite::create("images/gameplay/hp_content.png", CC_RECT_PIXELS_TO_POINTS(Rect(5, 0, 10, 14)));
    auto hpBack = Sprite::create("images/gameplay/hp_bar.png");
    player_hp = ProgressTimer::create(hpBar);
    player_hp->setScaleX(player->getContentSize().width / player_hp->getContentSize().width);
    //pT->setAnchorPoint(Vec2::ZERO);
    player_hp->setType(ProgressTimerType::BAR);
    player_hp->setBarChangeRate(Point(1, 0));
    player_hp->setMidpoint(Point(0, 0));
    player_hp->setPercentage(100);
    player_hp->setPosition(player->getContentSize().width / 2, player->getContentSize().height + 7.5);
    player->addChild(player_hp, Global::LAYER_GAMEPLAY + 1);
    hpBack->setScaleX(player->getContentSize().width / hpBack->getContentSize().width);
    hpBack->setPosition(player_hp->getPosition());
    player->addChild(hpBack, Global::LAYER_GAMEPLAY);

    // 怪物血条
    auto e1HpBar = Sprite::create("images/gameplay/hp_content.png", CC_RECT_PIXELS_TO_POINTS(Rect(5, 0, 10, 14)));
    auto e1HpBack = Sprite::create("images/gameplay/hp_bar.png");
    enemy1_hp = ProgressTimer::create(e1HpBar);
    enemy1_hp->setScaleX(enemy->getContentSize().width / enemy1_hp->getContentSize().width);
    enemy1_hp->setType(ProgressTimerType::BAR);
    enemy1_hp->setBarChangeRate(Point(1, 0));
    enemy1_hp->setMidpoint(Point(0, 0));
    enemy1_hp->setPercentage(100);
    enemy1_hp->setPosition(enemy->getContentSize().width / 2, enemy->getContentSize().height + 7.5);
    enemy->addChild(enemy1_hp, Global::LAYER_GAMEPLAY + 1);
    e1HpBack->setScaleX(enemy->getContentSize().width / e1HpBack->getContentSize().width);
    e1HpBack->setPosition(enemy1_hp->getPosition());
    enemy->addChild(e1HpBack, Global::LAYER_GAMEPLAY);

    // 蓄力条
	auto pt = progressTime::getInstance();
    addChild(pt, Global::LAYER_GAMEPLAY);

    bulletCount = 0;
}

bool GamePlay::hurt(SPRITE_TYPE spriteType, BULLET_TYPE bulletType) {
    const float bullet1 = Global::getBulletHurt(BULLET_TYPE::bullet1);
    const float bullet2 = Global::getBulletHurt(BULLET_TYPE::bullet2);
    switch (spriteType) {
        case SPRITE_TYPE::player:
            if (bulletType == Global::BITMASK_BULLET1) {
                playerHp -= bullet1;
            } else if (bulletType == Global::BITMASK_BULLET2) {
                playerHp -= bullet2;
            }
            if (playerHp < 0) {
                playerHp = 0;
            }
            /* 动画 */
            player_hp->runAction(ProgressTo::create(0.2f, playerHp * 100.0 / playerInitHP));
            if (playerHp == 0) {
                player->removeFromParentAndCleanup(true);
                return true;
            }
            break;
        case SPRITE_TYPE::enemy1:
            if (bulletType == BULLET_TYPE::bullet1) {
                enemy1Hp -= bullet1;
            } else if (bulletType == BULLET_TYPE::bullet2) {
                enemy1Hp -= bullet2;
            }
            if (enemy1Hp < 0) {
                enemy1Hp = 0;
            }
            /* 动画 */
            enemy1_hp->runAction(ProgressTo::create(0.2f, enemy1Hp * 100.0 / enemy1InitHP));
            if (enemy1Hp == 0) {
                unschedule(schedule_selector(GamePlay::attack));
                enemy->removeFromParentAndCleanup(true);
                Global::score += 500;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

void GamePlay::gameOver(bool ifWin) {
    this->unscheduleAllSelectors();
    _eventDispatcher->removeAllEventListeners();
    Global::ifWin = ifWin;
    // 返回
    auto returnItem = MenuItemImage::create(
        "images/startgame/return_normal.png",
        "images/startgame/return_selected.png",
        CC_CALLBACK_1(GamePlay::menuReturnMainCallback, this)
        );
    returnItem->setPosition(Vec2(origin.x + Global::getVisibleSize().width / 2,
        origin.x + Global::getVisibleSize().height / 2 - returnItem->getContentSize().height / 2 - 50));

    // create menu, it's an autorelease object
    auto menu = Menu::create(returnItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, Global::LAYER_UI);
    std::string over = ifWin ? "You Win" : "You Lose";
    auto label = Label::createWithTTF(over, "fonts/arial.ttf", 40);
    label->setPosition(Global::getVisibleSize().width / 2, 650);
    addChild(label, Global::LAYER_UI);
    if (ifWin) {
        auto label = Label::createWithTTF(Global::getScore(), "fonts/arial.ttf", 30);
        label->setPosition(Global::getVisibleSize().width / 2, 600);
        addChild(label, Global::LAYER_UI);
    }
}

void GamePlay::updateTime(float f) {
    // 更新时间
    scoreLabel->setString(Global::getSystemTime());
}

void GamePlay::updateScore() {
    scoreLabel->setString(Global::getScore());
    if (Global::score >= Global::winScore) {
        gameOver(true);
    }
}

void GamePlay::attack(float f) {
    auto enAkAnimation = Animation::createWithSpriteFrames(enemyAttackAnimation, 0.1f);
    auto enemyAttackAnimate = Animate::create(enAkAnimation);
    enemy->runAction(enemyAttackAnimate);

    // enemy石头坐标和player坐标
    Vec2 E = enemy->getPosition() + Vec2(-catapultSize / 2, catapultSize / 2), P = player->getPosition();
    float time1 = 1.5, time2; // 上升下落两段时间
    float g = this->getScene()->getPhysicsWorld()->getGravity().y * -1; // 重力
    float Vx, Vy = time1 * g;
    // 上升高度h1和下落高度h2
    float h1 = Vy*Vy / (2 * g), h2 = h1 - catapultSize / 2 + E.y - P.y;
    time2 = sqrt(2 * h2 / g);
    Vx = -(E.x - P.x + catapultSize / 2) / (time1 + time2);

    // 敌人的投石
    float radius = 15;
    auto enemyBullet = Sprite::create("images/gameplay/bullet.png");
    enemyBullet->setPosition(E);
    enemyBullet->setScale(radius * 2 / enemyBullet->getContentSize().width);
    // 子弹的物理body
    auto ebBody = PhysicsBody::createCircle(enemyBullet->getContentSize().height / 2);
    ebBody = PhysicsBody::createCircle(radius / enemyBullet->getScale());
    // 掩码
    auto enemyBulletBitmask = Global::getBullet1Bitmask(SPRITE_TYPE::enemy1);
    ebBody->setCategoryBitmask(enemyBulletBitmask.categoryBitmask); // 设置类别掩码
    ebBody->setCollisionBitmask(enemyBulletBitmask.collisionBitmask);
    cocos2d::log("colBit:%d", ebBody->getCollisionBitmask());
    ebBody->setContactTestBitmask(enemyBulletBitmask.contactTestBitmask); // 设置接触测试掩码
    ebBody->setVelocity(Vec2(Vx, Vy));
    enemyBullet->setPhysicsBody(ebBody);
    this->addChild(enemyBullet, Global::LAYER_GAMEPLAY);
}

void GamePlay::playerAttack(Vec2 touchPos) {
    // 坐标
    Vec2 dir = touchPos - player->getPosition();
    dir = dir.getNormalized();
    if (!(!flip && dir.x > 0) && !(flip && dir.x < 0)) {
        return;
    }
    // 子弹计数
    bulletCount = (bulletCount + 1) % ATTACK_COUNT;
    auto temp = Vec2(abs(dir.x), abs(dir.y));
    Vec2 bulletInitPos = player->getPosition();
    auto tempx = (!flip && dir.x > 0) ? 1 : -1;
    if (temp.x > temp.y) {
        bulletInitPos += Vec2(tempx * catapultSize / 2, catapultSize / 2 * dir.y / dir.x);
    } else {
        bulletInitPos += Vec2(tempx * catapultSize / 2 * dir.x / dir.y, catapultSize / 2);
    }
    // 速度公式 baseVelocity + argVelocity * 蓄力程度
    const float baseVelocity = 300;
    const float argVelocity = 300;
    Vec2 velocity = (baseVelocity + argVelocity * totalTimeForProgressBar) * dir;

    // 玩家的投石
    auto playerBullet = Sprite::create("images/gameplay/bullet.png");
    PhysicsBody * pbBody;
    float radius;
    if (bulletCount) { // 小子弹
        radius = 15;
        playerBullet->setScale(radius * 2 / playerBullet->getContentSize().width);
        pbBody = PhysicsBody::createCircle(radius / playerBullet->getScale());
        // 掩码
        auto playerBulletBitmask = Global::getBullet1Bitmask(SPRITE_TYPE::player);
        pbBody->setCategoryBitmask(playerBulletBitmask.categoryBitmask); // 设置类别掩码
        pbBody->setCollisionBitmask(playerBulletBitmask.collisionBitmask);
        pbBody->setContactTestBitmask(playerBulletBitmask.contactTestBitmask); // 设置接触测试掩码
        pbBody->setMass(1.0f);
    } else { // 大子弹
        radius = 30;
        playerBullet->setScale(radius * 2 / playerBullet->getContentSize().width);
        pbBody = PhysicsBody::createCircle(radius / playerBullet->getScale());
        // 掩码
        auto playerBulletBitmask = Global::getBullet2Bitmask(SPRITE_TYPE::player);
        pbBody->setCategoryBitmask(playerBulletBitmask.categoryBitmask); // 设置类别掩码
        pbBody->setCollisionBitmask(playerBulletBitmask.collisionBitmask);
        pbBody->setContactTestBitmask(playerBulletBitmask.contactTestBitmask); // 设置接触测试掩码
        pbBody->setMass(8.0f);
    }
    Vec2 add = (temp.x > temp.y) ? tempx * Vec2(radius, 0) : tempx * Vec2(0, radius);
    playerBullet->setPosition(bulletInitPos + add);
    pbBody->setVelocity(velocity + player->getPhysicsBody()->getVelocity());
    playerBullet->setPhysicsBody(pbBody);
    this->addChild(playerBullet, Global::LAYER_GAMEPLAY);

    /* 动作 */
    auto plAkAnimation = Animation::createWithSpriteFrames(playerAttackAnimation, 0.1f);
    auto playerAttackAnimate = Animate::create(plAkAnimation);
    player->runAction(playerAttackAnimate);

    playAudioEffect(); // 音效
}

void GamePlay::updateTimeForProgressBar(float f) {
    totalTimeForProgressBar += f;
    if (totalTimeForProgressBar >= 1.0) {
        progressTime::getInstance()->setProgressPercent(0);
        totalTimeForProgressBar = 0;
        return;
    } else {
        progressTime::getInstance()->setProgressPercent(totalTimeForProgressBar * 100);
    }
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

void GamePlay::playAudioEffect() {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->playEffect("musics/paodan.wav");
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
    // 触摸
    EventListenerTouchOneByOne* screenListener = EventListenerTouchOneByOne::create();
    screenListener->setSwallowTouches(true);// true不向下触摸，简单点来说，比如有两个sprite ,A和 B，A在上B在下（位置重叠）
    screenListener->onTouchBegan = CC_CALLBACK_2(GamePlay::onTouchBegan, this);
    screenListener->onTouchEnded = CC_CALLBACK_2(GamePlay::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(screenListener, this);
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

void GamePlay::menuReturnMainCallback(cocos2d::Ref * pSender) {
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    this->unscheduleAllSelectors(); // 停止所有调度
                                    // 创建场景
    auto MainPageScene = Mainpage::createScene();
    // 切换场景
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25f, MainPageScene));
}

bool GamePlay::onConcactBegan(cocos2d::PhysicsContact & contact) {
    auto body1 = contact.getShapeA()->getBody();
    auto body2 = contact.getShapeB()->getBody();
    auto sprite1 = (Sprite *)contact.getShapeA()->getBody()->getNode();
    auto sprite2 = (Sprite *)contact.getShapeB()->getBody()->getNode();

    if ((body1->getCategoryBitmask() & body2->getContactTestBitmask()) ||
        (body2->getCategoryBitmask() & body1->getContactTestBitmask())) {
        //// 子弹和地板碰撞要消除子弹
        //if (body1->getCategoryBitmask() & (Global::BITMASK_BULLET1 | Global::BITMASK_BULLET2) &&
        //    body2->getCategoryBitmask() & Global::BITMASK_BODY) {
        //    return true;
        //} else if (body2->getCategoryBitmask() & (Global::BITMASK_BULLET1 | Global::BITMASK_BULLET2) &&
        //    body1->getCategoryBitmask() & Global::BITMASK_BODY) {
        //    return true;
        //}
        if (!sprite1 || !sprite2) {
            return false;
        }
        // 子弹和地板碰撞要消除子弹
        if (body1->getCategoryBitmask() & (Global::BITMASK_BULLET1 | Global::BITMASK_BULLET2) &&
            body2->getTag() == Global::TAG_FOOTHOLD) {
            if (sprite1->getPosition().y < 80) {
                body1->removeFromWorld();
                if (sprite1) {
                    sprite1->removeFromParentAndCleanup(true);
                }
            }
            return false;
        }else if (body2->getCategoryBitmask() & (Global::BITMASK_BULLET1 | Global::BITMASK_BULLET2) &&
            body1->getTag() == Global::TAG_FOOTHOLD) {
            body2->removeFromWorld();
            if (sprite2) {
                sprite2->removeFromParentAndCleanup(true);
            }
            return false;
        }
        // 处理投石车和踏板的关系
        if (body1->getCategoryBitmask() & Global::BITMASK_BODY &&
            body2->getCategoryBitmask() & Global::BITMASK_EDGE) {
            canJump = true;
            if (body2->getTag() == Global::TAG_FOOTHOLD) {
                return contact.getContactData()->normal.y > 0;
            }
        } else if (body2->getCategoryBitmask() & Global::BITMASK_BODY &&
            body1->getCategoryBitmask() & Global::BITMASK_EDGE) {
            canJump = true;
            if (body1->getTag() == Global::TAG_FOOTHOLD) {
                return contact.getContactData()->normal.y > 0;
            }
        }

        // 条件成立， 执行相交通知
        return true;
    } else { // 没有相交通知
        return false;
    }
    return true;
}

bool GamePlay::onConcactPreSolve(cocos2d::PhysicsContact & contact) {
    auto body1 = contact.getShapeA()->getBody();
    auto body2 = contact.getShapeB()->getBody();
    auto sprite1 = (Sprite *)contact.getShapeA()->getBody()->getNode();
    auto sprite2 = (Sprite *)contact.getShapeB()->getBody()->getNode();

    if (sprite1 && sprite2) {
        // 二者都不是子弹则跳过处理
        if (!(body1->getCategoryBitmask() & (Global::BITMASK_BULLET1 | Global::BITMASK_BULLET2)) && !(body2->getCategoryBitmask() & (Global::BITMASK_BULLET1 | Global::BITMASK_BULLET2))) {
            return true;
        }
        // 爆炸粒子
        ParticleExplosion* explosion = ParticleExplosion::create();
        explosion->setScale(0.5);
        explosion->setPosition(sprite1->getPosition());
        // 添加粒子
        addChild(explosion);
        // 若二者都是子弹则都移除
        if (body1->getCategoryBitmask() & (Global::BITMASK_BULLET1 | Global::BITMASK_BULLET2) &&
            body2->getCategoryBitmask() & (Global::BITMASK_BULLET1 | Global::BITMASK_BULLET2)) {
            body1->removeFromWorld();
            body2->removeFromWorld();
            sprite1->removeFromParentAndCleanup(true);
            sprite2->removeFromParentAndCleanup(true);
            return true;
        }
        if (body2->getCategoryBitmask() & (Global::BITMASK_BULLET1 | Global::BITMASK_BULLET2)) {
            // 其中一个不是子弹则只移除子弹
            auto bulletType = (body2->getCategoryBitmask() & Global::BITMASK_BULLET1) ? BULLET_TYPE::bullet1 : BULLET_TYPE::bullet2;
            if (body1->getCategoryBitmask() & Global::BITMASK_EDGE) {
                bool tempFlag = false;
                if (body1->getTag() == Global::getMapElemTag(soil)) {
                    tempFlag = EdgeController::getInstance()->hurtSoil(sprite1, bulletType);
                    if (body2->getCategoryBitmask() & Global::BITMASK_PLAYER) {
                        Global::score += EdgeController::edgeScore[MAP_ELEM_TYPE::soil];
                    }
                } else if (body1->getTag() == Global::getMapElemTag(stock)) {
                    tempFlag = EdgeController::getInstance()->hurtStock(sprite1, bulletType);
                    if (body2->getCategoryBitmask() & Global::BITMASK_PLAYER) {
                        Global::score += EdgeController::edgeScore[MAP_ELEM_TYPE::stock];
                    }
                } else if (body1->getTag() == Global::getMapElemTag(meta)) {
                    tempFlag = EdgeController::getInstance()->hurtMeta(sprite1, bulletType);
                    if (body2->getCategoryBitmask() & Global::BITMASK_PLAYER) {
                        Global::score += EdgeController::edgeScore[MAP_ELEM_TYPE::meta];
                    }
                }
                if (tempFlag) {
                    // 成功摧毁障碍物则增加分数
                    updateScore();
                }
            } else if (body1->getCategoryBitmask() & Global::BITMASK_FLAG) {
                bool isEnemy = body1->getCategoryBitmask() & Global::BITMASK_ENEMY1;
                bool tempFlag = EdgeController::getInstance()->hurtFlag(sprite1, bulletType);
                if (tempFlag) {
                    // 敌方旗帜损坏-加分
                    // 我方旗帜损坏-游戏结束
                    if (isEnemy) {
                        updateScore();
                    } else {
                        gameOver(false);
                    }
                }
            } else if (body1->getCategoryBitmask() & Global::BITMASK_BODY) {
                SPRITE_TYPE temp = (body1->getCategoryBitmask() & Global::BITMASK_PLAYER) ? SPRITE_TYPE::player : SPRITE_TYPE::enemy1;
                bool tempFlag = hurt(temp, (body2->getCategoryBitmask() & Global::BITMASK_BULLET1) ? BULLET_TYPE::bullet1 : BULLET_TYPE::bullet2);
                if (tempFlag) {
                    // 敌方投石车损坏-加分
                    // 我方投石车损坏-游戏结束
                    if (temp != SPRITE_TYPE::player) {
                        updateScore();
                    } else {
                        gameOver(false);
                    }
                }
            }
            body2->removeFromWorld();
            sprite2->removeFromParentAndCleanup(true);
            return false;
        } else if (body1->getCategoryBitmask() & (Global::BITMASK_BULLET1 | Global::BITMASK_BULLET2)) {
            // 其中一个不是子弹则只移除子弹
            auto bulletType = (body1->getCategoryBitmask() & Global::BITMASK_BULLET1) ? BULLET_TYPE::bullet1 : BULLET_TYPE::bullet2;
            if (body2->getCategoryBitmask() & Global::BITMASK_EDGE) {
                bool tempFlag = false;
                if (body2->getTag() == Global::getMapElemTag(soil)) {
                    tempFlag = EdgeController::getInstance()->hurtSoil(sprite2, bulletType);
                } else if (body2->getTag() == Global::getMapElemTag(stock)) {
                    tempFlag = EdgeController::getInstance()->hurtStock(sprite2, bulletType);
                } else if (body2->getTag() == Global::getMapElemTag(meta)) {
                    tempFlag = EdgeController::getInstance()->hurtMeta(sprite2, bulletType);
                }
                if (tempFlag) {
                    // 成功摧毁障碍物则增加分数
                    updateScore();
                }
            } else if (body2->getCategoryBitmask() & Global::BITMASK_FLAG) {
                bool isEnemy = body2->getCategoryBitmask() & Global::BITMASK_ENEMY1;
                bool tempFlag = EdgeController::getInstance()->hurtFlag(sprite2, bulletType);
                if (tempFlag) {
                    // 敌方旗帜损坏-加分
                    // 我方旗帜损坏-游戏结束
                    if (isEnemy) {
                        updateScore();
                    } else {
                        gameOver(false);
                    }
                }
            } else if (body2->getCategoryBitmask() & Global::BITMASK_BODY) {
                SPRITE_TYPE temp = (body2->getCategoryBitmask() & Global::BITMASK_PLAYER) ? SPRITE_TYPE::player : SPRITE_TYPE::enemy1;
                bool tempFlag = hurt(temp, (body1->getCategoryBitmask() & Global::BITMASK_BULLET1) ? BULLET_TYPE::bullet1 : BULLET_TYPE::bullet2);
                if (tempFlag) {
                    // 敌方投石车损坏-加分
                    // 我方投石车损坏-游戏结束
                    if (temp != SPRITE_TYPE::player) {
                        updateScore();
                    } else {
                        gameOver(false);
                    }
                }
            }
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
            if (canJump) {
                canJump = false;
                player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() + Vec2(0, 250));
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

bool GamePlay::onTouchBegan(Touch *touch, Event *unused_event) {
	//PlayerController::touchBegan(touch->getLocation());
	Size visibleSize = Global::getVisibleSize();

	//Vec2 touchEndedPosition = Vec2(touchPoint.x + (cameraLocation.x - visibleSize.width / 2), touchPoint.y + (cameraLocation.y - visibleSize.height / 2));
	//archer_->aimAt(touchEndedPosition);

	totalTimeForProgressBar = 0;
	progressTime::getInstance()->setProgressPercent(0);
	this->schedule(schedule_selector(GamePlay::updateTimeForProgressBar), 0.1f);
	return true;
}

void GamePlay::onTouchEnded(Touch *touch, Event *unused_event) {
    auto touchPos = touch->getLocation();
    playerAttack(touchPos);
    progressTime::getInstance()->setProgressPercent(0);
    totalTimeForProgressBar = 0;
	this->unschedule(schedule_selector(GamePlay::updateTimeForProgressBar));
	// Vec2 beginPosition = archer_->getPosition();
}
