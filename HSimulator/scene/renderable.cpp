#include <scene/renderable.h>

HSim::Renderable::Renderable()
{
}

HSim::Renderable::Renderable(SpaceObject3_Ptr<PRECISION> spaceObject_, GraphicsObject_Ptr graphicsObject_)
{
	spaceObject = spaceObject_;
	graphicsObject = graphicsObject_;
}

HSim::Renderable::~Renderable()
{
}

void HSim::Renderable::draw()
{
}