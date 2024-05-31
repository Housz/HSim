#pragma once

#include <scene/graphics_objects/graphics_object.h>
#include <HSim/space_object3.h>
#include <HSim/aabb3.h>
#include <HSim/cell_center_scalar_grid3.h>
#include <HSim/face_center_grid3.h>

namespace HSim
{
	class CellCenterScalarGrid3GraphicsObject : public GObject
	{
	public:
		CellCenterScalarGrid3GraphicsObject();
		CellCenterScalarGrid3GraphicsObject(const CellCenterScalarGrid3GraphicsObject& other);
		CellCenterScalarGrid3GraphicsObject(const CellCenterScalarGrid3_Ptr<PRECISION> grid_, const PointMaterial_Ptr material_);
		~CellCenterScalarGrid3GraphicsObject();

		void clone(std::shared_ptr<GraphicsObject> &ptr) override;

	public:
		void buildRenderingData() override;

		void draw(const RenderParams &renderParams) override;

		bool isRendingDataValid() override;
	
	public:
		VertexBufferObject vbo;

		CellCenterScalarGrid3_Ptr<PRECISION> grid = nullptr;

	private:
		size_t numElements; 
	};

	using CellCenterScalarGrid3GObject = CellCenterScalarGrid3GraphicsObject;

	class FaceCenterGrid3GraphicsObject : public GObject
	{
	public:
		FaceCenterGrid3GraphicsObject();
		FaceCenterGrid3GraphicsObject(const FaceCenterGrid3GraphicsObject& other);
		FaceCenterGrid3GraphicsObject(const FaceCenterGrid3_Ptr<PRECISION> grid_, const PointMaterial_Ptr material_);
		~FaceCenterGrid3GraphicsObject();

		void clone(std::shared_ptr<GraphicsObject> &ptr) override;

	public:
		void buildRenderingData() override;

		void draw(const RenderParams &renderParams) override;

		bool isRendingDataValid() override;
	
	public:
		VertexBufferObject vbo;

		FaceCenterGrid3_Ptr<PRECISION> grid = nullptr;

	private:
		size_t numElements; 
	};
	
	using FaceCenterGrid3GObject = FaceCenterGrid3GraphicsObject;


} // namespace HSim
