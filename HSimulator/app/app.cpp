#include <app/app.h>

HSim::App::App()
{
}

HSim::App::~App()
{
}

void HSim::App::init(size_t width, size_t height)
{
	window = std::make_shared<RenderWindow>();
	window->init(width, height);

	simulator = std::make_shared<Simulator>();
}

void HSim::App::mainLoop()
{
	
}

void HSim::App::setScene(SceneGraph_ptr scene_)
{
	scene = scene_;
	window->scene = scene;
	simulator->scene = scene;
}

void HSim::App::setWindowTitle(const std::string &title_)
{
}