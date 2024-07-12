#ifndef __BLOOD_BAR_H__
#define __BLOOD_BAR_H__

#include "Headers.h"
#include "Config.h"

// 血条增长方向
enum BloodBarDirection {
	LEFT,
	RIGHT
};

class BloodBar : public cocos2d::Layer {
private:
	BloodBarDirection bloodBarDirection;
	int maxHP;
	int HP;

	Sprite* bloodBar;
	Sprite* bloodBarBackground;

public:
	BloodBar();

	~BloodBar();

	bool init(int _maxHP, int _HP, Point position, int z, 
		std::string bloodBarPath, std::string bloodBarBackgroundPath, BloodBarDirection _bloodBarDirection = RIGHT, int tag = -1);

	static BloodBar* create(int _maxHP, int _HP, Point position, int z,
		std::string bloodBarPath, std::string bloodBarBackgroundPath, BloodBarDirection _bloodBarDirection = RIGHT, int tag = -1);

	void setBloodBarDirection(BloodBarDirection _bloodBarDirection);

	BloodBarDirection getBloodBarDirection();

	void setMaxHP(int _maxHP);

	int getMaxHP();

	void setHP(int _HP);

	int getHP();
};

#endif