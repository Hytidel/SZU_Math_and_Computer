#ifndef __END_SCENE_H__
#define __END_SCENE_H__

#include "Headers.h"
#include "Config.h"
#include "BasicScene.h"

class BasicScene;

class EndScene : public BasicScene {
private:
	Sprite* story;

	float storyPercent;  // 剧情显示百分比

public:
	EndScene();

	~EndScene();

	static Scene* createScene();

	bool init();

	CREATE_FUNC(EndScene);

	void update(float dt);

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
};

#endif