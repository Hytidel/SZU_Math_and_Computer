#ifndef __BULLET_H__
#define __BULLET_H__

#include "Headers.h"
#include "Config.h"

class Bullet : public cocos2d::Layer {
private:
	Sprite* bullet;
	Vec2 speed;
	int damage;
	bool knockback;  // ÓÐ»÷ÍËÐ§¹û

public:
	Bullet();

	~Bullet();
	
	static Bullet* create(Point position, Vec2 _speed, int _damage, bool _knockback, int tag = -1);

	bool init(Point position, Vec2 _speed, int _damage, bool _knockback, int tag = -1);

	void update(float dt);

	void setBullet(Sprite* _bullet);

	Sprite* getBullet();

	void setSpeed(Vec2 _speed);

	Vec2 getSpeed();

	void setDamage(int _damage);

	int getDamage();

	void setKnockback(bool _knockback);

	bool getKnockback();

	void normalizeSpeed(float _speed);

	void removeBullet();
};

#endif