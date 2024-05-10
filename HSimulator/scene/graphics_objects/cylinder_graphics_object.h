#pragma once
#include <HSim/cylinder.h>
#include <scene/graphics_objects/graphics_object.h>

namespace HSim
{
    class CylinderGraphicsObject : public GObject
    {
    public:
        CylinderGraphicsObject();
        CylinderGraphicsObject(const CylinderGraphicsObject& cylinderGraphicsObject_);
        CylinderGraphicsObject(const Cylinder3_Ptr<PRECISION> cylinder_, const BasicMaterial_Ptr material_);
        ~CylinderGraphicsObject();

    public:
        void buildRenderingData() override;

        void draw(const RenderParams &renderParams) override;

        bool isRendingDataValid() override;

    public:
    	VertexBufferObject vbo;
		ElementBufferObject ebo;

        Cylinder3_Ptr<PRECISION> cylinder;

    private:
        const size_t segments = 30;

        std::vector<float> buildVertices();

		std::vector<unsigned int> buildIndices();
    };

    using CylinderGObject = CylinderGraphicsObject;

} // namespace HSim
