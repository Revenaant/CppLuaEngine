#include <iostream>
#include <algorithm>
#include "World.h"


World::World() : GameObject("root"), Service(), _deathMarkedObjects()
{
	_world = this;
}

void World::registerDeathMarked(GameObject* pObject) {
	_deathMarkedObjects.push_back(pObject);
}

void World::unregisterDeathMarked(GameObject* pObject) {
	if (_deathMarkedObjects.size() == 0) return;
	_deathMarkedObjects.erase(std::remove(_deathMarkedObjects.begin(), _deathMarkedObjects.end(), pObject), _deathMarkedObjects.end());
}

std::vector<GameObject*> World::getDeathMarks() const {
	return _deathMarkedObjects;
}
