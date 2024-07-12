#include "Bullet.h"

Bullet::Bullet() {

}

Bullet::~Bullet() {

}

Bullet* Bullet::create(Point position, Vec2 _speed, int _damage, bool _knockback, int tag) {
	Bullet* pRet = new(std::nothrow) Bullet();
	if (pRet && pRet->init(position, _speed, _damage, _knockback, tag)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool Bullet::init(Point position, Vec2 _speed, int _damage, bool _knockback, int tag) {
	if (!Layer::init()) {
		return false;
	}

	speed = _speed;
	damage = _damage;
	knockback = _knockback;

	int bulletTextureIndex = generateRandomInteger(0, 3);
	bullet = Sprite::create(BULLET_PATHS[bulletTextureIndex]);
	bullet->setPosition(position);
	if (~tag) {
		bullet->setTag(tag);
	}
	this->addChild(bullet);

	this->scheduleUpdate();

	return true;
}

void Bullet::update(float dt) {
	if (bullet) {
		bullet->setPosition(bullet->getPosition() + speed);
	}
}

void Bullet::setBullet(Sprite* _bullet) {
	bullet = _bullet;
}

Sprite* Bullet::getBullet() {
	return bullet;
}

void Bullet::setSpeed(Vec2 _speed) {
	speed = _speed;
}

Vec2 Bullet::getSpeed() {
	return speed;
}

void Bullet::setDamage(int _damage) {
	damage = _damage;
}

int Bullet::getDamage() {
	return damage;
}

void Bullet::setKnockback(bool _knockback) {
	knockback = _knockback;
}

bool Bullet::getKnockback() {
	return knockback;
}

void Bullet::normalizeSpeed(float _speed) {
	assert(speed.getLength() > 0);
	speed *= _speed / speed.getLength();
}

void Bullet::removeBullet() {
	this->unscheduleUpdate();
	this->removeChild(bullet);
}

