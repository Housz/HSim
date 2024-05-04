#pragma once

#include <HSim/common.h>

namespace HSim
{
	template <typename T>
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

		Frame operator++(int)
		{
			Frame result = *this;
			advance();
			return result;
		}

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
		virtual void update(Frame<T> frame) = 0;
	};

	template <typename T>
	using Solver_Ptr = std::shared_ptr<Solver<T>>;

} // namespace HSim
