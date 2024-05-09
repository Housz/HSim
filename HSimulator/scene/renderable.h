#pragma once

#include <HSim/space_object3.h>
#include <config/numerical_config.h>
#include <scene/graphics_objects.h>
#include <renderer/render_params.h>

namespace HSim
{
	// Deprecated
	struct RenderableState
	{
		bool visible = true;

		bool renderingDataNeedUpdate = true;
		bool aabbNeedUpdate = true;
		// ...
	};

	enum class RenderableUpdateType
	{
		STATIC,	 // static, e.g. ground, boundary, ...
		RIGID,	 // rigid transformation, only translation and rotation.
		DYNAMIC, // deformable, fluid, ...
		// PARTIAL // partial change
	};

	class Renderable
	{
	public:
		Renderable();
		Renderable(SpaceObject3_Ptr<PRECISION> spaceObject_, GraphicsObject_Ptr graphicsObject_);
		~Renderable();

		void draw(const RenderParams &renderParams);

	public:
		SpaceObject3_Ptr<PRECISION> spaceObject; // ptr to surface/grid/field/helper ...

		GraphicsObject_Ptr graphicsObject; // VAO VBO EBO, draw()

		// states
	public:
		bool visible = true;

		bool renderingDataNeedUpdate = true;
		// bool aabbNeedUpdate = true;

		RenderableUpdateType updateType = RenderableUpdateType::STATIC;
	};

	using Renderable_Ptr = std::shared_ptr<Renderable>;

} // namespace HSim
