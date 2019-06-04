#include <iostream>
#include <SFML\Graphics.hpp>
#include "LuaCardGame.h"
#include "DebugHud.h"
#include "Config.h"

#include "MouseInputBehaviour.h"
#include "Card.h"
#include "Input.h"

#include <SFML\Audio.hpp>
#include "SoundManager.h"

DebugHud* _dHud;

LuaCardGame::LuaCardGame()
{
}

LuaCardGame::~LuaCardGame()
{
}

int pcall(lua_State* stack, const int arguments, const int returns, const int level)
{
	const int error = lua_pcall(stack, arguments, returns, level);
	if (error != 0)
	{
		const char* errorType;
		switch (error)
		{
		case LUA_OK: errorType = "No Error"; break;
		case LUA_ERRRUN: errorType = "Runtime Error"; break;
		case LUA_ERRMEM: errorType = "Memory Error"; break;
		case LUA_ERRGCMM: errorType = "Garbage Collector Metamethod Error"; break;
		case LUA_ERRERR: errorType = "Error Message Handler Error"; break;
		default: errorType = "Unknown Error"; break;
		}
		std::cout << "Lua " << errorType << " : " << lua_tostring(stack, -1) << std::endl;
	}

	return error;
}

void LuaCardGame::_initializeScene()
{
	std::cout << "Starting Init" << std::endl;

	Input::setWindow(_window);		// I know this is not great.

	std::cout << "Init HUD" << std::endl;
	_dHud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl;
	std::cout << "Init Successful" << "\n\n";

	SoundManager* sm = new SoundManager();
	ServiceLocator::provideService(sm);

	// Register card functions
	LuaObject::registerLObject(_lua);
	Card::registerLObject(_lua);
	Input::registerLib(_lua);
	SoundManager::registerLib(_lua);

	if (luaL_dofile(_lua, (config::LUA_PATH + "Game.lua").c_str()))
		std::cerr << lua_tostring(_lua, -1);

	// Call the start function
	lua_getglobal(_lua, "start");

	//pcall(_lua, 0, 0, 0);
	lua_call(_lua, 0, 0);

	lua_pop(_lua, 1);	// Maybe remove this if shit breaks
	printf("Check stack: %d\n", lua_gettop(_lua));
}

void LuaCardGame::_variableUpdate(float pStep) {
	AbstractGame::_variableUpdate(pStep);

	lua_getglobal(_lua, "update");
	lua_pushnumber(_lua, pStep);
	lua_call(_lua, 1, 0);
}

void LuaCardGame::_render()
{
	AbstractGame::_render();

	// Actual game hud
	//_updateHud();
}

