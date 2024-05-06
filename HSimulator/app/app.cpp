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
	// std::thread renderer_thread(&HSim::RenderWindow::mainLoop, window);
	// renderer_thread.join();

	// std::mutex mtx;

	// std::unique_lock<std::mutex> lk(mtx);
	// lk.unlock();

	std::thread simulator_thread(&HSim::Simulator::mainLoop, simulator);

	window->mainLoop();

	simulator->mainLoop();

	simulator_thread.join();
}

void HSim::App::setScene(SceneGraph_ptr scene_)
{
	scene = scene_;
	// window->scene = scene;
	window->setScene(scene_);

	// simulator->scene = scene;
	simulator->setScene(scene_);
}

void HSim::App::setWindowTitle(const std::string &title_)
{
	
}
