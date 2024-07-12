#include "BasicScene.h"

// ---------= ×ÓÀàÐèÓÐ/ÖØÐ´ =---------

BasicScene::BasicScene() {
}

BasicScene::~BasicScene() {
}

Scene* BasicScene::createScene() {
	auto scene = Scene::create();
	auto layer = BasicScene::create();
	scene->addChild(layer);
	return scene;
}

bool BasicScene::init() {
	if (!Layer::init()) {
		return false;
	}

// ---------= ³õÊ¼»¯±äÁ¿ =---------
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	pause = true;

// ---------= ÓÒÏÂ½Ç¹Ø±Õ°´Å¥ =---------
	auto closeItem = MenuItemImage::create(
		CLOSE_BUTTON_PATH,
		CLOSE_BUTTON_SELECTED_PATH,
		CC_CALLBACK_1(BasicScene::closeMenuCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));
	auto closeMenu = Menu::create(closeItem, NULL);
	closeMenu->setPosition(Vec2::ZERO);
	this->addChild(closeMenu, 100);

// ---------= ¼ÓÔØ³¡¾° =---------
	

// ---------= Ìí¼Ó¼àÌý =---------
	addKeyboardListener();

// ---------= Update =---------
	this->scheduleUpdate();

	CCLOG("BasicScene initialized successfully.");

	return true;
}

void BasicScene::update(float dt) {
	if (pause) {
		return;
	}


}

// Ô¤¼ÓÔØÒôÀÖ
void BasicScene::preloadSounds() {
}

// ¼üÅÌ¼àÌýµÄ»Øµ÷º¯Êý
void BasicScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_ESCAPE: {  // ESC: ¹Ø±Õ
			Director::getInstance()->end();
			break;
		}
		case EventKeyboard::KeyCode::KEY_SPACE: {  // ¿Õ¸ñ: 
			pause = false;

			// ¸üÐÂ bgm ×´Ì¬µÈ
			break;
		}
		case EventKeyboard::KeyCode::KEY_P: {  // P: ÔÝÍ£
			pause ^= 1;

			// ¸üÐÂ bgm ×´Ì¬µÈ
			break;
		}
	}
}

// ÓÎÏ·Ê¤Àû
void BasicScene::workVictory() {
}

// ÓÎÏ·Ê§°Ü
void BasicScene::workDefeat() {
}

// ÓÎÏ·Ê¤¸ºÅÐ¶¨
void BasicScene::gameJudgement() {	
}

// ---------= ×ÓÀàÎÞÐèÖØÐ´ =---------

void BasicScene::closeMenuCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

// Ìí¼Ó¼üÅÌ¼àÌý
void BasicScene::addKeyboardListener() {
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(BasicScene::onKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

// ¼ÓÔØÍ¼Æ¬
void BasicScene::loadImage(std::string imagePath, Point position, int z, int tag) {
	auto image = Sprite::create(imagePath);
	if (image) {
		image->setPosition(position);
		if (~tag) {
			image->setTag(tag);
		}
		this->addChild(image, z);
	}
	else {
		CCLOG(("Image \"" + imagePath + "\" loaded failed.").c_str());
	}
}

// Ô¤¼ÓÔØÒôÐ§
void BasicScene::preloadSoundEffect(std::string soundEffectPath) {
	SimpleAudioEngine::getInstance()->preloadEffect(soundEffectPath.c_str());
}

// Ô¤¼ÓÔØ±³¾°ÒôÀÖ
void BasicScene::preloadBackgroundMusic(std::string backgroundMusicPath) {
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(backgroundMusicPath.c_str());
}

// ²¥·ÅÒôÐ§
void BasicScene::playSoundEffect(std::string soundEffectPath) {
	SimpleAudioEngine::getInstance()->playEffect(soundEffectPath.c_str());
}

// ²¥·Å±³¾°ÒôÀÖ
void BasicScene::playBackgroundMusic(std::string backgroundMusicPath) {
	SimpleAudioEngine::getInstance()->playBackgroundMusic(backgroundMusicPath.c_str());
}

// ÔÝÍ£ÒôÐ§
void BasicScene::pauseSoundEffect() {
	SimpleAudioEngine::getInstance()->pauseAllEffects();
}

// ÔÝÍ£±³¾°ÒôÀÖ
void BasicScene::pauseBackgroundMusic() {
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// »Ö¸´ÒôÐ§
void BasicScene::resumeSoundEffect() {
	SimpleAudioEngine::getInstance()->resumeAllEffects();
}

// »Ö¸´±³¾°ÒôÀÖ
void BasicScene::resumeBackgroundMusic() {
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

// Í£Ö¹ÒôÐ§
void BasicScene::stopSoundEffect() {
	SimpleAudioEngine::getInstance()->stopAllEffects();
}

// Í£Ö¹±³¾°ÒôÀÖ
void BasicScene::stopBackgroundMusic() {
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

// ÏÔÊ¾ÌáÊ¾ÐÅÏ¢
void BasicScene::displayWords(std::string words, Point position, int z, int tag) {
	auto wordsTip = Label::createWithBMFont(TIPS_FNT_PATH, words);
	if (wordsTip) {
		wordsTip->setPosition(position);
		if (~tag) {
			wordsTip->setTag(tag);
		}
		this->addChild(wordsTip, z);
	}
	else {
		CCLOG(("Font \"" + TIPS_FNT_PATH + "\" loaded failed.").c_str());
	}
}

// ¼ÓÔØ¼üÎ»ÌáÊ¾
void BasicScene::loadKeyboardHint(std::string hintWords, int z, int tag) {
	auto keyboardHint = Label::createWithBMFont(TIPS_FNT_PATH, hintWords);
	if (keyboardHint) {
		keyboardHint->setPosition(Point(visibleSize.width / 2, 2 * keyboardHint->getContentSize().height));
		keyboardHint->setScale(0.7);
		keyboardHint->setColor(Color3B::MAGENTA);
		if (~tag) {
			keyboardHint->setTag(tag);
		}

		auto actionSequence = Sequence::create(
			FadeOut::create(1),
			FadeIn::create(1),
			nullptr
		);
		auto action = RepeatForever::create(actionSequence);
		keyboardHint->runAction(action);

		this->addChild(keyboardHint, z);
	}
	else {
		CCLOG(("Font \"" + TIPS_FNT_PATH + "\" loaded failed.").c_str());
	}
}

