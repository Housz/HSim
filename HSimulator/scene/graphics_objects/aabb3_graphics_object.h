#pragma once

#include <HSim/space_object3.h>
#include <HSim/aabb3.h>
#include <HSim/surface3.h>
// #include <HSim/grid3.h> // draw grid  draw grid boundary
#include <scene/graphics_objects/graphics_object.h>


namespace HSim
{
	class AABB3GraphicsObject : public GObject
	{
	public:
		AABB3GraphicsObject();
		AABB3GraphicsObject(const AABB3GraphicsObject& other);
		// AABB3GObject(const AABB3_Ptr<PRECISION> aabb_, const BasicMaterial_Ptr material_);
		AABB3GraphicsObject(const AABB3<PRECISION> aabb_, const BasicMaterial_Ptr material_);
		// AABB3GObject(const SpaceObject3_Ptr<PRECISION> spaceObject_, const BasicMaterial_Ptr material_);
		// AABB3GObject(const Surface3_Ptr<PRECISION> surface_, const BasicMaterial_Ptr material_);
		~AABB3GraphicsObject();

		void clone(std::shared_ptr<GraphicsObject>& ptr) override;

	public:
		void buildRenderingData() override;

		void draw(const RenderParams &renderParams) override;

		bool isRendingDataValid() override;

	public:
		VertexBufferObject vbo;

		// AABB3_Ptr<PRECISION> aabb = nullptr;
		AABB3<PRECISION> aabb;
		
	};

	using AABB3GObject = AABB3GraphicsObject;
} // namespace HSim
