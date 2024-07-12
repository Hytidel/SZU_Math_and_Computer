#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::modifyScore(int _score) {
	// 清除旧计分板
	auto trash = this->getChildByTag(SCOREBOARD_TAG);
	if (trash) {
		this->removeChildByTag(SCOREBOARD_TAG);
	}

	score = _score;
	auto scoreboard = Label::createWithBMFont(TIPS_FNT_PATH, std::to_string(score));
	scoreboard->setPosition(Vec2(100, visibleSize.height / 2));
	scoreboard->setTag(SCOREBOARD_TAG);
	this->addChild(scoreboard, 1);
}

// 加载背景
void HelloWorld::loadBackground(std::string backgroundPath, int z)
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

// 加载面板
void HelloWorld::loadBoard(std::string boardPath, int z)
{
	auto board = Sprite::create(boardPath);
	if (board == nullptr)
	{
		CCLOG("Board loaded failed!");
	}
	else
	{
		board->setPosition(Vec2(BOARD_WIDTH_BIAS + board->getContentSize().width / 2, visibleSize.height / 2));
		this->addChild(board, z);
	}
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 添加颜色层
	// auto colorLayer = LayerColor::create(Color4B(128, 125, 200, 255), BOARD_WIDTH, BOARD_HEIGHT);
	// colorLayer->setPosition(Vec2(BOARD_WIDTH_BIAS, 0));
	// this->addChild(colorLayer);
	
	// 加载背景
	loadBackground(BACKGROUND_PATH, 0);
	// 加载面板
	loadBoard(BOARD1_PATH, 1);
	
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	// 移动精灵
	bean = Sprite::create("Chapter10/bean.png");
	bean->setPosition(Point(visibleSize.width/2, 100));
	this->addChild(bean, 6);

	// 摇杆
	m_joystick = Joystick::create(Vec2(100, 100), 63.0f, "Chapter10/j-btn.png", "Chapter10/j-bg.png");
	this->addChild(m_joystick, 4);
    
	// 随机掉落 3 种球
	this->schedule(schedule_selector(HelloWorld::addBall1), 1.0f);
	this->schedule(schedule_selector(HelloWorld::addBall2), 2.0f);
	this->schedule(schedule_selector(HelloWorld::addBall3), 3.0f);
	this->scheduleUpdate();

	// 计分板
	modifyScore(0);

	pause = false;

	// 添加键盘监听
	addKeyboardListener();

    return true;
}

void HelloWorld::addBall1(float dt){
	auto ball1 = Sprite::create(BALL1_PATH);  // 使用图片创建小球
	int width = ball1->getContentSize().width;
	ball1->setPosition(Point(
		rand() % (BOARD_WIDTH - width + 1) + BOARD_WIDTH_BIAS + width / 2, 
		BOARD_HEIGHT));// 设置小球的初始位置，这里在x方向使用了随机函数rand使得小球在随机位置出现
	ball1->setTag(BALL1_TAG);
	this->addChild(ball1, 5);

	this->ballVector.pushBack(ball1);  // 将小球放进 Vector 数组

	auto moveTo = MoveTo::create(rand() % 5 + 1, Point(ball1->getPositionX(), -10));  // 移动动作
	auto actionDone = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball1));  // 当小球移动到屏幕下方时回调removeBall函数，移除小球
	auto sequence = Sequence::create(moveTo, actionDone, nullptr);
	ball1->runAction(sequence);  // 执行动作
}

void HelloWorld::addBall2(float dt){
	auto ball2 = Sprite::create(BALL2_PATH);
	int width = ball2->getContentSize().width;
	ball2->setPosition(Point(
		rand() % (BOARD_WIDTH - width + 1) + BOARD_WIDTH_BIAS + width / 2,
		BOARD_HEIGHT));// 设置小球的初始位置，这里在x方向使用了随机函数rand使得小球在随机位置出现
	ball2->setTag(BALL2_TAG);
	this->addChild(ball2, 5);

	this->ballVector.pushBack(ball2);

	auto moveTo = MoveTo::create(rand() % 5 + 1, Point(ball2->getPositionX(), -10));
	auto actionDone = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball2));
	auto sequence = Sequence::create(moveTo, actionDone, nullptr);
	ball2->runAction(sequence);
}

void HelloWorld::addBall3(float dt){
	auto ball3 = Sprite::create(BALL3_PATH);
	int width = ball3->getContentSize().width;
	ball3->setPosition(Point(
		rand() % (BOARD_WIDTH - width + 1) + BOARD_WIDTH_BIAS + width / 2,
		BOARD_HEIGHT));// 设置小球的初始位置，这里在x方向使用了随机函数rand使得小球在随机位置出现
	ball3->setTag(BALL3_TAG);
	this->addChild(ball3, 5);

	this->ballVector.pushBack(ball3);

	auto moveTo = MoveTo::create(rand() % 5 + 1, Point(ball3->getPositionX(), -10));
	auto actionDone = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball3));
	auto sequence = Sequence::create(moveTo, actionDone, nullptr);
	ball3->runAction(sequence);
}

void HelloWorld::removeBall(Sprite* ball){
	this->removeChild(ball);
	ballVector.eraseObject(ball);
}

void HelloWorld::update(float dt){
	for (auto ball:ballVector)
	{
		//进行碰撞检测
		if (bean->getBoundingBox().intersectsRect(ball->getBoundingBox()))
		{
			// 得分
			int tag = ball->getTag();
			modifyScore(score + tag - BALL_TAG_BIAS);

			// 删除球
			auto actionDown = CallFunc::create(CC_CALLBACK_0(HelloWorld::removeBall, this, ball));
			ball->runAction(actionDown);
		}
	}
	
	// 控制贪食豆移动
	auto direction = m_joystick->getDirection();  // 摇杆移动方向
	auto destination = bean->getPosition() + direction * m_joystick->getVelocity() * 0.15;  // 预期目标点
	auto beanSize = bean->getContentSize();  // 贪食豆大小
	destination.x = std::max(destination.x, BOARD_WIDTH_BIAS + beanSize.width / 2),
		destination.x = std::min(destination.x, BOARD_WIDTH_BIAS + BOARD_WIDTH - beanSize.width / 2);
	destination.y = std::max(destination.y, 0 + beanSize.height / 2),
		destination.y = std::min(destination.y, BOARD_HEIGHT - beanSize.height / 2);
	bean->setPosition(destination);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
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

void HelloWorld::addKeyboardListener()
{
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_ESCAPE:  // ESC: 关闭
		{
			Director::getInstance()->end();
			break;
		}
		case EventKeyboard::KeyCode::KEY_SPACE:  // 空格: 跳跃
		{
			pause = false;

			break;
		}
		case EventKeyboard::KeyCode::KEY_P:  // P: 暂停
		{
			pause ^= 1;
			break;
		}
	}
}

void HelloWorld::loadHint(std::string hintPath, int tag, int z) {
	auto hint = Sprite::create(hintPath);
	if (hint == nullptr) {
		CCLOG("Hint loaded failed!");
	}
	else {
		hint->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
		hint->setTag(tag);
		this->addChild(hint, z);
	}
}

void HelloWorld::loadResultBoard(std::string resultBoardPath, int z) {
	auto resultBoard = GUIReader::getInstance()->widgetFromJsonFile(resultBoardPath.c_str());
	auto position = resultBoard->getPosition();
	resultBoard->setPosition(Vec2(position.x + 30, position.y + 80));
	this->addChild(resultBoard, z);

	// 重玩按钮监听
	auto replayButton = (Button*)resultBoard->getChildByName("Replay_Button");
	replayButton->addTouchEventListener(CC_CALLBACK_1(HelloWorld::replayButtonEvent, this));

	// 下一关按钮监听
	auto nextButton = (Button*)resultBoard->getChildByName("Next_Button");
	nextButton->addTouchEventListener(CC_CALLBACK_1(HelloWorld::nextButtonEvent, this));
}

void HelloWorld::replayButtonEvent(cocos2d::Ref* pSender) {
}

void HelloWorld::nextButtonEvent(cocos2d::Ref* pSender) {
}

void HelloWorld::displayScore() {
	std::string accuracy = std::to_string(perfect_notes + good_notes) + " / " + std::to_string(perfect_notes + good_notes + miss_notes);
	auto accuracyTip = Label::createWithBMFont(TIPS_FNT_PATH, accuracy);
	accuracyTip->setPosition(Point(visibleSize.width / 2 + 200, 475));
	accuracyTip->setTag(ACCURACY_TAG);
	this->addChild(accuracyTip, 11);

	auto perfectTip = Label::createWithBMFont(TIPS_FNT_PATH, std::to_string(perfect_notes));
	perfectTip->setPosition(Point(visibleSize.width / 2 + 200, 370));
	perfectTip->setTag(PERFECT_TAG);
	this->addChild(perfectTip, 11);

	auto goodTip = Label::createWithBMFont(TIPS_FNT_PATH, std::to_string(good_notes));
	goodTip->setPosition(Point(visibleSize.width / 2 + 200, 305));
	goodTip->setTag(GOOD_TAG);
	this->addChild(goodTip, 11);

	auto missTip = Label::createWithBMFont(TIPS_FNT_PATH, std::to_string(miss_notes));
	missTip->setPosition(Point(visibleSize.width / 2 + 200, 235));
	missTip->setTag(MISS_TAG);
	this->addChild(missTip, 11);

	auto badTip = Label::createWithBMFont(TIPS_FNT_PATH, std::to_string(bad_notes));
	badTip->setPosition(Point(visibleSize.width / 2 + 200, 170));
	badTip->setTag(BAD_TAG);
	this->addChild(badTip, 11);
}

void HelloWorld::displayWords(std::string words, int z) {
	auto wordsTip = Label::createWithBMFont(TIPS_FNT_PATH, words);
	wordsTip->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	wordsTip->setScale(2);
	wordsTip->setTag(WORDS_TAG);
	this->addChild(wordsTip, z);

	wordsTips.push_back(wordsTip);
}