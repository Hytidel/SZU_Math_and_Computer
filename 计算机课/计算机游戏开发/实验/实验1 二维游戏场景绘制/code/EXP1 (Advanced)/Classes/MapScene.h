// 第5章例子
#ifndef __MAPSCENE_H__
#define __MAPSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Config.h"
USING_NS_CC;
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

using namespace cocos2d::ui; 

class MapScene : public cocos2d::Layer
{
public:

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MapScene);

	// 加载背景
	void loadBackground(std::string backgroundPath);

	// 加载皮蛋
	void loadEgg(std::string eggPath, int tag, int x, int y);

	// 加载生米
	void loadGrain(std::string grainPath, int tag, int x, int y);

	// 加载麦克风
	void loadMicrophone(std::string microphonePath, int tag, int x, int y);

	// 加载地图
	void loadMap(std::string mapPath);

	// 显示人物
	void addPlayer(Vec2 pos);

	// 添加触屏监听
	void addTouchListener();
	bool onTouchBegan(Touch *touch, Event *unused_event);

	// 添加键盘监听
	void addKeyboardListener();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

	// 游戏逻辑
	void update(float t);

	void gameOver();
	void success();

	bool m_gameOver; // 游戏是否结束
	bool m_pause;  // 游戏是否暂停
	Dir m_jumpDir;   // 表示精灵的方向 1，up 2 down
	int m_jumpSpeed; // 跳跃速度
	bool m_isJump;   // 表示是否精灵在跳跃，如果跳跃不处理屏幕的触摸
};

#endif // __HELLOWORLD_SCENE_H__
