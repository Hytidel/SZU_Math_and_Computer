#include "Level2Scene.h"

Level2Scene::Level2Scene() {
}

Level2Scene::~Level2Scene() {
}

Scene* Level2Scene::createScene() {
	auto scene = Scene::create();
	auto layer = Level2Scene::create();
	scene->addChild(layer);
	return scene;
}

bool Level2Scene::init() {
	if (!Layer::init()) {
		return false;
	}

// ---------= 初始化变量 =---------
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	pause = true;
	// pause = false;

// ---------= 右下角关闭按钮 =---------
	auto closeItem = MenuItemImage::create(
		CLOSE_BUTTON_PATH,
		CLOSE_BUTTON_SELECTED_PATH,
		CC_CALLBACK_1(Level2Scene::closeMenuCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));
	auto closeMenu = Menu::create(closeItem, NULL);
	closeMenu->setPosition(Vec2::ZERO);
	this->addChild(closeMenu, 100);

// ---------= 加载场景 =---------
	loadImage(LEVEL2_BACKGROUND_PATH, Point(visibleSize.width / 2, visibleSize.height / 2), 0);

	loadCloudBackground(1, CLOUD_BACKGROUND_TAG);

	loadJoystick(JOYSTICK_PATH, JOYSTICK_BUTTON_PATH, JOYSTICK_RADIUS, Point(visibleSize.width / 12, 100), 4);

	displayWords("VS", Point(visibleSize.width / 2, visibleSize.height - 80), 5);

	loadKeyboardHint(START_AND_PAUSE_HINT, 10, START_AND_PAUSE_HINT_TAG);

	preloadSounds();

	playBackgroundMusic(DOKU_PATH);

	pauseBackgroundMusic();

	// Hero
	heroMaxHP = 100;
	heroWidth = 100;
	loadHero(heroMaxHP, Point(visibleSize.width / 6, 30), 3);
	loadHeroBloodBar(heroMaxHP, heroMaxHP, Point(visibleSize.width / 6, 9 * visibleSize.height / 10),
		5, HERO_BLOOD_BAR_PATH, HERO_BLOOD_BAR_BACKGROUND_PATH);

	// Tauren
	taurenMaxHP = 150;
	taurenWidth = 180;
	loadTauren(taurenMaxHP, Point(5 * visibleSize.width / 6, 160), 3);
	loadTaurenBloodBar(taurenMaxHP, taurenMaxHP, Point(5 * visibleSize.width / 6, 9 * visibleSize.height / 10),
		5, TAUREN_BLOOD_BAR_PATH, TAUREN_BLOOD_BAR_BACKGROUND_PATH, BloodBarDirection::LEFT);
	loadTaurenShield();

	// 计分板
	loadHeroScoreboard(heroScore, Point(visibleSize.width / 2 - 100, visibleSize.height - 80), 5);
	loadTaurenScoreboard(taurenScore, Point(visibleSize.width / 2 + 100, visibleSize.height - 80), 5);

	// Tauren AI
	TAUREN_SWITCH_ACTION = 300;
	taurenAI = TaurenAI::create(this);
	taurenAI->setAI(hero, tauren);
	taurenAI->stopAction();
	
	// Tauren 技能
	taurenAI->setDefenseCD(1);
	taurenAI->setStealthCD(1);

	// 碰撞检测
	collisionDetection = CollisionDetection::create(this, hero, tauren, taurenAI);

	// 技能按钮
	loadAttackButton(Point(visibleSize.width - 150, 100), 5, ATTACK_BUTTON_TAG);
	loadDefenseButton(Point(visibleSize.width - 80, 100), 5, DEFENSE_BUTTON_TAG);
	loadKnockbackButton(Point(visibleSize.width - 150, 170), 5, KNOCKBACK_BUTTON_TAG);
	
	// 技能 CD
	attackCD = 0;
	defenseCD = 0;
	knockbackCD = 0;

// ---------= 添加监听 =---------
	addKeyboardListener();

// ---------= Update =---------
	this->scheduleUpdate();

	// CCLOG("Level2Scene initialized successfully.");

	return true;
}

// 控制 Hero 移动
void Level2Scene::workHeroMovement() {
	Vec2 direction = joystick->getDirection();
	float dx = direction.x;
	int sgn = doubleCompare(dx, 0);
	if (!sgn) {
		hero->switchState(STAND);
	}
	else if (sgn < 0) {
		hero->switchState(MOVELEFT);
	}
	else {
		hero->switchState(MOVERIGHT);
	}
	
	Point destination = hero->getPosition() + Vec2(direction.x, 0) * joystick->getVelocity() * 0.1;  // 预期目标点
	destination.x = std::max(destination.x, heroWidth / 2),
		destination.x = std::min(destination.x, visibleSize.width - heroWidth / 2);
	hero->setPosition(destination);
}

// 控制 Tauren 移动 (测试用)
void Level2Scene::workTaurenMovement() {
	Vec2 direction = joystick->getDirection();
	float dx = direction.x;
	int sgn = doubleCompare(dx, 0);
	if (!sgn) {
		tauren->switchState(STAND);
	}
	else if (sgn < 0) {
		tauren->switchState(MOVELEFT);
	}
	else {
		tauren->switchState(MOVERIGHT);
	}

	Point destination = tauren->getPosition() + Vec2(direction.x, 0) * joystick->getVelocity() * 0.1;  // 预期目标点
	destination.x = std::max(destination.x, heroWidth / 2),
		destination.x = std::min(destination.x, visibleSize.width - heroWidth / 2);
	tauren->setPosition(destination);
}

// 更新血条
void Level2Scene::updateBloodBar() {
	heroBloodBar->setHP(hero->getHP());
	taurenBloodBar->setHP(tauren->getHP());
}

void Level2Scene::update(float dt) {
	if (pause) {
		return;
	}

	gameJudgement();

	workHeroMovement();

	updateBloodBar();

	updateCloudBackground();

	updateSkillButtons();

	updateTaurenShieldPosition();

	updateBullets();

	// CCLOG(std::to_string(tauren->getIsDefensing()).c_str());
}

// 预加载音乐
void Level2Scene::preloadSounds() {
	preloadBackgroundMusic(DOKU_PATH);
	preloadSoundEffect(HIT_METAL_PATH);
	preloadSoundEffect(GLASS_BREAK_PATH);
}

// 键盘监听的回调函数
void Level2Scene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_ESCAPE: {  // ESC: 关闭
			Director::getInstance()->end();
			break;
		}
		case EventKeyboard::KeyCode::KEY_SPACE: {  // 空格: 
			pause = false;

			auto startAndPauseHint = getChildByTag(START_AND_PAUSE_HINT_TAG);
			if (startAndPauseHint) {
				removeChildByTag(START_AND_PAUSE_HINT_TAG);
			}

			// 更新 bgm 状态等
			resumeBackgroundMusic();
			taurenAI->startAction();
			break;
		}
		case EventKeyboard::KeyCode::KEY_P: {  // P: 暂停
			pause ^= 1;

			// 更新 bgm 状态等
			if (pause) {
				pauseBackgroundMusic();
				taurenAI->stopAction();
			}
			else {
				resumeBackgroundMusic();
				taurenAI->startAction(taurenAI->getTaurenDirection());
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_MINUS: {  // -: Hero 受伤 (测试用)
			hero->playHurt();

			break;
		}
		case EventKeyboard::KeyCode::KEY_EQUAL: {  // =: Tauren 受伤 (测试用)
			tauren->playHurt();

			break;
		}
		case EventKeyboard::KeyCode::KEY_A: {  // A: Hero 攻击
			attackButtonCallback(nullptr);

			break;
		}
		case EventKeyboard::KeyCode::KEY_S: {  // S: Hero 防御
			defenseButtonCallback(nullptr);

			break;
		}
		case EventKeyboard::KeyCode::KEY_D: {  // D: Hero 击退
			knockbackButtonCallback(nullptr);

			break;
		}
		case EventKeyboard::KeyCode::KEY_N: {  // N: 下一关
			auto newScene = Level3Scene::createScene();
			Director::getInstance()->pushScene(newScene);

			break;
		}
		case EventKeyboard::KeyCode::KEY_R: {  // R: 重玩
			auto newScene = Level2Scene::createScene();
			Director::getInstance()->replaceScene(newScene);

			break;
		}
	}
}

// 游戏胜利
void Level2Scene::workVictory() {
	pause = true;

	loadKeyboardHint(REPLAY_AND_NEXT_HINT, 10);

	hero->stopAllActions();
	taurenAI->stopAllActions();

	// 停止技能
	taurenAI->setDefenseCD(0);
	taurenShield->setVisible(false);
	taurenAI->setStealthCD(0);

	updateHeroScoreboard(heroScore + 1);
}

// 游戏失败
void Level2Scene::workDefeat() {
	pause = true;

	loadKeyboardHint(REPLAY_AND_NEXT_HINT, 10);

	hero->stopAllActions();
	taurenAI->stopAllActions();

	// 停止技能
	taurenAI->setDefenseCD(0);
	taurenShield->setVisible(false);
	taurenAI->setStealthCD(0);

	updateTaurenScoreboard(taurenScore + 1);
}

// 游戏胜负判定
void Level2Scene::gameJudgement() {
	// 胜负判定
	if (doubleCompare(hero->getHP(), 0) <= 0) {
		workDefeat();
	}
	else if (doubleCompare(tauren->getHP(), 0) <= 0) {
		workVictory();
	}
}

// 加载摇杆
void Level2Scene::loadJoystick(std::string joystickPath, std::string joystickButtonPath, float joystickRadius, Point position, int z) {
	joystick = Joystick::create(position, joystickRadius, joystickButtonPath.c_str(), joystickPath.c_str());
	this->addChild(joystick, z);
}

// 加载 Hero
void Level2Scene::loadHero(int _maxHP, Point position, int z) {
	hero = Hero::create(_maxHP, position);
	this->addChild(hero, z);
}

// 加载 Tauren
void Level2Scene::loadTauren(int _maxHP, Point position, int z) {
	tauren = Tauren::create(_maxHP, position);
	this->addChild(tauren, z);
}

// 加载 Hero 血条
void Level2Scene::loadHeroBloodBar(int _maxHP, int _HP, Point position, int z,
	std::string bloodBarPath, std::string bloodBarBackgroundPath, BloodBarDirection _bloodBarDirection, int tag) {
	heroBloodBar = BloodBar::create(_maxHP, _HP, position, z, bloodBarPath, bloodBarBackgroundPath, _bloodBarDirection, tag);
	if (~tag) {
		heroBloodBar->setTag(tag);
	}
	this->addChild(heroBloodBar, z);
}

// 加载 Tauren 血条
void Level2Scene::loadTaurenBloodBar(int _maxHP, int _HP, Point position, int z,
	std::string bloodBarPath, std::string bloodBarBackgroundPath, BloodBarDirection _bloodBarDirection, int tag) {
	taurenBloodBar = BloodBar::create(_maxHP, _HP, position, z, bloodBarPath, bloodBarBackgroundPath, _bloodBarDirection, tag);
	if (~tag) {
		taurenBloodBar->setTag(tag);
	}
	this->addChild(taurenBloodBar, z);
}

// 加载 Hero 计分板
void Level2Scene::loadHeroScoreboard(int score, Point position, int z, int tag) {
	heroScore = score;
	heroScoreboard = Label::createWithBMFont(TIPS_FNT_PATH, std::to_string(heroScore));
	heroScoreboard->setPosition(position);
	if (~tag) {
		heroScoreboard->setTag(tag);
	}
	this->addChild(heroScoreboard, z);
}

// 加载 Tauren 计分板
void Level2Scene::loadTaurenScoreboard(int score, Point position, int z, int tag) {
	taurenScore = score;
	taurenScoreboard = Label::createWithBMFont(TIPS_FNT_PATH, std::to_string(taurenScore));
	taurenScoreboard->setPosition(position);
	if (~tag) {
		taurenScoreboard->setTag(tag);
	}
	this->addChild(taurenScoreboard, z);
}

// 更新 Hero 计分板
void Level2Scene::updateHeroScoreboard(int score) {
	Point position = heroScoreboard->getPosition();
	int z = heroScoreboard->getZOrder();
	int tag = heroScoreboard->getTag();
	this->removeChild(heroScoreboard);

	heroScore = score;
	heroScoreboard = Label::createWithBMFont(TIPS_FNT_PATH, std::to_string(heroScore));
	heroScoreboard->setPosition(position);
	if (~tag) {
		heroScoreboard->setTag(tag);
	}
	this->addChild(heroScoreboard, z);
}

// 更新 Tauren 计分板
void Level2Scene::updateTaurenScoreboard(int score) {
	Point position = taurenScoreboard->getPosition();
	int z = taurenScoreboard->getZOrder();
	int tag = taurenScoreboard->getTag();
	this->removeChild(taurenScoreboard);

	taurenScore = score;
	taurenScoreboard = Label::createWithBMFont(TIPS_FNT_PATH, std::to_string(taurenScore));
	taurenScoreboard->setPosition(position);
	if (~tag) {
		taurenScoreboard->setTag(tag);
	}
	this->addChild(taurenScoreboard, z);
}

// 加载云背景
void Level2Scene::loadCloudBackground(int z, int tag) {
	auto cloudBackground1 = Sprite::create(CLOUD_BACKGROUND_PATH);
	cloudBackground1->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	if (~tag) {
		cloudBackground1->setTag(tag);
	}
	this->addChild(cloudBackground1, z);

	auto cloudBackground2 = Sprite::create(CLOUD_BACKGROUND_PATH);
	cloudBackground2->setPosition(Point(cloudBackground1->getPositionX() + cloudBackground1->getContentSize().width, visibleSize.height / 2));
	if (~tag) {
		cloudBackground2->setTag(tag + 1);
	}
	this->addChild(cloudBackground2, z + 1);
}

// 更新云背景
void Level2Scene::updateCloudBackground() {
	auto cloudBackground1 = (Sprite*)this->getChildByTag(CLOUD_BACKGROUND_TAG);
	auto cloudBackground2 = (Sprite*)this->getChildByTag(CLOUD_BACKGROUND_TAG + 1);
	if (doubleCompare(cloudBackground2->getPositionX(), visibleSize.width / 2) >= 0) {
		cloudBackground1->setPositionX(cloudBackground1->getPositionX() - 1);
	}
	else {
		cloudBackground1->setPositionX(visibleSize.width / 2);
	}
	cloudBackground2->setPositionX(cloudBackground1->getPositionX() + cloudBackground1->getContentSize().width);
}

// 更新所有技能的按钮
void Level2Scene::updateSkillButtons() {
	if (attackCD) {
		attackCD++;
		if (attackCD >= ATTACK_CD) {
			attackCD = 0;
			this->getChildByTag(ATTACK_BUTTON_TAG)->setVisible(true);
		}
	}

	if (defenseCD) {
		defenseCD++;
		if (defenseCD >= DEFENSE_CD) {
			defenseCD = 0;
			this->getChildByTag(DEFENSE_BUTTON_TAG)->setVisible(true);
		}
	}

	if (knockbackCD) {
		knockbackCD++;
		if (knockbackCD >= KNOCKBACK_CD) {
			knockbackCD = 0;
			this->getChildByTag(KNOCKBACK_BUTTON_TAG)->setVisible(true);
		}
	}
}

// 加载攻击按钮
void Level2Scene::loadAttackButton(Point position, int z, int tag) {
	auto attackImage = MenuItemImage::create(
		ATTACK_BUTTON, ATTACK_BUTTON_PRESSED,
		CC_CALLBACK_1(Level2Scene::attackButtonCallback, this)
	);
	attackImage->setPosition(position);

	// 闪烁
	auto actionSequence = Sequence::create(
		FadeOut::create(1),
		FadeIn::create(1),
		nullptr
	);
	auto action = RepeatForever::create(actionSequence);
	attackImage->runAction(action);

	auto attackButton = Menu::create(attackImage, nullptr);
	attackButton->setPosition(Point::ZERO);
	if (~tag) {
		attackButton->setTag(tag);
	}

	this->addChild(attackButton, z);
}

// 攻击按钮的回调函数
void Level2Scene::attackButtonCallback(Ref* pSender) {
	hero->switchState(State::ATTACK);

	// 启动攻击按钮 CD
	attackCD = 1;
	this->getChildByTag(ATTACK_BUTTON_TAG)->setVisible(false);
}

// 加载防御按钮
void Level2Scene::loadDefenseButton(Point position, int z, int tag) {
	auto defenseImage = MenuItemImage::create(
		DEFENSE_BUTTON, DEFENSE_BUTTON_PRESSED,
		CC_CALLBACK_1(Level2Scene::defenseButtonCallback, this)
	);
	defenseImage->setPosition(position);

	// 闪烁
	auto actionSequence = Sequence::create(
		FadeOut::create(1),
		FadeIn::create(1),
		nullptr
	);
	auto action = RepeatForever::create(actionSequence);
	defenseImage->runAction(action);

	auto defenseButton = Menu::create(defenseImage, nullptr);
	defenseButton->setPosition(Point::ZERO);
	if (~tag) {
		defenseButton->setTag(tag);
	}

	this->addChild(defenseButton, z);
}

// 防御按钮的回调函数
void Level2Scene::defenseButtonCallback(Ref* pSender) {
	hero->switchState(State::DEFENSE);

	// 启动防御按钮 CD
	defenseCD = 1;
	this->getChildByTag(DEFENSE_BUTTON_TAG)->setVisible(false);
}

// 加载 Tauren 的盾牌
void Level2Scene::loadTaurenShield() {
	taurenShield = Sprite::create(TAUREN_SHIELD_PATH);
	taurenShield->setPositionY(100);
	taurenShield->setVisible(false);
	this->addChild(taurenShield, 12);
}

// 更新 Tauren 的盾牌的位置
void Level2Scene::updateTaurenShieldPosition() {
	Point taurenPoint = tauren->getArmature()->getBone("body")->getDisplayRenderNode()->convertToWorldSpaceAR(Point(0, 0));
	taurenShield->setPositionX(taurenPoint.x);

	if (tauren->getIsDefensing()) {
		taurenShield->setVisible(true);
	}
	else {
		taurenShield->setVisible(false);
	}
}

// 加载击退按钮
void Level2Scene::loadKnockbackButton(Point position, int z, int tag) {
	auto knockbackImage = MenuItemImage::create(
		KNOCKBACK_BUTTON, KNOCKBACK_BUTTON_PRESSED,
		CC_CALLBACK_1(Level2Scene::knockbackButtonCallback, this)
	);
	knockbackImage->setPosition(position);

	// 闪烁
	auto actionSequence = Sequence::create(
		FadeOut::create(1),
		FadeIn::create(1),
		nullptr
	);
	auto action = RepeatForever::create(actionSequence);
	knockbackImage->runAction(action);

	auto knockbackButton = Menu::create(knockbackImage, nullptr);
	knockbackButton->setPosition(Point::ZERO);
	if (~tag) {
		knockbackButton->setTag(tag);
	}

	this->addChild(knockbackButton, z);
}

// 击退按钮的回调函数
void Level2Scene::knockbackButtonCallback(Ref* pSender) {
	Bullet* bullet;
	if (!doubleCompare(hero->getArmature()->getScaleX(), 1)) {  // 向右
		bullet = Bullet::create(Point(hero->getPositionX() + heroWidth / 2, hero->getPositionY() + 100), 
			Vec2(BULLET_SPEED, 0), BULLET_BASIC_DAMAGE, true);
	}
	else if (!doubleCompare(hero->getArmature()->getScaleX(), -1)) {  // 向左
		bullet = Bullet::create(Point(hero->getPositionX() - heroWidth / 2, hero->getPositionY() + 100), 
			Vec2(-BULLET_SPEED, 0), BULLET_BASIC_DAMAGE, true);
	}
	this->addChild(bullet, 10);

	knockbackBullets.push_back(bullet);

	// 启动击退按钮 CD
	knockbackCD = 1;
	this->getChildByTag(KNOCKBACK_BUTTON_TAG)->setVisible(false);
}

// 判断子弹是否飞出屏幕
bool Level2Scene::isBulletOutOfScreen(Bullet* bullet) {
	float bulletX = bullet->getBullet()->getPositionX(), bulletY = bullet->getBullet()->getPositionY();
	return (bulletX < 0) || (bulletX > visibleSize.width) || (bulletY < 0) || (bulletY > visibleSize.height);
}

// 判断子弹是否击中 Tauren
bool Level2Scene::isBulletHitTauren(Bullet* bullet) {
	float bulletX = bullet->getBullet()->getPositionX();
	float taurenX = tauren->getPositionX();
	return taurenX - taurenWidth / 2 < bulletX && bulletX < taurenX + taurenWidth / 2;
}

// 处理子弹命中 Tauren
void Level2Scene::workBulletHitTauren(Bullet* bullet) {
	tauren->playHurt(bullet->getDamage());

	// 击退
	if (bullet->getKnockback()) {
		float targetX = tauren->getPositionX() + bullet->getSpeed().x * BULLET_FORCE;
		targetX = std::max(targetX, taurenWidth), targetX = std::min(targetX, visibleSize.width - taurenWidth / 2);
		tauren->setPositionX(targetX);
	}

	// 此处不直接删除, 而是将其移到屏幕外, 防止一个指针被多次删除
	bullet->getBullet()->setPositionX(2 * visibleSize.width);
}

// 更新子弹
void Level2Scene::updateBullets() {
	std::vector<Bullet*> trash;

	// 击退的子弹
	for (auto& bullet : knockbackBullets) {
		// 飞出屏幕
		if (isBulletOutOfScreen(bullet)) {
			trash.push_back(bullet);
			continue;
		}

		// 与 Tauren 的碰撞检测
		if (isBulletHitTauren(bullet)) {
			workBulletHitTauren(bullet);
		}
	}
	for (auto& knockbackBullet : trash) {
		knockbackBullet->removeBullet();
		auto it = find(knockbackBullets.begin(), knockbackBullets.end(), knockbackBullet);
		if (it != knockbackBullets.end()) {
			knockbackBullets.erase(it);
		}
	}
}


