#include "aabb3_graphics_object.h"

HSim::AABB3GObject::AABB3GObject()
{
}

HSim::AABB3GObject::AABB3GObject(const AABB3_Ptr<PRECISION> aabb_, const BasicMaterial_Ptr material_)
	: GraphicsObject(material_)
{
	aabb = aabb_;
	
	vbo.create();

	buildRenderingData();
}

HSim::AABB3GObject::~AABB3GObject()
{
}

void HSim::AABB3GObject::buildRenderingData()
{
	assert(aabb != nullptr);

	auto lowerCorner = aabb->lowerCorner;
	auto upperCorner = aabb->upperCorner;

	

}

bool HSim::AABB3GObject::isRendingDataValid()
{
	return (vao.isValid() && vbo.isValid());
}

void HSim::AABB3GObject::draw(const RenderParams &renderParams)
{
}
