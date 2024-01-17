#include "game_object.h"

HSim::GameObject::GameObject()
{
}

HSim::GameObject::~GameObject()
{
}

void HSim::GameObject::traverse(std::function<void(GameObject&)> callback)
{
	if (children.empty())
	{
		callback(this);
	}
	
}
