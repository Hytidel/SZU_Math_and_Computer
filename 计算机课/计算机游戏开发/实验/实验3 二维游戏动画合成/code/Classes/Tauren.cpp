#include "Tauren.h"

Tauren::Tauren() {
}

Tauren::~Tauren() {
}

void Tauren::setArmature(Armature* _armature) {
	armature = _armature;
}

Armature* Tauren::getArmature() {
	return armature;
}

int Tauren::getMaxHP() {
	return maxHP;
}

void Tauren::setHP(int _HP) {
	HP = _HP;
}

int Tauren::getHP() {
	return HP;
}

void Tauren::setState(State _state) {
	state = _state;
}

State Tauren::getState() {
	return state;
}

void Tauren::setIsRunning(bool _isRunning) {
	isRunning = _isRunning;
}

bool Tauren::getIsRunning() {
	return isRunning;
}

void Tauren::setIsAttacking(bool _isAttacking) {
	isAttacking = _isAttacking;
}

bool Tauren::getIsAttacking() {
	return isAttacking;
}

void Tauren::setIsHurt(bool _isHurt) {
	isHurt = _isHurt;
}

bool Tauren::getIsHurt() {
	return isHurt;
}

void Tauren::setIsDead(bool _isDead) {
	isDead = _isDead;
}

bool Tauren::getIsDead() {
	return isDead;
}

void Tauren::setIsDefensing(bool _isDefensing) {
	isDefensing = _isDefensing;
}

bool Tauren::getIsDefensing() {
	return isDefensing;
}

Tauren* Tauren::create(int _maxHP, Point position) {
	Tauren* pRet = new(std::nothrow) Tauren();
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

bool Tauren::init(int _maxHP, Point position) {
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

	ArmatureDataManager::getInstance()->addArmatureFileInfo(ANIMATION_TAUREN_PATH.c_str());
	armature = Armature::create("tauren");
	if (!armature) {
		CCLOG("Tauren loaded failed.");
		return false;
	}

	armature->setPosition(Point::ZERO);
	armature->getAnimation()->play("loading");
	armature->getAnimation()->setFrameEventCallFunc(
		CC_CALLBACK_0(Tauren::onFrameEvent, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)
	);
	this->addChild(armature);
	this->setPosition(position);
	this->scheduleUpdate();

	return true;
}

void Tauren::update(float dt) {
	if (HP <= 0) {
		switchState(DEAD);
	}

	switch (state) {
		case STAND: {
			// 跑动结束只执行一次 loading , 避免点击 attack 动作无法播放
			if (!isDead && isRunning && !isHurt) {
				armature->getAnimation()->play("loading");
				isRunning = false;
			}

			break;
		}
		case MOVELEFT: {
			if (!isDead && this->getPositionX() > 0 && !isHurt && !isAttacking) {
				if (!isRunning) {
					armature->getAnimation()->play("run");
					isRunning = true;
				}

				if (doubleCompare(armature->getScaleX(), 1)) {
					armature->setScaleX(1);
				}

				float targetX = this->getPositionX() - SPEED_X;
				targetX = std::max(targetX, 180.0f);
				this->setPositionX(targetX);
			}

			break;
		}
		case MOVERIGHT: {
			if (!isDead && this->getPositionX() < Director::getInstance()->getVisibleSize().width && !isHurt && !isAttacking) {
				if (!isRunning) {
					armature->getAnimation()->play("run");
					isRunning = true;
				}

				if (doubleCompare(armature->getScaleX(), -1)) {
					armature->setScaleX(-1);
				}

				float targetX = this->getPositionX() + SPEED_X;
				targetX = std::min(targetX, Director::getInstance()->getVisibleSize().width - 180.0f / 2);
				this->setPositionX(targetX);
			}

			break;
		}
		case ATTACK: {
			// if (!isDead && !isHurt) {
			if (!isDead) {
				armature->getAnimation()->play("attack");
				isAttacking = true;

				// 播放攻击音效
				SimpleAudioEngine::getInstance()->playEffect(GLASS_BREAK_PATH.c_str());
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
				isHurt = false;
			}

			break;
		}
	}
}

// 切换状态
void Tauren::switchState(State _state) {
	if (_state == SMITTEN) {
		armature->getAnimation()->play("smitten");
		isHurt = true;
	}
	
	state = _state;
}

// 帧动画
void Tauren::onFrameEvent(cocostudio::Bone* bone, const std::string& evt, int originFrameIndex, int currentFrameIndex) {
	if (evt == "attack_end") {
		armature->getAnimation()->play("loading");
		isAttacking = false;
	}
	else if (evt == "smitten_end") {
		armature->getAnimation()->play("loading");
	}
}

// 受伤
void Tauren::playHurt(int bulletDamage) {
	// Tauren 护甲
	if (isDefensing) {
		isDefensing = false;
		return;
	}

	if (bulletDamage) {  // 子弹
		displayBloodTip(bulletDamage);
	}
	else {  // 平 A
		displayBloodTips(BASIC_DAMAGE);
	}
	
	this->switchState(SMITTEN);
}

// 显示单个扣血 (子弹)
void Tauren::displayBloodTip(int basicDamage) {
	int damage = basicDamage + generateRandomInteger(0, 5);
	setHP(HP - damage);

	auto label = Label::createWithBMFont(TIPS_FNT_PATH, StringUtils::format("-%d", damage));
	label->setColor(Color3B::ORANGE);
	label->setPosition(Point(20 + generateRandomInteger(0, 80), 10 + generateRandomInteger(0, 80)));
	label->runAction(
		Sequence::create(
			MoveBy::create(0.7f, Point(0, 30)),
			CallFunc::create(CC_CALLBACK_0(Tauren::bloodTipFlyEnd, this, label)),
			nullptr
		)
	);

	this->addChild(label, 15);
}

// 显示多个扣血 (平 A)
void Tauren::displayBloodTips(int basicDamage) {
	// 20% Miss
	int missRand = generateRandomInteger(1, 10);
	if (missRand <= 2) {
		auto label = Label::createWithBMFont(TIPS_FNT_PATH, "Miss");
		label->setColor(Color3B::GREEN);
		label->setPosition(Point(20 + generateRandomInteger(0, 80), 10 + generateRandomInteger(0, 80)));
		label->runAction(
			Sequence::create(
				MoveBy::create(0.7f, Point(0, 30)),
				CallFunc::create(CC_CALLBACK_0(Tauren::bloodTipFlyEnd, this, label)),
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
				CallFunc::create(CC_CALLBACK_0(Tauren::bloodTipFlyEnd, this, label)),
				nullptr
			)
		);

		this->addChild(label, 15);
	}
}

// 扣血数字上移并消失
void Tauren::bloodTipFlyEnd(Label* label) {
	label->setVisible(false);
	label->removeFromParent();
}



