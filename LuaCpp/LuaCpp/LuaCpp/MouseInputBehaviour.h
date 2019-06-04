#ifndef MOUSEINPUTBEHAVIOR_HPP
#define MOUSEINPUTBEHAVIOR_HPP

#include "AbstractBehaviour.h"
#include <SFML\Graphics.hpp>

/**
* MouseInput allows you to move an object using the mouse in its own local space.
* Hold to drag, click to interact.
*/
class MouseInputBehaviour : public AbstractBehaviour
{
public:
	MouseInputBehaviour(sf::Window* pWindow, int pIndex, float pMoveSpeedX = 5.0f, float pMoveSpeedY = 5.0f);

	~MouseInputBehaviour() override;
	void fixedUpdate(float pStep) override;
	void variableUpdate(float pStep) override;

	// Inner move function to be used inside update
	//btVector3 move(int index, float pStep);
	void callDaPolice();
	void drag(float pStep);

private:
	float _moveSpeedX;
	float _moveSpeedY;

	bool _selected;
	int _index;
	sf::Window* _window;
};

#endif // !MOUSEINPUTBEHAVIOR_HPP
