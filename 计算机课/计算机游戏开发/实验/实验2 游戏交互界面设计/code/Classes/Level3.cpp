#include "Level3.h"
#include "Login.h"

USING_NS_CC;

Level3::Level3() : currentTime(0, 0, 0), DELTA_TIMER(100), END_TIME(Time(0, 15, 0)), TARGET_SCORE(10),
	MIN_LINE_Y(100), MAX_LINE_Y(400), MOVE_TIMER(200) {
}

Level3::~Level3() {
}

Scene* Level3::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Level3::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void Level3::loadHitLine(std::string linePath, int y, int z) {
	auto line = Sprite::create(linePath);
	hitLine.setLine(line);
	line->setPosition(Point(visibleSize.width / 2, y));
	this->addChild(line, z);
}

// 加载 note : 种类、时间、初始 x 坐标
// type = -1 时为随机类型, time = (0, 0, 0) 时为随机时间, x = -1 时为随机初始 x 坐标
void Level3::loadNote(int type, Time time, float x) {
	if (type == -1) {
		type = generateRandomInteger(1, 3);
	}

	// 根据 note 的种类得到路径和 tag
	std::string notePath;
	int tag;
	if (type == 1) {
		notePath = NOTE1_PATH;
		tag = NOTE1_TAG;
	}
	else if (type == 2) {
		notePath = NOTE2_PATH;
		tag = NOTE2_TAG;
	}
	else {
		notePath = NOTE3_PATH;
		tag = NOTE3_TAG;
	}

	// 新建 note
	Note newNote;
	auto note = Sprite::create(notePath);
	newNote.setNote(note);

	if (time == Time(0, 0, 0)) {
		time.setTime(0, generateRandomInteger(1, 5), 0);
	}

	newNote.setTime(time);
	newNote.setLine(&hitLine);
	newNote.setWidth(note->getContentSize().width);
	newNote.setHeight(note->getContentSize().height);
	newNote.setLastSpeed(generateRandomInteger(3, 5));
	newNote.setArrived(false);
	newNote.setLocked(false);
	newNote.setTimer(0);

	if (!cmp(x, -1)) {
		x = generateRandomInteger(
			std::floor(BOARD_WIDTH_BIAS + newNote.getWidth() / 2),
			std::ceil(BOARD_WIDTH_BIAS + BOARD_WIDTH - newNote.getWidth() / 2)
		);
	}

	// 将新 note 加入场景
	note->setPosition(Point(x, visibleSize.height));
	this->addChild(note, 5);

	// 将新 note 加入场景 notes 中
	notes.push_back(newNote);
}

void Level3::createNote(float dt) {
	total_notes++;
	loadNote(-1, Time(0, 0, 0), -1);
}

void Level3::preloadSounds() {
	// 预加载音效
	SimpleAudioEngine::getInstance()->preloadEffect(HIT_SOUND_PATH.c_str());
	
	// 预加载音乐
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MIRAGE_PATH.c_str());
}

bool Level3::init() {
	if (!Layer::init()) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 加载背景
	loadBackground(BACKGROUND_PATH, 0);
	// 加载面板
	loadBoard(BOARD3_PATH, 1);
	// 加载判定线
	loadHitLine(LINE_PATH, 200, 2);

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// 移动精灵
	bean = Sprite::create("Chapter10/bean.png");
	bean->setPosition(Point(visibleSize.width / 2, 100));
	this->addChild(bean, 6);

	// 摇杆
	m_joystick = Joystick::create(Vec2(100, 100), 63.0f, "Chapter10/j-btn.png", "Chapter10/j-bg.png");
	this->addChild(m_joystick, 4);

	this->scheduleUpdate();

	// loadNote(-1, Time(0, 1, 0), -1);
	// loadNote(-1, Time(0, 2, 0), -1);
	// loadNote(-1, Time(0, 3, 0), -1);

	// 计分板
	modifyScore(0);

	// 背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic(MIRAGE_PATH.c_str());
	
	// 暂停游戏
	pause = true;
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

	// 添加键盘监听
	addKeyboardListener();

	// 加载提示信息
	loadHint(LEVEL3_HINT_PATH, LEVEL3_HINT_TAG, 10);

	// note 计数器
	total_notes = 0;
	perfect_notes = 0;
	good_notes = 0;
	miss_notes = 0;
	bad_notes = 0;

	return true;
}

// 0 为 Miss, 1 为 Perfect, 2 为 Good, 3 为 Bad
void Level3::workHit(Note& note, int state) {
	Point position = note.getPosition();
	note.setLocked(true);
	
	switch (state) {
		case 0: {  // Miss
			// 文字
			auto miss = Sprite::create(MISS_PATH);
			miss->setPosition(position);
			this->addChild(miss, 8);
			hitTips.push_back(miss);

			// 计数
			miss_notes++;

			trash.push_back(note);
			break;
		}

		case 1: {  // Perfect
			// 文字
			auto perfect = Sprite::create(PERFECT_PATH);
			perfect->setPosition(position);
			this->addChild(perfect, 8);
			hitTips.push_back(perfect);

			// 效果
			auto hit = Sprite::create(HIT_PATH);
			hit->setPosition(position);
			this->addChild(hit, 7);
			hitEffects.push_back(hit);

			// 音效
			SimpleAudioEngine::getInstance()->playEffect(HIT_SOUND_PATH.c_str());

			// 得分
			modifyScore(score + 2);

			// 计数
			perfect_notes++;

			trash.push_back(note);

			break;
		}

		case 2: {  // Good
			// 文字
			auto good = Sprite::create(GOOD_PATH);
			good->setPosition(position);
			this->addChild(good, 8);
			hitTips.push_back(good);

			// 效果
			auto hit = Sprite::create(HIT_PATH);
			hit->setPosition(position);
			this->addChild(hit, 7);
			hitEffects.push_back(hit);

			// 音效
			SimpleAudioEngine::getInstance()->playEffect(HIT_SOUND_PATH.c_str());

			// 得分
			modifyScore(score + 1);

			// 计数
			good_notes++;

			trash.push_back(note);
			break;
		}

		case 3: {  // Bad
			// 文字
			auto bad = Sprite::create(BAD_PATH);
			bad->setPosition(position);
			this->addChild(bad, 8);
			hitTips.push_back(bad);

			// 得分
			modifyScore(score - 1);

			// 计数
			bad_notes++;

			break;
		}

		default:
			assert(false);
	}
}

void Level3::updateWords() {
	std::vector<Label*> trash;
	for (auto& wordTip : wordsTips) {
		auto opacity = wordTip->getOpacity();
		wordTip->setOpacity(opacity - DELTA_OPACITY);
		if (wordTip->getOpacity() <= 50) {
			trash.push_back(wordTip);
		}
	}

	for (auto& wordTip : trash) {
		this->removeChild(wordTip);
		wordsTips.erase(find(wordsTips.begin(), wordsTips.end(), wordTip));
	}
}

void Level3::updateNotes(Time currentTime) {
	for (auto& note : notes) {
		// note 已到判定线
		if (currentTime > note.getTime() || currentTime == note.getTime()) {
			note.setArrived(true);
			float dis = note.getLine()->getY() - note.getPosition().y;
			if (cmp(dis, note.getHeight()) > 0) {  // Miss
				workHit(note, 0);
				continue;
			}
		}

		// note 到判定线后以恒定速度下落
		float speed = note.getLastSpeed();
		if (!note.getArrived()) {
			speed = note.calculateSpeed(currentTime);
		}
		note.fall(speed);
		note.setLastSpeed(speed);

		note.updateTimer();
	}

	// 删除已 Miss 的 notes
	for (auto& note : trash) {
		this->removeChild(note.getNote());
		auto it = find(notes.begin(), notes.end(), note);
		auto idx = it - notes.begin();
		if (idx != -1) {
			notes.erase(it);
		}
	}
	trash.clear();
}

void Level3::updateHits() {
	// 命中特效
	std::vector<Sprite*> trashEffects;
	for (auto& effect : hitEffects) {
		float scale = effect->getScale();
		effect->setScale(scale + DELTA_SCALE);
		float angle = effect->getRotation();
		effect->setRotation(angle + DELTA_ANGLE);
		float opacity = effect->getOpacity();
		effect->setOpacity(opacity - DELTA_OPACITY);

		if (effect->getOpacity() < 75) {
			trashEffects.push_back(effect);
		}
	}
	for (auto& effect : trashEffects) {
		this->removeChild(effect);
		hitEffects.erase(find(hitEffects.begin(), hitEffects.end(), effect));
	}

	// 命中文字
	std::vector<Sprite*> trashTips;
	for (auto& tip : hitTips) {
		Point position = tip->getPosition();
		tip->setPosition(Point(position.x, position.y + DELTA_POSITION_Y));
		float opacity = tip->getOpacity();
		tip->setOpacity(opacity - DELTA_OPACITY);

		if (tip->getOpacity() < 75) {
			trashTips.push_back(tip);
		}
	}
	for (auto& tip : trashTips) {
		this->removeChild(tip);
		hitTips.erase(find(hitTips.begin(), hitTips.end(), tip));
	}
}

void Level3::displayResult() {
	loadResultBoard(LEVEL3_RESULT_PATH, 10);
	displayScore();
}

void Level3::update(float dt) {
	updateWords();

	if (pause) {
		return;
	}

	// 碰撞检测
	for (auto& note : notes) {
		if (note.getLocked()) {
			continue;
		}

		if (note.getNote()->getBoundingBox().intersectsRect(bean->getBoundingBox())) {
			int state = note.checkHit();
			workHit(note, state);
		}
	}

	// 随机移动判定线 (先)
	if (currentTime.getCs() % DELTA_TIMER == 0) {
		int direction = generateRandomInteger(0, 1);
		int distance = generateRandomInteger(5, 20);
		float targetY = hitLine.getY() + (direction ? distance : -distance);
		targetY = std::max(targetY, (float)MIN_LINE_Y);
		targetY = std::min(targetY, (float)MAX_LINE_Y);
		hitLine.updatePosition(targetY);
	}

	// 更新 notes (后)
	updateNotes(currentTime);
	updateHits();

	// 控制贪食豆移动
	auto direction = m_joystick->getDirection();  // 摇杆移动方向
	auto destination = bean->getPosition() + direction * m_joystick->getVelocity() * 0.15;  // 预期目标点
	auto beanSize = bean->getContentSize();  // 贪食豆大小
	destination.x = std::max(destination.x, BOARD_WIDTH_BIAS + beanSize.width / 2),
		destination.x = std::min(destination.x, BOARD_WIDTH_BIAS + BOARD_WIDTH - beanSize.width / 2);
	destination.y = std::max(destination.y, 0 + beanSize.height / 2),
		destination.y = std::min(destination.y, BOARD_HEIGHT - beanSize.height / 2);
	bean->setPosition(destination);

	currentTime++;

	// 生成 note
	if (currentTime.getCs() % DELTA_TIMER == 0) {
		createNote(0.1f);
	}

	// 胜负判定
	if (score >= TARGET_SCORE || currentTime > END_TIME) {
		pause = true;
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		displayResult();

		if (score >= TARGET_SCORE) {
			if (perfect_notes == perfect_notes + good_notes + miss_notes) {
				displayWords("ALL PERFECT!", 20);
			}
			else {
				displayWords("SUCCESS!", 20);
			}
		}
		else {
			displayWords("GAME OVER!", 20);
		}
	}
}

void Level3::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_ESCAPE: {  // ESC: 关闭
			Director::getInstance()->end();
			break;
		}
		case EventKeyboard::KeyCode::KEY_SPACE: {  // 空格: 开始游戏
			this->removeChildByTag(LEVEL3_HINT_TAG);
			pause = false;
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			break;
		}
		case EventKeyboard::KeyCode::KEY_P: {  // P: 暂停
			pause ^= 1;
			if (pause) {
				// 背景音乐
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			}
			else {
				// 背景音乐
				SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_K: {  // K: 下一关
			Director::getInstance()->popScene();
			loginScene = Login::createScene();
			Director::getInstance()->pushScene(loginScene);
			break;
		}
	}
}

void Level3::replayButtonEvent(cocos2d::Ref* pSender) {
	Director::getInstance()->popScene();
	level3Scene = Level3::createScene();
	Director::getInstance()->pushScene(level3Scene);
}

void Level3::nextButtonEvent(cocos2d::Ref* pSender) {
	Director::getInstance()->popScene();
	loginScene = Login::createScene();
	Director::getInstance()->pushScene(loginScene);
}