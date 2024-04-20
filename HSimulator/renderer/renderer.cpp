#include "renderer.h"

HSim::Renderer::Renderer()
{
}

HSim::Renderer::~Renderer()
{
}

void HSim::Renderer::init()
{
	// init shader
	// shader = HSim::Shader("./resources/shaders/basic.vs", "./resources/shaders/basic.fs");
}

void HSim::Renderer::setScene(SceneGraph_ptr scene_)
{
	this->scene = scene_;
}

// void HSim::Renderer::draw(RenderParams renderParams)
// {
// 	// std::cout << "renderer draw" << std::endl;
// 	shader.use();
// 	shader.setFloat4("ourColor", 0.5, 0.5, 0.5, 0.0f);
// 	shader.setVec3("lightPos", 20, 15, 15);

// 	glm::vec3 cameraPosition = glm::vec3(renderParams.transforms.view[3]);
// 	shader.setVec3("viewPos", cameraPosition);

// 	shader.setMat4("projection", renderParams.transforms.proj);
// 	shader.setMat4("view", renderParams.transforms.view);
// 	shader.setMat4("model", renderParams.transforms.model);

// 	glViewport(0, 0, renderParams.width, renderParams.height);

// 	// sphere.draw();
// 	ground.draw();

// 	// std::unique_lock<std::mutex> lk(mtx);
// 	this->scene->serialize();
// 	// lk.unlock();
// 	this->scene->draw();
// }

void HSim::Renderer::draw(const RenderParams& renderParams)
{
	// sphere.draw();
	ground.draw();

	scene->draw(renderParams);
}
