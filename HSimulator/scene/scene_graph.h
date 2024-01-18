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
		void traverse(std::function<void(GameObject_ptr)>& callback);

	public:
		void traverse(std::function<void(GameObject_ptr)>& callback, GameObject_ptr go);
		
	public:
		GameObject_ptr root;

	};


} // namespace HSim
