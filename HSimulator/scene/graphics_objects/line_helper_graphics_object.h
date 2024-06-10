#pragma once
#include <scene/graphics_objects/graphics_object.h>

namespace HSim
{
    class LineHelperGObject : public GObject
    {
    public:
        LineHelperGObject();
        LineHelperGObject(const LineHelperGObject& other);
        LineHelperGObject(const LineHelper_Ptr<PRECISION> lineHelper_, const LineMaterial_Ptr material_);
        ~LineHelperGObject();

        void clone(std::shared_ptr<GraphicsObject> &ptr) override;
        
    public:
        void buildRenderingData() override;

        void draw(const RenderParams &renderParams) override;

        bool isRendingDataValid() override;

    public:
        VertexBufferObject vbo;

        LineHelper_Ptr<PRECISION> lineHelper = nullptr;
    };

} // namespace HSim
