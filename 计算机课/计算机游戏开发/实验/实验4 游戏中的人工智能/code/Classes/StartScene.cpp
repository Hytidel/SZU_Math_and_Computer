#include "StartScene.h"

StartScene::StartScene() {
}

StartScene::~StartScene() {
}

Scene* StartScene::createScene() {
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

bool StartScene::init() {
	if (!Layer::init()) {
		return false;
	}

	// ---------= 初始化变量 =---------
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	pause = true;
	// pause = false;

	// ---------= 右下角关闭按钮 =---------
	auto closeItem = MenuItemImage::create(
		CLOSE_BUTTON_PATH,
		CLOSE_BUTTON_SELECTED_PATH,
		CC_CALLBACK_1(StartScene::closeMenuCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));
	auto closeMenu = Menu::create(closeItem, NULL);
	closeMenu->setPosition(Vec2::ZERO);
	this->addChild(closeMenu, 100);

	// ---------= 加载场景 =---------
	loadImage(START_BACKGROUND_PATH, Point(visibleSize.width / 2, visibleSize.height / 2), 0);

	loadPlayButton(Point(visibleSize.width / 2, 100 + 300 / 2), 5);

	// ---------= 添加监听 =---------
	addKeyboardListener();

	// ---------= Update =---------
	this->scheduleUpdate();

	CCLOG("StartScene initialized successfully.");

	return true;
}

void StartScene::update(float dt) {
	if (pause) {
		return;
	}

	playButton->setPositionY(playButton->getPositionY() + PLAY_BUTTON_FLY_SPEED);
	particleSystem->setPosition(Point(playButton->getPositionX(), playButton->getPositionY() - 50));

	// 开始按钮飞出屏幕
	if (doubleCompare(playButton->getPositionY(), visibleSize.height + playButton->getContentSize().height) >= 0) {
		Director::getInstance()->replaceScene(Level1Scene::createScene());
	}
}

void StartScene::loadPlayButton(Point position, int z, int tag) {
	ui::Scale9Sprite* buttonImage = ui::Scale9Sprite::create(PLAY_BUTTON_PATH);
	playButton = ControlButton::create(buttonImage);
	playButton->setBackgroundSpriteForState(buttonImage, Control::State::NORMAL);
	playButton->setAdjustBackgroundImage(false);
	playButton->setPosition(position);
	if (~tag) {
		playButton->setTag(tag);
	}
	this->addChild(playButton, z);

	// 闪烁
	auto actionSequence = Sequence::create(
		FadeOut::create(1),
		FadeIn::create(1),
		nullptr
	);
	auto action = RepeatForever::create(actionSequence);
	playButton->runAction(action);

	// 添加监听
	playButton->addTargetWithActionForControlEvents(
		this, 
		cccontrol_selector(StartScene::playButtonOnClick),
		extension::Control::EventType::TOUCH_DOWN);
}

void StartScene::playButtonOnClick(Ref* pSender, extension::Control::EventType event) {
	loadParticle();

	pause = false;
}

void StartScene::loadParticle() {
	particleSystem = CCParticleSystemQuad::create(PARTICLE_PATH);

	// particleSystem->setDuration(2);  // 持续 2 s
	particleSystem->setAutoRemoveOnFinish(true);  // 完成后自动移除
	this->addChild(particleSystem, 2);
}