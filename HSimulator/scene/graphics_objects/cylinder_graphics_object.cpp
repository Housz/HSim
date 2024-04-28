#include <scene/graphics_objects/cylinder_graphics_object.h>

HSim::CylinderGraphicsObject::CylinderGraphicsObject()
{
}

HSim::CylinderGraphicsObject::CylinderGraphicsObject(const Cylinder3_Ptr<PRECISION> cylinder_, const BasicMaterial_Ptr material_)
    : GraphicsObject(material_)
{
    cylinder = cylinder_;

	vbo.create();
	ebo.create();

	buildRenderingData();
}

HSim::CylinderGraphicsObject::~CylinderGraphicsObject()
{
}

void HSim::CylinderGraphicsObject::buildRenderingData()
{
    assert(cylinder != nullptr);

    vao.bind();

	auto vertices = buildVertices();
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

void HSim::CylinderGraphicsObject::draw(const RenderParams &renderParams)
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

	// bind vao and draw
	vao.bind();

	if (mat->wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	glDrawElements(GL_TRIANGLES, segments * 2 * 3 + segments * 3 *2, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	vao.unbind();
}

bool HSim::CylinderGraphicsObject::isRendingDataValid()
{
    return (vao.isValid() && vbo.isValid() && ebo.isValid());
}

std::vector<float> HSim::CylinderGraphicsObject::buildVertices()
{
    std::vector<float> vertices;

    auto height = cylinder->height;
    auto radiusTop = cylinder->radiusTop;
    auto radiusBottom = cylinder->radiusBottom;

    auto centerTop = Vec3f(0.0, height / 2.0, 0.0);
    auto centerBottom = Vec3f(0.0, -height / 2.0, 0.0);

    auto angleStep = PI_DOUBLE / segments;

    // top circle
    for (size_t i = 0; i <= segments; i++)
    {
        // x
        vertices.push_back(radiusTop * std::cos(i * angleStep));
        // y
        vertices.push_back(height / 2.0);
        // z
        vertices.push_back(-radiusTop * std::sin(i * angleStep));

        // normal
        Vec3f n(radiusTop * std::cos(i * angleStep), 0, -radiusTop * std::sin(i * angleStep));
        n.normalize();
        vertices.push_back(n.x);
        vertices.push_back(n.y);
        vertices.push_back(n.z);
    }

    // bottom circle
    for (size_t i = 0; i <= segments; i++)
    {
        // x
        vertices.push_back(radiusBottom * std::cos(i * angleStep));
        // y
        vertices.push_back(-height / 2.0);
        // z
        vertices.push_back(-radiusBottom * std::sin(i * angleStep));
        // normal
        Vec3f n(radiusBottom * std::cos(i * angleStep), 0, -radiusBottom * std::sin(i * angleStep));
        n.normalize();
        vertices.push_back(n.x);
        vertices.push_back(n.y);
        vertices.push_back(n.z);
    }

    // top center
    vertices.push_back(centerTop.x);
    vertices.push_back(centerTop.y);
    vertices.push_back(centerTop.z);
    vertices.push_back(0);
    vertices.push_back(1);
    vertices.push_back(0);

    // bottom center
    vertices.push_back(centerBottom.x);
    vertices.push_back(centerBottom.y);
    vertices.push_back(centerBottom.z);
    vertices.push_back(0);
    vertices.push_back(-1);
    vertices.push_back(0);

    return vertices;
}

std::vector<unsigned int> HSim::CylinderGraphicsObject::buildIndices()
{
    std::vector<unsigned int> indices;

    // side 
    for (size_t i = 0; i < segments; i++)
    {
        indices.push_back(i);
        indices.push_back(i + segments + 2);
        indices.push_back(i + segments + 1);

        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + segments + 2);
    }

    // top circle
    for (size_t i = 0; i < segments; i++)
    {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back((segments + 1) * 2);
    }

    // bottom circle
    for (size_t i = 0; i < segments; i++)
    {
        indices.push_back(i + segments + 1);
        indices.push_back(i + segments + 2);
        indices.push_back((segments + 1) * 2 + 1);
    }

    return indices;
}
