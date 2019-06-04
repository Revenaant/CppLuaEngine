#ifndef CARD_HPP
#define CARD_HPP
#include "LuaObject.h"
#include "lua.hpp"

class Card : public LuaObject 
{
public:
	Card();
	Card(std::string pName, std::string pFilename, sf::Vector2f pPosition = sf::Vector2f(0.0f, 0.0f));
	~Card();

	void fixedUpdate(float pStep) override;
	void variableUpdate(float pStep) override;
	void printAss();
	void setMyAss(std::string ass);
	std::string getAss();

	std::string ass;

	static void registerLObject(lua_State* L);

	//static void access();

private:
	static lua_State* _lua;
};

#endif // !CARD_HPP
