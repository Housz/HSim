#pragma once

#include <HSim/common.h>

namespace HSim
{
	struct Frame
	{
		Frame() {}

		Frame(size_t index_, T timeInterval_) : index(index_), timeInterval(timeInterval_) {}

		T elapsedTime()
		{
			return index * timeInterval;
		}

		void advance()
		{
			index++;
		}
		void advance(size_t delta)
		{
			index += delta;
		}

		Frame &operator++()
		{
			advance();
			return *this;
		}

		// Frame operator++(int)
		// {

		// }

		// index of this frame.
		size_t index = 0;
		// Time interval between two frames.
		T timeInterval = 1.0 / 60;
	};

	template <typename T>
	class Solver
	{
	public:
		Solver(){};
		~Solver(){};

	public:
		virtual void update(Frame frame) = 0;
	};

	// using Solver_Ptr

} // namespace HSim
