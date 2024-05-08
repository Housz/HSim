#include <scene/graphics_objects/sphere3_graphics_object.h>

HSim::Sphere3GObject::Sphere3GObject()
{
}

HSim::Sphere3GObject::Sphere3GObject(const Sphere3_Ptr<PRECISION> sphere_, const BasicMaterial_Ptr material_)
	: GraphicsObject(material_)
{
	sphere = sphere_;

	vbo.create();
	ebo.create();

	buildRenderingData();
}

HSim::Sphere3GObject::~Sphere3GObject()
{
}

void HSim::Sphere3GObject::buildRenderingData()
{
	assert(sphere != nullptr);

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

void HSim::Sphere3GObject::draw(const RenderParams &renderParams)
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
	// shader->setMat4("model", renderParams.transforms.model);

	glm::mat4 model = glm::mat4{1.f};
	// update transform.orientation identity
	const auto orientation = sphere->transform.orientation; 
	const auto translation = sphere->transform.translation;
	model = glm::rotate(model, orientation.angle(),
						glm::vec3(orientation.axis().x, orientation.axis().y, orientation.axis().z));
	model = glm::translate(model, glm::vec3(translation.x, translation.y, translation.z));

	// std::cout << glm::to_string(model);

	shader->setMat4("model", model);

	glViewport(0, 0, renderParams.width, renderParams.height);

	// bind vao and draw
	vao.bind();

	if (mat->wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	glDrawElements(GL_TRIANGLES, numSectors * numStacks * 2 * 3, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	vao.unbind();
}

bool HSim::Sphere3GObject::isRendingDataValid()
{
	return (vao.isValid() && vbo.isValid() && ebo.isValid());
}

std::vector<float> HSim::Sphere3GObject::buildVertices()
{
	std::vector<float> vertices;

	const auto radius = sphere->radius;
	const auto center = sphere->center;
	const auto transform = sphere->transform;

	for (size_t stack = 0; stack <= numStacks; stack++)
	{
		float phi = PI_HALF - PI * ((float)stack / numStacks);
		float y = radius * std::sin(phi);

		for (size_t sector = 0; sector <= numSectors; sector++)
		{
			float theta = PI_DOUBLE * ((float)sector / numSectors);

			float x = radius * std::cos(phi) * std::sin(theta);
			float z = radius * std::cos(phi) * std::cos(theta);

			// position for layout 0
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// position for layout 1
			vertices.push_back(x / radius);
			vertices.push_back(y / radius);
			vertices.push_back(z / radius);
		}
	}

	// transform
	// for (size_t i = 0; i < vertices.size(); i += 3)
	// {
	// 	// vertices[i] += center[0];
	// 	// vertices[i + 1] += center[1];
	// 	// vertices[i + 2] += center[2];

	// 	auto v = transform.mul({vertices[i], vertices[i + 1], vertices[i + 2]});
	// 	vertices[i] = v[0];
	// 	vertices[i + 1] = v[1];
	// 	vertices[i + 2] = v[2];
	// }

	return vertices;
}

std::vector<unsigned int> HSim::Sphere3GObject::buildIndices()
{
	std::vector<unsigned int> indices;
	// indices
	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1
	unsigned int k1, k2;

	for (size_t i = 0; i < numStacks; ++i)
	{
		k1 = i * (numSectors + 1);
		k2 = k1 + numSectors + 1;

		for (size_t j = 0; j < numSectors; j++, k1++, k2++)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (numStacks - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	return indices;
}