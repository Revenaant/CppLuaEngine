#include "Card.h"
#include <iostream>
#include <lua.hpp>
#include "ServiceLocator.h"
#include "World.h"
#include "Config.h"

Card::Card() : LuaObject("card")
{
}

Card::Card(std::string pName, std::string pFilename, sf::Vector2f pPosition) : LuaObject(pName, pFilename, pPosition) {

}

Card::~Card()
{
}

lua_State* Card::_lua;

void Card::printAss() {
	std::cout << _name << ass << ass << ass << std::endl;
}

void Card::setMyAss(std::string ass) {
	this->ass = ass;
}

std::string Card::getAss() {
	return ass + _name;
}

void Card::fixedUpdate(float pStep) {
	GameObject::fixedUpdate(pStep);
}

void Card::variableUpdate(float pStep) {
	GameObject::variableUpdate(pStep);
}


// MyObject identifier for the Lua metatable
#define LUA_CARDOBJ "Card"

// Free MyObject instance by Lua garbage collection
int myobject_delete(lua_State* lua) {
	delete *static_cast<Card**>(lua_touserdata(lua, 1));
	std::cout << "Deleted card" << std::endl;
	return 0;
}

// MyObject member functions in Lua
int myobject_set(lua_State* lua) {
	(*static_cast<Card**>(luaL_checkudata(lua, 1, LUA_CARDOBJ)))->setMyAss(luaL_checkstring(lua, -1));	// 2
	return 0;
}
int myobject_get(lua_State* lua) {
	lua_pushstring(lua, (*static_cast<Card**>(luaL_checkudata(lua, 1, LUA_CARDOBJ)))->getAss().c_str());
	return 1;
}

// Create & return MyObject instance to Lua
int newCardObject(lua_State* lua) {

	// Read constructor parameters from Lua
	sf::Vector2f position = sf::Vector2f(luaL_checknumber(lua, -2), luaL_checknumber(lua, -1));
	std::string SpriteFilename = config::ASSET_PATH + luaL_checkstring(lua, -3);
	std::string name = luaL_checkstring(lua, -4);

	// Create a new card and push it as userdata to lua. Add it to the world
	Card* newCard = *static_cast<Card**>(lua_newuserdata(lua, sizeof(Card*))) = new Card(name, SpriteFilename, position);
	ServiceLocator::getService<World>()->add(newCard);

	// Set the metatable of the object on top of the stack (newCard) as the one with the associated name (LUA_CARDOBJ)
	luaL_setmetatable(lua, LUA_CARDOBJ);
	return 1;
}

static const struct luaL_Reg cardLib[] = {
	{"set", myobject_set},
	{"get", myobject_get},
	//{"__gc", myobject_delete},
	{NULL, NULL}
};

// Register MyObject to Lua
void Card::registerLObject(lua_State* lua) {
	_lua = lua;

	lua_register(lua, LUA_CARDOBJ, newCardObject);
	luaL_newmetatable(lua, LUA_CARDOBJ);
	lua_pushvalue(lua, -1); lua_setfield(lua, -2, "__index");

	luaL_setfuncs(lua, cardLib, 0);
	luaL_setfuncs(lua, LuaObject::getLib(), 0);
	lua_pop(lua, 1);															// Pop the table
}
