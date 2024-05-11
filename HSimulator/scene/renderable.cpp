#include <scene/renderable.h>
#include "renderable.h"

HSim::Renderable::Renderable()
{
}

HSim::Renderable::Renderable(SpaceObject3_Ptr<PRECISION> spaceObject_, GraphicsObject_Ptr graphicsObject_)
{
	spaceObject = spaceObject_;
	graphicsObject = graphicsObject_;
}

HSim::Renderable::Renderable(const Renderable &other)
{
	visible = other.visible;
	
	renderingDataNeedUpdate = other.renderingDataNeedUpdate;
	
	// todo : all copy constructor
	// todo clone()
	// renderable_.spaceObject->clone(spaceObject);
	// spaceObject = std::make_shared<SpaceObject3<PRECISION>>(*(renderable_.spaceObject));
	other.spaceObject->clone(spaceObject);

	// todo clone()
	// renderable_.graphicsObject->clone(graphicsObject);
	// graphicsObject = std::make_shared<GraphicsObject>(*(renderable_.graphicsObject));
	other.graphicsObject->clone(graphicsObject);
}

HSim::Renderable::~Renderable()
{
}

void HSim::Renderable::draw(const RenderParams &renderParams)
{
	assert(graphicsObject != nullptr);

	if (visible)
	{
		if (renderingDataNeedUpdate)
		{
			graphicsObject->buildRenderingData();
			renderingDataNeedUpdate = false;
		}
		
		graphicsObject->draw(renderParams);
	}
	
}
