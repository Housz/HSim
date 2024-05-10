#pragma once
#include <HSim/sphere3.h>
#include <scene/graphics_objects/graphics_object.h>

namespace HSim
{
	class Sphere3GObject : public GObject
	{
	public:
		Sphere3GObject();
		Sphere3GObject(const Sphere3GObject& other);
		Sphere3GObject(const Sphere3_Ptr<PRECISION> sphere_, const BasicMaterial_Ptr material_);
		~Sphere3GObject();

	public:
		void buildRenderingData() override;

		void draw(const RenderParams &renderParams) override;

		bool isRendingDataValid() override;

	public:
		VertexBufferObject vbo;
		ElementBufferObject ebo;

		Sphere3_Ptr<PRECISION> sphere = nullptr;

	private:
		const size_t numSectors = 30;
		const size_t numStacks = 30;

		std::vector<float> buildVertices();

		std::vector<unsigned int> buildIndices();
	};
} // namespace HSim
