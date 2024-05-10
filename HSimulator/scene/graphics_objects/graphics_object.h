#pragma once

#include <HSim/common.h>
#include <HSim/surface3.h>

#include <scene/graphics_buffer.h>
#include <scene/material.h>
#include <scene/helper_object.h>
#include <config/numerical_config.h>
#include <renderer/render_params.h>


namespace HSim
{
	class GraphicsObject
	{
	public:
		GraphicsObject();
		GraphicsObject(const GraphicsObject& graphicsObject_);
		GraphicsObject(Material_Ptr material_);
		~GraphicsObject();

		virtual void buildRenderingData();

		virtual void draw(const RenderParams &renderParams);

	public:
		virtual bool isRendingDataValid();

	public:
		VertexArrayObject vao;

		Material_Ptr material;
	};

	using GObject = GraphicsObject;

	using GraphicsObject_Ptr = std::shared_ptr<GraphicsObject>;
	using GObject_Ptr = std::shared_ptr<GraphicsObject>;

} // namespace HSim

