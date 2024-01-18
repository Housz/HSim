#include "scene_graph.h"

HSim::SceneGraph::SceneGraph()
{
}

HSim::SceneGraph::~SceneGraph()
{
}

void HSim::SceneGraph::traverse(std::function<void(GameObject_ptr)>& callback)
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
