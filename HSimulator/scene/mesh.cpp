#include "mesh.h"

void HSim::Mesh::draw()
{
	if (surface_ptr != nullptr)
	{
		surface_ptr->draw();
		return;
	}

	if (grid_ptr != nullptr)
	{
		grid_ptr->drawBoundary();
		grid_ptr->drawData();
		return;
	}
}