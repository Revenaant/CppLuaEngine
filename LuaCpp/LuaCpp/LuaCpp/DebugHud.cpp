#include <cassert>
#include <iostream>
#include <SFML/Graphics/Text.hpp>
#include "DebugHud.h"

DebugHud::DebugHud(sf::RenderWindow * aWindow) : _window(aWindow), _debugInfo(), _font(), _debugText()
{
	assert(_window != NULL);

	_createDebugHud();
}

DebugHud::~DebugHud()
{
	//dtor
}

void DebugHud::_createDebugHud() {
	_debugText.setString("");
	_debugText.setFont(_font);
	_debugText.setCharacterSize(16);
	_debugText.setFillColor(sf::Color::White);
}

void DebugHud::setDebugInfo(std::string pInfo) {
	_debugText.setString(pInfo);
	_debugText.setPosition(10, 10);
}

void DebugHud::draw()
{
	//glDisable( GL_CULL_FACE );
	_window->pushGLStates();
	_window->draw(_debugText);
	_window->popGLStates();
}
