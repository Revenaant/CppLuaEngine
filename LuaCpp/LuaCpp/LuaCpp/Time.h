#ifndef TIME_HPP
#define TIME_HPP

#include <SFML\Graphics.hpp>

static class Time
{
public:
	static void Update();

	static float Current();
	static float Real();
	static float Elapsed();
	static float ToMilliseconds(float value);

private:
	Time();
	~Time();

	static sf::Clock clock;

	static float current;
	static float elapsed;
};

#endif // !TIME_HPP