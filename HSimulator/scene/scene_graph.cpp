#include <scene/scene_graph.h>
#include "scene_graph.h"

HSim::SceneGraph::SceneGraph()
{
}

HSim::SceneGraph::~SceneGraph()
{
}

void HSim::SceneGraph::traverse(std::function<void(GameObject_ptr)> &callback)
{
	traverse(callback, root);
}

void HSim::SceneGraph::traverse(std::function<void(GameObject_ptr)> &callback, GameObject_ptr go)
{
	callback(go);

	if (go->isLeaf())
	{
		return;
	}
	else
	{
		for (auto child : go->children)
		{
			traverse(callback, child);
		}
	}
}

void HSim::SceneGraph::serialize()
{
    // float r = 0;

	std::function<void(HSim::GameObject_ptr)> callback_serialize = [&](HSim::GameObject_ptr go)
	{
		if (go->surface_ptr != nullptr && go->drawable)
		{
			// std::cout << "callback_serialize" << std::endl;

			auto surface = go->surface_ptr;

			// update transform
			// surface->transform.translation.x = r;
			// r+=0.001;

			surface->serialize();
		}
	};

	this->traverse(callback_serialize);

}

void HSim::SceneGraph::draw()
{

	std::function<void(HSim::GameObject_ptr)> callback_draw = [&](HSim::GameObject_ptr go)
	{
		if (go->surface_ptr != nullptr && go->drawable)
		{
			// std::cout << "callback_draw" << std::endl;

			auto surface = go->surface_ptr;

			surface->draw();
			surface->drawBoundary();
		}

		if (go->grid_ptr != nullptr && go->drawable)
		{
			// std::cout << "callback_draw" << std::endl;
			auto grid = go->grid_ptr;
			grid->drawBoundary();
			
			grid->drawData();
		}
		
	};

	this->traverse(callback_draw);
}
