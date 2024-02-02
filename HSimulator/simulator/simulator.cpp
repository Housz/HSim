#include <simulator/simulator.h>
#include "simulator.h"

HSim::Simulator::Simulator()
{
}

HSim::Simulator::~Simulator()
{
}
void HSim::Simulator::init()
{
}

void HSim::Simulator::setScene(SceneGraph_ptr scene_)
{
	this->scene = scene_;
}

void HSim::Simulator::mainLoop()
{
	std::function<void(HSim::GameObject_ptr)> callback_anim = [&](HSim::GameObject_ptr go)
	{
		if (go->surface_ptr != nullptr && go->drawable)
		{
			std::cout << "callback_animation" << std::endl;

			auto surface = go->surface_ptr;

			std::unique_lock<std::mutex> lk(mtx);

			surface->transform.translation.x += .01;

			surface->updated = true;
			
			lk.unlock();

			std::cout << surface->transform.translation;

			// surface->serialize();
		}
	};

	// main loop
	for (;;)
	{
		std::cout << "simulator update" << std::endl;
		// scene->traverse(callback_anim);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
