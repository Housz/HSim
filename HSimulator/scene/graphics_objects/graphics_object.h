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

		virtual void clone(std::shared_ptr<GraphicsObject>& ptr) = 0;
	public:
		virtual void buildRenderingData() = 0;

		virtual void draw(const RenderParams &renderParams) = 0;

		// virtual void drawWithRigidTransfom(const RenderParams &renderParams, const Transform3<PRECISION>& transform) = 0;

	public:
		virtual bool isRendingDataValid() = 0;

	public:
		VertexArrayObject vao;

		Material_Ptr material;
	};

	using GObject = GraphicsObject;

	using GraphicsObject_Ptr = std::shared_ptr<GraphicsObject>;
	using GObject_Ptr = std::shared_ptr<GraphicsObject>;

} // namespace HSim

