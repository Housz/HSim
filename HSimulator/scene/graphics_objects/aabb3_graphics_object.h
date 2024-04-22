#pragma once

#include <HSim/space_object3.h>
#include <HSim/aabb3.h>
#include <HSim/surface3.h>
// #include <HSim/grid3.h> // draw grid  draw grid boundary
#include <scene/graphics_objects/graphics_object.h>


namespace HSim
{
	class AABB3GObject : public GObject
	{
	public:
		AABB3GObject();
		AABB3GObject(const AABB3_Ptr<PRECISION> aabb_, const BasicMaterial_Ptr material_);
		// AABB3GObject(const SpaceObject3_Ptr<PRECISION> spaceObject_, const BasicMaterial_Ptr material_);
		AABB3GObject(const Surface3_Ptr<PRECISION> spaceObject_, const BasicMaterial_Ptr material_);
		~AABB3GObject();

	public:
		void buildRenderingData() override;

		void draw(const RenderParams &renderParams) override;

		bool isRendingDataValid() override;

	public:
		VertexBufferObject vbo;

		AABB3_Ptr<PRECISION> aabb = nullptr;
		
	};
} // namespace HSim
