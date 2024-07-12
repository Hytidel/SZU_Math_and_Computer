#ifndef __LEVEL1_H__
#define __LEVEL1_H__

#include "HelloWorldScene.h"
#include "Note.h"

class Level1 : public HelloWorld {
private:
	Line hitLine;  // 判定线
	std::vector<Note> notes;
	std::vector<Sprite*> hitEffects;  // 命中特效
	std::vector<Sprite*> hitTips;  // 命中文字
	Time currentTime;
	std::vector<Note> trash;  // 要删除的 notes
	const int DELTA_TIMER;  // 生成 note 的时间间隔
	const Time END_TIME;  // 结束时间
	const int TARGET_SCORE;  // 目标分

public:
	Level1();

	~Level1();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	virtual void update(float dt);

	// implement the "static create()" method manually
	CREATE_FUNC(Level1);

	// 加载判定线
	void loadHitLine(std::string linePath, int y, int z);

	// 加载 Note
	void loadNote(int type, Time time, float x);

	// 新建 Note
	void createNote(float dt);

	// 处理命中
	void workHit(Note& note, int state);

	// 更新 notes
	void updateNotes(Time currentTime);

	// 更新命中
	void updateHits();

	// 预加载音效、音乐
	void preloadSounds();

	// 键盘监听的回调函数
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

	// 显示结果
	void displayResult();

	// 重玩按钮回调函数
	void replayButtonEvent(cocos2d::Ref* pSender);

	// 下一关按钮回调函数
	void nextButtonEvent(cocos2d::Ref* pSender);

	// 更新文本
	void updateWords();
};

#endif