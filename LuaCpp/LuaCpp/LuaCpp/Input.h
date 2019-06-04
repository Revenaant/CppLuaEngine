#ifndef INPUT_HPP
#define INPUT_HPP
#include <SFML\Graphics.hpp>
#include <lua.hpp>

class Input
{
public:
	Input();
	~Input();

	static void registerLib(lua_State* lua);
	void processEvents(sf::Window* pWindow);

	static void setWindow(sf::Window* pWindow);
	static sf::Window* getWindow();
private:
	static sf::Window* _window;
};

#endif // !INPUT_HPP
