#pragma once

#include "game_object.h"

namespace HSim
{
	class SceneGraph : public std::enable_shared_from_this<SceneGraph>
	{
	public:
		SceneGraph();
		~SceneGraph();

	public:
		void traverse(std::function<void(GameObject_ptr)>& callback);

	public:
		void traverse(std::function<void(GameObject_ptr)>& callback, GameObject_ptr go);
		

		void draw();

	public:
		GameObject_ptr root;

	};

	using SceneGraph_ptr = std::shared_ptr<SceneGraph>;


} // namespace HSim
