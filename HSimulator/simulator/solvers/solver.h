#pragma once

#include <HSim/common.h>

namespace HSim
{
	struct Frame
	{
		Frame() {}

		Frame(size_t index_, double timeInterval_) : index(index_), timeInterval(timeInterval_) {}

		double elapsedTime()
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
		double timeInterval = 1.0 / 600;
	};

	template <typename T>
	class Solver
	{
	public:
		Solver(){};
		~Solver(){};

	public:
		virtual void update(const Frame& frame) = 0;

		void run(size_t numFrame)
		{
			for (Frame frame; frame.index < numFrame; frame++)
			{
				// timer start
				
				update(frame);
				
				std::cout << "[FRAME] " << frame.index << " \n";

				// timer end
			}
			
		}
	};

	template <typename T>
	using Solver_Ptr = std::shared_ptr<Solver<T>>;

} // namespace HSim
