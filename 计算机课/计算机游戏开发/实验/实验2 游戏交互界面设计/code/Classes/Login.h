#ifndef __LOGIN_H__
#define __LOGIN_H__

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
#include "HelloWorldScene.h"
#include "Level1.h"
#include <cstring>

class Login : public cocos2d::Layer {
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	
	virtual void update();
    // implement the "static create()" method manually
    CREATE_FUNC(Login);

	// 加载背景函数
	void loadBackground(std::string backgroundPath, int z);

    // 加载登录面板
    void loadLoginBoard(std::string loginBoardPath, int z);

    // 滑动条事件回调函数
    void sliderEvent(cocos2d::Ref* pSender, SliderEventType type);

    // 预加载音效、音乐
    void preloadSounds();

    // 按钮事件回调函数
    void buttonEvent(cocos2d::Ref* pSender);

protected:
    Size visibleSize;
};

#endif