#pragma once 

#include <scene/scene_graph.h>

namespace HSim
{

	class Simulator
	{
	public:
		Simulator();
		~Simulator();

	public:
		SceneGraph_ptr scene;
	};
	using Simulator_ptr = std::shared_ptr<Simulator>;
	
} // namespace HSim
