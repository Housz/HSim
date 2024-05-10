#pragma once
#include <HSim/bvh3.h>
#include <scene/graphics_objects/graphics_object.h>

namespace HSim
{
	class BVH3GraphicsObject : public GObject
	{
	public:
		BVH3GraphicsObject();
		BVH3GraphicsObject(const BVH3GraphicsObject& other);
		BVH3GraphicsObject(const BVH3_Ptr<PRECISION> bvh_, const BasicMaterial_Ptr material_);
		~BVH3GraphicsObject();

	public:
		void buildRenderingData() override;

		void draw(const RenderParams &renderParams) override;

		bool isRendingDataValid() override;

	public:
		VertexBufferObject vbo;
		
		BVH3_Ptr<PRECISION> bvh;

	private:
		size_t numElements;

	};

} // namespace HSim
