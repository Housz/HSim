#include <scene/renderable.h>

HSim::Renderable::Renderable()
{
}

HSim::Renderable::Renderable(SpaceObject3_Ptr<PRECISION> object_, GraphicsObject_Ptr graphicsObject_)
{
	object = object_;
	graphicsObject = graphicsObject_;
}

HSim::Renderable::~Renderable()
{
}

void HSim::Renderable::draw()
{
}