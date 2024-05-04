#include <simulator/solvers/naive_solver.h>

namespace HSim
{
	template<typename T>
	void NaiveSolver<T>::update(Frame<T> frame)
	{
		if (frame.index > currentFrame.index)
		{
			
		}
		
	}
	
	template<typename T>
	void NaiveSolver<T>::advanceTimeStep(double timeInterval)
	{
		
	}
	
	template<typename T>
	void NaiveSolver<T>::advanceSubTimeStep(double subTimeInterval)
	{
		
	}
	
	template<typename T>
	void NaiveSolver<T>::init()
	{
		// pass
		std::cout << "[Simulator] Init Naive Solver.\n";
	}
}