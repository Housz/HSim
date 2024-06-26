#include <scene/graphics_objects/bvh3_graphics_object.h>
#include "bvh3_graphics_object.h"

HSim::BVH3GraphicsObject::BVH3GraphicsObject()
{
}

HSim::BVH3GraphicsObject::BVH3GraphicsObject(const BVH3GraphicsObject &other)
	: GraphicsObject(other)
{
	bvh = std::make_shared<BVH3<PRECISION>>(*(other.bvh));

	vbo.create();

	buildRenderingData();
}

HSim::BVH3GraphicsObject::BVH3GraphicsObject(const BVH3_Ptr<PRECISION> bvh_, const BasicMaterial_Ptr material_)
	: GraphicsObject(material_)
{
	bvh = bvh_;

	vbo.create();

	buildRenderingData();
}

HSim::BVH3GraphicsObject::~BVH3GraphicsObject()
{
}

void HSim::BVH3GraphicsObject::clone(std::shared_ptr<GraphicsObject> &ptr)
{
	ptr = std::make_shared<BVH3GraphicsObject>(*this);
}

void HSim::BVH3GraphicsObject::buildRenderingData()
{
	assert(bvh != nullptr);

	std::vector<float> vertices;

	auto callback = [&](BVH3Node_Ptr node)
	{
		// if (node->depth != 0)
		// {
		// 	return;
		// }

		auto l = node->aabb.lowerCorner;
		auto u = node->aabb.upperCorner;

		vertices.insert(vertices.end(), {
											(float)u[0],
											(float)u[1],
											(float)u[2],
											(float)u[0],
											(float)l[1],
											(float)u[2],
											(float)u[0],
											(float)u[1],
											(float)l[2],
											(float)u[0],
											(float)l[1],
											(float)l[2],
											(float)l[0],
											(float)u[1],
											(float)l[2],
											(float)l[0],
											(float)l[1],
											(float)l[2],
											(float)l[0],
											(float)u[1],
											(float)u[2],
											(float)l[0],
											(float)l[1],
											(float)u[2],

											(float)u[0],
											(float)u[1],
											(float)u[2],
											(float)u[0],
											(float)u[1],
											(float)l[2],
											(float)u[0],
											(float)u[1],
											(float)l[2],
											(float)l[0],
											(float)u[1],
											(float)l[2],
											(float)l[0],
											(float)u[1],
											(float)l[2],
											(float)l[0],
											(float)u[1],
											(float)u[2],
											(float)l[0],
											(float)u[1],
											(float)u[2],
											(float)u[0],
											(float)u[1],
											(float)u[2],

											(float)u[0],
											(float)l[1],
											(float)u[2],
											(float)u[0],
											(float)l[1],
											(float)l[2],
											(float)u[0],
											(float)l[1],
											(float)l[2],
											(float)l[0],
											(float)l[1],
											(float)l[2],
											(float)l[0],
											(float)l[1],
											(float)l[2],
											(float)l[0],
											(float)l[1],
											(float)u[2],
											(float)l[0],
											(float)l[1],
											(float)u[2],
											(float)u[0],
											(float)l[1],
											(float)u[2],
										});
	};

	bvh->traverse(callback);

	vao.bind();

	vbo.bind();
	vbo.allocate(vertices.size() * sizeof(float), GL_STATIC_DRAW);
	vbo.loadData(vertices.data(), vertices.size() * sizeof(float), 0);

	vao.bindVBO(vbo, 0, 3, 3 * sizeof(float), (void *)0);

	vao.unbind();

	numElements = vertices.size() / 3;
}

void HSim::BVH3GraphicsObject::draw(const RenderParams &renderParams)
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
	glDrawArrays(GL_LINES, 0, numElements);

	vao.unbind();
}

bool HSim::BVH3GraphicsObject::isRendingDataValid()
{
	return (vao.isValid() && vbo.isValid());
}