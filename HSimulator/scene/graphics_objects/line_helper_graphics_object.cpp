#include <scene/graphics_objects/line_helper_graphics_object.h>

HSim::LineHelperGObject::LineHelperGObject()
{
}

HSim::LineHelperGObject::~LineHelperGObject()
{
}

HSim::LineHelperGObject::LineHelperGObject(const LineHelper_Ptr lineHelper_, const BasicMaterial_Ptr material_)
    : GraphicsObject(material_)
{
    lineHelper = lineHelper_;

    vbo.create();

    buildRenderingData();
}

bool HSim::LineHelperGObject::isRendingDataValid()
{
    return (vao.isValid() && vbo.isValid());
}

void HSim::LineHelperGObject::buildRenderingData()
{
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
    shader->setVec3("lightPos", 20, 15, 15);
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