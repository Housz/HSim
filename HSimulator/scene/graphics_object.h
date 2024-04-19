#pragma once

#include <scene/graphics_buffer.h>
#include <config/numerical_config.h>
#include <scene/material.h>
#include <renderer/render_params.h>

#include <HSim/common.h>
#include <HSim/surface3.h>
#include <HSim/sphere3.h>

namespace HSim
{
	class GraphicsObject
	{
	public:
		GraphicsObject();
		GraphicsObject(Material_Ptr material_);
		~GraphicsObject();

		virtual void buildRenderingData() = 0;

		virtual void draw(const RenderParams& renderParams) = 0;

	public:
		virtual bool rendingDataValid() = 0;

	public:
		VertexArrayObject vao;

		Material_Ptr material;
	};

	using GObject = GraphicsObject;

	using GraphicsObject_Ptr = std::shared_ptr<GraphicsObject>;
	using GObject_Ptr = std::shared_ptr<GraphicsObject>;

	class Sphere3GObject : public GObject
	{
	public:
		Sphere3GObject();
		Sphere3GObject(const Sphere3_Ptr<PRECISION> sphere_, BasicMaterial_Ptr material_);
		~Sphere3GObject();

	public:

		void buildRenderingData() override;

		void draw(const RenderParams& renderParams) override;

		bool rendingDataValid() override;

	public:
		VertexBufferObject vbo;
		ElementBufferObject ebo;

		Sphere3_Ptr<PRECISION> sphere;

	private:
		const size_t numSectors = 30;
		const size_t numStacks = 30;

		std::vector<float> buildVertices();

		std::vector<unsigned int> buildIndices();
	};

	class GroundHelperGObject : public GObject
	{
	public:
		
	};

} // namespace HSim
