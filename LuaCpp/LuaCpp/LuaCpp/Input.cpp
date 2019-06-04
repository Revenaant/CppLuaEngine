#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

sf::Window* Input::_window;

int getMousePos(lua_State* lua) {
	sf::Vector2i pos = sf::Mouse::getPosition(*Input::getWindow());
	lua_pushinteger(lua, pos.x);
	lua_pushinteger(lua, pos.y);
	return 2;
}

int getMouseButton(lua_State* lua) {
	int button = luaL_checkinteger(lua, -1);
	bool pressed = false;

	switch (button)
	{
	case 0:	// LMB
		pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		break;
	case 1:	// RMB
		pressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		break;
	case 2:	// MMB
		pressed = sf::Mouse::isButtonPressed(sf::Mouse::Middle);
		break;
	default:	// Error, not recognized
		luaL_argerror(lua, -1, "The parameter does not correspond to a recognized mouse button!");
		break;
	}

	lua_pushboolean(lua, pressed);
	return 1;
}
//
//int getKeyboardButton(lua_State* lua) {
//
//}

static const struct luaL_Reg InputLib[] = {
	{"getMousePos", getMousePos},
	{"getMouseButton", getMouseButton},
	{NULL, NULL} // Signals the end of the registry
};

void Input::registerLib(lua_State* lua) {
	luaL_newlib(lua, InputLib);
	lua_setglobal(lua, "Input");
}

void Input::processEvents(sf::Window* pWindow)
{
	sf::Event event;
	bool exit = false;
	
	//we must empty the event queue
	while (pWindow->pollEvent(event)) 
	{
		switch (event.type) 
		{
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
		//case sf::Event::MouseWheelScrolled:
		//	_mouseWheelScrollDelta += event.mouseWheelScroll.delta;
		//	break;


		//case sf::Event::MouseButtonPressed:
		//	if (event.mouseButton.button == sf::Mouse::Left)
		//		if (card.getGlobalBounds().
		//			contains(sf::Mouse::getPosition(window).x,
		//				sf::Mouse::getPosition(window).y))
		//		{
		//			// the problem is here
		//			card.setOrigin(event.mouseButton.x, event.mouseButton.y);
		//			selected = true;
		//		}

		//	break;
		//case sf::Event::MouseMoved:

		//	if (selected)
		//	{
		//		// stuff might need to be added here too
		//		card.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		//	}
		//	break;
		//case sf::Event::MouseButtonReleased:
		//	selected = false;
		//	break;



		default:
			break;
		}
	}

	if (exit) {
		pWindow->close();
	}

}

void Input::setWindow(sf::Window* pWindow) {
	_window = pWindow;
}

sf::Window* Input::getWindow() {
	return _window;
}