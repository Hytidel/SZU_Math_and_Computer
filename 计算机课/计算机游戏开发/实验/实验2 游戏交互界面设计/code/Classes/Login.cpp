#include "Login.h"

USING_NS_CC;

Scene* Login::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Login::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// ¼ÓÔØ±³¾°
void Login::loadBackground(std::string backgroundPath, int z)
{
	auto background = Sprite::create(backgroundPath);
	if (background == nullptr)
	{
		CCLOG("Background loaded failed!");
	}
	else
	{
		background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		this->addChild(background, z);
	}
}

// ¼ÓÔØµÇÂ¼Ãæ°å
void Login::loadLoginBoard(std::string loginBoardPath, int z) {
    auto loginBoard = GUIReader::getInstance()->widgetFromJsonFile(loginBoardPath.c_str());
    auto position = loginBoard->getPosition();
    loginBoard->setPosition(Vec2(position.x + 30, position.y + 80));
    this->addChild(loginBoard);

    // »¬¶¯Ìõ¼àÌý
    auto slider = (Slider*)loginBoard->getChildByTag(301);
    slider->addEventListenerSlider(this, sliderpercentchangedselector(Login::sliderEvent));

    // °´Å¥¼àÌý
    auto button = (Button*)loginBoard->getChildByTag(302);
    button->addTouchEventListener(CC_CALLBACK_1(Login::buttonEvent, this));
}

void Login::preloadSounds() {
    // Ô¤¼ÓÔØÒôÐ§
    SimpleAudioEngine::getInstance()->preloadEffect(SHAOGUANWO_SOUND_PATH.c_str());
}

bool Login::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	// ¼ÓÔØ±³¾°
    loadBackground(BACKGROUND_PATH, 0);

    // ¼ÓÔØµÇÂ¼Ãæ°å
    loadLoginBoard(LOGINBOARD_PATH, 1);
	
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Login::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	this->scheduleUpdate();

    return true;
}

void Login::update(){
    


}

void Login::sliderEvent(cocos2d::Ref* pSender, SliderEventType type) {
    if (type == SLIDER_PERCENTCHANGED) {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        VOLUME = slider->getPercent() / 100.0;
        SimpleAudioEngine::getInstance()->setEffectsVolume(VOLUME);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(VOLUME);
        SimpleAudioEngine::getInstance()->playEffect(SHAOGUANWO_SOUND_PATH.c_str());
    }
}

void Login::buttonEvent(cocos2d::Ref* pSender) {
    level1Scene = Level1::createScene();
    // Director::getInstance()->replaceScene(level1Scene);
    Director::getInstance()->pushScene(level1Scene);
}

void Login::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
