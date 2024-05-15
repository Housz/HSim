#include <simulator/solvers/solver.h>

namespace HSim
{
	Solver::Solver()
	{
	}

	Solver::~Solver()
	{
	}

	void Solver::run(size_t numFrame)
	{
		// for (SimFrame frame; frame.index < numFrame; frame++)
		for (SimFrame frame(0, 1.0/20); frame.index < numFrame; frame++)
		{
			// timer start
			
			update(frame);
			
			// std::cout << "[FRAME] " << frame.index << " \n";

			// timer end
		}
		std::cout << "[FRAME] done!" << " \n";
		
	}

	void Solver::writeRendererBuffer()
	{
	}
	

} // namespace HSim
