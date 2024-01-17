#pragma once

#include "game_object.h"

namespace HSim
{
	class SceneGraph
	{
	public:
		SceneGraph();
		~SceneGraph();

	public:
		void traverse(std::function<void(GameObject&)> callback);

	public:
		GameObject_ptr root;


	};


} // namespace HSim
