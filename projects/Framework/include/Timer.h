#pragma once
#include <sys/time.h>

class Timer
{
private:
	timeval m_previous;

	float m_totalTime;
	float m_elapsedTime;

	void updateTimeValues();

public:
	Timer();

	float elapsedTime();
	float totalTime();

	float restart();
};