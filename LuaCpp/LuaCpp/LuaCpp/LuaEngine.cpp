#include <iostream>
#include "AbstractGame.h"
#include "LuaCardGame.h"

int main(void)
{
	std::cout << "Starting LuaEngineGame, close with Escape" << '\n';
	AbstractGame* game = new LuaCardGame();
	game->initialize();
	game->run();
	game->close();
	delete game;

	std::cout << "Closed Game" << '\n';
	//std::cin.get();
	return 0;
}

