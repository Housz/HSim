#pragma once

#include <scene/graphics_buffer.h>

#include <HSim/surface3.h>
#include <HSim/sphere3.h>

namespace HSim
{
	class GraphicsObject
	{
	public:
		GraphicsObject();
		~GraphicsObject();

		virtual void buildRenderingData() = 0;


	public:
		VertexArrayObject vao;
	};
	
	using GObject = GraphicsObject;

	class Sphere3GObject : public GObject
	{
	public:
		Sphere3GObject();
		Sphere3GObject(const Sphere3_Ptr sphere_);
		~Sphere3GObject();

		void buildRenderingData() override;

	public:
		VertexBufferObject vbo;
		ElementBufferObject ebo;

		Sphere3_Ptr sphere;
	}

	
} // namespace HSim
