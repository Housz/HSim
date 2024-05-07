#include "frame.h"

HSim::Frame::Frame()
{
}

HSim::Frame::Frame(size_t index_, double timeInterval_)
	: index(index_), timeInterval(timeInterval_)
{
}

double HSim::Frame::elapsedTime()
{
	return index * timeInterval;
}

void HSim::Frame::advance()
{
	index++;
}

void HSim::Frame::advance(size_t delta)
{
	index += delta;
}

HSim::Frame &HSim::Frame::operator++()
{
	advance();
	return *this;
}
