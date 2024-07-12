#ifndef __LEVEL2_SCENE_H
#define __LEVEL2_SCENE_H

#include "Headers.h"
#include "Config.h"
#include "BasicScene.h"
#include "TiledMap.h"
#include "AStar.h"
#include "Level1Scene.h"

class BasicScene;

class Level2Scene : public BasicScene {
private:
	// 迷宫地图
	TiledMap* tiledMaze;  // 迷宫地图
	std::vector<std::vector<bool>> accessibleMatrix;  // 可通过矩阵
	std::set<std::pair<int, int>> availablePoints;  // 未占用的点
	std::set<std::pair<int, int>> usedAvailablePoints;  // 已占用的未占用的点

	// 可视化路径
	DrawNode* pathDrawer;  // 路径的绘图节点
	float moveSpeed;  // 移动到(八)相邻节点的时间间隔

	// 角色
	Sprite* cattail;  // 猫尾草
	Sprite* walnut;  // 坚果墙

	// item
	Sprite* forbidden;
	std::vector<Sprite*> portals;
	std::vector<Sprite*> skulls;
	std::vector<Sprite*> speedDowns;
	std::vector<Sprite*> speedUps;

	bool canPlace;  // 能否放置坚果墙
	bool walnutInDangerous;  // 坚果墙是否在危险位置

	Label* placeTip;
	Label* findTip;
	Label* noWayTip;
	Label* dangerousTip;

// ---------= 调试用 =---------
	Point touchingPoint;  // 当前触屏的点对应地图上的坐标 (调试用)

public:
	Level2Scene();

	~Level2Scene();

	static Scene* createScene();

	bool init();

	CREATE_FUNC(Level2Scene);

	void update(float dt);

	// 预加载音乐
	void preloadSounds();

	// 键盘监听的回调函数
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

	// 触屏开始的回调函数
	bool onTouchBegan(Touch* touch, Event* event);

	// 触屏结束的回调函数
	void onTouchEnded(Touch* touch, Event* event);

	// 加载绘图节点
	void loadPathDrawer(int z);

	// 加载提示信息
	void loadPlaceTip();
	void loadFindTip();
	void loadNoWayTip();
	void loadDangerousTip();

	// 加载层数信息
	void displayLevel();

	// 加载纯色背景
	void loadPureBackground();

	// 加载迷宫地图
	void loadTiledMaze(int mapWidth, int mapHeight, Point position, int z, std::string mapPath, int mapUnit = 32, int tag = -1);

	// 屏幕坐标转地图坐标
	std::pair<int, int> screenCoordinate2MapCoordinate(Point screenPoint);
	std::pair<int, int> screenCoordinate2MapCoordinate(float screenX, float screenY);

	// 地图坐标转屏幕坐标 (tile 的中心)
	Point mapCoordinate2ScreenCoordinate(std::pair<int, int> mapPoint);
	Point mapCoordinate2ScreenCoordinate(int mapX, int mapY);

	// 加载 Tile (后续不用该精灵)
	void loadTile(std::string path, Point position, int z, bool visible = true, int tag = -1);

	// 加载猫尾草
	void loadCattail(Point position, int z, bool visible = true, int tag = -1);

	// 加载坚果墙
	void loadWalnut(Point position, int z, bool visible = true, int tag = -1);

	// 加载禁止
	void loadForbidden(Point position, int z, bool visible = false, int tag = -1);

	// 加载传送门
	void loadPortal(Point position, int z, bool visible = false, int tag = -1);

	// 加载头颅
	void loadSkull(Point position, int z, bool visible = false, int tag = -1);

	// 加载减速
	void loadSpeedDown(Point position, int z, bool visible = false, int tag = -1);

	// 加载加速
	void loadSpeedUp(Point position, int z, bool visible = false, int tag = -1);

	// 随机取一个可用的点
	std::pair<int, int> getRandomAvailablePoint();
	
	// 占用一个可用的点
	void occupyAvailablePoint(std::pair<int, int> point);

	// 归还一个可用的点
	void returnAvailablePoint(std::pair<int, int> point);

	// 恢复可用的点
	void recoverAvailablePoints();

	// 碰撞检测
	void collisionDetection();
	
	// 随机在未占用的点上生成 item
	void generateRandomItems();
};

#endif