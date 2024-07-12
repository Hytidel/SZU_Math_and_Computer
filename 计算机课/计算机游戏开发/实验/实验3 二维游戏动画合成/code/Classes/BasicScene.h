#ifndef __BASIC_SCENE_H__
#define __BASIC_SCENE_H__

#include "Headers.h"
#include "Config.h"

class BasicScene;

// 场景类基类
class BasicScene : public cocos2d::Layer {
protected:
	Size visibleSize;
	Point origin;
	bool pause;

public:
	BasicScene();

	~BasicScene();

	static cocos2d::Scene* createScene();

	virtual bool init();

	// 子类需有
	CREATE_FUNC(BasicScene);

	virtual void update(float dt);

	void closeMenuCallback(cocos2d::Ref* pSender);

	// 添加键盘监听
	void addKeyboardListener();

	// 键盘监听的回调函数
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

	// 加载图片
	void loadImage(std::string imagePath, Point position, int z, int tag = -1);

	// 预加载音效
	void preloadSoundEffect(std::string soundEffectPath);

	// 预加载背景音乐
	void preloadBackgroundMusic(std::string backgroundMusicPath);
	
	// 预加载音乐
	virtual void preloadSounds();

	// 播放音效
	void playSoundEffect(std::string soundEffectPath);

	// 播放背景音乐
	void playBackgroundMusic(std::string backgroundMusicPath);

	// 暂停音效
	void pauseSoundEffect();

	// 暂停背景音乐
	void pauseBackgroundMusic();

	// 恢复音效
	void resumeSoundEffect();

	// 恢复背景音乐
	void resumeBackgroundMusic();

	// 停止音效
	void stopSoundEffect();

	// 停止背景音乐
	void stopBackgroundMusic();

	// 显示提示信息
	void displayWords(std::string words, Point position, int z, int tag = -1);

	// 游戏胜利
	virtual void workVictory();

	// 游戏失败
	virtual void workDefeat();

	// 游戏胜负判定
	virtual void gameJudgement();

	// 加载键位提示
	void loadKeyboardHint(std::string hintWords, int z, int tag = -1);
};

#endif