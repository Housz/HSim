#pragma once

#include <scene/graphics_object.h>

namespace HSim
{
	struct Renderable
	{
		virtual void buildRenderingData() = 0;

		VertexArrayObject vao;
	};

	using Renderable_Ptr = std::shared_ptr<Renderable>;

	struct SphereHelperRenderable : public Renderable
	{
		void buildRenderingData() override;
	};

	// struct 

} // namespace HSim
