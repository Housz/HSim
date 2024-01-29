#pragma once 

#include <HSim/common.h>
#include <app/mtx.h>


#include <scene/scene_graph.h>

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
	};
	using Simulator_ptr = std::shared_ptr<Simulator>;
	
} // namespace HSim
