#include "TiledMap.h"

TiledMap::TiledMap() {

}

TiledMap::~TiledMap() {

}

bool TiledMap::init(int _mapWidth, int _mapHeight, std::string mapPath, int mapUnit, int tag) {
	mapWidth = _mapWidth;
	mapHeight = _mapHeight;

	auto tiledMap = TMXTiledMap::create(mapPath);
	// Size mapSize = tiledMap->getContentSize();
	tiledMap->setAnchorPoint(Point(0.5, 0.5));
	this->addChild(tiledMap);

	wallLayer = tiledMap->getLayer("Wall");

	transformIntoBooleanMatrix();

	return true;
}

TiledMap* TiledMap::create(int _mapWidth, int _mapHeight, std::string mapPath, int mapUnit, int tag) {
	TiledMap* pRet = new(std::nothrow) TiledMap();
	if (pRet && pRet->init(_mapWidth, _mapHeight, mapPath, mapUnit, tag)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void TiledMap::setTiledMap(TMXTiledMap* _tiledMap) {
	tiledMap = _tiledMap;
}

TMXTiledMap* TiledMap::getTiledMap() {
	return tiledMap;
}

int TiledMap::getMapUnit() {
	return mapUnit;
}

void TiledMap::setMapUnit(int _mapUnit) {
	mapUnit = _mapUnit;
}

int TiledMap::getMapWidth() {
	return mapWidth;
}

void TiledMap::setMapWidth(int _mapWidth) {
	mapWidth = _mapWidth;
}

int TiledMap::getMapHeight() {
	return mapHeight;
}

void TiledMap::setMapHeight(int _mapHeight) {
	mapHeight = _mapHeight;
}

void TiledMap::setAccessibleMatrix(std::vector<std::vector<bool>> _accessibleMatrix) {
	accessibleMatrix = _accessibleMatrix;
}

std::vector<std::vector<bool>> TiledMap::getAccessibleMatrix() {
	return accessibleMatrix;
}

// 将迷宫地图转化为是否可通行的 bool 矩阵 (0-indexed)
void TiledMap::transformIntoBooleanMatrix() {
	accessibleMatrix = std::vector<std::vector<bool>>(mapWidth, std::vector<bool>(mapHeight));
	for (int i = 0; i < mapWidth; i++) {
		for (int j = 0; j < mapHeight; j++) {
			int gid = wallLayer->getTileGIDAt(Point(i, j));
			accessibleMatrix[i][j] = (std::find(INACCESSIBLE_TILES.begin(), INACCESSIBLE_TILES.end(), gid) == INACCESSIBLE_TILES.end());
		}
	}
}



