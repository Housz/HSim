#pragma once
#include <scene/graphics_objects/graphics_object.h>
#include <HSim/triangle_mesh3.h>

namespace HSim
{
	class TriangleMesh3GObject : public GObject
	{
	public:
		TriangleMesh3GObject();
		TriangleMesh3GObject(const TriangleMesh3GObject &other);
		TriangleMesh3GObject(const TriangleMesh3_Ptr<PRECISION> mesh_, const BasicMaterial_Ptr material_);
		~TriangleMesh3GObject();

		void clone(std::shared_ptr<GraphicsObject> &ptr) override;

	public:
		void buildRenderingData() override;

		void draw(const RenderParams &renderParams) override;

		bool isRendingDataValid() override;

	public:
		VertexBufferObject vbo;
		ElementBufferObject ebo;

		TriangleMesh3_Ptr<PRECISION> mesh = nullptr;

	private:
		void buildRenderingDataNaive();
		void buildRenderingDataFlat();
		void buildRenderingDataSmooth();

		std::vector<float> buildVerticesNaive();
		std::vector<float> buildVerticesFlat();
		std::vector<float> buildVerticesSmooth();
		std::vector<unsigned int> buildIndices();
	};

} // namespace HSim
