#include "BloodBar.h"

BloodBar::BloodBar() {
}

BloodBar::~BloodBar() {
}

bool BloodBar::init(int _maxHP, int _HP, Point position, int z,
	std::string bloodBarPath, std::string bloodBarBackgroundPath, BloodBarDirection _bloodBarDirection, int tag) {
	if (!Layer::init()) {
		return false;
	}

	bloodBarDirection = _bloodBarDirection;
	maxHP = _maxHP;
	HP = _HP;

	// Ñª²Û
	bloodBarBackground = Sprite::create(bloodBarBackgroundPath);
	bloodBarBackground->setPosition(position);
	if (_bloodBarDirection == LEFT) {
		bloodBarBackground->setScaleX(-1);
	}
	this->addChild(bloodBarBackground, z);

	// ÑªÌõ
	bloodBar = Sprite::create(bloodBarPath);
	bloodBar->setPosition(position);
	if (_bloodBarDirection == LEFT) {
		bloodBar->setScaleX(-1);
	}
	this->addChild(bloodBar, z + 1);

	/*auto anchorPoint = bloodBar->getAnchorPoint();
	CCLOG(std::to_string(anchorPoint.x).c_str());
	CCLOG(std::to_string(anchorPoint.y).c_str());*/

	/*auto rect = bloodBar->getTextureRect();
	CCLOG(std::to_string(rect.origin.x).c_str());
	CCLOG(std::to_string(rect.origin.y).c_str());
	CCLOG(std::to_string(rect.size.width).c_str());
	CCLOG(std::to_string(rect.size.height).c_str());*/

	return true;
}

BloodBar* BloodBar::create(int _maxHP, int _HP, Point position, int z,
	std::string bloodBarPath, std::string bloodBarBackgroundPath, BloodBarDirection _bloodBarDirection, int tag) {
	BloodBar* pRet = new(std::nothrow) BloodBar();
	if (pRet && pRet->init(_maxHP, _HP, position, z, bloodBarPath, bloodBarBackgroundPath, _bloodBarDirection, tag)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void BloodBar::setBloodBarDirection(BloodBarDirection _bloodBarDirection) {
	bloodBarDirection = _bloodBarDirection;
}

BloodBarDirection BloodBar::getBloodBarDirection() {
	return bloodBarDirection;
}

void BloodBar::setMaxHP(int _maxHP) {
	maxHP = _maxHP;
}

int BloodBar::getMaxHP() {
	return maxHP;
}

void BloodBar::setHP(int _HP) {
	HP = _HP;

	float percent = (float)HP / maxHP;
	if (bloodBarDirection == BloodBarDirection::RIGHT) {
		bloodBar->setTextureRect(Rect(0, 0, 123 + 202 * percent, bloodBar->getContentSize().height));
		bloodBar->setPositionX(bloodBarBackground->getPositionX() - 12 - 202 * (1 - percent) / 2);
	}
	else {
		bloodBar->setTextureRect(Rect(0, 0, 123 + 202 * percent, bloodBar->getContentSize().height));
		bloodBar->setPositionX(bloodBarBackground->getPositionX() + 12 + 202 * (1 - percent) / 2);
	}
}

int BloodBar::getHP() {
	return HP;
}

