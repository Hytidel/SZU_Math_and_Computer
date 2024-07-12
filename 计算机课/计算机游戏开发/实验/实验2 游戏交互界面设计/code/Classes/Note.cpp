#include "Note.h"

int cmp(double a, double b) {
	if (fabs(a - b) <= eps) {
		return 0;
	}
	return a < b ? -1 : 1;
}

// ----------

Time::Time(int _min, int _s, int _cs) : min(_min), s(_s), cs(_cs) {
}

Time::~Time() {
}

int Time::getCs() {
	return min * 60 * 100 + s * 100 + cs;
}

void Time::setTime(int _min, int _s, int _cs) {
	min = _min, s = _s, cs = _cs;
}

Time Time::getTime(int cs) {
	return Time(cs / (60 * 100), cs % (60 * 100) / 100, cs % 100);
}

Time operator+(Time A, Time B) {
	int res = A.getCs() + B.getCs();
	return Time::getTime(res);
}

Time operator-(Time A, Time B) {
	int res = A.getCs() - B.getCs();
	assert(res >= 0);
	return Time::getTime(res);
}

bool operator<(Time A, Time B) {
	return A.getCs() < B.getCs();
}

bool operator>(Time A, Time B) {
	return A.getCs() > B.getCs();
}

bool operator==(Time A, Time B) {
	return A.getCs() == B.getCs();
}

Time Time::operator++(int) {
	auto old = *this;
	if (++cs >= 100) {
		s += cs / 100;
		cs %= 100;
	}
	if (s >= 60) {
		min += s / 60;
		s %= 60;
	}
	return old;
}

// ----------

Line::Line() {
}

Line::~Line() {
}

void Line::setLine(Sprite* _line) {
	line = _line;
}

Sprite* Line::getLine() {
	return line;
}

float Line::getY() {
	return line->getPosition().y;
}

void Line::updatePosition(float y) {
	line->setPosition(Point(line->getPosition().x, y));
}

// ----------

Note::Note() : lastSpeed(0), arrived(false) {
}

Note::~Note() {
}

void Note::setNote(Sprite* _note) {
	note = _note;
}

Sprite* Note::getNote() {
	return note;
}

void Note::setTime(Time _time) {
	time = _time;
}

Time Note::getTime() {
	return time;
}

void Note::setLine(Line* _line) {
	line = _line;
}

Line* Note::getLine() {
	return line;
}

void Note::setWidth(float _width) {
	width = _width;
}

float Note::getWidth() {
	return width;
}

void Note::setHeight(float _height) {
	height = _height;
}

float Note::getHeight() {
	return height;
}

void Note::setLastSpeed(float _lastSpeed) {
	lastSpeed = _lastSpeed;
}

float Note::getLastSpeed() {
	return lastSpeed;
}

void Note::setArrived(bool _arrived) {
	arrived = _arrived;
}

bool Note::getArrived() {
	return arrived;
}

void Note::setLocked(bool _locked) {
	locked = _locked;
}

bool Note::getLocked() {
	return locked;
}

void Note::setTimer(int _timer) {
	timer = _timer;
}

void Note::updateTimer() {
	if (!locked) {
		return;
	}

	if (++timer >= MAX_TIMER) {
		timer = 0;
		locked = false;
	}
}

Point Note::getPosition() {
	return note->getPosition();
}

// 0 为 Miss, 1 为 Perfect, 2 为 Good, 3 为 Bad
int Note::checkHit() {
	// dis > 0 时 note 在判定线上方
	float dis = getPosition().y - line->getY();

	if (cmp(std::fabs(dis), height / 2) <= 0) {  // Perfect
		return 1;
	}
	else if (cmp(std::fabs(dis), height) <= 0) {  // Good
		return 2;
	}
	else if (dis > 0 && dis > height) {  // Bad
		return 3;
	}
	else {  // Miss
		return 0;
	}
	// assert(false);
}

float Note::calculateSpeed(Time currentTime) {
	int timeDifference = (time - currentTime).getCs();
	float yDifference = getPosition().y - line->getY();
	return yDifference / timeDifference;
}

void Note::fall(float dy) {
	note->setPosition(Point(getPosition().x, getPosition().y - dy));
}

bool operator==(Note A, Note B) {
	return A.note == B.note;
}




