#pragma once 

#include <scene/scene_graph.h>

namespace HSim
{

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void init();
	
	

	};

	using Renderer_ptr = std::shared_ptr<Renderer>;
	
} // namespace HSim
