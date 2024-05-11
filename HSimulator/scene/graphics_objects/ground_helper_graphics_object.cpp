#include <scene/graphics_objects/ground_helper_graphics_object.h>
#include "ground_helper_graphics_object.h"

HSim::GroundHelperGObject::GroundHelperGObject()
{
}

HSim::GroundHelperGObject::GroundHelperGObject(const GroundHelperGObject &other)
	: GraphicsObject(other)
{
	groundHelper = std::make_shared<GroundHelper>(*(other.groundHelper));

	vbo.create();
	ebo.create();

	buildRenderingData();
}

HSim::GroundHelperGObject::~GroundHelperGObject()
{
}

void HSim::GroundHelperGObject::clone(std::shared_ptr<GraphicsObject> &ptr)
{
	ptr = std::make_shared<GroundHelperGObject>(*this);
}

HSim::GroundHelperGObject::GroundHelperGObject(const GroundHelper_Ptr groundHelper_, const BasicMaterial_Ptr material_)
	: GraphicsObject(material_)
{
	groundHelper = groundHelper_;

	vbo.create();
	ebo.create();

	buildRenderingData();
}

bool HSim::GroundHelperGObject::isRendingDataValid()
{
	return (vao.isValid() && vbo.isValid() && ebo.isValid());
}

void HSim::GroundHelperGObject::draw(const RenderParams &renderParams)
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
	glLineWidth(1.0f);
	glDrawElements(GL_LINES, numElements, GL_UNSIGNED_INT, 0);
	vao.unbind();
}

void HSim::GroundHelperGObject::buildRenderingData()
{
	assert(groundHelper != nullptr);

	vao.bind();

	std::vector<glm::vec3> vertices;
	std::vector<glm::uvec4> indices;

	auto slices = groundHelper->slices;

	for (int j = 0; j <= slices; ++j)
	{
		for (int i = 0; i <= slices; ++i)
		{
			float x = (float)i;
			float y = 0;
			float z = (float)j;
			vertices.push_back(glm::vec3(x - slices / 2, y, z - slices / 2));
		}
	}

	for (int j = 0; j < slices; ++j)
	{
		for (int i = 0; i < slices; ++i)
		{

			int row1 = j * (slices + 1);
			int row2 = (j + 1) * (slices + 1);

			indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
			indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));
		}
	}

	// vbo
	vbo.bind();
	vbo.allocate((unsigned int)vertices.size() * sizeof(glm::vec3), GL_STATIC_DRAW);
	vbo.loadData(vertices.data(), (unsigned int)vertices.size() * sizeof(glm::vec3), 0);

	vao.bindVBO(vbo, 0, 3, 3 * sizeof(float), (void *)0);

	// ebo
	ebo.allocate((unsigned int)indices.size() * sizeof(glm::uvec4), GL_STATIC_DRAW);
	ebo.loadData(indices.data(), (unsigned int)indices.size() * sizeof(glm::uvec4), 0);

	vao.bindEBO(ebo);

	numElements = indices.size() * 4;

	vao.unbind();
}