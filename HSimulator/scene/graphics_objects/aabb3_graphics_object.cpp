#include <scene/graphics_objects/aabb3_graphics_object.h>
#include "aabb3_graphics_object.h"

HSim::AABB3GraphicsObject::AABB3GraphicsObject()
{
}

HSim::AABB3GraphicsObject::AABB3GraphicsObject(const AABB3GraphicsObject &other)
	: GraphicsObject(other), aabb(other.aabb)
{
	vbo.create();

	buildRenderingData();
}

// HSim::AABB3GObject::AABB3GObject(const AABB3_Ptr<PRECISION> aabb_, const BasicMaterial_Ptr material_)
// 	: GraphicsObject(material_)
// {
// 	aabb = aabb_;

// 	vbo.create();

// 	buildRenderingData();
// }

HSim::AABB3GraphicsObject::AABB3GraphicsObject(const AABB3<PRECISION> aabb_, const BasicMaterial_Ptr material_)
	: GraphicsObject(material_)
{
	aabb = aabb_;

	vbo.create();

	buildRenderingData();
}

HSim::AABB3GraphicsObject::~AABB3GraphicsObject()
{
}

void HSim::AABB3GraphicsObject::clone(std::shared_ptr<GraphicsObject> &ptr)
{
	ptr = std::make_shared<AABB3GraphicsObject>(*this);
}

void HSim::AABB3GraphicsObject::buildRenderingData()
{
	// assert(aabb != nullptr);

	auto l = aabb.lowerCorner;
	auto u = aabb.upperCorner;

	float vertices[] =
		{
			u[0],
			u[1],
			u[2],
			u[0],
			l[1],
			u[2],
			u[0],
			u[1],
			l[2],
			u[0],
			l[1],
			l[2],
			l[0],
			u[1],
			l[2],
			l[0],
			l[1],
			l[2],
			l[0],
			u[1],
			u[2],
			l[0],
			l[1],
			u[2],

			u[0],
			u[1],
			u[2],
			u[0],
			u[1],
			l[2],
			u[0],
			u[1],
			l[2],
			l[0],
			u[1],
			l[2],
			l[0],
			u[1],
			l[2],
			l[0],
			u[1],
			u[2],
			l[0],
			u[1],
			u[2],
			u[0],
			u[1],
			u[2],

			u[0],
			l[1],
			u[2],
			u[0],
			l[1],
			l[2],
			u[0],
			l[1],
			l[2],
			l[0],
			l[1],
			l[2],
			l[0],
			l[1],
			l[2],
			l[0],
			l[1],
			u[2],
			l[0],
			l[1],
			u[2],
			u[0],
			l[1],
			u[2],
		};

	vao.bind();

	vbo.bind();
	vbo.allocate(sizeof(vertices), GL_STATIC_DRAW);
	vbo.loadData(vertices, sizeof(vertices), 0);

	vao.bindVBO(vbo, 0, 3, 3 * sizeof(float), (void *)0);

	vao.unbind();
}

bool HSim::AABB3GraphicsObject::isRendingDataValid()
{
	return (vao.isValid() && vbo.isValid());
}

void HSim::AABB3GraphicsObject::draw(const RenderParams &renderParams)
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
	glLineWidth(1.0f);
	glDrawArrays(GL_LINES, 0, 24);
	vao.unbind();
}
