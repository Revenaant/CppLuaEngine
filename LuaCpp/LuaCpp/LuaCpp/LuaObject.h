#ifndef LUAOBJECT_HPP
#define LUAOBJECT_HPP
#include "GameObject.h"
#include "lua.hpp"

class LuaObject : public GameObject
{
public:
	LuaObject(const std::string& pName, const std::string& pTextureFilename, const sf::Vector2f& pPosition = sf::Vector2f(0.0f, 0.0f));
	LuaObject(const std::string& pName, const sf::Vector2f& pPosition = sf::Vector2f(0.0f, 0.0f));
	~LuaObject();

	static void registerLObject(lua_State* L);
	static const luaL_Reg* getLib();
};

#endif // !LUAOBJECT_HPP
