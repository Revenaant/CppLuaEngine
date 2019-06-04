#include "LuaExampleCode.h"
#include <SFML/Graphics.hpp>
#include <SFML\OpenGL.hpp>

LuaExampleCode::LuaExampleCode()
{

}

#pragma region Using Variables

void LuaExampleCode::Learning() 
{
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);
	luaL_loadfile(lua, "globalvariable.lua");

	// Get a global variable with casting
	lua_call(lua, 0, 0);

	lua_getglobal(lua, "grade");
	if (lua_isnumber(lua, -1)) {
		int grade = lua_tonumber(lua, -1);
		printf("The grade is %d\n", grade);
	}
	// End gettinga global variable

	// This is one function call with args
	lua_getglobal(lua, "MouseClick");
	lua_pushinteger(lua, 20);
	lua_pushinteger(lua, 30);
	lua_pushstring(lua, "Middle Button");

	printf("Check stack: %d\n", lua_checkstack(lua, 1));

	lua_call(lua, 3, 1);
	// End calling a function

	// Getting a return value
	int returnValue = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	printf("Return is %d\n", returnValue);
	// End Getting a return value

	// Get multiple return values
	int top = lua_gettop(lua);
	printf("top is %d\n", top);
	for (int i = 1; i <= top; i++) { // top is the number at the top of the stack
		if (!lua_isnil(lua, -1)) {
			int value = lua_tonumber(lua, -1);
			printf("[%d] = %d\n", i, value);
			lua_pop(lua, 1);
		}
	}
	top = lua_gettop(lua);
	printf("top is %d\n", top);
	// End get return variable

	// Setting a variable
	lua_pushstring(lua, "1");
	lua_setglobal(lua, "input");

	lua_call(lua, 0, 0);
	// End Setting a varible
}
#pragma endregion

#pragma region Utility Error Checking

int LuaExampleCode::CheckInit(lua_State* lua, std::string filePath)
{
	int initError = luaL_loadfile(lua, "../Debug/Lua/sfmlsettings.lua");
	switch (initError)
	{
	case 0:
		// The file loaded okay, so call it as a protected function - to stop fatal errors from exiting the program
		//lua_pcall(lua, 0, 0, 0);
		break;
	case LUA_ERRFILE:
		std::cerr << "Cannot find / open lua script file: " << std::endl << "Skipping Lua init." << std::endl;
		break;
	case LUA_ERRSYNTAX:
		std::cerr << "Syntax error during pre-compilation of script file: " << std::endl << "Skipping Lua init." << std::endl;
		break;
	case LUA_ERRMEM:
		// Treating this as fatal, since it means other Lua calls are unlikely to work either
		std::cerr << "Fatal memory allocation error during processing of script file: " << std::endl;
		break;
	}

	return initError;
}

int LuaExampleCode::pcall(lua_State* stack, const int arguments, const int returns, const int level)
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
#pragma endregion

void LuaExampleCode::GettingAndSetting(lua_State* lua) 
{
	lua_getglobal(lua, "readTable");	// Get the table
	lua_pushinteger(lua, 2);			// Push the index (key)
	lua_pushinteger(lua, 42);			// Push the value
	lua_settable(lua, -3);				// "Confirm" the setter, pops the values
	lua_pop(lua, 1);					// Pop table after use

	lua_pushinteger(lua, 3);		// At this index..
	lua_createtable(lua, 1, 0);		// Create a table
	lua_pushinteger(lua, 1);		// At this index in the 2nd table..
	lua_pushinteger(lua, 424242);	// Add this number value
	lua_settable(lua, -3);			// Close the inner table
	lua_settable(lua, -3);			// Close the outside table

	lua_pushinteger(lua, 3);				// From this index..
	lua_gettable(lua, -2);					// Of this table (assumed)
	bool isTable = lua_istable(lua, -1);	// Check if it's a table (it is)
	lua_pushinteger(lua, 1);				// From this index..
	lua_gettable(lua, -2);					// Of the inner table..
	int value = lua_tonumber(lua, -1);		// Get the value
	lua_pop(lua, 1);						// Pop the inner table after use
	printf("Value is: %d\n", value);
	lua_pop(lua, 1);						// Pop the outside table after use
}

#pragma region Sending functions
// This is the signature of a function to be sent to Lua
static int l_sin(lua_State* lua) {
	double d = lua_tonumber(lua, -1);	// Read the first argument from the stack
	lua_pushnumber(lua, sin(d));		// Send result back to the stack
	return 1;							// Returns the number of results
}

static int l_cos(lua_State* lua) {
	double d = lua_tonumber(lua, -1);
	lua_pushnumber(lua, cos(d));
	return 1;
}

static int l_tan(lua_State* lua) {
	double d = lua_tonumber(lua, -1);
	lua_pushnumber(lua, tan(d));
	return 1;
}

static const struct luaL_Reg mathLib[] = {
	{"mysin", l_sin},
	{"mycos", l_cos},
	{"mytan", l_tan},
	{NULL, NULL} // Signals the end of the registry
};

int SendingFunctions(void) // main 
{
	std::cout << "Starting LuaEngine, close with Escape" << '\n';
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);

	// Registering C functions
	luaL_newlib(lua, mathLib);
	lua_setglobal(lua, "mathLib");

	// Send function to Lua
	lua_pushcfunction(lua, l_sin);
	lua_setglobal(lua, "ToLuaSin");	// Sets the top of the stack as a new field in __G table

	// Loading the file and calling has to be done AFTER the function pushing.
	luaL_loadfile(lua, "../Debug/Lua/CommunicationTest.lua");
	lua_call(lua, 0, 0);

	// Get function from lua
	lua_getglobal(lua, "FromLuaSin");
	lua_pushnumber(lua, 60);
	lua_call(lua, 1, 0);

	// Creating a module
	lua_getglobal(lua, "CMathModule");	// Get table
	lua_pushstring(lua, "cos");			// Set index
	lua_pushcfunction(lua, l_cos);		// Set value to store at index
	lua_settable(lua, -3);				// Confirm the set table operation

	lua_pop(lua, -1);
	lua_getglobal(lua, "printMyCos");
	lua_pushnumber(lua, 60);
	lua_call(lua, 1, 0);

	std::cin.get();
	lua_close(lua);
	std::cout << "Closed Game" << '\n';
	return 0;
}
#pragma endregion

#pragma region Userdatas
class TestObject {
public:
	int counter;
	int multiplier;
};

static int newTestObject(lua_State* lua) {
	TestObject* object;

	// Order of parameters is inverted (first is at bottom, second on top..)
	int counter = luaL_checkinteger(lua, -2);
	int multiplier = luaL_checkinteger(lua, -1);

	size_t nbytes = sizeof(TestObject);
	object = static_cast<TestObject*>(lua_newuserdata(lua, nbytes));
	object->counter = counter;
	object->multiplier = multiplier;

	luaL_getmetatable(lua, "cmgt.testObject");
	lua_setmetatable(lua, -2);

	return 1;
}

static int getTestObject(lua_State* lua) {
	TestObject* object = new TestObject();
	object->counter = 20;
	lua_pushlightuserdata(lua, object);
	return 1;
}

static int updateCounter(lua_State* lua) {
	TestObject* object = static_cast<TestObject*>(luaL_checkudata(lua, 1, "cmgt.testObject"));	// Checks if the argument is a udata with this metatable
	object->counter++;
	return 0;
}

static int getCounter(lua_State* lua) {
	TestObject* object = static_cast<TestObject*>(luaL_checkudata(lua, 1, "cmgt.testObject"));
	lua_pushinteger(lua, object->counter);
	return 1;
}

static const struct luaL_Reg testObjectLib[] = {
	{"new", newTestObject},
	{"getTestObject",getTestObject},
	{"getCounter", getCounter},
	{NULL, NULL}
};

static const struct luaL_Reg testObjectMetaLib[] = {
	{"update", updateCounter},
	{"getCounter", getCounter},
	{NULL, NULL}
};


int notmain()
{
	std::cout << "Starting LuaEngine, close with Enter" << '\n';
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);

	luaL_newmetatable(lua, "cmgt.testObject");	// Makes new table with the associated name and puts in on top of the stack
	lua_pushstring(lua, "__index");
	lua_pushvalue(lua, -2);			// Pushes the metatable
	lua_settable(lua, -3);

	luaL_setfuncs(lua, testObjectMetaLib, 0);	// Sets all functions in an array to the table on top of the stack

	luaL_newlib(lua, testObjectLib);
	luaL_getmetatable(lua, "cmgt.testObject");
	lua_setmetatable(lua, -2);
	lua_setglobal(lua, "testObject");


	luaL_loadfile(lua, "../Debug/Lua/UserData.lua");
	lua_call(lua, 0, 0);
	
	std::cin.get();
	lua_close(lua);
	std::cout << "Closed Game" << '\n';
	return 0;
}
#pragma endregion

LuaExampleCode::~LuaExampleCode()
{
}
