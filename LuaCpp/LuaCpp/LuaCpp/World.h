#ifndef WORLD_HPP
#define WORLD_HPP
#include "GameObject.h"
#include "ServiceLocator.h"

class World : public GameObject, public Service
{
public:
	World();

	void registerDeathMarked(GameObject* pObject);
	void unregisterDeathMarked(GameObject* pObject);
	std::vector<GameObject*> getDeathMarks() const;

	//std::vector<Collider*> colliders;

	bool gameDone = false;

private:
	std::vector<GameObject*> _deathMarkedObjects;

	World(const World&);
	World& operator=(const World&);
};


#endif // !WORLD_HPP
