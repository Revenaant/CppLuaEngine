#include "Time.h"

Time::Time() { }
Time::~Time() { }

float Time::current = 0.0f;
float Time::elapsed = 0.0f;
sf::Clock Time::clock;

void Time::Update()
{
	float lastTime = current;
	current = clock.getElapsedTime().asSeconds();
	elapsed = current - lastTime;
}

float Time::Current() {
	return current;
}

float Time::Real() {
	return clock.getElapsedTime().asSeconds();
}

float Time::Elapsed() {
	return elapsed;
}

float Time::ToMilliseconds(float value) {
	return value * 1000;
}