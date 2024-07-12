#ifndef __TAUREN_SCENE_H__
#define __TAUREN_SCENE_H__

#include "Headers.h"
#include "Config.h"

class Tauren : public cocos2d::Sprite {
private:
	int maxHP;
	int HP;

	Armature* armature;
	State state;
	bool isRunning;
	bool isAttacking;
	bool isHurt;
	bool isDead;

	bool isDefensing;

public:
	Tauren();
	
	~Tauren();

	void setArmature(Armature* _armature);

	Armature* getArmature();

	int getMaxHP();

	void setHP(int _HP);

	int getHP();

	void setState(State _state);

	State getState();

	void setIsRunning(bool _isRunning);

	bool getIsRunning();

	void setIsAttacking(bool _isAttacking);

	bool getIsAttacking();

	void setIsHurt(bool _isHurt);

	bool getIsHurt();

	void setIsDead(bool _isDead);

	bool getIsDead();

	void setIsDefensing(bool _isDefensing);

	bool getIsDefensing();

	static Tauren* create(int _maxHP, Point position);

	bool init(int _maxHP, Point position);

	void update(float dt);

	// 切换状态
	void switchState(State _state);

	// 帧动画
	void onFrameEvent(cocostudio::Bone* bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	// 受伤
	void playHurt(int bulletDamage = 0);

	// 显示单个扣血 (子弹)
	void displayBloodTip(int basicDamage);

	// 显示多个扣血 (平 A)
	void displayBloodTips(int basicDamage);

	// 扣血数字上移并消失
	void bloodTipFlyEnd(Label* label);

	// 
};

#endif