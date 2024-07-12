#include "CollisionDetection.h"

CollisionDetection::CollisionDetection() {
}

CollisionDetection::~CollisionDetection() {
}

CollisionDetection* CollisionDetection::create(Node* parent, Hero* _hero, Tauren* _tauren, TaurenAI* _taurenAI) {
	CollisionDetection* pRet = new(std::nothrow) CollisionDetection();
	if (pRet && pRet->init(parent, _hero, _tauren, _taurenAI)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool CollisionDetection::init(Node* parent, Hero* _hero, Tauren* _tauren, TaurenAI* _taurenAI) {
	if (!Node::init()) {
		return false;
	}

	hero = _hero;
	tauren = _tauren;
	taurenAI = _taurenAI;
	parent->addChild(this);

	this->scheduleUpdate();

	return true;
}

void CollisionDetection::update(float dt) {
	if (canHeroAttackTauren()) {
		// Hero 攻击 Tauren
		Point heroPoint1 = hero->getArmature()->getBone("Layer17")->getDisplayRenderNode()->convertToWorldSpaceAR(Point(0, 0));
		Point heroPoint2 = hero->getArmature()->getBone("Layer17")->getDisplayRenderNode()->convertToWorldSpaceAR(Point(0, 15));
		Point heroPoint3 = hero->getArmature()->getBone("Layer17")->getDisplayRenderNode()->convertToWorldSpaceAR(Point(0, 30));
		Point heroPoint4 = hero->getArmature()->getBone("Layer17")->getDisplayRenderNode()->convertToWorldSpaceAR(Point(0, 50));
		Point heroPoint5 = hero->getArmature()->getBone("Layer17")->getDisplayRenderNode()->convertToWorldSpaceAR(Point(0, 70));  // 新增关键点
		std::vector<Point> heroPoints = { heroPoint1, heroPoint2, heroPoint3, heroPoint4, heroPoint5 };

		// Hero 朝左时需镜像关键点
		/*if (!doubleCompare(hero->getScaleX(), -1)) {
			for (auto& heroPoint : heroPoints) {
				float distance = std::abs(heroPoint.x - hero->getPositionX());
				heroPoint.x -= 2 * distance;
			}
		}*/

		// CCLOG((std::to_string(heroPoints[4].x) + ", " + std::to_string(heroPoints[4].y)).c_str());

		// CCLOG((std::to_string(hero->getPositionX()) + " " + std::to_string(hero->getPositionX())).c_str());

		if (!tauren->getIsDead() && hero->getIsAttacking()) {
			// Rect taurenRect = Rect(tauren->getPositionX(), tauren->getPositionY() - 40, 20, 40);
			Rect taurenRect = Rect(tauren->getPositionX() - 20, tauren->getPositionY() - 40, 20, 40);
			for (auto& heroPoint : heroPoints) {
				if (taurenRect.containsPoint(heroPoint)) {
					tauren->playHurt();
					hero->setIsAttacking(false);
					break;
			}
				}
		}
	}
	
	if (canTaurenAttackHero()) {
		// Tauren 攻击 Hero
		Point taurenPoint1 = tauren->getArmature()->getBone("ax")->getDisplayRenderNode()->convertToWorldSpaceAR(Point(0, 0));
		Point taurenPoint2 = tauren->getArmature()->getBone("ax")->getDisplayRenderNode()->convertToWorldSpaceAR(Point(0, 30));
		Point taurenPoint3 = tauren->getArmature()->getBone("ax")->getDisplayRenderNode()->convertToWorldSpaceAR(Point(0, 70));  // 新增关键点
		std::vector<Point> taurenPoints = { taurenPoint1, taurenPoint2, taurenPoint3 };

		if (!hero->getIsDead() && tauren->getIsAttacking()) {
			Rect heroRect = Rect(hero->getPositionX() + 10, hero->getPositionY() + 10, 20, 40);
			for (auto& taurenPoint : taurenPoints) {
				if (heroRect.containsPoint(taurenPoint)) {
					hero->playHurt();
					tauren->setIsAttacking(false);
					// taurenAI->setTaurenState(State::STAND);  // ?
					break;
				}
			}
		}
	}
}

void CollisionDetection::setHero(Hero* _hero) {
	hero = _hero;
}

Hero* CollisionDetection::getHero() {
	return hero;
}

void CollisionDetection::setTauren(Tauren* _tauren) {
	tauren = _tauren;
}

Tauren* CollisionDetection::getTauren() {
	return tauren;
}

// 判断 Hero 能否攻击到 Tauren
bool CollisionDetection::canHeroAttackTauren() {
	return true;
	/*return (!doubleCompare(hero->getScaleX(), 1) && doubleCompare(hero->getPositionX(), tauren->getPositionX()) <= 0)
		|| (!doubleCompare(hero->getScaleX(), -1) && doubleCompare(hero->getPositionX(), tauren->getPositionX()) >= 0);
		// 后者有误
	*/
}

// 判断 Tauren 能否攻击到 Hero
bool CollisionDetection::canTaurenAttackHero() {
	return true;
	/*return (!doubleCompare(tauren->getScaleX(), 1) && doubleCompare(tauren->getPositionX(), hero->getPositionX()) >= 0)
		|| (!doubleCompare(tauren->getScaleX(), -1) && doubleCompare(tauren->getPositionX(), hero->getPositionX()) <= 0);
		// 后者有误
	*/
}

