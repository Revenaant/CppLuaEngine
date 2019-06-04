#ifndef LUACARDGAME_HPP
#define LUACARDGAME_HPP

#include "AbstractGame.h"
#include <typeindex>
#include "World.h"

class LuaCardGame : public AbstractGame
{
public:
	LuaCardGame();
	~LuaCardGame();

	GameObject* newGameObject(const std::string& pName, const sf::Vector2f& pPosition = sf::Vector2f(0.0f, 0.0f));
	GameObject* newGameObject(const std::string& pName, const std::string& pTextureFilename, const sf::Vector2f& pPosition = sf::Vector2f(0.0f, 0.0f));

	template<class T>
	T* newGO(T* pGO) {
		_world->add(pGO);
		return pGO;
	}

	//static int makeNewCard(lua_State* lua);

protected:
	virtual void _initializeScene();

	void _render() override;
	void _variableUpdate(float pStep) override;
};

#endif // !LUACARDGAME_HPP
