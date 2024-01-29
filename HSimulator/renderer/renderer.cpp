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
	shader = HSim::Shader("./resources/shaders/camera.vs", "./resources/shaders/camera.fs");

	// init ground helper
	ground.init(10, 10);
}

void HSim::Renderer::setScene(SceneGraph_ptr scene_)
{
	this->scene = scene_;
}

void HSim::Renderer::draw(RenderParams renderParams)
{
	// std::cout << "renderer draw" << std::endl;
	shader.use();
	shader.setFloat4("ourColor", 0.5, 0.1, 0.3, 0.0f);
	shader.setVec3("lightPos", 20, 20, 20);
	// shader.setVec3("viewPos", cameraPos);

		// 	renderParams.transforms.model = glm::mat4(1);
		// renderParams.transforms.view = camera->getViewMat();
		// renderParams.transforms.proj

	shader.setMat4("projection", renderParams.transforms.proj);
	shader.setMat4("view", renderParams.transforms.view);
	shader.setMat4("model", renderParams.transforms.model);


	glViewport(0, 0, renderParams.width, renderParams.height);
	// std::cout << 

	ground.draw();
}
