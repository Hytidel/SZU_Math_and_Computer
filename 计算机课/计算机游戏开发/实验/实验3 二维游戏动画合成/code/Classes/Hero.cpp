#include "Hero.h"

Hero::Hero() {
}

Hero::~Hero() {
}

void Hero::setArmature(Armature* _armature) {
	armature = _armature;
}

Armature* Hero::getArmature() {
	return armature;
}

int Hero::getMaxHP() {
	return maxHP;
}

void Hero::setHP(int _HP) {
	HP = _HP;
}

int Hero::getHP() {
	return HP;
}

void Hero::setState(State _state) {
	state = _state;
}

State Hero::getState() {
	return state;
}

void Hero::setIsRunning(bool _isRunning) {
	isRunning = _isRunning;
}

bool Hero::getIsRunning() {
	return isRunning;
}

void Hero::setIsAttacking(bool _isAttacking) {
	isAttacking = _isAttacking;
}

bool Hero::getIsAttacking() {
	return isAttacking;
}

void Hero::setIsHurt(bool _isHurt) {
	isHurt = _isHurt;
}

bool Hero::getIsHurt() {
	return isHurt;
}

void Hero::setIsDead(bool _isDead) {
	isDead = _isDead;
}

bool Hero::getIsDead() {
	return isDead;
}

Hero* Hero::create(int _maxHP, Point position) {
	Hero* pRet = new(std::nothrow) Hero();
	if (pRet && pRet->init(_maxHP, position)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Hero::init(int _maxHP, Point position) {
	if (!Sprite::init()) {
		return false;
	}

	maxHP = _maxHP;
	HP = maxHP;
	isRunning = false;
	isAttacking = false;
	isHurt = false;
	isDead = false;
	isDefensing = false;
	smittenLock = 0;
	struckFlySpeed = 0;

	ArmatureDataManager::getInstance()->addArmatureFileInfo(ANIMATION_HERO_PATH.c_str());
	armature = Armature::create("Hero");
	if (!armature) {
		CCLOG("Hero loaded failed.");
		return false;
	}

	armature->setPosition(Point::ZERO);
	armature->getAnimation()->play("loading");
	armature->getAnimation()->setFrameEventCallFunc(
		CC_CALLBACK_0(Hero::onFrameEvent, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
	);

	this->addChild(armature);
	this->setPosition(position);
	this->scheduleUpdate();

	return true;
}

void Hero::update(float dt) {
	if (HP <= 0) {
		switchState(DEAD);
	}

	switch (state) {
		case STAND: {
			// 跑动结束只执行一次 loading , 避免点击 attack 动作无法播放
			// if (!isDead && isRunning && !isHurt) {
			if (!isDead && isRunning) {
				armature->getAnimation()->play("loading");
				isRunning = false;
			}

			break;
		}
		case MOVELEFT: {
			// if (!isDead && this->getPositionX() > 0 && !isHurt && !isAttacking) {
			if (!isDead && this->getPositionX() > 0 && !isAttacking) {
				if (!isRunning) {
					armature->getAnimation()->play("run");
					isRunning = true;
				}

				// if (armature->getScaleX() != 1) {
				if (doubleCompare(armature->getScaleX(), -1)) {
					armature->setScaleX(-1);
				}

				// this->setPositionX(this->getPositionX() - SPEED_X);
			}

			break;
		}
		case MOVERIGHT: {
			// if (!isDead && this->getPositionX() < Director::getInstance()->getVisibleSize().width && !isHurt && !isAttacking) {
			if (!isDead && this->getPositionX() < Director::getInstance()->getVisibleSize().width && !isAttacking) {
				if (!isRunning) {
					armature->getAnimation()->play("run");
					isRunning = true;
				}

				// if (armature->getScaleX() != -1) {
				if (doubleCompare(armature->getScaleX(), 1)) {
					armature->setScaleX(1);
				}

				// this->setPositionX(this->getPositionX() + SPEED_X);
			}

			break;
		}
		case ATTACK: {
			// if (!isDead && !isHurt) {
			if (!isDead) {
				armature->getAnimation()->play("attack");
				isAttacking = true;

				// 播放攻击音效
				SimpleAudioEngine::getInstance()->playEffect(HIT_METAL_PATH.c_str());
			}

			break;
		}
		case DEAD: {
			if (!isDead) {
				armature->getAnimation()->play("death");
				isDead = true;
			}
			
			break;
		}
		case SMITTEN: {
			if (!isDead && isHurt) {
				armature->getAnimation()->play("smitten");
				// isHurt = false;
			}

			break;
		}
		case DEFENSE: {
			if (!isDead) {
				armature->getAnimation()->play("defend");
				isDefensing = true;
			}

			break;
		}
	}

	// 受伤的锁
	if (smittenLock) {
		smittenLock++;
		if (smittenLock >= HERO_SMITTEN_LOCK) {
			smittenLock = 0;
		}
	}
}

// 切换状态
void Hero::switchState(State _state) {
	if (_state == SMITTEN) {
		armature->getAnimation()->play("smitten");
		isHurt = true;
		isDefensing = false;
	}
	
	state = _state;
}

// 帧动画
void Hero::onFrameEvent(cocostudio::Bone* bone, const std::string& evt, int originFrameIndex, int currentFrameIndex) {
	if (evt == "attack_end") {
		armature->getAnimation()->play("loading");
		isAttacking = false;
	}
	else if (evt == "smitten_end") {
		armature->getAnimation()->play("loading");
		isHurt = false;
	}
	else if (evt == "defense_end") {
		armature->getAnimation()->play("loading");
		isDefensing = false;
	}
}

// 受伤
void Hero::playHurt() {
	// 上受伤锁期间不受伤
	if (smittenLock) {
		return;
	}

	smittenLock = 1;
	if (isDefensing) {
		displayBloodTips(std::max(BASIC_DAMAGE - DEFENSE_DAMAGE, 0));
	}
	else {
		displayBloodTips(BASIC_DAMAGE);
	}
	this->switchState(SMITTEN);
}

// 显示扣血 (平 A)
void Hero::displayBloodTips(int basicDamage) {
	// 20% Miss
	int missRand = generateRandomInteger(1, 10);
	if (missRand <= 2) {
		auto label = Label::createWithBMFont(TIPS_FNT_PATH, "Miss");
		label->setColor(Color3B::GREEN);
		label->setPosition(Point(20 + generateRandomInteger(0, 80), 10 + generateRandomInteger(0, 80)));
		label->runAction(
			Sequence::create(
				MoveBy::create(0.7f, Point(0, 30)),
				CallFunc::create(CC_CALLBACK_0(Hero::bloodTipFlyEnd, this, label)),
				nullptr
			)
		);

		this->addChild(label, 15);

		return;
	}

	// 20% 二连击, 5% 三连击
	int hitCount = 1;  // 连击次数
	int hitRand = generateRandomInteger(1, 100);
	if (hitRand <= 5) {
		hitCount = 3;
	}
	else if (hitRand <= 20) {
		hitCount = 2;
	}
	// hitCount = 1;  // 取消连击

	for (int i = 0; i < hitCount; i++) {
		int damage;

		// 20% 暴击
		int criticalRand = generateRandomInteger(1, 10);
		if (criticalRand <= 2) {
			damage = basicDamage + generateRandomInteger(10, 20);
		}
		else {
			damage = basicDamage + generateRandomInteger(0, 10);
		}
		// damage = basicDamage;  // 取消随机攻击力
		setHP(HP - damage);

		auto label = Label::createWithBMFont(TIPS_FNT_PATH, StringUtils::format("-%d", damage));
		if (criticalRand <= 2) {
			label->setColor(Color3B::RED);  // 暴击为红色
		}
		else {
			label->setColor(Color3B::ORANGE);  // 非暴击为橙色
		}
		label->setPosition(Point(20 + generateRandomInteger(0, 80), 10 + generateRandomInteger(0, 80)));
		label->runAction(
			Sequence::create(
				MoveBy::create(0.7f, Point(0, 30)), 
				CallFunc::create(CC_CALLBACK_0(Hero::bloodTipFlyEnd, this, label)),
				nullptr
			)
		);

		this->addChild(label, 15);
	}
}

// 扣血数字上移并消失
void Hero::bloodTipFlyEnd(Label* label) {
	label->setVisible(false);
	label->removeFromParent();
}

void Hero::setIsDefensing(bool _isDefensing) {
	isDefensing = _isDefensing;
}

bool Hero::getIsDefensing() {
	return isDefensing;
}

void Hero::setStruckFlySpeed(float _struckFlySpeed) {
	struckFlySpeed = _struckFlySpeed;
}

float Hero::getStruckFlySpeed() {
	return struckFlySpeed;
}

// 显示扣血 (击飞)
void Hero::displayBloodTip(int basicDamage) {
	int damage = basicDamage + generateRandomInteger(0, 10);
	setHP(HP - damage);

	auto label = Label::createWithBMFont(TIPS_FNT_PATH, StringUtils::format("-%d", damage));
	label->setColor(Color3B::RED);
	label->setPosition(Point(20 + generateRandomInteger(0, 80), 10 + generateRandomInteger(0, 80)));
	label->runAction(
		Sequence::create(
			MoveBy::create(0.7f, Point(0, 30)),
			CallFunc::create(CC_CALLBACK_0(Hero::bloodTipFlyEnd, this, label)),
			nullptr
		)
	);

	this->addChild(label, 15);
}
