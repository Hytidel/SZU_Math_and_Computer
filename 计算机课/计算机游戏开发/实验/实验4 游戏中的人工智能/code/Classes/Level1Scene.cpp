#include "Level1Scene.h"

Level1Scene::Level1Scene() {
}

Level1Scene::~Level1Scene() {
}

Scene* Level1Scene::createScene() {
	auto scene = Scene::create();
	auto layer = Level1Scene::create();
	scene->addChild(layer);
	return scene;
}

bool Level1Scene::init() {
	if (!Layer::init()) {
		return false;
	}

// ---------= 初始化变量 =---------
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	// pause = true;
	pause = false;

// ---------= 右下角关闭按钮 =---------
	auto closeItem = MenuItemImage::create(
		CLOSE_BUTTON_PATH,
		CLOSE_BUTTON_SELECTED_PATH,
		CC_CALLBACK_1(Level1Scene::closeMenuCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));
	auto closeMenu = Menu::create(closeItem, NULL);
	closeMenu->setPosition(Vec2::ZERO);
	this->addChild(closeMenu, 100);

// ---------= 加载场景 =---------
	// 加载纯色背景
	loadPureBackground();

	// 加载迷宫地图
	loadTiledMaze(MAP_WIDTH, MAP_HEIGHT, Point(visibleSize.width / 2, visibleSize.height - MAP_HEIGHT * MAP_UNIT / 2), 0, TILED_MAZE_1_PATH);  // 画面中上方

	// 加载绘图节点
	loadPathDrawer(5);

	// 加载猫尾草
	auto cattailPoint = getRandomAvailablePoint();
	Point cattailPosition = mapCoordinate2ScreenCoordinate(cattailPoint);
	loadCattail(cattailPosition, 3);
	occupyAvailablePoint(cattailPoint);

	// 加载坚果墙
	loadWalnut(Point(0, 2 * visibleSize.height), 3, false);

	// 加载禁止
	loadForbidden(Point(0, 0), 5, false);

	// 加载提示信息
	loadPlaceTip();
	loadFindTip();
	loadNoWayTip();
	loadDangerousTip();

	// 加载层数信息
	displayLevel();

	// 随机生成 item
	generateRandomItems();

	// 初始化变量
	// touchingPoint = Point(0, 0);
	canPlace = true;
	walnutInDangerous = false;

// ---------= 音乐与音效 =---------
	preloadSounds();
	playBackgroundMusic(BGM_PATH);

// ---------= 添加监听 =---------
	addKeyboardListener();  // 添加键盘监听
	addTouchListener();  // 添加触屏监听

// ---------= Update =---------
	this->scheduleUpdate();

	CCLOG("Level1Scene initialized successfully.");

	return true;
}

void Level1Scene::update(float dt) {
	if (pause) {
		return;
	}

	collisionDetection();


// ---------= 调试用 =---------
	// CCLOG(("(" + std::to_string(touchingPoint.x) + ", " + std::to_string(touchingPoint.y) + ")").c_str());

	/* // 输出迷宫的可通过矩阵的转置
	auto accessibleMatrix = tiledMaze->getAccessibleMatrix();
	std::vector<std::string> accessibleText(MAP_WIDTH);
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			accessibleText[i] += std::to_string(accessibleMatrix[i][j]) + " ";
		}
		CCLOG(accessibleText[i].c_str());
	}
	CCLOG("");*/

	/* // 输出猫尾草到坚果墙的最短路的长度和路径的点数
	auto accessibleMatrix = tiledMaze->getAccessibleMatrix();
	std::pair<int, int> st = screenCoordinate2MapCoordinate(cattail->getPosition());
	std::pair<int, int> ed = screenCoordinate2MapCoordinate(walnut->getPosition());
	AStar solver(MAP_WIDTH, MAP_HEIGHT, accessibleMatrix, st, ed);
	CCLOG((std::to_string(solver.getMinDistance())).c_str());
	CCLOG(("    " + std::to_string(solver.getPath().size())).c_str());*/
}

// 预加载音乐
void Level1Scene::preloadSounds() {
	preloadBackgroundMusic(BGM_PATH);
	preloadSoundEffect(FIND_SOUND_PATH);
	preloadSoundEffect(DANGEROUS_SOUND_PATH);
	preloadSoundEffect(SPEED_UP_SOUND_PATH);
	preloadSoundEffect(SPEED_DOWN_SOUND_PATH);
	preloadSoundEffect(VICTORY_SOUND_PATH);
}

// 键盘监听的回调函数
void Level1Scene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_ESCAPE: {  // ESC: 关闭
			Director::getInstance()->end();
			break;
		}
		case EventKeyboard::KeyCode::KEY_R: {  // R: 重玩
			Director::getInstance()->replaceScene(Level1Scene::createScene());
			break;
		}
		case EventKeyboard::KeyCode::KEY_SPACE: {  // 空格: 
			pause = false;

			// 更新 bgm 状态等
			break;
		}
		case EventKeyboard::KeyCode::KEY_P: {  // P: 暂停
			pause ^= 1;

			// 更新 bgm 状态等
			break;
		}
		case EventKeyboard::KeyCode::KEY_F: {  // F: 刷新猫尾草位置
			// 归还旧点
			auto cattailPoint = screenCoordinate2MapCoordinate(cattail->getPosition());
			returnAvailablePoint(cattailPoint);

			cattailPoint = getRandomAvailablePoint();
			Point cattailPosition = mapCoordinate2ScreenCoordinate(cattailPoint);
			cattail->setPosition(cattailPosition);
			occupyAvailablePoint(cattailPoint);
			
			break;
		}
		case EventKeyboard::KeyCode::KEY_G: {  // G: 刷新 item 位置
			// 传送门
			for (auto portal : portals) {
				// 归还旧点
				auto portalPoint = screenCoordinate2MapCoordinate(portal->getPosition());
				returnAvailablePoint(portalPoint);

				portalPoint = getRandomAvailablePoint();
				Point portalPosition = mapCoordinate2ScreenCoordinate(portalPoint);
				portal->setPosition(portalPosition);
				occupyAvailablePoint(portalPoint);
			}

			// 头颅
			for (auto skull : skulls) {
				// 归还旧点
				auto skullPoint = screenCoordinate2MapCoordinate(skull->getPosition());
				returnAvailablePoint(skullPoint);

				skullPoint = getRandomAvailablePoint();
				Point skullPosition = mapCoordinate2ScreenCoordinate(skullPoint);
				skull->setPosition(skullPosition);
				occupyAvailablePoint(skullPoint);
			}

			// todo: 减速、加速

			break;
		}
	}
}

// 触屏开始的回调函数
bool Level1Scene::onTouchBegan(Touch* touch, Event* event) {
	// 屏幕坐标转地图坐标
	auto [touchX, touchY] = screenCoordinate2MapCoordinate(touch->getLocation());
	// touchingPoint = Point(touchX, touchY);

	if (!canPlace || touchX < 0 || touchX >= MAP_WIDTH || touchY < 0 || touchY >= MAP_HEIGHT || !accessibleMatrix[touchX][touchY]) {  // 不可放置或越界或不可到达
		forbidden->setPosition(mapCoordinate2ScreenCoordinate(touchX, touchY));
		forbidden->setVisible(true);
	}
	else if (screenCoordinate2MapCoordinate(cattail->getPosition()) == std::pair<int, int>(touchX, touchY)) {  // 坚果墙与猫尾草重合
		forbidden->setPosition(mapCoordinate2ScreenCoordinate(touchX, touchY));
		forbidden->setVisible(true);
	}
	else {  // 成功放置
		// 隐藏提示信息
		placeTip->setVisible(false);
		findTip->setVisible(false);
		noWayTip->setVisible(false);
		dangerousTip->setVisible(false);

		// 放置坚果墙
		canPlace = false;
		walnutInDangerous = false;
		walnut->setPosition(mapCoordinate2ScreenCoordinate(touchX, touchY));
		walnut->setVisible(true);

		// 寻路
		std::pair<int, int> st = screenCoordinate2MapCoordinate(cattail->getPosition());
		std::pair<int, int> ed = screenCoordinate2MapCoordinate(walnut->getPosition());
		AStar solver(MAP_WIDTH, MAP_HEIGHT, accessibleMatrix, st, ed);
		
		if (doubleCompare(solver.getMinDistance(), INF / 2) >= 0) {  // 最短距离 >= INF / 2, 不连通
			placeTip->setVisible(false);
			noWayTip->setVisible(true);

			canPlace = true;
		}
		else {  // 连通, 显示路径
			Vector<FiniteTimeAction*> actionVector;  // 动作序列

			auto path = solver.getPath();
			auto color = COLORS[generateRandomInteger(0, (int)COLORS.size() - 1)];
			for (int i = 1; i < path.size(); i++) {
				auto currentPoint = mapCoordinate2ScreenCoordinate(path[i]);
				pathDrawer->drawLine(mapCoordinate2ScreenCoordinate(path[i - 1]), currentPoint, color);

				auto moveAction = MoveTo::create(moveSpeed, currentPoint);
				actionVector.pushBack(moveAction);
			}

			if (actionVector.empty()) {
				return true;
			}

			auto actionSequence = Sequence::create(actionVector);
			cattail->runAction(actionSequence);
		}
	}

	return true;
}

// 触屏结束的回调函数
void Level1Scene::onTouchEnded(Touch* touch, Event* event) {
	forbidden->setVisible(false);

	// canPlace = true;
}

// 加载绘图节点
void Level1Scene::loadPathDrawer(int z) {
	pathDrawer = DrawNode::create();
	this->addChild(pathDrawer, z);

	moveSpeed = 0.1;
}

void Level1Scene::loadPlaceTip() {
	placeTip = Label::createWithBMFont(TIPS_FNT_PATH, PLACE_TIP);
	placeTip->setPosition(Point(
		placeTip->getContentSize().width / 2 + 20,
		(visibleSize.height - MAP_HEIGHT * MAP_UNIT) / 2
	));
	placeTip->setColor(Color3B::YELLOW);

	auto actionSequence = Sequence::create(
		FadeOut::create(1),
		FadeIn::create(1),
		nullptr
	);
	auto action = RepeatForever::create(actionSequence);
	placeTip->runAction(action);

	this->addChild(placeTip, 5);
}

void Level1Scene::loadFindTip() {
	findTip = Label::createWithBMFont(TIPS_FNT_PATH, FIND_TIP);
	findTip->setPosition(Point(
		findTip->getContentSize().width / 2 + 20,
		(visibleSize.height - MAP_HEIGHT * MAP_UNIT) / 2
	));
	findTip->setColor(Color3B::GREEN);
	findTip->setVisible(false);

	this->addChild(findTip, 5);
}

void Level1Scene::loadNoWayTip() {
	noWayTip = Label::createWithBMFont(TIPS_FNT_PATH, NO_WAY_TIP);
	noWayTip->setPosition(Point(
		noWayTip->getContentSize().width / 2 + 20,
		(visibleSize.height - MAP_HEIGHT * MAP_UNIT) / 2
	));
	noWayTip->setColor(Color3B::BLUE);
	noWayTip->setVisible(false);

	this->addChild(noWayTip, 5);
}

void Level1Scene::loadDangerousTip() {
	dangerousTip = Label::createWithBMFont(TIPS_FNT_PATH, DANGEROUS_TIP);
	dangerousTip->setPosition(Point(
		dangerousTip->getContentSize().width / 2 + 20,
		(visibleSize.height - MAP_HEIGHT * MAP_UNIT) / 2
	));
	dangerousTip->setColor(Color3B::RED);
	dangerousTip->setVisible(false);

	this->addChild(dangerousTip, 5);
}

// 加载层数信息
void Level1Scene::displayLevel() {
	auto levelTip = Label::createWithBMFont(TIPS_FNT_PATH, "Level 1");
	levelTip->setPosition(Point(
		visibleSize.width - levelTip->getContentSize().width / 2 - 20,
		(visibleSize.height - MAP_HEIGHT * MAP_UNIT) / 2
	));
	levelTip->setColor(Color3B::BLACK);
	this->addChild(levelTip);
}

// 加载纯色背景
void Level1Scene::loadPureBackground() {
	auto pureLayer = LayerColor::create(Color4B(222, 241, 254, 255), visibleSize.width, visibleSize.height);
	pureLayer->setPosition(Point(0, 0));
	this->addChild(pureLayer, -1);
}

// 加载迷宫地图
void Level1Scene::loadTiledMaze(int mapWidth, int mapHeight, Point position, int z, std::string mapPath, int mapUnit, int tag) {
	tiledMaze = TiledMap::create(mapWidth, mapHeight, mapPath, mapUnit, tag);
	tiledMaze->setPosition(position);
	if (~tag) {
		tiledMaze->setTag(tag);
	}
	this->addChild(tiledMaze, z);

	// 预处理可用点
	accessibleMatrix = tiledMaze->getAccessibleMatrix();
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			if (accessibleMatrix[i][j]) {
				availablePoints.insert({ i, j });
			}
		}
	}
}

// 屏幕坐标转地图坐标
std::pair<int, int> Level1Scene::screenCoordinate2MapCoordinate(Point screenPoint) {
	return screenCoordinate2MapCoordinate(screenPoint.x, screenPoint.y);
}

// 屏幕坐标转地图坐标
std::pair<int, int> Level1Scene::screenCoordinate2MapCoordinate(float screenX, float screenY) {
	int mapX = screenX / MAP_UNIT;
	int mapY = (visibleSize.height - screenY) / MAP_UNIT;
	return std::pair<int, int>(mapX, mapY);
}

// 地图坐标转屏幕坐标 (tile 的中心)
Point Level1Scene::mapCoordinate2ScreenCoordinate(std::pair<int, int> mapPoint) {
	return mapCoordinate2ScreenCoordinate(mapPoint.first, mapPoint.second);
}

// 地图坐标转屏幕坐标 (tile 的中心)
Point Level1Scene::mapCoordinate2ScreenCoordinate(int mapX, int mapY) {
	float screenX = mapX * MAP_UNIT + 0.5 * MAP_UNIT;
	float screenY = visibleSize.height - mapY * MAP_UNIT - 0.5 * MAP_UNIT;
	return Point(screenX, screenY);
}

// 加载 Tile (后续不用该精灵)
void Level1Scene::loadTile(std::string path, Point position, int z, bool visible, int tag) {
	auto sprite = Sprite::create(path);
	sprite->setPosition(position);
	sprite->setVisible(visible);
	if (~tag) {
		sprite->setTag(tag);
	}
	this->addChild(sprite, z);
}

// 加载猫尾草
void Level1Scene::loadCattail(Point position, int z, bool visible, int tag) {
	cattail = Sprite::create(CATTAIL_TILE_PATH);
	cattail->setPosition(position);
	cattail->setVisible(visible);
	if (~tag) {
		cattail->setTag(tag);
	}
	this->addChild(cattail, z);
}

// 加载坚果墙
void Level1Scene::loadWalnut(Point position, int z, bool visible, int tag) {
	walnut = Sprite::create(WALNUT_TILE_PATH);
	walnut->setPosition(position);
	walnut->setVisible(visible);
	if (~tag) {
		walnut->setTag(tag);
	}
	this->addChild(walnut, z);
}

// 加载禁止
void Level1Scene::loadForbidden(Point position, int z, bool visible, int tag) {
	forbidden = Sprite::create(FORBIDDEN_TILE_PATH);
	forbidden->setPosition(position);
	forbidden->setVisible(visible);
	if (~tag) {
		forbidden->setTag(tag);
	}
	this->addChild(forbidden, z);
}

// 加载传送门
void Level1Scene::loadPortal(Point position, int z, bool visible, int tag) {
	auto portal = Sprite::create(PORTAL_TILE_PATH);
	portal->setPosition(position);
	portal->setVisible(visible);
	if (~tag) {
		portal->setTag(tag);
	}
	this->addChild(portal, z);

	portals.push_back(portal);
}

// 加载头颅
void Level1Scene::loadSkull(Point position, int z, bool visible, int tag) {
	auto skull = Sprite::create(SKULL_TILE_PATH);
	skull->setPosition(position);
	skull->setVisible(visible);
	if (~tag) {
		skull->setTag(tag);
	}
	this->addChild(skull, z);

	skulls.push_back(skull);
}

// 加载减速
void Level1Scene::loadSpeedDown(Point position, int z, bool visible, int tag) {
	auto speedDown = Sprite::create(SPEED_DOWN_TILE_PATH);
	speedDown->setPosition(position);
	speedDown->setVisible(visible);
	if (~tag) {
		speedDown->setTag(tag);
	}
	this->addChild(speedDown, z);

	speedDowns.push_back(speedDown);
}

// 加载加速
void Level1Scene::loadSpeedUp(Point position, int z, bool visible, int tag) {
	auto speedUp = Sprite::create(SPEED_UP_TILE_PATH);
	speedUp->setPosition(position);
	speedUp->setVisible(visible);
	if (~tag) {
		speedUp->setTag(tag);
	}
	this->addChild(speedUp, z);

	speedUps.push_back(speedUp);
}

// 随机取一个可用的点
std::pair<int, int> Level1Scene::getRandomAvailablePoint() {
	std::vector<std::pair<int, int>> candidates;
	for (auto availablePoint : availablePoints) {
		candidates.push_back(availablePoint);
	}

	return candidates[generateRandomInteger(0, (int)candidates.size() - 1)];
}

// 占用一个可用的点
void Level1Scene::occupyAvailablePoint(std::pair<int, int> point) {
	availablePoints.erase(point);
	usedAvailablePoints.insert(point);
}

// 归还一个可用的点
void Level1Scene::returnAvailablePoint(std::pair<int, int> point) {
	usedAvailablePoints.erase(point);
	availablePoints.insert(point);
}

// 恢复可用的点
void Level1Scene::recoverAvailablePoints() {
	for (auto availablePoint : usedAvailablePoints) {
		availablePoints.insert(availablePoint);
	}
	usedAvailablePoints.clear();
}

// 碰撞检测
void Level1Scene::collisionDetection() {
	// 猫尾草与头颅 (先判断危险)
	for (auto skull : skulls) {
		if (screenCoordinate2MapCoordinate(cattail->getPosition()) == screenCoordinate2MapCoordinate(skull->getPosition())) {
			dangerousTip->setVisible(true);

			cattail->stopAllActions();

			canPlace = false;

			playSoundEffect(DANGEROUS_SOUND_PATH);
		}

		// 坚果墙在头颅位置
		if (screenCoordinate2MapCoordinate(skull->getPosition()) == screenCoordinate2MapCoordinate(walnut->getPosition())) {
			walnutInDangerous = true;
			return;
		}
	}

	// 猫尾草与坚果墙
	if (!walnutInDangerous && screenCoordinate2MapCoordinate(cattail->getPosition()) == screenCoordinate2MapCoordinate(walnut->getPosition())) {
		findTip->setVisible(true);

		walnut->setPosition(Point(0, 2 * visibleSize.height));
		walnut->setVisible(false);

		canPlace = true;

		playSoundEffect(FIND_SOUND_PATH);
	}

	// 猫尾草与传送门
	for (auto portal : portals) {
		if (screenCoordinate2MapCoordinate(cattail->getPosition()) == screenCoordinate2MapCoordinate(portal->getPosition())) {
			Director::getInstance()->replaceScene(Level2Scene::createScene());
		}
	}

	/* // 猫尾草与减速
	for (auto speedDown : speedDowns) {
		if (screenCoordinate2MapCoordinate(cattail->getPosition()) == screenCoordinate2MapCoordinate(speedDown->getPosition())) {

		}
	}

	// 猫尾草与加速
	for (auto speedUp : speedUps) {
		if (screenCoordinate2MapCoordinate(cattail->getPosition()) == screenCoordinate2MapCoordinate(speedUp->getPosition())) {

		}
	}*/
}

// 随机在未占用的点上生成 item
void Level1Scene::generateRandomItems() {
	std::vector<std::pair<int, int>> candidates;
	for (auto availablePoint : availablePoints) {
		candidates.push_back(availablePoint);
	}
	shuffle(candidates.begin(), candidates.end(), rnd);

	assert(candidates.size());

	// 生成随机数量的 item , 至少 1 个
	int cnt = generateRandomInteger(1, std::max((int)(candidates.size() * 0.1) - 1, 1));
	for (int i = 0; i < cnt; i++) {
		Point position = mapCoordinate2ScreenCoordinate(candidates[i]);

		// 随机生成一种 item
		// int randomItemIndex = generateRandomInteger(1, 4);
		int randomItemIndex = generateRandomInteger(1, 2);  // 目前只支持传送门和头颅
		if (randomItemIndex == 1) {
			loadPortal(position, 3, true);
		}
		else if (randomItemIndex == 2) {
			loadSkull(position, 3, true);
		}
		else if (randomItemIndex == 3) {
			loadSpeedDown(position, 3, true);
		}
		else {
			loadSpeedUp(position, 3, true);
		}

		occupyAvailablePoint(candidates[i]);
	}
}



