#include "aabb3_graphics_object.h"

HSim::AABB3GObject::AABB3GObject()
{
}


// HSim::AABB3GObject::AABB3GObject(const AABB3_Ptr<PRECISION> aabb_, const BasicMaterial_Ptr material_)
// 	: GraphicsObject(material_)
// {
// 	aabb = aabb_;
	
// 	vbo.create();

// 	buildRenderingData();
// }

HSim::AABB3GObject::AABB3GObject(const AABB3<PRECISION> aabb_, const BasicMaterial_Ptr material_)
	: GraphicsObject(material_)
{
	aabb = aabb_;
	
	vbo.create();

	buildRenderingData();
}


HSim::AABB3GObject::~AABB3GObject()
{
}

void HSim::AABB3GObject::buildRenderingData()
{
	// assert(aabb != nullptr);

	auto l = aabb.lowerCorner;
	auto u = aabb.upperCorner;

	float vertices[] =
	{
		u[0], u[1], u[2],	u[0], l[1], u[2],
		u[0], u[1], l[2],	u[0], l[1], l[2],		
		l[0], u[1], l[2],	l[0], l[1], l[2],	
		l[0], u[1], u[2],	l[0], l[1], u[2],	

		u[0], u[1], u[2],	u[0], u[1], l[2],
		u[0], u[1], l[2],	l[0], u[1], l[2],
		l[0], u[1], l[2],	l[0], u[1], u[2],
		l[0], u[1], u[2],	u[0], u[1], u[2],

		u[0], l[1], u[2],	u[0], l[1], l[2],
		u[0], l[1], l[2],	l[0], l[1], l[2],
		l[0], l[1], l[2],	l[0], l[1], u[2],
		l[0], l[1], u[2],	u[0], l[1], u[2],
	};

	vbo.bind();
	vbo.allocate(sizeof(vertices), GL_STATIC_DRAW);
    vbo.loadData(vertices, sizeof(vertices), 0);

    vao.bindVBO(vbo, 0, 3, 3 * sizeof(float), (void *)0);
}

bool HSim::AABB3GObject::isRendingDataValid()
{
	return (vao.isValid() && vbo.isValid());
}

void HSim::AABB3GObject::draw(const RenderParams &renderParams)
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
    glLineWidth(1.0f);
    glDrawArrays(GL_LINES, 0, 24);  
    vao.unbind();
}
