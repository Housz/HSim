#pragma once

#include <HSim/space_object3.h>
#include <config/numerical_config.h>
#include <scene/graphics_object.h>

namespace HSim
{
	struct RenderableState
	{
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

		void draw();

	public:
		RenderableState state;

		SpaceObject3_Ptr<PRECISION> spaceObject; // ptr to surface/grid/field/helper ...

		GraphicsObject_Ptr graphicsObject; // VAO VBO EBO, draw()

	};

	using Renderable_Ptr = std::shared_ptr<Renderable>;
	
	// class SurfaceRenderable : public Renderable
	// {

	// };

	// class GridRenderable : public Renderable
	// {

	// };

} // namespace HSim
