#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;
using namespace CocosDenshion;

#include "Config.h"
#include "Joystick.h"
#include <algorithm>
#include <cstring>

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void addBall1(float dt);
	void addBall2(float dt);
	void addBall3(float dt);
	void removeBall(Sprite* ball);
	
	virtual void update(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	// 更新计分板函数
	void modifyScore(int _score);

	// 加载背景函数
	void loadBackground(std::string backgroundPath, int z);

	// 加载面板函数
	void loadBoard(std::string boardPath, int z);

	// 添加键盘监听
	void addKeyboardListener();

	// 键盘监听的回调函数
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

	// 加载提示
	void loadHint(std::string hintPath, int tag, int z);

	// 加载结果面板
	void loadResultBoard(std::string resultBoardPath, int z);

	// 重玩按钮回调函数
	virtual void replayButtonEvent(cocos2d::Ref* pSender);

	// 下一关按钮回调函数
	virtual void nextButtonEvent(cocos2d::Ref* pSender);

	// 展示分数
	void displayScore();

	// 展示文本
	void displayWords(std::string words, int z);

protected:
	Sprite* bean;
	Size visibleSize;
	Joystick* m_joystick;
	Vector<Sprite*> ballVector;
	bool pause;

	int score;

	int total_notes;  // 总 note 数
	int perfect_notes;  // Perfect 的 note 数
	int good_notes;  // Good 的 note 数
	int miss_notes;  // Miss 的 note 数
	int bad_notes;  // Bad 的 note 数

	std::vector<Label*> wordsTips;  // 文本
};

#endif // __HELLOWORLD_SCENE_H__