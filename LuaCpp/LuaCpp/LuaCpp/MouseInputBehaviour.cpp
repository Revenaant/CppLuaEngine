#include "MouseInputBehaviour.h"
#include "GameObject.h"
#include <SFML\Window\Mouse.hpp>
#include <iostream>

MouseInputBehaviour::MouseInputBehaviour(sf::Window* pWindow, int pIndex, float pMoveSpeedX, float pMoveSpeedY) : AbstractBehaviour(), _index(pIndex), _moveSpeedX(pMoveSpeedX), _moveSpeedY(pMoveSpeedY), _window(pWindow)
{

}

MouseInputBehaviour::~MouseInputBehaviour() { }
void MouseInputBehaviour::fixedUpdate(float pStep) { }

void MouseInputBehaviour::variableUpdate(float pStep) {
	_owner->setSpriteOrigin(_owner->getSpriteCenter()); 
	drag(pStep);
}

void MouseInputBehaviour::callDaPolice() {
	std::cout << "WOOOP WOOP IS THE SAUND OF DA POLICE " << _owner->getName() << std::endl;
}

float t = 5000;

void MouseInputBehaviour::drag(float pStep) {
	sf::Vector2f newpos = sf::Vector2f(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y);
	sf::Vector2f oldpos = _owner->getLocalPosition();

	sf::Vector2f distance = newpos - oldpos;
	float distMagnitude = (distance.x * distance.x) + (distance.y * distance.y);
	float scaling = distMagnitude / t;

	if (sf::Mouse::isButtonPressed(_index == 0 ? sf::Mouse::Button::Left : sf::Mouse::Button::Right))
	{
		//sf::Vector2f mousePos = _window->mapPixelToCoords(sf::Mouse::getPosition(*_window);

		if (_owner->getSprite().getGlobalBounds().contains(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y))
		{
			//card.setOrigin(card.getTextureRect().width / 2, card.getTextureRect().height / 2);
			//_owner->setSpriteOrigin(sf::Vector2f(newpos.x - (_owner->getSprite().getOrigin().x),
			//									 newpos.y - (_owner->getSprite().getOrigin().y)));

			
			_owner->setLocalPosition(oldpos + (newpos - oldpos) * ((scaling) * pStep));

			//card.setPosition(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y);
			_selected = true;
		}
	}
	else {
		_selected = false;
	}


	if (_selected)
	{
		// stuff might need to be added here too
		_owner->setLocalPosition(oldpos + (newpos - oldpos) * ((scaling) * pStep));
	}
}