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
		void traverse(const std::function<void(GameObject_ptr)>& callback);

	public:
		void traverse(std::function<void(GameObject_ptr)>& callback, GameObject_ptr go);
		void traverse(const std::function<void(GameObject_ptr)>& callback, GameObject_ptr go);

		void draw();

		void draw(const RenderParams& renderParams);

	public:
		void addGround(size_t slices = 100);

		void addSphere(const Vec3f& center, const float radius, const Vec3f& color = {0.8, 0.4, 0.4});

		void addBox(const Vec3f& lowerCorner, const Vec3f& upperCorner, const Vec3f& color = {0.8, 0.4, 0.4});

		void addLine(const Vec3f& start, const Vec3f& end, const Vec3f& color = {0.9, 0.9, 0.0});

		void addCylinder(const float radiusTop, const float radiusBottom, const float height, const Vec3f& color = {0.7, 0.7, 0.7});

	public:
		void serialize(); // to .obj, .vdb, ...

		void snapshot(SceneGraph& sg);

	public:
		GameObject_ptr root = nullptr;
	};

	using SceneGraph_ptr = std::shared_ptr<SceneGraph>;


} // namespace HSim
