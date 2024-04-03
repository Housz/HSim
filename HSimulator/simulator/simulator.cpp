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

			surface->resetStatusFlags();

			lk.unlock();

			std::cout << surface->transform.translation;

			// surface->serialize();
		}

		if (go->grid_ptr != nullptr && go->drawable)
		{
			auto grid = std::dynamic_pointer_cast<HSim::CellCenterScalarGrid3<PRECISION>>(go->grid_ptr);

			std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution(-5, 5);

			auto fillGrid = [&](size_t i, size_t j, size_t k)
			{
				int dice_roll = distribution(generator);

				(*grid)(i, j, k) = dice_roll;
			};

			grid->parallelForEachCell(fillGrid);

			grid->updated = true;

		}
	};

	// main loop
	for (;;)
	{
		// std::cout << "simulator update" << std::endl;
		// scene->traverse(callback_anim);
		// std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
