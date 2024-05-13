#pragma once

#include <HSim/space_object3.h>
#include <config/configs.h>
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
		// INSTANCED // instanced 
	};

	class Renderable
	{
	public:
		Renderable();
		Renderable(SpaceObject3_Ptr<PRECISION> spaceObject_, GraphicsObject_Ptr graphicsObject_);
		Renderable(const Renderable& renderable_);
		~Renderable();

	public:
		// sync simulation and renderering
		void draw(const RenderParams &renderParams);

		// async simulation and renderering
		void drawAsync(const RenderParams &renderParams);

	// data
	public:
		SpaceObject3_Ptr<PRECISION> spaceObject; // ptr to surface/grid/field/helper ...

		GraphicsObject_Ptr graphicsObject; // VAO VBO EBO, draw()

		// states
	public:
		bool visible = true;

		RenderableUpdateType updateType = RenderableUpdateType::STATIC;

		// for RenderableUpdateType::DYNAMIC
		bool renderingDataNeedUpdate = true;
		// bool aabbNeedUpdate = true;

		// for RenderableUpdateType::RIGID
		Transform3<PRECISION> rigidTransfrom;

	};

	using Renderable_Ptr = std::shared_ptr<Renderable>;

} // namespace HSim
