#pragma once

#include <scene/game_object.h>

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
		

		void serialize();
		void draw();

		void draw(const RenderParams& renderParams);

		void addGround(size_t slices = 100);

	public:
		GameObject_ptr root = nullptr;

	};

	using SceneGraph_ptr = std::shared_ptr<SceneGraph>;


} // namespace HSim
