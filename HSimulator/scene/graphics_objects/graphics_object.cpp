#include <scene/graphics_objects/graphics_object.h>
#include "graphics_object.h"

HSim::GraphicsObject::GraphicsObject()
{
	// VAO init
	vao.create();
}

HSim::GraphicsObject::GraphicsObject(const GraphicsObject &graphicsObject_)
{
	// VAO init
	vao.create();

	material = std::make_shared<Material>(*(graphicsObject_.material));
}

HSim::GraphicsObject::GraphicsObject(const Material_Ptr material_)
{
	vao.create();

	material = material_;
}

HSim::GraphicsObject::~GraphicsObject()
{
}

bool HSim::GraphicsObject::isRendingDataValid()
{
	return false;
}

void HSim::GraphicsObject::draw(const RenderParams &renderParams)
{
}

void HSim::GraphicsObject::buildRenderingData()
{
}

/*********************************************************************/

// HSim::Sphere3GObject::Sphere3GObject()
// {
// }

// HSim::Sphere3GObject::Sphere3GObject(const Sphere3_Ptr<PRECISION> sphere_, const BasicMaterial_Ptr material_)
// 	: GraphicsObject(material_)
// {
// 	sphere = sphere_;

// 	vbo.create();
// 	ebo.create();

// 	buildRenderingData();
// }

// HSim::Sphere3GObject::~Sphere3GObject()
// {
// }

// void HSim::Sphere3GObject::buildRenderingData()
// {
// 	assert(sphere != nullptr);

// 	vao.bind();

// 	auto vertices = buildVertices();
// 	auto indices = buildIndices();

// 	// vbo
// 	vbo.bind();
// 	vbo.allocate((unsigned int)vertices.size() * sizeof(float), GL_STATIC_DRAW);
// 	vbo.loadData(vertices.data(), (unsigned int)vertices.size() * sizeof(float), 0);

// 	vao.bindVBO(vbo, 0, 3, 6 * sizeof(float), (void *)0);
// 	vao.bindVBO(vbo, 1, 3, 6 * sizeof(float), (void *)(3 * sizeof(float)));

// 	// ebo
// 	ebo.bind();
// 	ebo.allocate((unsigned int)indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);
// 	ebo.loadData(indices.data(), (unsigned int)indices.size() * sizeof(unsigned int), 0);

// 	vao.bindEBO(ebo);
// }

// void HSim::Sphere3GObject::draw(const RenderParams &renderParams)
// {
// 	if (!isRendingDataValid())
// 	{
// 		std::cout << vao.id << " " << vbo.id << " " << ebo.id << "\n";
// 		buildRenderingData();
// 	}

// 	auto mat = std::static_pointer_cast<HSim::BasicMaterial>(material);
// 	auto color = mat->color;
// 	auto shader = mat->shader;

// 	// use shader with renderParams
// 	shader->use();
// 	shader->setFloat4("ourColor", color.r, color.g, color.b, 0.0f);
// 	shader->setVec3("lightPos", 20, 15, 15);
// 	glm::vec3 cameraPosition = glm::vec3(renderParams.transforms.view[3]);
// 	shader->setVec3("viewPos", cameraPosition);

// 	shader->setMat4("projection", renderParams.transforms.proj);
// 	shader->setMat4("view", renderParams.transforms.view);
// 	shader->setMat4("model", renderParams.transforms.model);

// 	glViewport(0, 0, renderParams.width, renderParams.height);

// 	// bind vao and draw
// 	vao.bind();

// 	if (mat->wireframe)
// 	{
// 		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// 	}
// 	glDrawElements(GL_TRIANGLES, numSectors * numStacks * 2 * 3, GL_UNSIGNED_INT, 0);
// 	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

// 	vao.unbind();
// }

// bool HSim::Sphere3GObject::isRendingDataValid()
// {
// 	return (vao.isValid() && vbo.isValid() && ebo.isValid());
// }

// std::vector<float> HSim::Sphere3GObject::buildVertices()
// {
// 	std::vector<float> vertices;

// 	const auto radius = sphere->radius;
// 	const auto center = sphere->center;

// 	for (size_t stack = 0; stack <= numStacks; stack++)
// 	{
// 		float phi = PI_HALF - PI * ((float)stack / numStacks);
// 		float y = radius * std::sin(phi);

// 		for (size_t sector = 0; sector <= numSectors; sector++)
// 		{
// 			float theta = PI_DOUBLE * ((float)sector / numSectors);

// 			float x = radius * std::cos(phi) * std::sin(theta);
// 			float z = radius * std::cos(phi) * std::cos(theta);

// 			// position for layout 0
// 			vertices.push_back(x);
// 			vertices.push_back(y);
// 			vertices.push_back(z);

// 			// position for layout 1
// 			vertices.push_back(x / radius);
// 			vertices.push_back(y / radius);
// 			vertices.push_back(z / radius);
// 		}
// 	}

// 	// transform
// 	for (size_t i = 0; i < vertices.size(); i += 3)
// 	{
// 		// auto v = transform.mul({vertices[i], vertices[i + 1], vertices[i + 2]});

// 		vertices[i] += center[0];
// 		vertices[i + 1] += center[1];
// 		vertices[i + 2] += center[2];
// 	}

// 	return vertices;
// }

// std::vector<unsigned int> HSim::Sphere3GObject::buildIndices()
// {
// 	std::vector<unsigned int> indices;
// 	// indices
// 	//  k1--k1+1
// 	//  |  / |
// 	//  | /  |
// 	//  k2--k2+1
// 	unsigned int k1, k2;

// 	for (size_t i = 0; i < numStacks; ++i)
// 	{
// 		k1 = i * (numSectors + 1);
// 		k2 = k1 + numSectors + 1;

// 		for (size_t j = 0; j < numSectors; j++, k1++, k2++)
// 		{
// 			if (i != 0)
// 			{
// 				indices.push_back(k1);
// 				indices.push_back(k2);
// 				indices.push_back(k1 + 1);
// 			}

// 			if (i != (numStacks - 1))
// 			{
// 				indices.push_back(k1 + 1);
// 				indices.push_back(k2);
// 				indices.push_back(k2 + 1);
// 			}
// 		}
// 	}

// 	return indices;
// }

/*********************************************************************/

// HSim::GroundHelperGObject::GroundHelperGObject()
// {
// }

// HSim::GroundHelperGObject::~GroundHelperGObject()
// {
// }

// HSim::GroundHelperGObject::GroundHelperGObject(const GroundHelper_Ptr groundHelper_, const BasicMaterial_Ptr material_)
// 	: GraphicsObject(material_)
// {
// 	groundHelper = groundHelper_;

// 	vbo.create();
// 	ebo.create();

// 	buildRenderingData();
// }

// bool HSim::GroundHelperGObject::isRendingDataValid()
// {
// 	return (vao.isValid() && vbo.isValid() && ebo.isValid());
// }

// void HSim::GroundHelperGObject::draw(const RenderParams &renderParams)
// {
// 	if (!isRendingDataValid())
// 	{
// 		std::cout << vao.id << " " << vbo.id << " " << ebo.id << "\n";
// 		buildRenderingData();
// 	}

// 	auto mat = std::static_pointer_cast<HSim::BasicMaterial>(material);
// 	auto color = mat->color;
// 	auto shader = mat->shader;

// 	// use shader with renderParams
// 	shader->use();
// 	shader->setFloat4("ourColor", color.r, color.g, color.b, 0.0f);
// 	shader->setVec3("lightPos", 20, 15, 15);
// 	glm::vec3 cameraPosition = glm::vec3(renderParams.transforms.view[3]);
// 	shader->setVec3("viewPos", cameraPosition);

// 	shader->setMat4("projection", renderParams.transforms.proj);
// 	shader->setMat4("view", renderParams.transforms.view);
// 	shader->setMat4("model", renderParams.transforms.model);

// 	glViewport(0, 0, renderParams.width, renderParams.height);

// 	// bind vao and draw
// 	vao.bind();
// 	glLineWidth(1.0f);
// 	glDrawElements(GL_LINES, numElements, GL_UNSIGNED_INT, 0);
// 	vao.unbind();
// }

// void HSim::GroundHelperGObject::buildRenderingData()
// {
// 	std::vector<glm::vec3> vertices;
// 	std::vector<glm::uvec4> indices;

// 	auto slices = groundHelper->slices;

// 	for (int j = 0; j <= slices; ++j)
// 	{
// 		for (int i = 0; i <= slices; ++i)
// 		{
// 			float x = (float)i;
// 			float y = 0;
// 			float z = (float)j;
// 			vertices.push_back(glm::vec3(x - slices / 2, y, z - slices / 2));
// 		}
// 	}

// 	for (int j = 0; j < slices; ++j)
// 	{
// 		for (int i = 0; i < slices; ++i)
// 		{

// 			int row1 = j * (slices + 1);
// 			int row2 = (j + 1) * (slices + 1);

// 			indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
// 			indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));
// 		}
// 	}

// 	// vbo
// 	vbo.bind();
// 	vbo.allocate((unsigned int)vertices.size() * sizeof(glm::vec3), GL_STATIC_DRAW);
// 	vbo.loadData(vertices.data(), (unsigned int)vertices.size() * sizeof(glm::vec3), 0);

// 	vao.bindVBO(vbo, 0, 3, 3 * sizeof(float), (void *)0);

// 	// ebo
// 	ebo.allocate((unsigned int)indices.size() * sizeof(glm::uvec4), GL_STATIC_DRAW);
// 	ebo.loadData(indices.data(), (unsigned int)indices.size() * sizeof(glm::uvec4), 0);

// 	vao.bindEBO(ebo);

// 	numElements = indices.size() * 4;
// }

/*********************************************************************/

// HSim::LineHelperGObject::LineHelperGObject()
// {
// }

// HSim::LineHelperGObject::~LineHelperGObject()
// {
// }

// HSim::LineHelperGObject::LineHelperGObject(const LineHelper_Ptr lineHelper_, const BasicMaterial_Ptr material_)
// 	: GraphicsObject(material_)
// {
// 	lineHelper = lineHelper_;

// 	vbo.create();

// 	buildRenderingData();
// }

// bool HSim::LineHelperGObject::isRendingDataValid()
// {
// 	return (vao.isValid() && vbo.isValid());
// }

// void HSim::LineHelperGObject::buildRenderingData()
// {
// 	float vertices[6] =
// 	{
// 		lineHelper->start[0],
// 		lineHelper->start[1],
// 		lineHelper->start[2],
// 		lineHelper->end[0],
// 		lineHelper->end[1],
// 		lineHelper->end[2],
// 	};

// 	vbo.bind();
// 	vbo.allocate(6*sizeof(float), GL_STATIC_DRAW);
// 	vbo.loadData(vertices, 6*sizeof(float), 0);

// 	vao.bindVBO(vbo, 0, 3, 3 * sizeof(float), (void *)0);
// }

// void HSim::LineHelperGObject::draw(const RenderParams &renderParams)
// {
// 	if (!isRendingDataValid())
// 	{
// 		std::cout << vao.id << " " << vbo.id << "\n";
// 		buildRenderingData();
// 	}

// 	auto mat = std::static_pointer_cast<HSim::BasicMaterial>(material);
// 	auto color = mat->color;
// 	auto shader = mat->shader;

// 	// use shader with renderParams
// 	shader->use();
// 	shader->setFloat4("ourColor", color.r, color.g, color.b, 0.0f);
// 	shader->setVec3("lightPos", 20, 15, 15);
// 	glm::vec3 cameraPosition = glm::vec3(renderParams.transforms.view[3]);
// 	shader->setVec3("viewPos", cameraPosition);

// 	shader->setMat4("projection", renderParams.transforms.proj);
// 	shader->setMat4("view", renderParams.transforms.view);
// 	shader->setMat4("model", renderParams.transforms.model);

// 	glViewport(0, 0, renderParams.width, renderParams.height);

// 	// bind vao and draw
// 	vao.bind();
// 	glLineWidth(3.0f);
// 	glDrawArrays(GL_LINES, 0, 6);
// 	vao.unbind();
// }
