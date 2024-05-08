#pragma once 

#include <HSim/common.h>
#include <scene/game_object.h>
#include <simulator/solvers/solver.h>

namespace HSim
{
	class SmokeSolver : public Solver
	{
	public:
		SmokeSolver();
		~SmokeSolver();
	
	public:
		void update(const SimFrame &frame) override;


	public:
		PRECISION gravity;	
		
	};

	using SmokeSolver_Ptr = std::shared_ptr<SmokeSolver>;

} // namespace HSim
