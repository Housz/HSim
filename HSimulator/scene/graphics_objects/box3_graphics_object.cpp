#include <scene/graphics_objects/box3_graphics_object.h>
#include "box3_graphics_object.h"

HSim::Box3GraphicsObject::Box3GraphicsObject()
{
}

HSim::Box3GraphicsObject::Box3GraphicsObject(const Box3GraphicsObject &box3GraphicsObject)
    : GraphicsObject(box3GraphicsObject)
{
    box = std::make_shared<Box3<PRECISION>>(*(box3GraphicsObject.box));

    vbo.create();

    buildRenderingData();
}

HSim::Box3GraphicsObject::Box3GraphicsObject(const Box3_Ptr<PRECISION> box_, const BasicMaterial_Ptr material_)
    : GraphicsObject(material_)
{
    box = box_;

    vbo.create();

    buildRenderingData();
}

HSim::Box3GraphicsObject::~Box3GraphicsObject()
{
}

void HSim::Box3GraphicsObject::clone(std::shared_ptr<GraphicsObject> &ptr)
{
    ptr = std::make_shared<Box3GraphicsObject>(*this);
}

void HSim::Box3GraphicsObject::buildRenderingData()
{
    assert(box != nullptr);

    vao.bind();

    auto lowerCorner = box->lowerCorner;
    auto upperCorner = box->upperCorner;
    auto transform = box->transform;

    float vertices[] = {
        lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
        upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
        upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,

        upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
        lowerCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
        lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,

        lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
        upperCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
        upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,

        upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
        lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
        lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,

        lowerCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,
        upperCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,
        upperCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,

        upperCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,
        lowerCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,
        lowerCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,

        lowerCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,
        upperCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,
        upperCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,

        upperCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,
        lowerCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,
        lowerCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,

        lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,
        lowerCorner[0], upperCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,
        lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,

        lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,
        lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,
        lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,

        upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f,
        upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f,
        upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,

        upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,
        upperCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,
        upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f};

    // transform
    for (size_t i = 0; i < sizeof(vertices) / sizeof(float); i += 3)
    {
        // vertices[i], vertices[i+1], vertices[i+2]
        auto v = transform.mul({vertices[i], vertices[i + 1], vertices[i + 2]});
        vertices[i] = v[0];
        vertices[i + 1] = v[1];
        vertices[i + 2] = v[2];
    }

    // vbo
    vbo.bind();
    vbo.allocate((unsigned int)sizeof(vertices), GL_STATIC_DRAW);
    vbo.loadData(vertices, (unsigned int)sizeof(vertices), 0);

    vao.bindVBO(vbo, 0, 3, 6 * sizeof(float), (void *)0);
    vao.bindVBO(vbo, 1, 3, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    vao.unbind();
}

void HSim::Box3GraphicsObject::draw(const RenderParams &renderParams)
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

    if (mat->wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    vao.unbind();
}

bool HSim::Box3GraphicsObject::isRendingDataValid()
{
    return (vao.isValid() && vbo.isValid());
}
