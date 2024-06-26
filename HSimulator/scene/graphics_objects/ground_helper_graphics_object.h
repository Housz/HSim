#pragma once
#include <scene/graphics_objects/graphics_object.h>


namespace HSim
{
    class GroundHelperGObject : public GObject
    {
    public:
        GroundHelperGObject();
        GroundHelperGObject(const GroundHelperGObject& other);
        GroundHelperGObject(const GroundHelper_Ptr<PRECISION> groundHelper_, const BasicMaterial_Ptr material_);
        ~GroundHelperGObject();

        void clone(std::shared_ptr<GraphicsObject> &ptr) override;
        
    public:

        void buildRenderingData() override;

        void draw(const RenderParams &renderParams) override;

        bool isRendingDataValid() override;

    public:
        VertexBufferObject vbo;
        ElementBufferObject ebo;

        GroundHelper_Ptr<PRECISION> groundHelper = nullptr;

    private:
        size_t numElements;
    };
} // namespace HSim
