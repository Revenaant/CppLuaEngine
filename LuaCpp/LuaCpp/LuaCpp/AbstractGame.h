#ifndef ABSTRACTGAME_HPP
#define ABSTRACTGAME_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <lua.hpp>
#include "GameObject.h"

class World;

/**
 * Defines the basic structure for a game, with defaults already filled in.
 * The only item that MUST be overridden/implemented is _initializeScene()
 * where you build up your world, the rest is optional to override (with care/caution).
 */
class AbstractGame
{
public:

	AbstractGame();
	virtual ~AbstractGame();

	//creates a window, initializes glew, a renderer and a world instance
	virtual void initialize();
	//run the actual process of updating all objects, rendering them and processing events
	virtual void run();
	//Close all dependencies, such as lua
	virtual void close();

	float GetScrollDelta();

protected:

	//methods above delegate behaviour to the methods below so that you can override it in a subclass

	//initialize sfml rendering context
	virtual void _initializeWindow();
	//initialize a scene root to which we can attach/add objects
	virtual void _initializeWorld();

	virtual void _setUpServices();

	sf::View CalcView(const sf::Vector2u &windowSize, const sf::Vector2u &desiredSize);

	//initialize the actual scene, HAS to be done by a subclass
	virtual void _initializeScene() = 0;

	//call update on all game objects in the display root
	virtual void _fixedUpdate(float pStep);
	virtual void _variableUpdate(float pStep);
	//Delete all unused gameobjects
	virtual void _collectGarbage();
	//render all game objects in the display root
	virtual void _render();
	//process any sfml window events (see SystemEventDispatcher/Listener)
	virtual void _processEvents();

	// Lua
	virtual lua_State* luaInit();
	virtual void luaClose();

	lua_State* _lua;			//the lua state
	sf::RenderWindow* _window;  //sfml window to render into
	World* _world;              //the root game object that represents our scene
	float _fps;                 //stores the real fps
	int const MS_PER_FRAME = 16;

	GameObject* _camera;
	float _mouseWheelScrollDelta = 0;

private:
	AbstractGame(const AbstractGame&);
	AbstractGame& operator=(const AbstractGame&);
};

#endif // ABSTRACTGAME_HPP
