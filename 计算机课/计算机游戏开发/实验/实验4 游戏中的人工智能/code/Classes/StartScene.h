#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "Headers.h"
#include "Config.h"
#include "BasicScene.h"
#include "Level1Scene.h"

class BasicScene;

class StartScene : public BasicScene {
private:
	ControlButton* playButton;
	CCParticleSystemQuad* particleSystem;  // Á£×ÓÏµÍ³

public:
	StartScene();

	~StartScene();

	static Scene* createScene();

	bool init();

	CREATE_FUNC(StartScene);

	void update(float dt);

	void loadPlayButton(Point position, int z, int tag = -1);

	void playButtonOnClick(Ref* pSender, extension::Control::EventType event);

	void loadParticle();
};

#endif