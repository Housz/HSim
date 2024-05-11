#include <scene/graphics_objects/line_helper_graphics_object.h>
#include "line_helper_graphics_object.h"

HSim::LineHelperGObject::LineHelperGObject()
{
}

HSim::LineHelperGObject::LineHelperGObject(const LineHelperGObject &other)
    : GraphicsObject(other)
{
    lineHelper = std::make_shared<LineHelper>(*(other.lineHelper));

    vbo.create();

    buildRenderingData();
}

HSim::LineHelperGObject::LineHelperGObject(const LineHelper_Ptr lineHelper_, const LineMaterial_Ptr material_)
    : GraphicsObject(material_)
{
    lineHelper = lineHelper_;

    vbo.create();

    buildRenderingData();
}

HSim::LineHelperGObject::~LineHelperGObject()
{
}

void HSim::LineHelperGObject::clone(std::shared_ptr<GraphicsObject> &ptr)
{
    ptr = std::make_shared<LineHelperGObject>(*this);
}

bool HSim::LineHelperGObject::isRendingDataValid()
{
    return (vao.isValid() && vbo.isValid());
}

void HSim::LineHelperGObject::buildRenderingData()
{
	vao.bind();

    float vertices[6] =
        {
            lineHelper->start[0],
            lineHelper->start[1],
            lineHelper->start[2],
            lineHelper->end[0],
            lineHelper->end[1],
            lineHelper->end[2],
        };

    vbo.bind();
    vbo.allocate(6 * sizeof(float), GL_STATIC_DRAW);
    vbo.loadData(vertices, 6 * sizeof(float), 0);

    vao.bindVBO(vbo, 0, 3, 3 * sizeof(float), (void *)0);

	vao.unbind();
}

void HSim::LineHelperGObject::draw(const RenderParams &renderParams)
{
    if (!isRendingDataValid())
    {
        std::cout << vao.id << " " << vbo.id << "\n";
        buildRenderingData();
    }

    auto mat = std::static_pointer_cast<HSim::BasicMaterial>(material);
    auto color = mat->color;
    auto shader = mat->shader;

    // use shader with renderParams
    shader->use();
    shader->setFloat4("ourColor", color.r, color.g, color.b, 0.0f);
    shader->setVec3("lightPos", renderParams.lightPos.x, renderParams.lightPos.y, renderParams.lightPos.z);
    shader->setVec3("lightColor", renderParams.lightColor.r, renderParams.lightColor.g, renderParams.lightColor.b);

    glm::vec3 cameraPosition = glm::vec3(renderParams.transforms.view[3]);
    shader->setVec3("viewPos", cameraPosition);

    shader->setMat4("projection", renderParams.transforms.proj);
    shader->setMat4("view", renderParams.transforms.view);
    shader->setMat4("model", renderParams.transforms.model);

    glViewport(0, 0, renderParams.width, renderParams.height);

    // bind vao and draw
    vao.bind();
    glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 0, 6);
    vao.unbind();
}
