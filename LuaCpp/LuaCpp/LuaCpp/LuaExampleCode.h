#pragma once
#include <lua.hpp>
#include<iostream>

class LuaExampleCode
{
public:
	LuaExampleCode();
	void Learning();
	void GettingAndSetting(lua_State* lua);
	int CheckInit(lua_State* lua, std::string filePath);
	int pcall(lua_State* stack, const int arguments, const int returns, const int level);
	~LuaExampleCode();
};

struct pos
{
	float x, y, z;
	pos(float x, float y, float z) : x(x), y(y), z(z) {}
};

