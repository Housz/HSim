#pragma once
#include <scene/graphics_objects/graphics_object.h>


namespace HSim
{
    class LineHelperGObject : public GObject
    {
    public:
        LineHelperGObject();
        LineHelperGObject(const LineHelper_Ptr lineHelper_, const LineMaterial_Ptr material_);
        ~LineHelperGObject();

        void buildRenderingData() override;

        void draw(const RenderParams &renderParams) override;

        bool isRendingDataValid() override;

    public:
        VertexBufferObject vbo;

        LineHelper_Ptr lineHelper = nullptr;
    };

} // namespace HSim
