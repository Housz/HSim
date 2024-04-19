#pragma once

#include <HSim/space_object3.h>
#include <config/numerical_config.h>
#include <scene/graphics_object.h>
#include <renderer/render_params.h>

namespace HSim
{
	struct RenderableState
	{
		bool visible = true;

		bool renderingDataNeedUpdate = true;
		bool aabbNeedUpdate = true;
		// ...
	};

	class Renderable
	{
	public:
		Renderable();
		Renderable(SpaceObject3_Ptr<PRECISION> spaceObject_, GraphicsObject_Ptr graphicsObject_);
		~Renderable();

		void draw(const RenderParams& renderParams);

	public:
		SpaceObject3_Ptr<PRECISION> spaceObject; // ptr to surface/grid/field/helper ...

		GraphicsObject_Ptr graphicsObject; // VAO VBO EBO, draw()

		// states
	public:
		// RenderableState state;
		
		bool visible = true;

		bool renderingDataNeedUpdate = true;
		bool aabbNeedUpdate = true;
	};

	using Renderable_Ptr = std::shared_ptr<Renderable>;

} // namespace HSim
