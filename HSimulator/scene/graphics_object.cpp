#include <scene/graphics_object.h>
#include "graphics_object.h"

HSim::GraphicsObject::GraphicsObject()
{
}

HSim::GraphicsObject::~GraphicsObject()
{
}

HSim::Sphere3GObject::Sphere3GObject()
{
}

HSim::Sphere3GObject::Sphere3GObject(const Sphere3_Ptr sphere_)
{
	sphere = sphere_;
	buildRenderingData();
}

HSim::Sphere3GObject::~Sphere3GObject()
{
}

void HSim::Sphere3GObject::buildRenderingData()
{
	assert(sphere != nullptr);

	
}
