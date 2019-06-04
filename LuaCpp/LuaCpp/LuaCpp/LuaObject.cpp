#include <iostream>
#include "LuaObject.h"
#include "Config.h"
#include "ServiceLocator.h"
#include "World.h"
#include "Input.h"

LuaObject::LuaObject(const std::string & pName, const std::string & pTextureFilename, const sf::Vector2f & pPosition) : GameObject(pName, pTextureFilename, pPosition) { }
LuaObject::LuaObject(const std::string & pName, const sf::Vector2f & pPosition) : GameObject(pName, pPosition) { }
LuaObject::~LuaObject() { }


#define LUAOBJECT "LuaObject"
#define CASTUDATA (*static_cast<LuaObject**>(lua_touserdata(lua, 1)))

//--------------------------------------Name
int getName(lua_State* lua) {
	lua_pushstring(lua, CASTUDATA->getName().c_str());
	return 1;
}

int setName(lua_State* lua) {
	CASTUDATA->setName(luaL_checkstring(lua, -1));	// 2
	return 0;
}

//---------------------------------------Transformable
int translate(lua_State* lua) {
	CASTUDATA->translate({ (float)luaL_checknumber(lua, -2), (float)luaL_checknumber(lua, -1) });
	return 0;
}

int rotate(lua_State* lua) {
	CASTUDATA->rotate(luaL_checknumber(lua, -1));
	return 0;
}

int scale(lua_State* lua) {
	CASTUDATA->scale((float)luaL_checknumber(lua, -1));
	return 0;
}

int getPosition(lua_State* lua) {
	sf::Vector2f pos = CASTUDATA->getLocalPosition();
	lua_pushnumber(lua, pos.x);
	lua_pushnumber(lua, pos.y);
	return 2;
}

int setPosition(lua_State* lua) {
	CASTUDATA->setLocalPosition({ (float)luaL_checknumber(lua, -2), (float)luaL_checknumber(lua, -1) });
	return 0;
}

int getRotation(lua_State* lua) {
	float angle = CASTUDATA->getLocalRotation();
	lua_pushnumber(lua, angle);
	return 1;
}

int setRotation(lua_State* lua) {
	CASTUDATA->setLocalRotation(luaL_checknumber(lua, -1));
	return 0;
}

int getScale(lua_State* lua) {
	sf::Vector2f scale = CASTUDATA->getLocalScale();
	lua_pushnumber(lua, scale.x);
	lua_pushnumber(lua, scale.y);
	return 2;
}

int setScale(lua_State* lua) {
	CASTUDATA->setLocalScale({ (float)luaL_checknumber(lua, -2), (float)luaL_checknumber(lua, -1) });
	return 0;
}

//----------------------------------------Sprite
int setSprite(lua_State* lua) {
	CASTUDATA->loadnSetSprite(config::ASSET_PATH + luaL_checkstring(lua, -1));
	return 0;
}

int setSpriteOrigin(lua_State* lua) {
	CASTUDATA->setSpriteOrigin({ (float)luaL_checknumber(lua, -2), (float)luaL_checknumber(lua, -1) });
	return 0;
}

int setSpriteColor(lua_State* lua) {
	CASTUDATA->setSpriteColor({ (sf::Uint8)luaL_checkinteger(lua, -4), (sf::Uint8)luaL_checkinteger(lua, -3), (sf::Uint8)luaL_checkinteger(lua, -2), (sf::Uint8)luaL_checkinteger(lua, -1) });
	return 0;
}

int getSpriteCenter(lua_State* lua) {
	sf::Vector2f center = CASTUDATA->getSpriteCenter();
	lua_pushnumber(lua, center.x);
	lua_pushnumber(lua, center.y);
	return 2;
}


//------------------------------------------Functional
int destroy(lua_State* lua) {
	LuaObject* luaObject = CASTUDATA;
	World* world = luaObject->getWorld();
	
	world->registerDeathMarked(luaObject);
	return 0;
}

int setAsBackground(lua_State* lua) {
	LuaObject* luaObject = CASTUDATA;
	World* world = luaObject->getWorld();

	world->setAsFirstChild(luaObject);
	return 0;
}

//Addchild
//Getchildren

//GameObject* LuaCardGame::newGameObject(const std::string& pName, const sf::Vector2f& pPosition) {
//	GameObject* newGO = new GameObject(pName, pPosition);
//	_world->add(newGO);
//	return newGO;
//}
//
//GameObject* LuaCardGame::newGameObject(const std::string& pName, const std::string& pTextureFilename, const sf::Vector2f& pPosition) {
//	GameObject* newGO = new GameObject(pName, pTextureFilename, pPosition);
//	_world->add(newGO);
//	return newGO;
//}

// Create & return MyObject instance to Lua
int newLuaObject(lua_State* lua) {

	// Read constructor parameters from Lua
	sf::Vector2f position = sf::Vector2f(luaL_checknumber(lua, -2), luaL_checknumber(lua, -1));
	std::string SpriteFilename = config::ASSET_PATH + luaL_checkstring(lua, -3);
	std::string name = luaL_checkstring(lua, -4);

	// Create a new card and push it as userdata to lua. Add it to the world
	LuaObject* newObj = *static_cast<LuaObject**>(lua_newuserdata(lua, sizeof(LuaObject*))) = new LuaObject(name, SpriteFilename, position);
	ServiceLocator::getService<World>()->add(newObj);

	// Set the metatable of the object on top of the stack (newCard) as the one with the associated name (LUA_CARDOBJ)
	luaL_setmetatable(lua, LUAOBJECT);
	return 1;
}

#include <iostream>

int isMouseHovering(lua_State* lua) {
	LuaObject* luaObject = CASTUDATA;
	// Check if there is a sprite
	if (luaObject->getSprite().getTexture() != nullptr) {
		sf::Window* window = Input::getWindow();

		// If there is a sprite, check if the mouse is over it
		bool inBounds = luaObject->getSprite().getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window)));
		lua_pushboolean(lua, inBounds);
		return 1;
	}
	else {
		return luaL_argerror(lua, -1, "The LuaObject sent does not have a sprite to check the bounds of!");
	}
}

const struct luaL_Reg luaGOLib[] = {
	{"getName", getName},
	{"setName", setName},

	{"translate", translate},
	{"rotate", rotate},
	{"scale", scale},

	{"getPosition", getPosition}, {"setPosition", setPosition},
	{"getRotation", getRotation}, {"setRotation", setRotation},
	{"getScale", getScale}, {"setScale", setScale},

	{"setSprite", setSprite},
	{"setSpriteOrigin", setSpriteOrigin},
	{"setSpriteColor", setSpriteColor},
	{"getSpriteCenter", getSpriteCenter},

	{"isMouseHovering", isMouseHovering},

	{"setAsBackground", setAsBackground},
	{"destroy", destroy},
	{NULL, NULL}
};

const luaL_Reg* LuaObject::getLib() {
	return luaGOLib;
}

// Register MyObject to Lua
void LuaObject::registerLObject(lua_State* lua) {

	lua_register(lua, LUAOBJECT, newLuaObject);
	luaL_newmetatable(lua, LUAOBJECT);
	lua_pushvalue(lua, -1); lua_setfield(lua, -2, "__index");

	luaL_setfuncs(lua, luaGOLib, 0);
	lua_pop(lua, 1);															// Pop the table
}