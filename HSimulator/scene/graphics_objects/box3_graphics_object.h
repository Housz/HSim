#pragma once
#include <scene/graphics_objects/graphics_object.h>
#include <HSim/box3.h>

namespace HSim
{
    class Box3GObject : public GObject
    {
    public:
        Box3GObject();
        Box3GObject(const Box3_Ptr<PRECISION> box_, const BasicMaterial_Ptr material_);
        ~Box3GObject();

    public:
        void buildRenderingData() override;

        void draw(const RenderParams &renderParams) override;

        bool isRendingDataValid() override;

    public:
        VertexBufferObject vbo;

        Box3_Ptr<PRECISION> box = nullptr;

    };
} // namespace HSim
