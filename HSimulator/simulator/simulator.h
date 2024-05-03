#pragma once 

#include <HSim/common.h>
// #include <app/mtx.h>
#include <HSim/lock.h>
#include <config/numerical_config.h>

#include <scene/scene_graph.h>

#include <HSim/solver.h>

namespace HSim
{

	class Simulator
	{
	public:
		Simulator();
		~Simulator();

	public:
		void init();
		void setScene(SceneGraph_ptr scene_);
	
	public:
		void mainLoop();

	public:
		SceneGraph_ptr scene;

		// Solver<PRECISION> solver;
	};
	using Simulator_ptr = std::shared_ptr<Simulator>;
	
} // namespace HSim
