#include "TaurenAI.h"

TaurenAI::TaurenAI() {
}

TaurenAI::~TaurenAI() {
}

TaurenAI* TaurenAI::create(Node* parent) {
	TaurenAI* pRet = new(std::nothrow) TaurenAI();
	if (pRet && pRet->init(parent)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool TaurenAI::init(Node* parent) {
	if (!Node::init()) {
		return false;
	}

	taurenDirection = TaurenDirection::FACELEFT;
	taurenSwitchAction = 0;

	defenseCD = 0;
	stealthCD = 0;
	strikeFlyCD = 0;

	parent->addChild(this);
	return true;
}

void TaurenAI::update(float dt) {
	if (taurenState == State::DEAD) {
		return;
	}

	tauren->switchState(taurenState);

	updateTaurenAction();

	updateSkills();
}

void TaurenAI::setAI(Hero* _hero, Tauren* _tauren) {
	hero = _hero;
	tauren = _tauren;

	this->scheduleUpdate();

	// startAction(TaurenDirection::FACELEFT);
}

void TaurenAI::stand() {
	taurenState = State::STAND;
}

void TaurenAI::moveLeft() {
	taurenState = State::MOVELEFT;
}

void TaurenAI::moveRight() {
	taurenState = State::MOVERIGHT;
}

void TaurenAI::attack() {
	taurenState = State::ATTACK;
}

void TaurenAI::setTaurenState(State _taurenState) {
	taurenState = _taurenState;
}

State TaurenAI::getTaurenState() {
	return taurenState;
}

// 启动 TaurenAI
void TaurenAI::startAction(TaurenDirection _taurenDirection) {
	taurenDirection = _taurenDirection;

	Sequence* sequence;
	if (taurenDirection == TaurenDirection::FACELEFT) {
		sequence = Sequence::create(
			DelayTime::create(0.8f),
			CallFunc::create(CC_CALLBACK_0(TaurenAI::moveLeft, this)),
			DelayTime::create(1.0f),
			CallFunc::create(CC_CALLBACK_0(TaurenAI::attack, this)),
			DelayTime::create(0.3f),
			CallFunc::create(CC_CALLBACK_0(TaurenAI::moveRight, this)),
			DelayTime::create(0.81f),
			CallFunc::create(CC_CALLBACK_0(TaurenAI::stand, this)),
			DelayTime::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(TaurenAI::attack, this)),
			nullptr
		);
		RepeatForever* action = RepeatForever::create(sequence);
		this->runAction(action);
	}
	else {
		sequence = Sequence::create(
			DelayTime::create(0.8f),
			CallFunc::create(CC_CALLBACK_0(TaurenAI::moveRight, this)),
			DelayTime::create(1.0f),
			CallFunc::create(CC_CALLBACK_0(TaurenAI::attack, this)),
			DelayTime::create(0.3f),
			CallFunc::create(CC_CALLBACK_0(TaurenAI::moveLeft, this)),
			DelayTime::create(0.81f),
			CallFunc::create(CC_CALLBACK_0(TaurenAI::stand, this)),
			DelayTime::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(TaurenAI::attack, this)),
			nullptr
		);
		RepeatForever* action = RepeatForever::create(sequence);
		this->runAction(action);
	}
}

// 停止 TaurenAI
void TaurenAI::stopAction() {
	this->stopAllActions();
}

void TaurenAI::setTaurenDirection(TaurenDirection _taurenDirection) {
	taurenDirection = _taurenDirection;
}

TaurenDirection TaurenAI::getTaurenDirection() {
	return taurenDirection;
}

void TaurenAI::setTaurenSwitchAction(int _taurenSwitchAction) {
	taurenSwitchAction = _taurenSwitchAction;
}

int TaurenAI::getTaurenSwitchAction() {
	return taurenSwitchAction;
}

// 更新动作序列
void TaurenAI::updateTaurenAction() {
	taurenSwitchAction++;
	if (taurenSwitchAction >= TAUREN_SWITCH_ACTION) {
		if (tauren->getState() == State::STAND || tauren->getState() == State::SMITTEN) {
			if (doubleCompare(hero->getPositionX(), tauren->getPositionX()) <= 0) {
				stopAction();
				startAction(TaurenDirection::FACELEFT);
			}
			else {
				stopAction();
				startAction(TaurenDirection::FACERIGHT);
			}

			taurenSwitchAction = 0;
		}
	}
}

// 更新所有技能
void TaurenAI::updateSkills() {
	if (defenseCD) {
		defenseCD++;
		if (defenseCD >= TAUREN_DEFENSE_CD) {
			tauren->setIsDefensing(true);

			defenseCD = 1;
		}
	}

	if (stealthCD) {
		stealthCD++;
		if (stealthCD >= TAUREN_STEALTH_CD) {
			auto action = Sequence::create(
				FadeOut::create(0.5f),
				DelayTime::create(4.0f),
				FadeIn::create(0.5f),
				nullptr
			);
			tauren->getArmature()->runAction(action);

			stealthCD = 1;
		}
	}

	if (strikeFlyCD) {
		strikeFlyCD++;
		if (strikeFlyCD >= TAUREN_STRIKE_FLY_CD) {
			tauren->getArmature()->getAnimation()->play("attack");

			// Hero 离 Tauren 越近, 击飞的初速度越大
			float distance = fabs(hero->getPositionX() - tauren->getPositionX());
			hero->setStruckFlySpeed(STRUCK_FLY_BASIC_SPEED * (1 - distance / Director::getInstance()->getVisibleSize().width));

			strikeFlyCD = 1;
		}
	}
}

void TaurenAI::setDefenseCD(int _defenseCD) {
	defenseCD = _defenseCD;
}

int TaurenAI::getDefenseCD() {
	return defenseCD;
}

void TaurenAI::setStealthCD(int _stealthCD) {
	stealthCD = _stealthCD;
}

int TaurenAI::getStealthCD() {
	return stealthCD;
}

void TaurenAI::setStrikeFlyCD(int _strikeFlyCD) {
	strikeFlyCD = _strikeFlyCD;
}

int TaurenAI::getStrikeFlyCD() {
	return strikeFlyCD;
}

