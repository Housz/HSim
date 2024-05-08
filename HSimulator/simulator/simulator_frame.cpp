#include <simulator/simulator_frame.h>

HSim::SimulatorFrame::SimulatorFrame()
{
}

HSim::SimulatorFrame::SimulatorFrame(size_t index_, double timeInterval_)
	: index(index_), timeInterval(timeInterval_)
{
}

double HSim::SimulatorFrame::elapsedTime()
{
	return index * timeInterval;
}

void HSim::SimulatorFrame::advance()
{
	index++;
}

void HSim::SimulatorFrame::advance(size_t delta)
{
	index += delta;
}

HSim::SimulatorFrame &HSim::SimulatorFrame::operator++()
{
	advance();
	return *this;
}
