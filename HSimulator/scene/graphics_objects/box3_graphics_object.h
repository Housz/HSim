#pragma once
#include <scene/graphics_objects/graphics_object.h>
#include <HSim/box3.h>

namespace HSim
{
    class Box3GraphicsObject : public GObject
    {
    public:
        Box3GraphicsObject();
        Box3GraphicsObject(const Box3_Ptr<PRECISION> box_, const BasicMaterial_Ptr material_);
        ~Box3GraphicsObject();

    public:
        void buildRenderingData() override;

        void draw(const RenderParams &renderParams) override;

        bool isRendingDataValid() override;

    public:
        VertexBufferObject vbo;

        Box3_Ptr<PRECISION> box = nullptr;

    };

    using Box3GObject = Box3GraphicsObject;
    
} // namespace HSim
