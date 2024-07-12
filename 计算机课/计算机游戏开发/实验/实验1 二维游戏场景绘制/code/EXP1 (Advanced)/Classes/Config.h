#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <vector>
using namespace std;
USING_NS_CC;

static const int PLAYER_TAG = 110;  // player tag
static const int MAP_TAG = 111;     // map tag

static int PLAYER_WIDTH = 80;		// player img width
static int PLAYER_HEIGHT = 100;	    // player img height
static int PLAYER_JUMP_INITIAL_SPEED = 2;  // 玩家的初始竖直速度
static int PLAYER_JUMP_MAX_SPEED = 30;  // 玩家的最大竖直速度
// static int PLAYER_MAP_MOVE_SPEED = 3;  // 玩家和地图的移动速度
static int ITEM_WIDTH = 32;  // 道具宽度
static int ITEM_HEIGHT = 32;  // 道具高度
static float EPS = 1;  // 碰撞
static int EFFECT_TIME = 75;  // 道具作用时间

static string BACKGROUND_FILE_PATH = "whale.png";  // 背景;
static string MAP_TMX_FILE_PATH = "Chapter05/beach (empty).tmx";  // 地图 (不含道具)
static string MAP_BG_LAYER_NAME = "Layer 1";  // map background layer name
static string EGG_FILE_PATH = "egg.png";  // 皮蛋
static string GRAIN_FILE_PATH = "grain.png";  // 生米
static string MICROPHONE_FILE_PATH = "microphone.png";  // 麦

// static int MAP_FLOUR_NUM = 6;
// static int MAP_FLOUR[] = { 7,8,151,170,171,172 };
// static vector<int> MAP_FLOUR = { 7, 8, 151, 170, 171, 172 };  // 优化实现
// static vector<int> MAP_FLOUR = { 7, 8, 151, 170, 171, 172, 149, 133 };  // 修复树枝检测 Bug
// Tield (0-indexed): 0, 1, 18, 19, 20, 24, 25, 26, 30, 31, 32, 21, 22, 23, 27, 28, 29, 33, 34, 35
static vector<int> MAP_FLOUR = { 1, 2, 19, 20, 21, 25, 26, 27, 31, 32, 33, 22, 23, 24, 28, 29, 30, 34, 35, 36, 8, 14 };  // 地面 tile
static vector<int> MAP_BLOCK;  // 阻挡 tile
static vector<int> MAP_THORN;  // 尖刺 tile
static vector<int> MAP_ENDPOINT = { 11, 17 };  // 终点

static string GAME_OVER_TIPS = "Game Over!";  // game failed tips words
static string GAME_SUCCESS = "Success!";      // game success tips words

static string PICKUP_SOUND_EFFECT = "pick up.mp3";  // 捡起道具音效
static string SUCCESS_SOUND_EFFECT = "success.mp3";  // 胜利音效
static string GAMEOVER_SOUND_EFFECT = "game over.mp3";  // 失败音效

//	fnt file path
static string TIPS_FNT_PATH = "fonts/futura-48.fnt";

// player img path
static string PLAYER_IMG_PATH[4] = {
	"Chapter05/player/player1.png",
	"Chapter05/player/player2.png",
	"Chapter05/player/player3.png",
	"Chapter05/player/player4.png"
} ;

enum Dir
{
	STOP,
	UP,
	DOWN
};

// 皮蛋
static vector<int> EGG_TAGS = {
	200,  // 皮蛋 0
	201,  // 皮蛋 1
	202,  // 皮蛋 2

};
static vector<Vec2> EGG_POSITIONS = {
	{ 19, 23 },  // 皮蛋 0
	{ 39, 23 },  // 皮蛋 1
	{ 61, 19 },  // 皮蛋 2
};

// 生米
static vector<int> GRAIN_TAGS = {
	300,  // 生米 0
	301,  // 生米 1
	302,  // 生米 2
};
static vector<Vec2> GRAIN_POSITIONS = {
	{ 8, 23 }, // 生米 0
	{ 53, 18 },  // 生米 1
	{ 69, 19 },  // 生米 2
};

// 麦克风
static vector<int> MICROPHONE_TAGS = {
	400,  // 麦克风 0
	401,  // 麦克风 1
};
static vector<Vec2> MICROPHONE_POSITIONS = {
	{ 23, 22 }, // 麦克风 0
	{ 46, 21 },  // 麦克风 1
};

// 判定 tile 是否是地面
static bool isFloor(int gid)
{
	for (auto floorGID : MAP_FLOUR)
	{
		if (gid == floorGID)
		{
			return true;
		}
	}

	return false;
}

// 判定 tile 是否是阻挡
static bool isBlock(int gid)
{
	for (auto blockGID : MAP_BLOCK)
	{
		if (gid == blockGID)
		{
			return true;
		}
	}
	for (auto floorGID : MAP_FLOUR)
	{
		if (gid == floorGID)
		{
			return true;
		}
	}

	return false;
}

// 判定 tile 是否是尖刺
static bool isThorn(int gid)
{
	return false;

	for (auto thornGID : MAP_THORN)
	{
		if (gid == thornGID)
		{
			return true;
		}
	}

	return false;
}

// 判定 tile 是否是终点
static bool isEndpoint(int gid)
{
	for (auto endpointGID : MAP_ENDPOINT)
	{
		if (gid == endpointGID)
		{
			return true;
		}
	}

	return false;
}

#endif