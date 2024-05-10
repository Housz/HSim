#include <scene/graphics_objects/triangle_mesh3_graphics_object.h>
#include "triangle_mesh3_graphics_object.h"

HSim::TriangleMesh3GObject::TriangleMesh3GObject()
{
}

HSim::TriangleMesh3GObject::TriangleMesh3GObject(const TriangleMesh3GObject &other)
    : GraphicsObject(other)
{
    mesh = std::make_shared<TriangleMesh3<PRECISION>>(*(other.mesh));

    vbo.create();
    ebo.create();

    buildRenderingData();
}

HSim::TriangleMesh3GObject::TriangleMesh3GObject(const TriangleMesh3_Ptr<PRECISION> mesh_, const BasicMaterial_Ptr material_)
    : GraphicsObject(material_)
{
    mesh = mesh_;

    vbo.create();
    ebo.create();

    buildRenderingData();
}

HSim::TriangleMesh3GObject::~TriangleMesh3GObject()
{
}

void HSim::TriangleMesh3GObject::buildRenderingData()
{
    auto mat = std::static_pointer_cast<HSim::BasicMaterial>(material);

    switch (mat->renderingType)
    {
    case RenderingType::NAIVE:
        buildRenderingDataNaive();
        break;
    case RenderingType::FLAT:
        buildRenderingDataFlat();
        break;
    case RenderingType::SMOOTH:
        buildRenderingDataSmooth();
        break;
    }
}

void HSim::TriangleMesh3GObject::draw(const RenderParams &renderParams)
{
    if (!isRendingDataValid())
    {
        std::cout << vao.id << " " << vbo.id << " " << ebo.id << "\n";
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

    vao.bind();
    if (mat->wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    switch (mat->renderingType)
    {
    case RenderingType::NAIVE:
        // drawNaive();
        glDrawElements(GL_TRIANGLES, mesh->numTrianlges() * 3, GL_UNSIGNED_INT, 0);
        break;
    case RenderingType::FLAT:
        // drawFlat();
        glDrawArrays(GL_TRIANGLES, 0, mesh->numTrianlges() * 3);
        break;
    case RenderingType::SMOOTH:
        // drawSmooth();
		glDrawElements(GL_TRIANGLES, mesh->numTrianlges() * 3, GL_UNSIGNED_INT, 0);
        break;
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    vao.unbind();
}

bool HSim::TriangleMesh3GObject::isRendingDataValid()
{
    return (vao.isValid() && vbo.isValid() && ebo.isValid());
}

void HSim::TriangleMesh3GObject::buildRenderingDataNaive()
{
    assert(mesh != nullptr);

    vao.bind();

    auto vertices = buildVerticesNaive();

    /** debug
    std::cout << "vertices:\n";
    size_t i = 0;
    for (auto ele : vertices)
    {
        std::cout << ele << " ";
        i++;
        if (!(i % 6))
            std::cout << std::endl;
    }
    std::cout << std::endl;
    */

    auto indices = buildIndices();

    /** debug
    std::cout << "\nindices:\n";
    i = 0;
    for (auto ele : indices)
    {
        std::cout << ele << " ";
        i++;
        if (!(i % 3))
            std::cout << std::endl;
    }
    std::cout << std::endl;
    */

    // vbo
    vbo.bind();
    vbo.allocate((unsigned int)vertices.size() * sizeof(float), GL_STATIC_DRAW);
    vbo.loadData(vertices.data(), (unsigned int)vertices.size() * sizeof(float), 0);

    vao.bindVBO(vbo, 0, 3, 6 * sizeof(float), (void *)0);
    vao.bindVBO(vbo, 1, 3, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    // ebo
    ebo.bind();
    ebo.allocate((unsigned int)indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);
    ebo.loadData(indices.data(), (unsigned int)indices.size() * sizeof(unsigned int), 0);

    vao.bindEBO(ebo);

    vao.unbind();
}

void HSim::TriangleMesh3GObject::buildRenderingDataFlat()
{
    assert(mesh != nullptr);

    vao.bind();

    auto vertices = buildVerticesFlat();

    // vbo
    vbo.bind();
    vbo.allocate((unsigned int)vertices.size() * sizeof(float), GL_STATIC_DRAW);
    vbo.loadData(vertices.data(), (unsigned int)vertices.size() * sizeof(float), 0);

    vao.bindVBO(vbo, 0, 3, 6 * sizeof(float), (void *)0);
    vao.bindVBO(vbo, 1, 3, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    vao.unbind();
}

void HSim::TriangleMesh3GObject::buildRenderingDataSmooth()
{
    assert(mesh != nullptr);

    auto vertices = buildVerticesSmooth();
    auto indices = buildIndices();

    // vbo
    vbo.bind();
    vbo.allocate((unsigned int)vertices.size() * sizeof(float), GL_STATIC_DRAW);
    vbo.loadData(vertices.data(), (unsigned int)vertices.size() * sizeof(float), 0);

    vao.bindVBO(vbo, 0, 3, 6 * sizeof(float), (void *)0);
    vao.bindVBO(vbo, 1, 3, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    // ebo
    ebo.bind();
    ebo.allocate((unsigned int)indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);
    ebo.loadData(indices.data(), (unsigned int)indices.size() * sizeof(unsigned int), 0);

    vao.bindEBO(ebo);

    vao.unbind();
}

std::vector<float> HSim::TriangleMesh3GObject::buildVerticesNaive()
{
    std::vector<float> vertices;

    std::vector<Vec3<float>> tempNormals;
    if (!mesh->hasNormal())
    {
        // compute normals
        // todo Flat-shading https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ

        tempNormals.resize(mesh->numPoints());

        for (size_t triIdx = 0; triIdx < mesh->numTrianlges(); triIdx++)
        {
            auto aIndex = mesh->pointIndices[triIdx][0];
            auto bIndex = mesh->pointIndices[triIdx][1];
            auto cIndex = mesh->pointIndices[triIdx][2];

            auto a = mesh->points[aIndex];
            auto b = mesh->points[bIndex];
            auto c = mesh->points[cIndex];

            auto normal = (b - a).cross(c - a).getNormalized();

            tempNormals[aIndex] = normal;
            tempNormals[bIndex] = normal;
            tempNormals[cIndex] = normal;
        }
    }

    for (size_t i = 0; i < mesh->numPoints(); i++)
    {
        auto point = mesh->points[i];
        
        // transform
        point = mesh->transform.mul(point);

        vertices.push_back(point.x);
        vertices.push_back(point.y);
        vertices.push_back(point.z);

        Vec3<float> normal;
        if (mesh->hasNormal())
        {
            normal = mesh->normals[i];
        }
        else
        {
            normal = tempNormals[i];
        }
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);
    }

    return vertices;
}

std::vector<float> HSim::TriangleMesh3GObject::buildVerticesFlat()
{
    std::vector<float> vertices;

    for (size_t triIdx = 0; triIdx < mesh->numTrianlges(); triIdx++)
    {
        auto aIndex = mesh->pointIndices[triIdx][0];
        auto bIndex = mesh->pointIndices[triIdx][1];
        auto cIndex = mesh->pointIndices[triIdx][2];

        auto a = mesh->points[aIndex];
        auto b = mesh->points[bIndex];
        auto c = mesh->points[cIndex];

        // transform
        a = mesh->transform.mul(a);
        b = mesh->transform.mul(b);
        c = mesh->transform.mul(c);

        auto normal = (b - a).cross(c - a).getNormalized();

        vertices.push_back(a.x);
        vertices.push_back(a.y);
        vertices.push_back(a.z);
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);
        vertices.push_back(b.x);
        vertices.push_back(b.y);
        vertices.push_back(b.z);
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);
        vertices.push_back(c.x);
        vertices.push_back(c.y);
        vertices.push_back(c.z);
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);
    }

    return vertices;
}

std::vector<float> HSim::TriangleMesh3GObject::buildVerticesSmooth()
{
    std::vector<float> vertices;

    std::vector<Vec3<float>> smoothNormals(mesh->numPoints());
    std::vector<size_t> normalCount(mesh->numPoints());

    for (size_t triIdx = 0; triIdx < mesh->numTrianlges(); triIdx++)
    {
        auto aIndex = mesh->pointIndices[triIdx][0];
        auto bIndex = mesh->pointIndices[triIdx][1];
        auto cIndex = mesh->pointIndices[triIdx][2];

        auto a = mesh->points[aIndex];
        auto b = mesh->points[bIndex];
        auto c = mesh->points[cIndex];

        auto normal = (b - a).cross(c - a).getNormalized();

        normalCount[aIndex]++;
        normalCount[bIndex]++;
        normalCount[cIndex]++;

        smoothNormals[aIndex] = normal / normalCount[aIndex] +
                                smoothNormals[aIndex] * (normalCount[aIndex] - 1) / normalCount[aIndex];
        smoothNormals[bIndex] = normal / normalCount[bIndex] +
                                smoothNormals[bIndex] * (normalCount[bIndex] - 1) / normalCount[bIndex];
        smoothNormals[cIndex] = normal / normalCount[cIndex] +
                                smoothNormals[cIndex] * (normalCount[cIndex] - 1) / normalCount[cIndex];
    }

    for (size_t i = 0; i < mesh->numPoints(); i++)
    {
        auto point = mesh->points[i];

        // transform
        point = mesh->transform.mul(point);

        vertices.push_back(point.x);
        vertices.push_back(point.y);
        vertices.push_back(point.z);

        auto normal = smoothNormals[i];

        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);
    }

    return vertices;
}

std::vector<unsigned int> HSim::TriangleMesh3GObject::buildIndices()
{
    std::vector<unsigned int> indices;
    for (auto element : mesh->pointIndices)
    {
        indices.push_back((unsigned int)element[0]);
        indices.push_back((unsigned int)element[1]);
        indices.push_back((unsigned int)element[2]);
    }
    return indices;
}
