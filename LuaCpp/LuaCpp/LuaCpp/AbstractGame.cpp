#include <iostream>
#include <assert.h>

#include "AbstractGame.h"
#include "ServiceLocator.h"
#include "World.h"
#include "Time.h"
#include "Config.h"

AbstractGame::AbstractGame() :_window(NULL), _world(NULL), _fps(0)
{
	//ctor
}

AbstractGame::~AbstractGame()
{
	//dtor
	delete _window;
	delete _world;
}

void AbstractGame::initialize() {
	std::cout << "Initializing engine..." << std::endl << std::endl;
	_lua = luaInit();
	_initializeWindow();
	_initializeWorld();
	_setUpServices();
	_initializeScene();
	std::cout << std::endl << "Engine initialized." << std::endl << std::endl;
}

///SETUP

sf::View AbstractGame::CalcView(const sf::Vector2u &windowSize, const sf::Vector2u &desiredSize) {
	sf::FloatRect viewport(0.0f, 0.0f, 1.0f, 1.0f);

	float screenWidth = windowSize.x / static_cast<float>(desiredSize.x);
	float screenHeight = windowSize.y / static_cast<float>(desiredSize.y);

	if (screenWidth > screenHeight) {
		viewport.width = screenHeight / screenWidth;
		viewport.left = (1.0f - viewport.width) * 0.5f;
	}
	else if (screenWidth < screenHeight) {
		viewport.height = screenWidth / screenHeight;
		viewport.top = (1.0f - viewport.height) * 0.5f;
	}

	sf::View view(sf::FloatRect(0, 0, desiredSize.x, desiredSize.y));
	view.setViewport(viewport);

	return view;
}
//---------------------------------------------------------------Pass ownership of the window to Lua Engine 
void AbstractGame::_initializeWindow() {
	std::cout << "Initializing window..." << std::endl;

	assert(_lua != nullptr);

	// Set the table to the top of the stack
	lua_getglobal(_lua, "windowTable");

	// Get the window width
	lua_pushstring(_lua, "width");
	lua_gettable(_lua, -2);
	int width = lua_tonumber(_lua, -1);
	// Remove the result (width variable)
	lua_pop(_lua, 1);

	// Get the window height
	lua_pushstring(_lua, "height");
	lua_gettable(_lua, -2);
	int height = lua_tonumber(_lua, -1);
	// Remove the result (height variable)
	lua_pop(_lua, 1);

	// Get the window title
	lua_pushstring(_lua, "title");
	lua_gettable(_lua, -2);
	const char* title = lua_tostring(_lua, -1);
	lua_pop(_lua, 1);

	lua_pushstring(_lua, "fullscreen");
	lua_gettable(_lua, -2);
	bool fullscreen = lua_toboolean(_lua, -1);
	lua_pop(_lua, 1);


	_window = fullscreen ? new sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Fullscreen, sf::ContextSettings(24, 8, 0, 3, 3))
						 : new sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Default, sf::ContextSettings(24, 8, 0, 3, 3));

	//_window->setVerticalSyncEnabled(true);
	std::cout << "Window initialized." << std::endl << std::endl;
}

void AbstractGame::_initializeWorld() {
	//setup the world
	std::cout << "Initializing world..." << std::endl;
	_world = new World();
	std::cout << "World initialized." << std::endl << std::endl;
}

void AbstractGame::_setUpServices() {
	ServiceLocator* serviceLocator = new ServiceLocator();

	ServiceLocator::provideService(_world);
}

void AbstractGame::close() {
	luaClose();

	// Other things that may need to be closed
}

void AbstractGame::luaClose()
{
	assert(_lua != nullptr);
	lua_close(_lua);

	std::cout << "Closed Lua Successfully" << std::endl;
}

//-------------------------------------------------------------------------How to handle this specific filename(s)?
lua_State* AbstractGame::luaInit()
{
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);
	luaL_loadfile(lua, (config::LUA_PATH + "sfmlsettings.lua").c_str());

	int luaError = lua_pcall(lua, 0, 0, 0);
	if (luaError) printf((std::string("Is there an error? : ") + static_cast<std::string>(lua_tostring(lua, -1))).c_str());

	return lua;
}

///MAIN GAME LOOP

void AbstractGame::run()
{
	//setting to calculate fps
	sf::Clock renderClock;
	int frameCount = 0;
	float timeSinceLastFPSCalculation = 0;

	float last = Time::Current();
	float lag = 0.0f;
	bool running = true;

	while (_window->isOpen())
	{
		Time::Update();

		float current = Time::Current();
		float elapsed = current - last;
		last = current;
		lag += Time::ToMilliseconds(elapsed);

		// Input
		_processEvents();

		// Game loop
		while (lag >= MS_PER_FRAME) {
			// This is a fixed update because it catches up inside this scope
			_fixedUpdate(Time::ToMilliseconds(MS_PER_FRAME));								// TODO: Check that delta time is correct
			_collectGarbage();

			lag -= MS_PER_FRAME;
		}

		// This is where a normal update would be
		_variableUpdate(elapsed);
		_render();

#pragma region FpsCounter
		//fps count is updated once every 1 second
		frameCount++;
		timeSinceLastFPSCalculation += renderClock.restart().asSeconds();
		if (timeSinceLastFPSCalculation > 1) {
			_fps = frameCount / timeSinceLastFPSCalculation;
			timeSinceLastFPSCalculation -= 1;
			frameCount = 0;
#pragma endregion
		}
	}

}

void AbstractGame::_fixedUpdate(float pStep) {
	_world->fixedUpdate(pStep);
}

void AbstractGame::_variableUpdate(float pStep) {
	_world->variableUpdate(pStep);
}

void AbstractGame::_collectGarbage() {

	for (int i = _world->getDeathMarks().size() - 1; i >= 0; --i) {
		if (_world->getDeathMarks().size() == 0) return;
		GameObject* go = _world->getDeathMarks()[i];

		go->setParent(nullptr);
		_world->unregisterDeathMarked(go);
		//if (go->getCollider() == nullptr) delete go->getCollider();
		delete go;
	}
}

void AbstractGame::_render() {
	_window->clear();
	_world->draw(*_window, _world->getTransform());
	_window->display();
}

float AbstractGame::GetScrollDelta() {
	return _mouseWheelScrollDelta;
}

void AbstractGame::_processEvents() //--------------------------------------------REPLACEEE
{
	sf::Event event;
	bool exit = false;

	//we must empty the event queue
	while (_window->pollEvent(event)) {
		//give all system event listeners a chance to handle events
		//optionally to be implemented by you for example you could implement a
		//SystemEventDispatcher / SystemEventListener pair which allows Listeners to
		//register with the dispatcher and then do something like:
		//SystemEventDispatcher::dispatchEvent(event);

		switch (event.type) {
		case sf::Event::Closed:
			exit = true;
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) {
				exit = true;
			}
			break;
		case sf::Event::Resized:
			//would be better to move this to the renderer
			//this version implements nonconstrained match viewport scaling
			//_window->setSize({	static_cast<float>(event.size.width),
			//					static_cast<float>(event.size.height) });
			break;
		case sf::Event::MouseWheelScrolled:
			_mouseWheelScrollDelta += event.mouseWheelScroll.delta;
			break;
		default:
			break;
		}
	}

	if (exit) {
		_window->close();
	}
}



