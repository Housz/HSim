#pragma once

#include <HSim/common.h>
#include <simulator/solvers/frame.h>

namespace HSim
{
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
