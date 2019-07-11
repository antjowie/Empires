#include "Timer.h"

void Timer::updateTimeValues()
{
	timeval current;
	gettimeofday(&current, nullptr);
	float delta = float(current.tv_sec - m_previous.tv_sec) + (float(current.tv_usec - m_previous.tv_usec) * 1.e-6f);

	m_totalTime += delta;
	m_elapsedTime += delta;

	m_previous = current;
}

Timer::Timer():
	m_totalTime(0),
	m_elapsedTime(0)
{
	gettimeofday(&m_previous, nullptr);
}

float Timer::elapsedTime()
{
	updateTimeValues();
	return m_elapsedTime;
}

float Timer::totalTime()
{
	updateTimeValues();
	return m_totalTime;
}

float Timer::restart()
{
	m_elapsedTime = 0;
	return elapsedTime();
}
