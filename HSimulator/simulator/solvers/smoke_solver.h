#pragma once 

#include <HSim/common.h>
#include <scene/game_object.h>
#include <simulator/solvers/solver.h>

namespace HSim
{
	template <typename T>
	class SmokeSolver : public Solver<T>
	{
	public:
		SmokeSolver();
		~SmokeSolver();
	
	public:
		void update(const Frame &frame) override;

		
	};

	
	template<typename T>
	SmokeSolver<T>::SmokeSolver()
	{
		
	}
	
	template<typename T>
	SmokeSolver<T>::~SmokeSolver()
	{
		
	}
	
	template<typename T>
	void SmokeSolver<T>::update(const Frame &frame)
	{
		
	}
	


} // namespace HSim
