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

HSim::Renderable::Renderable(const Renderable &renderable_)
{
	visible = renderable_.visible;
	
	renderingDataNeedUpdate = renderable_.renderingDataNeedUpdate;
	
	// todo : all copy constructor
	spaceObject = std::make_shared<SpaceObject3<PRECISION>>(*(renderable_.spaceObject));

	graphicsObject = std::make_shared<GraphicsObject>(*(renderable_.graphicsObject));
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
