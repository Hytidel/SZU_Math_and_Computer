#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"
#include <cstring>
#include <cmath>
USING_NS_CC;

const double eps = 1e-6;

int cmp(double a, double b);

class Time {
private:
	int min;
	int s;
	int cs;  // 1 s = 100 cs

public:
	Time(int _min = 0, int _s = 0, int _cs = 0);

	~Time();

	// Time 换算成 cs
	int getCs();

	void setTime(int _min, int _s, int _cs);

	// cs 换算成 Time
	static Time getTime(int cs);

	friend Time operator+(Time A, Time B);

	friend Time operator-(Time A, Time B);

	friend bool operator<(Time A, Time B);

	friend bool operator>(Time A, Time B);

	friend bool operator==(Time A, Time B);

	Time operator++(int);
};

class Line {
private:
	Sprite* line;
	Size visibleSize;

public:
	Line();

	~Line();

	void setLine(Sprite* _line);

	Sprite* getLine();

	float getY();

	void updatePosition(float y);
};

const int MAX_TIMER = 20;

class Note {
private:
	Sprite* note;
	Time time;  // 到达判定线的时间, 含预备时间
	Line* line;  // 对应的判定线
	float width;  // note 的宽度
	float height;  // note 的高度
	float lastSpeed;  // 上一次的速度
	bool arrived;  // 是否到达判定线
	bool locked;  // 是否被锁定
	int timer;  // 锁定计时器
	
public:
	Note();

	~Note();

	void setNote(Sprite* _note);

	Sprite* getNote();

	void setTime(Time _time);

	Time getTime();

	void setLine(Line* _line);

	Line* getLine();

	void setWidth(float _width);

	float getWidth();

	void setHeight(float _height);

	float getHeight();

	void setLastSpeed(float _lastSpeed);

	float getLastSpeed();

	void setArrived(bool _arrived);

	bool getArrived();

	void setLocked(bool _locked);

	bool getLocked();

	void setTimer(int _timer);

	void updateTimer();

	Point getPosition();

	// 检查命中状态
	int checkHit();

	// 计算下落速度
	float calculateSpeed(Time currentTime);

	// 下落
	void fall(float y);

	friend bool operator==(Note A, Note B);
};

#endif