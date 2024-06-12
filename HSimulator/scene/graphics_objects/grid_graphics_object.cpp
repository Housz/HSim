#include "grid_graphics_object.h"

HSim::CellCenterScalarGrid3GraphicsObject::CellCenterScalarGrid3GraphicsObject()
{
}

HSim::CellCenterScalarGrid3GraphicsObject::CellCenterScalarGrid3GraphicsObject(const CellCenterScalarGrid3GraphicsObject &other)
	: GraphicsObject(other)
{
	grid = std::make_shared<CellCenterScalarGrid3<PRECISION>>(*(other.grid));

	vbo.create();

	vbo.allocate((unsigned int)(grid->sizeX() * grid->sizeY() * grid->sizeZ() * 6) * sizeof(float), GL_DYNAMIC_DRAW);

	buildRenderingData();
}

HSim::CellCenterScalarGrid3GraphicsObject::CellCenterScalarGrid3GraphicsObject(const CellCenterScalarGrid3_Ptr<PRECISION> grid_, const PointMaterial_Ptr material_)
	: GraphicsObject(material_)
{
	grid = grid_;

	vbo.create();

	vbo.allocate((unsigned int)(grid->sizeX() * grid->sizeY() * grid->sizeZ() * 6) * sizeof(float), GL_DYNAMIC_DRAW);

	buildRenderingData();
}

HSim::CellCenterScalarGrid3GraphicsObject::~CellCenterScalarGrid3GraphicsObject()
{
}

void HSim::CellCenterScalarGrid3GraphicsObject::clone(std::shared_ptr<GraphicsObject> &ptr)
{
	ptr = std::make_shared<CellCenterScalarGrid3GraphicsObject>(*this);
}

bool HSim::CellCenterScalarGrid3GraphicsObject::isRendingDataValid()
{
	return (vao.isValid() && vbo.isValid());
}

void HSim::CellCenterScalarGrid3GraphicsObject::draw(const RenderParams &renderParams)
{
	if (!isRendingDataValid())
	{
		std::cout << "CellCenterScalarGrid3GraphicsObject RendingDataInValid" << vao.id << " " << vbo.id << "\n";
		buildRenderingData();
	}

	auto mat = std::static_pointer_cast<HSim::PointMaterial>(material);
	// auto color = mat->color;
	auto shader = mat->shader;

	// use shader with renderParams
	shader->use();
	// shader->setFloat4("ourColor", color.r, color.g, color.b, 0.0f);
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
	glPointSize(4.0f);
	glDrawArrays(GL_POINTS, 0, numElements);
	vao.unbind();
}

void HSim::CellCenterScalarGrid3GraphicsObject::buildRenderingData()
{
	std::vector<float> vertices;

	auto func = [&](size_t i, size_t j, size_t k)
	{
		auto data = grid->dataAt(i, j, k);
		if (data > 0)
		{
			auto p = grid->positionAt(i, j, k);
			vertices.push_back((float)p.x);
			vertices.push_back((float)p.y);
			vertices.push_back((float)p.z);
			vertices.push_back((float)(data / 1.0));
			vertices.push_back((float)(1.0 - data / 1.0));
			vertices.push_back((float)(1.0 - data / 1.0));
		}
	};

	grid->forEachCell(func);

	vao.bind();

	vbo.bind();
	// vbo.allocate((unsigned int)vertices.size() * sizeof(float), GL_DYNAMIC_DRAW);
	vbo.loadData(vertices.data(), (unsigned int)vertices.size() * sizeof(float), 0);

	vao.bindVBO(vbo, 0, 3, 6 * sizeof(float), (void *)0);
	vao.bindVBO(vbo, 1, 3, 6 * sizeof(float), (void *)(3 * sizeof(float)));

	vao.unbind();

	numElements = vertices.size() / 3;
}

/**********************************************************************************/

HSim::FaceCenterGrid3GraphicsObject::FaceCenterGrid3GraphicsObject()
{
}

HSim::FaceCenterGrid3GraphicsObject::~FaceCenterGrid3GraphicsObject()
{
}

HSim::FaceCenterGrid3GraphicsObject::FaceCenterGrid3GraphicsObject(const FaceCenterGrid3GraphicsObject &other)
	: GraphicsObject(other)
{
	grid = std::make_shared<FaceCenterGrid3<PRECISION>>(*(other.grid));

	vbo.create();

	vbo.allocate((unsigned int)(grid->sizeX() * grid->sizeY() * grid->sizeZ() * 6) * sizeof(float), GL_DYNAMIC_DRAW);

	buildRenderingData();
}

HSim::FaceCenterGrid3GraphicsObject::FaceCenterGrid3GraphicsObject(const FaceCenterGrid3_Ptr<PRECISION> grid_, const LineFieldMaterial_Ptr material_)
	: GraphicsObject(material_)
{
	grid = grid_;

	vbo.create();

	vbo.allocate((unsigned int)(grid->sizeX() * grid->sizeY() * grid->sizeZ() * 6) * sizeof(float), GL_DYNAMIC_DRAW);

	buildRenderingData();
}

void HSim::FaceCenterGrid3GraphicsObject::clone(std::shared_ptr<GraphicsObject> &ptr)
{
	ptr = std::make_shared<FaceCenterGrid3GraphicsObject>(*this);
}

void HSim::FaceCenterGrid3GraphicsObject::buildRenderingData()
{

	std::vector<float> vertices;

	auto spacing = grid->gridSpacing.x;
	spacing /= 2.;

	auto callback = [&](size_t i, size_t j, size_t k)
	{
		auto startPos = grid->positionAt(i, j, k);

		startPos = {0, 0, 0};

		auto vel = grid->dataAtCellCenter(i, j, k);
		auto direction = vel.getNormalized();
		// auto endPos = startPos + spacing * direction;
		auto endPos = startPos + vel;
		endPos = {10, 20, 30};

		// std::cout << i << " " << j << " " << k << std::endl;
		// std::cout << startPos;
		// std::cout << endPos;

		// start position
		// vertices.push_back(startPos.x);
		// vertices.push_back(startPos.y);
		// vertices.push_back(startPos.z);
		vertices.push_back(0.);
		vertices.push_back(0.);
		vertices.push_back(0.);
		// start color
		// vertices.push_back(vel.length() / 5.0);
		// vertices.push_back(1.0 - vel.length() / 5.0);
		// vertices.push_back(1.0 - vel.length() / 5.0);
		vertices.push_back(1.);
		vertices.push_back(0.);
		vertices.push_back(0.);

		// end position
		// vertices.push_back(endPos.x);
		// vertices.push_back(endPos.y);
		// vertices.push_back(endPos.z);
		vertices.push_back(10.);
		vertices.push_back(10.);
		vertices.push_back(10.);
		// end color
		// vertices.push_back(vel.length() / 5.0);
		// vertices.push_back(1.0 - vel.length() / 5.0);
		// vertices.push_back(1.0 - vel.length() / 5.0);
		vertices.push_back(1.);
		vertices.push_back(0.);
		vertices.push_back(0.);
	};

	grid->forEachCell(callback);

	// std::cout << grid->gridResolution;
	// std::cout << vertices.size() << std::endl;

	vao.bind();
	vbo.bind();

	vbo.loadData(vertices.data(), (unsigned int)vertices.size() * sizeof(float), 0);

	vao.bindVBO(vbo, 0, 3, 6 * sizeof(float), (void *)0);
	vao.bindVBO(vbo, 1, 3, 6 * sizeof(float), (void *)(3 * sizeof(float)));

	vao.unbind();

	numElements = vertices.size() / 6;
}

void HSim::FaceCenterGrid3GraphicsObject::draw(const RenderParams &renderParams)
{
	if (!isRendingDataValid())
	{
		std::cout << "FaceCenterGrid3GraphicsObject RendingDataInValid" << vao.id << " " << vbo.id << "\n";
		buildRenderingData();
	}

	auto mat = std::static_pointer_cast<HSim::PointMaterial>(material);
	auto shader = mat->shader;

	// use shader with renderParams
	shader->use();

	shader->setMat4("projection", renderParams.transforms.proj);
	shader->setMat4("view", renderParams.transforms.view);
	shader->setMat4("model", renderParams.transforms.model);

	glViewport(0, 0, renderParams.width, renderParams.height);

	// bind vao and draw
	vao.bind();
	// glLineWidth(2.0);
	glDrawArrays(GL_LINES, 0, numElements);
	// glDrawArrays(GL_TRIANGLES, 0, numElements);
	// glLineWidth(1.0);
	vao.unbind();
}

bool HSim::FaceCenterGrid3GraphicsObject::isRendingDataValid()
{
	return (vao.isValid() && vbo.isValid());
}
