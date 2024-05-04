#pragma once

#include <HSim/common.h>
#include <HSim/solver.h>

#include <scene/game_object.h>

namespace HSim
{
	template <typename T>
	class NaiveSolver : public Solver<T>
	{
	public:
		void update(Frame<T> frame) override;

	public:
		GameObject_ptr go = nullptr;
	};

} // namespace HSim
