#include "game_object.h"

HSim::GameObject::GameObject()
{
}

HSim::GameObject::~GameObject()
{
}

bool HSim::GameObject::isLeaf()
{
	if (children.empty())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void HSim::GameObject::traverseChildren(std::function<void(GameObject_ptr)> &callback)
{
	for (auto child : children)
	{
		callback(child);
	}
	
}
